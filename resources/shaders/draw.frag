#version 330

uniform sampler2D sampler;

uniform uint objectID;
uniform uint selectedID;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;

uniform vec3 ambientLight;
uniform vec4 lightPositions[10];
uniform vec3 lightColors[10];
uniform uint numLights;

in vec4 fPosition;
in vec2 fTextureCoord;
in vec3 fNormal;

out vec4 color;

void computeLightEffect(vec4 lightPos, vec3 lightColor, out vec3 diffuseEffect, out vec3 specularEffect)
{
	vec3 L;
	if (lightPos.w == 0)
		L = normalize(lightPos.xyz);
	else
		L = normalize(lightPos.xyz / lightPos.w - fPosition.xyz / fPosition.w);
	vec3 N = normalize(fNormal);
    if (!gl_FrontFacing)
         N = -N;
	vec3 V = normalize(-fPosition.xyz / fPosition.w);
	vec3 H = normalize(L + V);

	float diffuseComponent = max(0, dot(L, N));
	float specularComponent = max(0, pow(dot(H, N), shininess));
	if (diffuseComponent == 0)
		specularComponent = 0;

	diffuseEffect = diffuseComponent * diffuseColor * lightColor;
	specularEffect = specularComponent * specularColor * lightColor;
}

void main()
{
	vec3 totalDiffuseEffect = vec3(0, 0, 0);
	vec3 totalSpecularEffect = vec3(0, 0, 0);
	vec3 ambientEffect = ambientLight * diffuseColor;
	for (uint i = 0u; i < numLights; ++i)
	{
		vec3 diffuseEffect;
		vec3 specularEffect;
		computeLightEffect(lightPositions[i], lightColors[i], diffuseEffect, specularEffect);
		totalDiffuseEffect += diffuseEffect;
		totalSpecularEffect += specularEffect;
	}
	
	vec4 texColor = texture(sampler, fTextureCoord);
	vec3 highlight = vec3(0, 0, 0);
	if (objectID == selectedID)
		highlight = vec3(0.5, 0, 0.5);

	color.rgb = min(vec3(1), highlight + (ambientEffect + totalDiffuseEffect) * texColor.rgb + totalSpecularEffect);
	color.a = texColor.a;
}
