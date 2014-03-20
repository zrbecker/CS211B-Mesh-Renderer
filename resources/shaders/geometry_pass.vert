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
    fNormal = transpose(inverse(mat3(modelview))) * normal;
	gl_Position = projection * modelview * position;
}
