#version 330

uniform sampler2D texPosition;
uniform sampler2D texDiffuse;
uniform sampler2D texNormal;
uniform sampler2D texDiffuseColor;
uniform sampler2D texSpecularColor;
uniform sampler2D texShininess;

uniform uint objectID;
uniform uint selectedID;

uniform vec3 ambientLight;
uniform vec4 lightPositions[10];
uniform vec3 lightColors[10];
uniform uint numLights;

uniform float screenWidth;
uniform float screenHeight;

out vec4 color;

vec2 calcTextureCoord()
{
	return vec2(gl_FragCoord.x / screenWidth, gl_FragCoord.y / screenHeight);
}

void computeLightEffect(vec4 lightPos, vec3 lightColor, out vec3 diffuseEffect, out vec3 specularEffect)
{
	vec2 textureCoord = calcTextureCoord();
	vec4 fPosition = vec4(texture(texPosition, textureCoord).xyz, 1.0);
	vec3 fNormal = texture(texNormal, textureCoord).xyz;
	vec3 diffuseColor = texture(texDiffuseColor, textureCoord).xyz;
	vec3 specularColor = texture(texSpecularColor, textureCoord).xyz;
	float shininess = texture(texShininess, textureCoord).x;

	vec3 L;
	if (lightPos.w == 0)
		L = normalize(lightPos.xyz);
	else
		L = normalize(lightPos.xyz / lightPos.w - fPosition.xyz / fPosition.w);
	vec3 N = normalize(fNormal);
	vec3 V = normalize(-fPosition.xyz / fPosition.w);
	vec3 H = normalize(L + V);

	float diffuseComponent = max(0, dot(L, N));
	float specularComponent = max(0, pow(dot(H, N), shininess));
	if (diffuseComponent == 0)
		specularComponent = 0;

	diffuseEffect = diffuseComponent * 1 * lightColor;
	specularEffect = specularComponent * specularColor * lightColor;
}

void main()
{
	vec2 textureCoord = calcTextureCoord();
	vec3 fDiffuse = texture(texDiffuse, textureCoord).xyz;
	vec3 diffuseColor = texture(texDiffuseColor, textureCoord).xyz;

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
	vec3 highlight = vec3(0, 0, 0);
	// if (objectID == selectedID)
	// 	highlight = vec3(0.5, 0, 0.5);

	color.rgb = min(vec3(1), highlight + (ambientEffect + totalDiffuseEffect) * fDiffuse.rgb + totalSpecularEffect);
	color.a = 1.0;
}
