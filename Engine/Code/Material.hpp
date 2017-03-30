#pragma once

#include "StrongPointer.hpp"
#include "Color.hpp"

namespace ft
{
	// fw
	class Texture;
	class ShaderProgram;

	enum EMaterialProperty
	{
		E_DIFFUSE = 0,
		E_AMBIENT,
		E_SPECULAR,
		E_EMISSIVE,
		E_COLOR_PROPERTIES_COUNT,

		E_NORMAL_MAP = E_COLOR_PROPERTIES_COUNT,
		E_TEXTURE_PROPERTIES_COUNT,

		E_SHININESS = 0,
		E_SCALAR_PROPERTIES_COUNT
	};

	class Material : public CountableSPtr
	{
	public:

		Material();
		virtual ~Material();

		void	SetShader(const SPtr<ShaderProgram>& xShader);

		void	SetProperty(EMaterialProperty eProperty, float fValue);
		void	SetProperty(EMaterialProperty eProperty, const Color3& oColor);
		void	SetProperty(EMaterialProperty eProperty, const SPtr<Texture>& xTexture);

	private:

		SPtr<ShaderProgram>	m_xShader;

		Color3			m_oDiffuseColor;
		Color3			m_oAmbientColor;
		Color3			m_oSpecularColor;
		Color3			m_oEmissiveColor;
		FT_STATIC_ASSERT(E_COLOR_PROPERTIES_COUNT == 4);

		float			m_fShininess;
		FT_STATIC_ASSERT(E_SCALAR_PROPERTIES_COUNT == 1);

		SPtr<Texture>	m_xDiffuseTexture;
		SPtr<Texture>	m_xAmbientTexture;
		SPtr<Texture>	m_xSpecularTexture;
		SPtr<Texture>	m_xEmissiveTexture;
		SPtr<Texture>	m_xNormalsTexture;
		FT_STATIC_ASSERT(E_TEXTURE_PROPERTIES_COUNT == 5);
	};
}
