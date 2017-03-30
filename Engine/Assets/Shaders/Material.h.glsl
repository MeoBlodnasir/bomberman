#ifndef _MATERIAL_H_
#	define _MATERIAL_H_

struct TextureSet
{
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

struct Material
{
	vec3		vDiffuseColor;
	vec3		vAmbientColor;
	vec3		vSpecularColor;
	vec3		vEmissiveColor;
	float		fShininess;

	TextureSet	oTextureSet;
};

#endif // _MATERIAL_H_
