#include "Animation.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
//#define STB_IMAGE_IMPLEMENTATION
bool Animation::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	Gizmos::create();
	m_Camera = FlyCamera();
	m_Camera.SetSpeed(5.0f);
	m_timer = 0;
	m_file = new FBXFile;

	m_file->load("./models/characters/Enemyelite/EnemyElite.fbx");
	m_file->initialiseOpenGLTextures();
	GenerateGLMeshes(m_file);
	LoadShaders("./Shaders/AnimationVertex.glsl",nullptr, "./Shaders/AnimationFragment.glsl", &m_ProgramID);
	return true;
}

bool Animation::Update()
{
	Gizmos::clear();
	if (Application::Update() == false)
	{
		return false;
	}
	m_Camera.setPosition(vec3(2, 2, 2));
	float dt = (float)glfwGetTime();
	m_timer += dt;
	glfwSetTime(0.0);

	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	vec4 blue(0, 0, 1, 1);
	vec4 red(1, 0, 0, 1);
	vec4 yellow(1, 1, 0, 1);
	vec4 green(0, 1, 0, 1);
	int x = 2000;
	for (int i = 0; i <= x; i++)
	{
		Gizmos::addLine(vec3(-x / 2 + i, 0, -x / 2), vec3(-x / 2 + i, 0, x / 2), i == x / 2 ? yellow : blue);
		Gizmos::addLine(vec3(-x / 2, 0, -x / 2 + i), vec3(x / 2, 0, -x / 2 + i), i == x / 2 ? green : red);
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_G) == GLFW_PRESS)
	{
		Gizmos::addLine(vec3(0, 0, 0),m_Camera.m_WorldTransform[3].xyz, vec4(0, 0, 0, 1));
		glLineWidth(3);
	}
	Gizmos::addLine(vec3(0, 0, 0), m_Camera.m_WorldTransform[3].xyz, vec4(0, 0, 0, 1));
	FBXSkeleton * skele = m_file->getSkeletonByIndex(0);
	FBXAnimation* anim = m_file->getAnimationByIndex(0);
	//skele->evaluate(anim, m_timer);
	EvaluateSkeleton(anim, skele, m_timer);
	for (unsigned int i = 0; i < skele->m_boneCount; i++)
	{
		skele->m_nodes[i]->updateGlobalTransform();
		mat4 nodeGlobal = skele->m_nodes[i]->m_globalTransform;
		vec3 nodePos = nodeGlobal[3].xyz;
		//Gizmos::addAABBFilled(nodePos, vec3(65.0f), vec4(1, 0, 0, 1), &nodeGlobal);
		if (skele->m_nodes[i]->m_parent != nullptr)
		{
			vec3 parentPos = skele->m_nodes[i]->m_parent->m_globalTransform[3].xyz;
			Gizmos::addLine(nodePos, parentPos, vec4(0, 1, 0, 1));
		}
	}

	m_Camera.update(dt);

	return true;
}
void Animation::ShutDown()
{
	m_file->unload();
	delete m_file;
	Gizmos::destroy();
	Application::ShutDown();
}
void Animation::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_ProgramID);
	int proj_veiw_uniform = glGetUniformLocation(m_ProgramID, "projection_view");
	glUniformMatrix4fv(proj_veiw_uniform, 1, GL_FALSE, (float*)&m_Camera.getProjectionView());
	FBXSkeleton* skeleton = m_file->getSkeletonByIndex(0);
	//skeleton->updateBones();
	UpdateBones(skeleton);
	int BonesUniform = glGetUniformLocation(m_ProgramID, "bones");
	glUniformMatrix4fv(BonesUniform, skeleton->m_boneCount, GL_FALSE, (float*)skeleton->m_bones);

	int diffuseUniform = glGetUniformLocation(m_ProgramID, "diffuse");
	glUniform1i(diffuseUniform, 0);
	for (int i = 0; i < m_meshes.size(); i++)
	{
		FBXMeshNode* currMesh = m_file->getMeshByIndex(i);
		FBXMaterial* meshMaterial = currMesh->m_material;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, meshMaterial->textures[FBXMaterial::DiffuseTexture]->handle);

		mat4 worldTransform = m_file->getMeshByIndex(0)->m_globalTransform;
		int world_uniform = glGetUniformLocation(m_ProgramID, "world");
		glUniformMatrix4fv(world_uniform, 1, GL_FALSE, (float*)&worldTransform);

		glBindVertexArray(m_meshes[i].m_VAO);
		glDrawElements(GL_TRIANGLES, m_meshes[i].m_index_count, GL_UNSIGNED_INT, 0);
	}
	Gizmos::draw(m_Camera.getProjectionView());
	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
}
void Animation::GenerateGLMeshes(FBXFile* fbx)
{
	unsigned int meshCount = fbx->getMeshCount();
	m_meshes.resize(meshCount);

	for (unsigned int meshIndex = 0; meshIndex < meshCount; meshIndex++)
	{
		FBXMeshNode* currMesh = fbx->getMeshByIndex(meshIndex);
		m_meshes[meshIndex].m_index_count = currMesh->m_indices.size();
		glGenBuffers(1, &m_meshes[meshIndex].m_VBO);
		glGenBuffers(1, &m_meshes[meshIndex].m_IBO);
		glGenVertexArrays(1, &m_meshes[meshIndex].m_VAO);

		glBindVertexArray(m_meshes[meshIndex].m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_meshes[meshIndex].m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(FBXVertex)*currMesh->m_vertices.size(), currMesh->m_vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshes[meshIndex].m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*currMesh->m_indices.size(), currMesh->m_indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); //pos
		glEnableVertexAttribArray(1); //texcoord
		glEnableVertexAttribArray(2); //Bone indices
		glEnableVertexAttribArray(3); //Bone weights

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::PositionOffset);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::TexCoord1Offset);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::IndicesOffset);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::WeightsOffset);
		
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}
}

void Animation::EvaluateSkeleton(FBXAnimation* anim, FBXSkeleton* skeleton, float timer)
{
	float FPS = 24;
	int currentFrame = (int)(timer * FPS);
	//loop through all the tracks
	for (unsigned int trackIndex = 0; trackIndex < anim->m_trackCount; trackIndex++)
	{
		//get the right bone for the given track
		//wraps back to the start of the track if weve gone passed the end
		//currentFrame %= anim->m_tracks[trackIndex].m_keyframeCount;
		int trackFrameCount = anim->m_tracks[trackIndex].m_keyframeCount;
		int trackFrame = currentFrame%trackFrameCount;
		//find the key frames affecting the bones
		FBXKeyFrame currFrame = anim->m_tracks[trackIndex].m_keyframes[trackFrame];
		FBXKeyFrame nextFrame = anim->m_tracks[trackIndex].m_keyframes[(trackFrame + 1) % trackFrameCount];
		//interpolate between key frames to generate the matrix for the current pose
		float timeSinceFramFlip = timer - (currentFrame / FPS);
		float t = timeSinceFramFlip * FPS;

		vec3 newPos = glm::mix(currFrame.m_translation, nextFrame.m_translation, t);
		vec3 newScale = glm::mix(currFrame.m_scale, nextFrame.m_scale, t);
		glm::quat newRot = glm::slerp(currFrame.m_rotation, nextFrame.m_rotation, t);

		mat4 localTransform = glm::translate(newPos)*glm::toMat4(newRot) * glm::scale(newScale);

		//set FBXNode's local transforms to match
		unsigned int boneIndex = anim->m_tracks[trackIndex].m_boneIndex;
		if (boneIndex < skeleton->m_boneCount)
		{
			skeleton->m_nodes[boneIndex]->m_localTransform = localTransform;
		}
	}
}

void Animation::UpdateBones(FBXSkeleton* skeleton)
{
	//loop through all the nodes int the skeleton
	for (unsigned int boneIndex = 0; boneIndex < skeleton->m_boneCount; boneIndex++)
	{
	//generate the global transforms
		int parentIndex = skeleton->m_parentIndex[boneIndex];
		if (parentIndex == -1)
		{
			skeleton->m_bones[boneIndex] = skeleton->m_nodes[boneIndex]->m_localTransform;
		}
		else
		{
			skeleton->m_bones[boneIndex] = skeleton->m_bones[parentIndex] * skeleton->m_nodes[boneIndex]->m_localTransform;
		}
		//multiply the global transform by the inverse bimd pose
	}
	for (unsigned int boneIndex = 0; boneIndex < skeleton->m_boneCount; boneIndex++)
	{
		skeleton->m_bones[boneIndex] = skeleton->m_bones[boneIndex] * skeleton->m_bindPoses[boneIndex];
	}
}