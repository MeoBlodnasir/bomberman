#pragma once

#include "Core.hpp"
#include "ErrorCode.hpp"

namespace ft
{
	// fw
	template <typename TResourceType> class SpecificResourceManager;
	class TextureResource;
	class ShaderResource;
	class ShaderProgramResource;

	// Classe générale qui gère tous les SpecificResourceManager

	typedef SpecificResourceManager<TextureResource>		TextureResourceManager;
	typedef SpecificResourceManager<ShaderResource>			ShaderResourceManager;
	typedef SpecificResourceManager<ShaderProgramResource>	ShaderProgramResourceManager;

	class ResourceManager
	{
	public:

		ResourceManager();
		~ResourceManager();

		ErrorCode	Create();
		ErrorCode	Destroy();

		TextureResourceManager*			GetTextureResourceManager() const		{ FT_ASSERT(m_pTextureResourceManager != nullptr); return m_pTextureResourceManager; }
		ShaderResourceManager*			GetShaderResourceManager() const		{ FT_ASSERT(m_pShaderResourceManager != nullptr); return m_pShaderResourceManager; }
		ShaderProgramResourceManager*	GetShaderProgramResourceManager() const	{ FT_ASSERT(m_pShaderProgramResourceManager != nullptr); return m_pShaderProgramResourceManager; }

	private:

		TextureResourceManager*			m_pTextureResourceManager;
		ShaderResourceManager*			m_pShaderResourceManager;
		ShaderProgramResourceManager*	m_pShaderProgramResourceManager;

		ResourceManager(const ResourceManager&) FT_DELETED;
		ResourceManager& operator = (const ResourceManager&) FT_DELETED;
	};
}
