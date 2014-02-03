#version 330

layout (location = 0) in vec4 position;

uniform mat4 projection;
uniform mat4 modelview;

void main()
{
	gl_Position = projection * modelview * position;
}
