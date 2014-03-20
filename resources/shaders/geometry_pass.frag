#version 330

in vec4 fPosition;
in vec2 fTextureCoord;
in vec3 fNormal;

uniform uint objectID;
uniform uint selectedID;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;

layout (location = 0) out vec3 oPosition;
layout (location = 1) out vec3 oDiffuse;
layout (location = 2) out vec3 oNormal;
layout (location = 3) out vec3 oTextureCoord;
layout (location = 4) out vec3 oDiffuseColor;
layout (location = 5) out vec3 oSpecularColor;
layout (location = 6) out float oShininess;

uniform sampler2D sampler;

void main()
{
	oPosition = fPosition.xyz / fPosition.w;
	vec3 highlight = vec3(0, 0, 0);
	if (objectID == selectedID)
		highlight = vec3(0.5, 0, 0.5);
	oDiffuse = highlight + texture(sampler, fTextureCoord).xyz;
    if (!gl_FrontFacing)
		oNormal = normalize(-fNormal);
	else
		oNormal = normalize(fNormal);
	oTextureCoord = vec3(fTextureCoord, 0.0);
	oDiffuseColor = diffuseColor;
	oSpecularColor = specularColor;
	oShininess = shininess;
}
