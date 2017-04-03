
#include "Renderer.hpp"
#include "Camera.hpp"
#include "ShaderType.hpp"
#include "ShaderProgram.hpp"
#include "ResourceManager.hpp"

namespace ft
{
	Renderer::Renderer()
		: m_xActive3DCamera(nullptr)
		, m_xTextureShaderProgram(nullptr)
		, m_xColorShaderProgram(nullptr)
		, m_xColorValueShaderProgram(nullptr)
		, m_xBlinnPhongShaderProgram(nullptr)
	{
	}

	Renderer::~Renderer()
	{
	}

	ErrorCode	Renderer::Create(ResourceManager& oResourceManager)
	{
		m_xTextureShaderProgram		= new ShaderProgram;
		m_xColorShaderProgram		= new ShaderProgram;
		m_xColorValueShaderProgram	= new ShaderProgram;
		m_xBlinnPhongShaderProgram	= new ShaderProgram;
		{
			ShaderProgramResourceInfos	oShaderProgramResInfos;
			SPtr<ShaderProgramResource>	xShaderProgramResource;

			oShaderProgramResInfos.iShaderTypesFlags = SHADER_TYPE_FLAG(E_VERTEX_SHADER) | SHADER_TYPE_FLAG(E_FRAGMENT_SHADER);

			oShaderProgramResInfos.sName = "TextureShader";
			FT_TEST(oShaderProgramResInfos.oVertexShaderFilePath.Set("./Engine/Assets/Shaders/PositionUV.vs.glsl"));
			FT_TEST(oShaderProgramResInfos.oFragmentShaderFilePath.Set("./Engine/Assets/Shaders/Texture.fs.glsl"));
			FT_TEST(oResourceManager.GetShaderProgramResourceManager()->Load(oShaderProgramResInfos, xShaderProgramResource) == FT_OK);
			FT_TEST(m_xTextureShaderProgram->Create(xShaderProgramResource) == FT_OK);

			oShaderProgramResInfos.sName = "ColorShader";
			FT_TEST(oShaderProgramResInfos.oVertexShaderFilePath.Set("./Engine/Assets/Shaders/PositionColor.vs.glsl"));
			FT_TEST(oShaderProgramResInfos.oFragmentShaderFilePath.Set("./Engine/Assets/Shaders/Color.fs.glsl"));
			FT_TEST(oResourceManager.GetShaderProgramResourceManager()->Load(oShaderProgramResInfos, xShaderProgramResource) == FT_OK);
			FT_TEST(m_xColorShaderProgram->Create(xShaderProgramResource) == FT_OK);

			oShaderProgramResInfos.sName = "ColorValueShader";
			FT_TEST(oShaderProgramResInfos.oVertexShaderFilePath.Set("./Engine/Assets/Shaders/Position.vs.glsl"));
			FT_TEST(oShaderProgramResInfos.oFragmentShaderFilePath.Set("./Engine/Assets/Shaders/ColorValue.fs.glsl"));
			FT_TEST(oResourceManager.GetShaderProgramResourceManager()->Load(oShaderProgramResInfos, xShaderProgramResource) == FT_OK);
			FT_TEST(m_xColorValueShaderProgram->Create(xShaderProgramResource) == FT_OK);

			oShaderProgramResInfos.sName = "BlinnPhongShader";
			FT_TEST(oShaderProgramResInfos.oVertexShaderFilePath.Set("./Engine/Assets/Shaders/BlinnPhong.vs.glsl"));
			FT_TEST(oShaderProgramResInfos.oFragmentShaderFilePath.Set("./Engine/Assets/Shaders/BlinnPhong.fs.glsl"));
			FT_TEST(oResourceManager.GetShaderProgramResourceManager()->Load(oShaderProgramResInfos, xShaderProgramResource) == FT_OK);
			FT_TEST(m_xBlinnPhongShaderProgram->Create(xShaderProgramResource) == FT_OK);
		}

		return FT_OK;
	}

	ErrorCode	Renderer::Destroy()
	{
		m_xActive3DCamera = nullptr;
		m_xTextureShaderProgram = nullptr;
		m_xColorShaderProgram = nullptr;
		m_xColorValueShaderProgram = nullptr;
		m_xBlinnPhongShaderProgram = nullptr;
		return FT_OK;
	}

	void	Renderer::RenderScene(const SceneNode* pSceneNode) const
	{
		FT_ASSERT(pSceneNode != nullptr);

		RenderContext	oRenderContext;

		if (m_xActive3DCamera != nullptr)
		{
			m_xActive3DCamera->MakeViewContext(&oRenderContext.oViewContext);

			for (SceneNodeConstIt it(pSceneNode); it != nullptr; ++it)
				it->Render(oRenderContext);
		}
	}

	void	Renderer::SetActive3DCamera(const SPtr<Camera>& xCamera)
	{
		if (xCamera != nullptr)
			m_xActive3DCamera = xCamera;
	}

	void	RenderContext::SendToShader(const SPtr<ShaderProgram>& xShaderProgram) const
	{
		FT_ASSERT(xShaderProgram != nullptr);

		xShaderProgram->SetUniform("mView", oViewContext.mView);
		xShaderProgram->SetUniform("mProjection", oViewContext.mProjection);
		xShaderProgram->SetUniform("vViewPosition", oViewContext.vViewPosition);
	}
}
