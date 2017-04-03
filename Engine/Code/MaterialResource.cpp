
#include "MaterialResource.hpp"

#include "AssimpConversions.hpp"
#include "ResourceManager.hpp"
#include "Texture.hpp"

#include <assimp/material.h>

namespace ft
{
	MaterialResource::MaterialResource()
	{
	}

	MaterialResource::~MaterialResource()
	{
		FT_ASSERT(!IsLoadedAndValid());
	}

	bool	 MaterialResource::IsLoadedAndValid() const
	{
		return m_oProperties.size() > 0;
	}

	ErrorCode	MaterialResource::Load(ResourceManager& oResourceManager, const MaterialResourceInfos& oInfos)
	{
		switch (oInfos.eSource)
		{
		case E_DEFAULT_MATERIAL:	{ FT_TEST_RETURN(LoadDefaultMaterial() == FT_OK, FT_FAIL); break; }
		case E_ASSIMP_MATERIAL:		{ FT_TEST_RETURN(LoadAssimpMaterial(oResourceManager, oInfos) == FT_OK, FT_FAIL); break; }
		default:					{ return FT_FAIL; }
		}

		m_oResourceInfos = oInfos;

		return FT_OK;
	}

	ErrorCode	MaterialResource::Unload()
	{
		m_oProperties.clear();
		return FT_OK;
	}

	ErrorCode	MaterialResource::LoadAssimpMaterial(ResourceManager& oResourceManager, const MaterialResourceInfos& oInfos)
	{
		const aiMaterial*			pMaterial = oInfos.pAiMaterial;
		MaterialResourceProperty	oMaterialProperty;
		TextureResourceInfos		oTexturesResourceInfos;
		aiString					oString;
		aiColor3D					oColor;
		float32						fFloat;

		oMaterialProperty.eType = E_TEXTURE;
		oTexturesResourceInfos.eTextureTarget = E_TEXTURE_2D;
		
		if (pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), oString) == AI_SUCCESS)
		{
			oTexturesResourceInfos.oFilePath = oInfos.oLocalPath;
			oTexturesResourceInfos.oFilePath.Add(oString.C_Str());
			FT_TEST(oResourceManager.GetTextureResourceManager()->Load(oTexturesResourceInfos, oMaterialProperty.xTextureResource) == FT_OK);
			m_oProperties[E_DIFFUSE] = oMaterialProperty;
		}
		if (pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_SPECULAR, 0), oString) == AI_SUCCESS)
		{
			oTexturesResourceInfos.oFilePath = oInfos.oLocalPath;
			oTexturesResourceInfos.oFilePath.Add(oString.C_Str());
			FT_TEST(oResourceManager.GetTextureResourceManager()->Load(oTexturesResourceInfos, oMaterialProperty.xTextureResource) == FT_OK);
			m_oProperties[E_SPECULAR] = oMaterialProperty;
		}
		if (pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_EMISSIVE, 0), oString) == AI_SUCCESS)
		{
			oTexturesResourceInfos.oFilePath = oInfos.oLocalPath;
			oTexturesResourceInfos.oFilePath.Add(oString.C_Str());
			FT_TEST(oResourceManager.GetTextureResourceManager()->Load(oTexturesResourceInfos, oMaterialProperty.xTextureResource) == FT_OK);
			m_oProperties[E_EMISSIVE] = oMaterialProperty;
		}
		if (pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), oString) == AI_SUCCESS)
		{
			oTexturesResourceInfos.oFilePath = oInfos.oLocalPath;
			oTexturesResourceInfos.oFilePath.Add(oString.C_Str());
			FT_TEST(oResourceManager.GetTextureResourceManager()->Load(oTexturesResourceInfos, oMaterialProperty.xTextureResource) == FT_OK);
			m_oProperties[E_NORMALS] = oMaterialProperty;
		}


		oMaterialProperty.eType = E_COLOR3;

		if (pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, oColor) == AI_SUCCESS)
		{
			if (m_oProperties.count(E_DIFFUSE) == 0) // priorité sur la texture
			{
				oMaterialProperty.oColor = FromAssimp(oColor);
				m_oProperties[E_DIFFUSE] = oMaterialProperty;
			}
		}
		if (pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, oColor) == AI_SUCCESS)
		{
			if (m_oProperties.count(E_SPECULAR) == 0) // priorité sur la texture
			{
				oMaterialProperty.oColor = FromAssimp(oColor);
				m_oProperties[E_SPECULAR] = oMaterialProperty;
			}
		}
		if (pMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, oColor) == AI_SUCCESS)
		{
			if (m_oProperties.count(E_EMISSIVE) == 0) // priorité sur la texture
			{
				oMaterialProperty.oColor = FromAssimp(oColor);
				m_oProperties[E_EMISSIVE] = oMaterialProperty;
			}
		}

		
		oMaterialProperty.eType = E_SCALAR;

		if (pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, oColor) == AI_SUCCESS)
		{
			oMaterialProperty.fValue = (oColor.r + oColor.g + oColor.b) / 3.f;
			m_oProperties[E_AMBIENT] = oMaterialProperty;
		}
		if (pMaterial->Get(AI_MATKEY_SHININESS, fFloat) == AI_SUCCESS)
		{
			oMaterialProperty.fValue = fFloat;
			m_oProperties[E_SHININESS] = oMaterialProperty;
		}

		return FT_OK;
	}

	ErrorCode	MaterialResource::LoadDefaultMaterial()
	{
		MaterialResourceProperty oMaterialProperty;

		oMaterialProperty.eType = E_COLOR3;
		oMaterialProperty.oColor = Color3(1.f, 0.f, 1.f); // bon magenta bien dégueu
		m_oProperties[E_DIFFUSE] = oMaterialProperty;
		oMaterialProperty.oColor = Color3(0.f, 0.f, 0.f);
		m_oProperties[E_SPECULAR] = oMaterialProperty;
		oMaterialProperty.oColor = Color3(0.f, 0.f, 0.f);
		m_oProperties[E_EMISSIVE] = oMaterialProperty;

		oMaterialProperty.eType = E_SCALAR;
		oMaterialProperty.fValue = 0.5f;
		m_oProperties[E_AMBIENT] = oMaterialProperty;
		oMaterialProperty.fValue = 0.f;
		m_oProperties[E_SHININESS] = oMaterialProperty;

		return FT_OK;
	}
}
