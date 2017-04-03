#pragma once

#include "StrongPointer.hpp"
#include "ErrorCode.hpp"
#include "Matrix44.hpp"
#include "Vector3.hpp"

namespace ft
{
	// fw
	class SceneNode;
	class Camera;
	class ShaderProgram;
	class ResourceManager;

	struct ViewContext
	{
		Vector3		vViewPosition;
		Matrix44	mView;
		Matrix44	mProjection;
	};

	struct RenderContext
	{
		ViewContext	oViewContext;

		void	SendToShader(const SPtr<ShaderProgram>& xShaderProgram) const;
	};

	class Renderer
	{
	public:

		Renderer();
		~Renderer();

		ErrorCode		Create(ResourceManager& oResourceManager);
		ErrorCode		Destroy();

		void			RenderScene(const SceneNode* pSceneNode) const;

		void			SetActive3DCamera(const SPtr<Camera>& xCamera);

	private:

		SPtr<Camera>	m_xActive3DCamera;

		// Pseudo-builtin shaders
		SPtr<ShaderProgram>	m_xTextureShaderProgram;
		SPtr<ShaderProgram>	m_xColorShaderProgram;
		SPtr<ShaderProgram>	m_xColorValueShaderProgram;
		SPtr<ShaderProgram>	m_xBlinnPhongShaderProgram;
	};
}
