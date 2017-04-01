
#include "ResourceManager.hpp"

#include "MeshResource.hpp"
#include "ModelResource.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"

namespace ft
{
	ResourceManager::ResourceManager()
		: m_pMeshResourceManager(nullptr)
		, m_pModelResourceManager(nullptr)
		, m_pTextureResourceManager(nullptr)
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
		m_pMeshResourceManager			= new MeshResourceManager(*this);
		m_pModelResourceManager			= new ModelResourceManager(*this);
		m_pTextureResourceManager		= new TextureResourceManager(*this);
		m_pShaderResourceManager		= new ShaderResourceManager(*this);
		m_pShaderProgramResourceManager	= new ShaderProgramResourceManager(*this);

		return FT_OK;
	}

	ErrorCode	ResourceManager::Destroy()
	{
		if (m_pModelResourceManager != nullptr)
		{
			FT_TEST(m_pModelResourceManager->UnloadAll() == FT_OK);
			FT_DELETE(m_pModelResourceManager);
		}

		if (m_pMeshResourceManager != nullptr)
		{
			FT_TEST(m_pMeshResourceManager->UnloadAll() == FT_OK);
			FT_DELETE(m_pMeshResourceManager);
		}

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
