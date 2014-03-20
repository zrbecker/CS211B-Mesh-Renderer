#version 330

in vec4 fPosition;
in vec2 fTextureCoord;
in vec3 fNormal;

uniform uint objectID;
uniform uint selectedID;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;

layout (location = 0) out vec4 oPosition;
layout (location = 1) out vec4 oDiffuse;
layout (location = 2) out vec4 oNormal;
layout (location = 3) out vec4 oTextureCoord;
layout (location = 4) out vec4 oDiffuseColor;
layout (location = 5) out vec4 oSpecularColor;
layout (location = 6) out vec4 oShininess;

uniform sampler2D sampler;

void main()
{
	oPosition = fPosition;
	vec4 highlight = vec4(0, 0, 0, 0);
	if (objectID == selectedID)
		highlight = vec4(0.7, 0, 0.7, 0);
	oDiffuse = highlight + texture(sampler, fTextureCoord);
    if (!gl_FrontFacing)
		oNormal = vec4(normalize(-fNormal), 1);
	else
		oNormal = vec4(normalize(fNormal), 1);
	oTextureCoord = vec4(fTextureCoord, 0.0, 0.0);
	oDiffuseColor = vec4(diffuseColor, 1);
	oSpecularColor = vec4(specularColor, 1);
	oShininess = vec4(vec3(shininess), 1);
}
