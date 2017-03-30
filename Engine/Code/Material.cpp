
#include "Material.hpp"

#include "ShaderProgram.hpp"
#include "Texture.hpp"

namespace ft
{

	Material::Material()
		: m_oDiffuseColor(1.f, 0.f, 1.f)	// un bon magenta dégueu
		, m_oAmbientColor(0.5f, 0.5f, 0.5f)	// gris 50
		, m_oSpecularColor(0.f, 0.f, 0.f)
		, m_oEmissiveColor(0.f, 0.f, 0.f)
		, m_fShininess(0.f)
		, m_xDiffuseTexture(nullptr)
		, m_xAmbientTexture(nullptr)
		, m_xSpecularTexture(nullptr)
		, m_xEmissiveTexture(nullptr)
		, m_xNormalsTexture(nullptr)
	{
	}

	Material::~Material()
	{
	}

	void Material::SetShader(const SPtr<ShaderProgram>& xShader)
	{
		FT_ASSERT(xShader != nullptr);
		m_xShader = xShader;
	}

	void	Material::SetProperty(EMaterialProperty eProperty, float fValue)
	{
		FT_ASSERT(eProperty < E_SCALAR_PROPERTIES_COUNT);
		switch (eProperty)
		{
		case E_SHININESS:	{ m_fShininess = fValue; break; }
		default:			{ FT_FAILED_ASSERTION(eProperty); break; }
		}
	}

	void	Material::SetProperty(EMaterialProperty eProperty, const Color3& oColor)
	{
		FT_ASSERT(eProperty < E_COLOR_PROPERTIES_COUNT);
		switch (eProperty)
		{
		case E_DIFFUSE:		{ m_oDiffuseColor = oColor; break; }
		case E_AMBIENT:		{ m_oAmbientColor = oColor; break; }
		case E_SPECULAR:	{ m_oSpecularColor = oColor; break; }
		case E_EMISSIVE:	{ m_oEmissiveColor = oColor; break; }
		default:			{ FT_FAILED_ASSERTION(eProperty); break; }
		}
	}

	void	Material::SetProperty(EMaterialProperty eProperty, const SPtr<Texture>& xTexture)
	{
		FT_ASSERT(eProperty < E_TEXTURE_PROPERTIES_COUNT);
		switch (eProperty)
		{
		case E_DIFFUSE:		{ m_xDiffuseTexture = xTexture; break; }
		case E_AMBIENT:		{ m_xAmbientTexture = xTexture; break; }
		case E_SPECULAR:	{ m_xSpecularTexture = xTexture; break; }
		case E_EMISSIVE:	{ m_xEmissiveTexture = xTexture; break; }
		case E_NORMAL_MAP:	{ m_xEmissiveTexture = xTexture; break; }
		default:			{ FT_FAILED_ASSERTION(eProperty); break; }
		}
	}
}
