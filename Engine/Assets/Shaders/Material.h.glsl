#ifndef _MATERIAL_H_
#	define _MATERIAL_H_

struct PhongMaterial
{
	vec3		vDiffuseColor;
	vec3		vAmbientColor;
	vec3		vSpecularColor;
	vec3		vEmissiveColor;
	float		fShininess;

	bool		bHasDiffuseTexture;
	bool		bHasAmbientTexture;
	bool		bHasSpecularTexture;
	bool		bHasEmissiveTexture;
	bool		bHasNormalsTexture;

	sampler2D	oDiffuseTexture;
	sampler2D	oAmbientTexture;
	sampler2D	oSpecularTexture;
	sampler2D	oEmissiveTexture;
	sampler2D	oNormalsTexture;
};

#endif // _MATERIAL_H_
