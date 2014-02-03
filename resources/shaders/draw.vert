#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec3 normal;

uniform mat4 projection;
uniform mat4 modelview;

out vec4 fPosition;
out vec2 fTextureCoord;
out vec3 fNormal;

void main()
{
	fPosition = modelview * position;
	fTextureCoord = textureCoord;
	fNormal = vec3(modelview * vec4(normal, 0));
	gl_Position = projection * modelview * position;
}
