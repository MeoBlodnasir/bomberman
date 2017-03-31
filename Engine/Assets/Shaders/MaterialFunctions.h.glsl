#ifndef _MATERIALFUNCTIONS_H_
#	define _MATERIALFUNCTIONS_H_

vec3	GetMaterialDiffuse()
{
	if (oMaterial.bHasDiffuseTexture == true)
		return vec3(texture(oMaterial.oDiffuseTexture, vUV_vs.xy));
	return oMaterial.vDiffuseColor;
}

vec3	GetMaterialSpecular()
{
	if (oMaterial.bHasSpecularTexture == true)
		return vec3(texture(oMaterial.oSpecularTexture, vUV_vs.xy));
	return oMaterial.vSpecularColor;
}

vec3	GetMaterialEmissive()
{
	if (oMaterial.bHasEmissiveTexture == true)
		return vec3(texture(oMaterial.oEmissiveTexture, vUV_vs.xy));
	return oMaterial.vEmissiveColor;
}

vec3	GetMaterialNormal()
{
	/*
	if (oMaterial.bHasNormalTexture == true)
    {
        mat3 tbn;
        tbn[0] = vTangent_vs;
        tbn[1] = normalize(cross(vNormal_vs, vTangent_vs));
        tbn[2] = vNormal_vs;
        return normalize(tbn * normalize(vec3(texture(oMaterial.oNormalTexture, vUV_vs.xy)) * 2.0f - 1.0f));
    }
	*/
	return normalize(vNormal_vs);
}

#endif // _BLINNPHONGMATERIALFUNCTIONS_H_
