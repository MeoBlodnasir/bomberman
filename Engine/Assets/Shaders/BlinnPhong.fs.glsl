#version 410 core

#include "MaterialStruct.h.glsl"

uniform Material	oMaterial;

uniform vec3	vLightDirection = vec3(-0.5f, -1.0f, -0.3f);
uniform vec3	vLightDiffuse = vec3(1.0f, 1.0f, 1.0f);
uniform vec3	vLightAmbient = vec3(0.2f, 0.2f, 0.2f);
uniform vec3	vLightSpecular = vec3(1.0f, 1.0f, 1.0f); // == vLightDiffuse

uniform vec3 vViewPosition;

in	vec3 vFragPos_vs;  
in	vec3 vNormal_vs;
in	vec2 vUV_vs;

out	vec4 vColor_fs;

#include "MaterialFunctions.h.glsl"

void main()
{
	// Diffuse
	vec3	vNormal = normalize(vNormal_vs);
	//vec3	vLightDir = normalize(vLightPosition - vFragPos_vs);
	vec3	vLightDir = normalize(-vLightDirection);
	float	fDiffuseStrength = max(dot(vNormal, vLightDir), 0.0f);
	vec3	vMaterialDiffuse = GetMaterialDiffuse();
	vec3	vDiffuseColor = vLightDiffuse * (fDiffuseStrength * vMaterialDiffuse);

	// Ambient
	vec3	vAmbientColor = vLightAmbient * vMaterialDiffuse * oMaterial.fAmbient;
	
	// Specular
	vec3	vViewDir = normalize(vViewPosition - vFragPos_vs);
	vec3	vReflectDir = reflect(-vLightDir, vNormal);  
	float	fSpecular = pow(max(dot(vViewDir, vReflectDir), 0.0f), oMaterial.fShininess);
	vec3	vSpecularColor = vLightSpecular * (fSpecular * GetMaterialSpecular());

	vec3	vResult = vAmbientColor + vDiffuseColor + vSpecularColor + GetMaterialEmissive();
			vResult = pow(vResult, vec3(1.0f/2.2f));
	
	vColor_fs = vec4(vResult, 1.0f);
}
