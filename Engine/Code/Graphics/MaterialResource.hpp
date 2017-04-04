#pragma once

#include "Core/StrongPointer.hpp"
#include "Core/Path.hpp"
#include "Resource/Resource.hpp"
#include "Graphics/Color.hpp"
#include "Graphics/Texture.hpp"

#include <map>

// fw
struct aiMaterial;

namespace ft
{
	// fw
	template <typename TResourceType> class SpecificResourceManager;

	enum EMaterialProperty
	{
		E_DIFFUSE = 0,
		E_AMBIENT,
		E_SPECULAR,
		E_SHININESS,
		E_EMISSIVE,
		E_NORMALS,
	};

	enum EMaterialPropertyType
	{
		E_SCALAR = 0,
		E_COLOR3,
		E_TEXTURE
	};

	struct MaterialResourceProperty
	{
		EMaterialPropertyType	eType;
		float32					fValue;
		Color3					oColor;
		SPtr<TextureResource>	xTextureResource;

		MaterialResourceProperty()
			: eType(E_SCALAR)
			, fValue(0.f)
			, oColor(0.f, 0.f, 0.f)
			, xTextureResource(nullptr)
		{}
	};
	typedef std::map<EMaterialProperty, MaterialResourceProperty>	MaterialResourcePropertiesMap;

	enum EMaterialResourceSource
	{
		E_DEFAULT_MATERIAL = 0x00010000,
		E_ASSIMP_MATERIAL = 0x00020000,
	};

	struct MaterialResourceInfos
	{
		std::string					sName;
		EMaterialResourceSource		eSource;
		union
		{
			struct
			{
				Path				oLocalPath;
				const aiMaterial*	pAiMaterial;
			};
		};
	};

	class MaterialResource : public Resource<MaterialResourceInfos>
	{
	public:

		virtual ~MaterialResource();

		virtual bool	IsLoadedAndValid() const override;
		
	protected:
		friend class Material;

		MaterialResourceInfos			m_oResourceInfos;
		MaterialResourcePropertiesMap	m_oProperties;

		MaterialResource();

		void			AddProperty(const MaterialResourceProperty& oProperty);
		ErrorCode		LoadAssimpMaterial(ResourceManager& oResourceManager, const MaterialResourceInfos& oInfos);
		ErrorCode		LoadDefaultMaterial();

	private:
		friend SpecificResourceManager<MaterialResource>;

		virtual ErrorCode	Load(ResourceManager& oResourceManager, const MaterialResourceInfos& oInfos) override;
		virtual ErrorCode	Unload() override;
	};
}

// Nécessaire pour SpecificResourceManager
namespace std
{
	template <> struct hash<ft::MaterialResourceInfos>
	{
		size_t	operator ()	(const ft::MaterialResourceInfos& oObj) const
		{
			return hash<std::string>()(oObj.sName);
		}
	};
}
