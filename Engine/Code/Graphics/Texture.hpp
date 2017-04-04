#pragma once

#include "Core/Handled.hpp"
#include "Core/Path.hpp"
#include "Resource/Resource.hpp"
#include "Math/Vector2.hpp"

namespace ft
{
	// fw
	class TextureResource;

	enum ETextureTarget
	{
		E_TEXTURE_INVALID = -1,
		E_TEXTURE_2D = 0,
	};

	class Texture : public CountableSPtr
	{
	public:

		Texture();
		virtual ~Texture();

		ErrorCode		Create(const SPtr<TextureResource>& xResource);
		ErrorCode		Destroy();

		const Vector2&	GetSize() const;

		void			Bind(uint32 iTextureUnit) const;

	protected:

		SPtr<TextureResource>	m_xResource;

	private:

		Texture(const Texture&) FT_DELETED;
		Texture& operator = (const Texture&) FT_DELETED;
	};


	// fw
	template <typename TResourceType> class SpecificResourceManager;

	struct TextureResourceInfos
	{
		Path			oFilePath;
		ETextureTarget	eTextureTarget;

		TextureResourceInfos()
			: eTextureTarget(E_TEXTURE_INVALID)
		{}
	};

	class TextureResource : public Handled, public Resource<TextureResourceInfos>
	{
	public:

		virtual ~TextureResource();

		virtual bool				IsLoadedAndValid() const override;

		inline	const Path&			GetFilePath() const			{ return m_oResourceInfos.oFilePath; }
		inline	ETextureTarget		GetTextureTarget() const	{ return m_oResourceInfos.eTextureTarget; }
		inline	const Vector2&		GetSize() const				{ return m_vImageSize; }

		static	uint32				GLTextureTarget(ETextureTarget eTextureTarget);

	protected:

		TextureResource();

		TextureResourceInfos	m_oResourceInfos;
		Vector2					m_vImageSize;

	private:
		friend SpecificResourceManager<TextureResource>;

		virtual ErrorCode	Load(ResourceManager& oResourceManager, const TextureResourceInfos& oInfos) override;
		virtual ErrorCode	Unload() override;
	};
}

// Nécessaire pour SpecificResourceManager
namespace std
{
	template <> struct hash<ft::TextureResourceInfos>
	{
		size_t	operator ()	(const ft::TextureResourceInfos& oObj) const
		{
			return hash<std::string>()(oObj.oFilePath.GetFullPath());
		}
	};
}
