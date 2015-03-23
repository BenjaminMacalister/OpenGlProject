#version 410

in vec2 fTexCoord;

out vec4 fragColour;

uniform sampler2D inputTexture;
uniform float time;

vec4 normalBlend()
{
	return texture(inputTexture, fTexCoord);
}
vec4 BoxBlur()
{
	vec2 texelUV = 1.0f / textureSize(inputTexture, 0).xy;

	vec4 colour = texture(inputTexture, fTexCoord);
	colour += texture(inputTexture, fTexCoord + vec2(-texelUV.x, texelUV.y));
	colour += texture(inputTexture, fTexCoord + vec2(-texelUV.x, 0));
	colour += texture(inputTexture, fTexCoord + vec2(-texelUV.x, -texelUV.y));
	colour += texture(inputTexture, fTexCoord + vec2(0, texelUV.y));
	colour += texture(inputTexture, fTexCoord + vec2(0, -texelUV.y));
	colour += texture(inputTexture, fTexCoord + vec2(texelUV.x, texelUV.y));
	colour += texture(inputTexture, fTexCoord + vec2(texelUV.x, 0));
	colour += texture(inputTexture, fTexCoord + vec2(texelUV.x, -texelUV.y));

	return colour/9;
}
vec4 CoordTest()
{
	return vec4(fTexCoord, 0,1);
}

vec4 Distort()
{
	vec2 mid = vec2(0.5f);
	float distanceFromCenter = distance(fTexCoord, mid);
	vec2 normalizeCoord = normalize(fTexCoord - mid);
	float bias = distanceFromCenter + sin((distanceFromCenter * 15)+time) *0.02f;

	vec2 newCoord = mid + bias * normalizeCoord;
	return texture(inputTexture, newCoord);
}

vec4 sharpen()
{
	vec2 texelSize = 1.0f / textureSize( inputTexture, 0 );
	vec4 gl_FragColor;
	gl_FragColor = texture( inputTexture, fTexCoord ) * (11 / 3);
	gl_FragColor += texture( inputTexture, fTexCoord + vec2( 0, texelSize.y ) ) * (-2 / 3);
	gl_FragColor += texture( inputTexture, fTexCoord - vec2( 0, texelSize.y ) ) * (-2 / 3);
	gl_FragColor += texture( inputTexture, fTexCoord + vec2( texelSize.x, 0 ) ) * (-2 / 3);
	gl_FragColor += texture( inputTexture, fTexCoord - vec2( texelSize.x, 0 ) ) * (-2 / 3);
	gl_FragColor.a = 1.0f;	return gl_FragColor;
}

vec4 invert(vec4 a_colour)
{
	return vec4(vec3(1,1,1) - a_colour.rgb, a_colour.a);
}
vec4 fisheye()
{
	float apiture = 128.0f;
	float apitureHalf = 0.5f * apiture * (3.14159256 / 180.0f);
	float maxFactor = sin(apitureHalf);
	vec2 uv;
	vec2 xy = 2.0 * fTexCoord - 1.0f;

	float d = length(xy);
	if(d<(2.0-maxFactor))
	{
		d = length(xy * maxFactor);
		float z = sqrt(1.0 - d * d);
		float r = atan(d,z) / 3.1415926;
		float phi = atan(xy.y, xy.x);

		uv.x = r*cos(phi) + 0.5f;
		uv.y = r*sin(phi) + 0.5f;
	}
	else
	{
		uv = texture(inputTexture, fTexCoord).xy;
	}
	vec4 c = texture(inputTexture, uv);
	return c;
}

void main()
{

	//fragColour = fisheye();
	fragColour =  invert(fisheye());
	

}