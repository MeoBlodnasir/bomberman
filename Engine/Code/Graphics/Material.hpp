#pragma once

#include "Graphics/MaterialResource.hpp"

namespace ft
{
	// fw
	class ShaderProgram;
	class Texture;

	struct MaterialProperty
	{
		EMaterialPropertyType	eType;
		float32					fValue;
		Color3					oColor;
		SPtr<Texture>			xTexture;

		MaterialProperty()
			: eType(E_SCALAR)
			, fValue(0.f)
			, oColor(0.f, 0.f, 0.f)
			, xTexture(nullptr)
		{}

		ErrorCode	CreateFrom(const MaterialResourceProperty& oResource);
	};
	typedef std::map<EMaterialProperty, MaterialProperty>	MaterialPropertiesMap;

	class Material
	{
	public:

		Material();
		Material(const Material& oMaterial);
		~Material();

		Material&	operator = (const Material& oMaterial);

		ErrorCode	Create(const SPtr<MaterialResource>& xResource);
		ErrorCode	Destroy();

		void	SetProperty(EMaterialProperty eProperty, float fValue);
		void	SetProperty(EMaterialProperty eProperty, const Color3& oColor);
		void	SetProperty(EMaterialProperty eProperty, const SPtr<Texture>& xTexture);

		void	SendToShader(const SPtr<ShaderProgram>& xShaderProgram) const;

	private:

		SPtr<MaterialResource>	m_xResource;
		MaterialPropertiesMap	m_oProperties;
	};
}
