#version 410 core

uniform vec3 vObjectColor;
uniform vec3 vLightPosition;
uniform vec3 vLightColor;
uniform vec3 vViewPosition;

in	vec3 vFragPos_vs;  
in	vec3 vNormal_vs;

out	vec4 vColor_fs;

void main()
{
	// Ambient
	float fAmbientStrength = 0.1f;
	vec3 vAmbientColor = fAmbientStrength * vLightColor;

	// Diffuse
	vec3 vNormal = normalize(vNormal_vs);
	vec3 vLightDir = normalize(vLightPosition - vFragPos_vs);
	float fDiffuseStrength = max(dot(vNormal, vLightDir), 0.0);
	vec3 vDiffuseColor = fDiffuseStrength * vLightColor;

	// Specular
	float fSpecularStrength = 0.5f;
	vec3 vViewDir = normalize(vViewPosition - vFragPos_vs);
	vec3 vReflectDir = reflect(-vLightDir, vNormal);  
	float fSpecular = pow(max(dot(vViewDir, vReflectDir), 0.0), 32);
	vec3 vSpecularColor = fSpecularStrength * fSpecular * vLightColor;

	vColor_fs = vec4((vAmbientColor + vDiffuseColor + vSpecularColor) * vObjectColor, 1.0f);
}
