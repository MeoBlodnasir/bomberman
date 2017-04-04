
#include "Graphics/Material.hpp"

#include "Graphics/Texture.hpp"
#include "Graphics/ShaderProgram.hpp"

namespace ft
{
	Material::Material()
		: m_xResource(nullptr)
	{
	}

	Material::Material(const Material& oMaterial)
		: m_xResource(oMaterial.m_xResource)
		, m_oProperties(oMaterial.m_oProperties)
	{
	}

	Material::~Material()
	{
	}

	Material&	Material::operator = (const Material& oMaterial)
	{
		m_xResource = oMaterial.m_xResource;
		m_oProperties = oMaterial.m_oProperties;
		return *this;
	}

	ErrorCode	MaterialProperty::CreateFrom(const MaterialResourceProperty& oResource)
	{
		eType = oResource.eType;
		switch (oResource.eType)
		{
		case E_SCALAR:	{ fValue = oResource.fValue; break; }
		case E_COLOR3:	{ oColor = oResource.oColor; break; }
		case E_TEXTURE:
			{
				xTexture = new Texture;
				FT_TEST_RETURN(xTexture->Create(oResource.xTextureResource) == FT_OK, FT_FAIL);
				break;
			}
		}

		return FT_OK;
	}

	ErrorCode	Material::Create(const SPtr<MaterialResource>& xResource)
	{
		FT_ASSERT(xResource != nullptr);

		FT_TEST_RETURN(xResource->IsLoadedAndValid(), FT_FAIL);
		m_xResource = xResource;

		for (const std::pair<EMaterialProperty, MaterialResourceProperty>& oResourceProperty : xResource->m_oProperties)
		{
			FT_TEST(m_oProperties[oResourceProperty.first].CreateFrom(oResourceProperty.second) == FT_OK);
		}

		return FT_OK;
	}

	ErrorCode	Material::Destroy()
	{
		m_xResource = nullptr;
		m_oProperties.clear();
		return FT_OK;
	}

	void	Material::SetProperty(EMaterialProperty eProperty, float fValue)
	{
		MaterialProperty& oProperty = m_oProperties[eProperty];
		oProperty.eType = E_SCALAR;
		oProperty.fValue = fValue;
	}

	void	Material::SetProperty(EMaterialProperty eProperty, const Color3& oColor)
	{
		MaterialProperty& oProperty = m_oProperties[eProperty];
		oProperty.eType = E_COLOR3;
		oProperty.oColor = oColor;
	}

	void	Material::SetProperty(EMaterialProperty eProperty, const SPtr<Texture>& xTexture)
	{
		MaterialProperty& oProperty = m_oProperties[eProperty];
		oProperty.eType = E_TEXTURE;
		oProperty.xTexture = xTexture;
	}

	void	Material::SendToShader(const SPtr<ShaderProgram>& xShaderProgram) const
	{
		uint32 iTextureUnit = 0;

		for (const std::pair<EMaterialProperty, MaterialProperty>& oProperty : m_oProperties)
		{
			switch (oProperty.first)
			{
			case E_DIFFUSE:
				{
					if (oProperty.second.eType == E_TEXTURE)
					{
						oProperty.second.xTexture->Bind(iTextureUnit);
						xShaderProgram->SetUniform("oMaterial.oDiffuseTexture", oProperty.second.xTexture, iTextureUnit++);
						xShaderProgram->SetUniform("oMaterial.bHasDiffuseTexture", 1);
					}
					else
					{
						xShaderProgram->SetUniform("oMaterial.vDiffuseColor", oProperty.second.oColor);
						xShaderProgram->SetUniform("oMaterial.bHasDiffuseTexture", 0);
					}
					break;
				}

			case E_AMBIENT:
				{
					xShaderProgram->SetUniform("oMaterial.fAmbient", oProperty.second.fValue);
					break;
				}

			case E_SPECULAR:
				{
					if (oProperty.second.eType == E_TEXTURE)
					{
						oProperty.second.xTexture->Bind(iTextureUnit);
						xShaderProgram->SetUniform("oMaterial.oSpecularTexture", oProperty.second.xTexture, iTextureUnit++);
						xShaderProgram->SetUniform("oMaterial.bHasSpecularTexture", 1);
					}
					else
					{
						xShaderProgram->SetUniform("oMaterial.vSpecularColor", oProperty.second.oColor);
						xShaderProgram->SetUniform("oMaterial.bHasSpecularTexture", 0);
					}
					break;
				}

			case E_SHININESS:
				{
					xShaderProgram->SetUniform("oMaterial.fShininess", oProperty.second.fValue);
					break;
				}

			case E_EMISSIVE:
				{
					if (oProperty.second.eType == E_TEXTURE)
					{
						oProperty.second.xTexture->Bind(iTextureUnit);
						xShaderProgram->SetUniform("oMaterial.oEmissiveTexture", oProperty.second.xTexture, iTextureUnit++);
						xShaderProgram->SetUniform("oMaterial.bHasEmissiveTexture", 1);
					}
					else
					{
						xShaderProgram->SetUniform("oMaterial.vEmissiveColor", oProperty.second.oColor);
						xShaderProgram->SetUniform("oMaterial.bHasEmissiveTexture", 0);
					}
					break;
				}

			case E_NORMALS:
				{
					if (oProperty.second.eType == E_TEXTURE)
					{
						oProperty.second.xTexture->Bind(iTextureUnit);
						xShaderProgram->SetUniform("oMaterial.oNormalsTexture", oProperty.second.xTexture, iTextureUnit++);
						xShaderProgram->SetUniform("oMaterial.bHasNormalsTexture", 1);
					}
					else
						xShaderProgram->SetUniform("oMaterial.bHasDiffuseTexture", 0);
					break;
				}

			default:
				{
					FT_FAILED_ASSERTION(oProperty.first);
					break;
				}
			}
		}
	}
}
