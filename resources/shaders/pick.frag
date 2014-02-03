#version 330

uniform uint objectID;

out uint color;

void main()
{
	color = objectID;
}
