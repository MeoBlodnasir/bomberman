#version 410 core

#include "MaterialStruct.h.glsl"

struct Light
{
	vec3	vPosition;
	vec3	vAmbientColor;
	vec3	vDiffuseColor;
	vec3	vSpecularColor;
};

uniform Material	oMaterial;
uniform Light		oLight;

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
	vec3	vLightDir = normalize(oLight.vPosition - vFragPos_vs);
	float	fDiffuseStrength = max(dot(vNormal, vLightDir), 0.0f);
	vec3	vMaterialDiffuse = GetMaterialDiffuse();
	vec3	vDiffuseColor = oLight.vDiffuseColor * (fDiffuseStrength * vMaterialDiffuse);

	// Ambient
	vec3	vAmbientColor = oLight.vAmbientColor * vMaterialDiffuse * oMaterial.fAmbient;
	
	// Specular
	vec3	vViewDir = normalize(vViewPosition - vFragPos_vs);
	vec3	vReflectDir = reflect(-vLightDir, vNormal);  
	float	fSpecular = pow(max(dot(vViewDir, vReflectDir), 0.0f), oMaterial.fShininess);
	vec3	vSpecularColor = oLight.vSpecularColor * (fSpecular * GetMaterialSpecular());

	vec3	vResult = vAmbientColor + vDiffuseColor + vSpecularColor + GetMaterialEmissive();
			vResult = pow(vResult, vec3(1.0f/2.2f));
	
	vColor_fs = vec4(vResult, 1.0f);
}
