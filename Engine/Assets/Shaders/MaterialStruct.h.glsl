#ifndef _MATERIALSTRUCT_H_
#	define _MATERIALSTRUCT_H_

struct Material
{
	vec3		vDiffuseColor;
	vec3		vSpecularColor;
	vec3		vEmissiveColor;
	float		fAmbient;
	float		fShininess;

	bool		bHasDiffuseTexture;
	bool		bHasSpecularTexture;
	bool		bHasEmissiveTexture;
	bool		bHasNormalsTexture;

	sampler2D	oDiffuseTexture;
	sampler2D	oSpecularTexture;
	sampler2D	oEmissiveTexture;
	sampler2D	oNormalsTexture;
};

#endif // _MATERIALSTRUCT_H_
