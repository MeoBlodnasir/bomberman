#pragma once

#include "Core.hpp"
#include "ErrorCode.hpp"

namespace ft
{
	// fw
	class TextureResourceManager;
	class ShaderFileResourceManager;
	class ShaderResourceManager;

	// Classe générale qui gère tous les SpecificResourceManager
	class ResourceManager
	{
	public:

		ResourceManager();
		~ResourceManager();

		ErrorCode					Create();
		ErrorCode					Destroy();

		TextureResourceManager*		GetTextureResManager() const		{ FT_ASSERT(m_pTextureResManager != nullptr);		return m_pTextureResManager; }
		ShaderFileResourceManager*	GetShaderFileResManager() const		{ FT_ASSERT(m_pShaderFileResManager != nullptr);	return m_pShaderFileResManager; }
		ShaderResourceManager*		GetShaderResManager() const			{ FT_ASSERT(m_pShaderResManager != nullptr);		return m_pShaderResManager; }

	private:

		TextureResourceManager*		m_pTextureResManager;
		ShaderFileResourceManager*	m_pShaderFileResManager;
		ShaderResourceManager*		m_pShaderResManager;

		ResourceManager(const ResourceManager&) FT_DELETED;
		ResourceManager& operator = (const ResourceManager&) FT_DELETED;
	};
}
