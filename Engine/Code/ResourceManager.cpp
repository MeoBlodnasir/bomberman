
#include "ResourceManager.hpp"
#include "TextureResourceManager.hpp"
#include "ShaderResourceManager.hpp"

namespace ft
{
	ResourceManager::ResourceManager()
		: m_pTextureResManager(nullptr)
		, m_pShaderFileResManager(nullptr)
		, m_pShaderResManager(nullptr)
	{
	}

	ResourceManager::~ResourceManager()
	{
		FT_TEST(Destroy() == FT_OK);
	}

	ErrorCode	ResourceManager::Create()
	{
		m_pTextureResManager	= new TextureResourceManager(*this);
		m_pShaderFileResManager	= new ShaderFileResourceManager(*this);
		m_pShaderResManager		= new ShaderResourceManager(*this);

		return FT_OK;
	}

	ErrorCode	ResourceManager::Destroy()
	{
		if (m_pTextureResManager != nullptr)
		{
			FT_TEST(m_pTextureResManager->UnloadAll() == FT_OK);
			FT_DELETE(m_pTextureResManager);
		}

		if (m_pShaderFileResManager != nullptr)
		{
			FT_TEST(m_pShaderFileResManager->UnloadAll() == FT_OK);
			FT_DELETE(m_pShaderFileResManager);
		}

		if (m_pShaderResManager != nullptr)
		{
			FT_TEST(m_pShaderResManager->UnloadAll() == FT_OK);
			FT_DELETE(m_pShaderResManager);
		}

		return FT_OK;
	}
}
