
#include "ResourceManager.hpp"

#include "Texture.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "SpecificResourceManager.hpp"

namespace ft
{
	ResourceManager::ResourceManager()
		: m_pTextureResourceManager(nullptr)
		, m_pShaderResourceManager(nullptr)
		, m_pShaderProgramResourceManager(nullptr)
	{
	}

	ResourceManager::~ResourceManager()
	{
		FT_TEST(Destroy() == FT_OK);
	}

	ErrorCode	ResourceManager::Create()
	{
		m_pTextureResourceManager = new TextureResourceManager(*this);
		m_pShaderResourceManager = new ShaderResourceManager(*this);
		m_pShaderProgramResourceManager = new ShaderProgramResourceManager(*this);

		return FT_OK;
	}

	ErrorCode	ResourceManager::Destroy()
	{
		if (m_pTextureResourceManager != nullptr)
		{
			FT_TEST(m_pTextureResourceManager->UnloadAll() == FT_OK);
			FT_DELETE(m_pTextureResourceManager);
		}

		if (m_pShaderResourceManager != nullptr)
		{
			FT_TEST(m_pShaderResourceManager->UnloadAll() == FT_OK);
			FT_DELETE(m_pShaderResourceManager);
		}

		if (m_pShaderProgramResourceManager != nullptr)
		{
			FT_TEST(m_pShaderProgramResourceManager->UnloadAll() == FT_OK);
			FT_DELETE(m_pShaderProgramResourceManager);
		}

		return FT_OK;
	}
}
