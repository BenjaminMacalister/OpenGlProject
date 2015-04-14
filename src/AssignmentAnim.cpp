#include "AssignmentAnim.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
//#define STB_IMAGE_IMPLEMENTATION
bool Animation::StartUp()
{

	m_timer = 2.5;
	m_file = new FBXFile;
	m_file->load("./models/characters/Enemyelite/EnemyElite.fbx");
	m_file->initialiseOpenGLTextures();
	GenerateGLMeshes1(m_file);
	LoadShaders("./Shaders/AnimationVertex.glsl", nullptr, "./Shaders/AnimationFragment.glsl", &m_animationID);
	return true;
}

bool Animation::Update(float dt)
{

	//float dt = (float)glfwGetTime();
	m_timer += dt;


	if (m_timer > 4.8)
	{
		m_timer = 2.7;
	}
	FBXSkeleton * skele = m_file->getSkeletonByIndex(0);
	FBXAnimation* anim = m_file->getAnimationByIndex(0);

	EvaluateSkeleton1(anim, skele, m_timer);
	for (unsigned int i = 0; i < skele->m_boneCount; i++)
	{
		skele->m_nodes[i]->updateGlobalTransform();
		mat4 nodeGlobal = skele->m_nodes[i]->m_globalTransform;
		vec3 nodePos = nodeGlobal[3].xyz * 400;
	}

	return true;
}
void Animation::ShutDown()
{
	m_file->unload();
	delete m_file;
}
void Animation::Draw(FlyCamera a_camera)
{

	glUseProgram(m_animationID);
	int proj_veiw_uniform = glGetUniformLocation(m_animationID, "projection_view");
	glUniformMatrix4fv(proj_veiw_uniform, 1, GL_FALSE, (float*)&a_camera.getProjectionView());
	int position_uniform = glGetUniformLocation(m_animationID, "nPosition");
	glUniform4f(position_uniform, 10, 10, 10, 10);
	FBXSkeleton* skeleton = m_file->getSkeletonByIndex(0);

	UpdateBones1(skeleton);
	int BonesUniform = glGetUniformLocation(m_animationID, "bones");
	glUniformMatrix4fv(BonesUniform, skeleton->m_boneCount, GL_FALSE, (float*)skeleton->m_bones);

	int diffuseUniform = glGetUniformLocation(m_animationID, "diffuse");
	glUniform1i(diffuseUniform, 0);
	for (int i = 0; i < m_meshes.size(); i++)
	{
		FBXMeshNode* currMesh = m_file->getMeshByIndex(i);
		FBXMaterial* meshMaterial = currMesh->m_material;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, meshMaterial->textures[FBXMaterial::DiffuseTexture]->handle);

		mat4 worldTransform = m_file->getMeshByIndex(0)->m_globalTransform;
		int world_uniform = glGetUniformLocation(m_animationID, "world");
		glUniformMatrix4fv(world_uniform, 1, GL_FALSE, (float*)&worldTransform);

		glBindVertexArray(m_meshes[i].m_VAO);
		glDrawElements(GL_TRIANGLES, m_meshes[i].m_index_count, GL_UNSIGNED_INT, 0);
	}

}
void Animation::GenerateGLMeshes1(FBXFile* fbx)
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
		glEnableVertexAttribArray(4);//normal

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::PositionOffset);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::TexCoord1Offset);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::IndicesOffset);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::WeightsOffset);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::NormalOffset);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}
}

void Animation::EvaluateSkeleton1(FBXAnimation* anim, FBXSkeleton* skeleton, float timer)
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

void Animation::UpdateBones1(FBXSkeleton* skeleton)
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