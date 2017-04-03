
#include "ResourceManager.hpp"

#include "MeshResource.hpp"
#include "ModelResource.hpp"
#include "MaterialResource.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"

namespace ft
{
	ResourceManager::ResourceManager()
		: m_pShaderResourceManager(nullptr)
		, m_pTextureResourceManager(nullptr)
		, m_pShaderProgramResourceManager(nullptr)
		, m_pMaterialResourceManager(nullptr)
		, m_pMeshResourceManager(nullptr)
		, m_pModelResourceManager(nullptr)
	{
	}

	ResourceManager::~ResourceManager()
	{
		FT_TEST(Destroy() == FT_OK);
	}

	ErrorCode	ResourceManager::Create()
	{
		m_pShaderResourceManager		= new ShaderResourceManager(*this);
		m_pTextureResourceManager		= new TextureResourceManager(*this);
		m_pShaderProgramResourceManager	= new ShaderProgramResourceManager(*this);
		m_pMaterialResourceManager		= new MaterialResourceManager(*this);
		m_pMeshResourceManager			= new MeshResourceManager(*this);
		m_pModelResourceManager			= new ModelResourceManager(*this);

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

		if (m_pMaterialResourceManager != nullptr)
		{
			FT_TEST(m_pMaterialResourceManager->UnloadAll() == FT_OK);
			FT_DELETE(m_pMaterialResourceManager);
		}

		if (m_pShaderProgramResourceManager != nullptr)
		{
			FT_TEST(m_pShaderProgramResourceManager->UnloadAll() == FT_OK);
			FT_DELETE(m_pShaderProgramResourceManager);
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

		return FT_OK;
	}

	ErrorCode	ResourceManager::LoadDefaultResources()
	{
		TextureResourceInfos oTextureResourceInfos;
		oTextureResourceInfos.eTextureTarget = E_TEXTURE_2D;
		FT_TEST(oTextureResourceInfos.oFilePath.Set("./Engine/Assets/Textures/Default.png"));
		FT_TEST_RETURN(m_pTextureResourceManager->SetAsDefault(oTextureResourceInfos) == FT_OK, FT_FAIL);

		ShaderProgramResourceInfos oShaderProgramResourceInfos;
		oShaderProgramResourceInfos.sName = ENGINE_DEFAULT_RESOURCE_NAME;
		oShaderProgramResourceInfos.iShaderTypesFlags = SHADER_TYPE_FLAG(E_VERTEX_SHADER) | SHADER_TYPE_FLAG(E_FRAGMENT_SHADER);
		FT_TEST(oShaderProgramResourceInfos.oVertexShaderFilePath.Set("./Engine/Assets/Shaders/Position.vs.glsl"));
		FT_TEST(oShaderProgramResourceInfos.oFragmentShaderFilePath.Set("./Engine/Assets/Shaders/HardMagenta.fs.glsl"));
		FT_TEST_RETURN(m_pShaderProgramResourceManager->SetAsDefault(oShaderProgramResourceInfos) == FT_OK, FT_FAIL);

		MaterialResourceInfos oMaterialResourceInfos;
		oMaterialResourceInfos.sName = ENGINE_DEFAULT_RESOURCE_NAME;
		oMaterialResourceInfos.eSource = E_DEFAULT_MATERIAL;
		FT_TEST_RETURN(m_pMaterialResourceManager->SetAsDefault(oMaterialResourceInfos) == FT_OK, FT_FAIL);

		return FT_OK;
	}
}
