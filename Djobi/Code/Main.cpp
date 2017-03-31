
#if defined(_WIN32)
#	include <crtdbg.h>
#endif

#include <Output.hpp>
#include <OpenGL.hpp>
#include <ProfilerBlock.hpp>
#include <Mesh.hpp>
#include <Model.hpp>
#include <Shader.hpp>
#include <ShaderProgram.hpp>
#include <Texture.hpp>
#include <Color.hpp>
#include <Camera.hpp>
#include <Quaternion.hpp>
#include <ResourceManager.hpp>
#include <TextureResourceManager.hpp>
#include <ShaderResourceManager.hpp>

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>

#include <glm/gtx/matrix_decompose.hpp>

int		main()
{
#if defined(_WIN32)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//long	iBreakAlloc = -1;
	//_CrtSetBreakAlloc(iBreakAlloc);
#endif

	using namespace ft;

	FT_COUT << "Bienvenu sur DJOBI!" << std::endl;

	ResourceManager	oResourceManager;
	FT_TEST(oResourceManager.Create() == FT_OK);

	sf::Window* pWindow = nullptr;
	{
		ProfilerBlockPrint oProfilerBlock("Fenetre SFML");
		sf::ContextSettings oOpenGLContext;
		oOpenGLContext.depthBits		 = 24;
		oOpenGLContext.stencilBits		 = 8;
		oOpenGLContext.antialiasingLevel = 4;
		oOpenGLContext.majorVersion		 = 4;
		oOpenGLContext.minorVersion		 = 1;
		oOpenGLContext.attributeFlags	 = sf::ContextSettings::Core;
		pWindow = new sf::Window(sf::VideoMode(1080, 720), "Bomberman", sf::Style::Default, oOpenGLContext);
	}
	{
		pWindow->setFramerateLimit(60);

		// Après l'initialisation d'un contexte OpenGL
		{
			ProfilerBlockPrint oProfilerBlock("Init GLEW");
			glewExperimental = GL_TRUE; // core profile
			const GLenum eGlewError = glewInit();
			if (eGlewError != GLEW_OK)
			{
				FT_CERR << "Erreur dans l'initialisation de GLEW: " << glewGetErrorString(eGlewError) << std::endl;
				return -1;
			}
		}

		glViewport(0, 0, (GLsizei)pWindow->getSize().x, (GLsizei)pWindow->getSize().y);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.05f, 0.05f, 0.1f, 1.f);

		sf::Clock oSFClock;
		oSFClock.restart();
		float fDt;

		
		SPtr<Model>	xBombermanModel = new Model;
		{
			ProfilerBlockPrint oProfilerBlock("Chargement Bomberman.FBX");

			SPtr<ModelResource> xBombermanResource = new ModelResource;
			Model::Desc oBombermanDesc;
			FT_TEST(xBombermanResource->LoadFromFile(Path("./Djobi/Assets/Models/Bomberman.FBX")) == FT_OK);
			oBombermanDesc.pParent = nullptr;
			FT_TEST(xBombermanModel->Create(&oBombermanDesc, xBombermanResource) == FT_OK);
		}
		

		SPtr<Model>	xCubeSphereModel = new Model;
		{
			ProfilerBlockPrint oProfilerBlock("Chargement CubeSphere.FBX");

			SPtr<ModelResource> xResource = new ModelResource;
			Model::Desc oDesc;
			FT_TEST(xResource->LoadFromFile(Path("./Djobi/Assets/Models/CubeSphere.FBX")) == FT_OK);
			oDesc.pParent = nullptr;
			FT_TEST(xCubeSphereModel->Create(&oDesc, xResource) == FT_OK);
		}
		

		SPtr<Mesh> xAxisMesh = new Mesh;
		{
			SPtr<MeshData> xData = new MeshData;
			FT_TEST(xData->MakePrimitiveMatrixAxis(E_VERTEX_PROP_POSITION | E_VERTEX_PROP_COLOR) == FT_OK);
			FT_TEST(xAxisMesh->Create(xData) == FT_OK);
		}


		SPtr<Camera>	xCamera = new Camera;
		Camera::Desc	oCameraDesc;
		ViewContext		oViewContext;
		oCameraDesc.eProjectionType = E_PROJECTION_PERSPECTIVE;
		oCameraDesc.fFov = glm::radians(60.f);
		oCameraDesc.fNear = 0.1f;
		oCameraDesc.fFar = 100.f;
		oCameraDesc.fWidth = (float)pWindow->getSize().x;
		oCameraDesc.fRatio = oCameraDesc.fWidth / (float)pWindow->getSize().y;
		FT_TEST(xCamera->Create(&oCameraDesc) == FT_OK);


		ShaderResourceInfos	oShaderResInfos;
		SPtr<ShaderProgram> xTextureShader		= new ShaderProgram;
		SPtr<ShaderProgram> xColorShader		= new ShaderProgram;
		SPtr<ShaderProgram> xColorValueShader	= new ShaderProgram;
		SPtr<ShaderProgram> xBlinnPhongShader	= new ShaderProgram;
		{
			ProfilerBlockPrint oProfilerBlock("Chargement Shaders");

			SPtr<ShaderResource>	xShaderResource;

			oShaderResInfos.iShaderTypesFlags = SHADER_TYPE_FLAG(E_VERTEX_SHADER) | SHADER_TYPE_FLAG(E_FRAGMENT_SHADER);
			FT_TEST(oShaderResInfos.oVertexShaderFilePath.Set("./Engine/Assets/Shaders/PositionUV.vs.glsl"));
			FT_TEST(oShaderResInfos.oFragmentShaderFilePath.Set("./Engine/Assets/Shaders/Texture.fs.glsl"));
			FT_TEST(oResourceManager.GetShaderResManager()->Load(oShaderResInfos, xShaderResource) == FT_OK);
			FT_TEST(xTextureShader->Create(xShaderResource) == FT_OK);

			oShaderResInfos.iShaderTypesFlags = SHADER_TYPE_FLAG(E_VERTEX_SHADER) | SHADER_TYPE_FLAG(E_FRAGMENT_SHADER);
			FT_TEST(oShaderResInfos.oVertexShaderFilePath.Set("./Engine/Assets/Shaders/PositionColor.vs.glsl"));
			FT_TEST(oShaderResInfos.oFragmentShaderFilePath.Set("./Engine/Assets/Shaders/Color.fs.glsl"));
			FT_TEST(oResourceManager.GetShaderResManager()->Load(oShaderResInfos, xShaderResource) == FT_OK);
			FT_TEST(xColorShader->Create(xShaderResource) == FT_OK);

			oShaderResInfos.iShaderTypesFlags = SHADER_TYPE_FLAG(E_VERTEX_SHADER) | SHADER_TYPE_FLAG(E_FRAGMENT_SHADER);
			FT_TEST(oShaderResInfos.oVertexShaderFilePath.Set("./Engine/Assets/Shaders/Position.vs.glsl"));
			FT_TEST(oShaderResInfos.oFragmentShaderFilePath.Set("./Engine/Assets/Shaders/ColorValue.fs.glsl"));
			FT_TEST(oResourceManager.GetShaderResManager()->Load(oShaderResInfos, xShaderResource) == FT_OK);
			FT_TEST(xColorValueShader->Create(xShaderResource) == FT_OK);

			oShaderResInfos.iShaderTypesFlags = SHADER_TYPE_FLAG(E_VERTEX_SHADER) | SHADER_TYPE_FLAG(E_FRAGMENT_SHADER);
			FT_TEST(oShaderResInfos.oVertexShaderFilePath.Set("./Engine/Assets/Shaders/BlinnPhong.vs.glsl"));
			FT_TEST(oShaderResInfos.oFragmentShaderFilePath.Set("./Engine/Assets/Shaders/BlinnPhong.fs.glsl"));
			FT_TEST(oResourceManager.GetShaderResManager()->Load(oShaderResInfos, xShaderResource) == FT_OK);
			FT_TEST(xBlinnPhongShader->Create(xShaderResource) == FT_OK);
		}
		
		SPtr<Texture> xCubeSphereDiffuse	= new Texture;
		SPtr<Texture> xCubeSphereAmbient	= new Texture;
		SPtr<Texture> xCubeSphereSpecular	= new Texture;
		SPtr<Texture> xCubeSphereEmissive	= new Texture;
		{
			ProfilerBlockPrint oProfilerBlock("Chargement Textures");

			SPtr<TextureResource>	xTextureResource;
			TextureResourceInfos oTextureResInfos;

			oTextureResInfos.oFilePath = Path("./Djobi/Assets/Textures/CubeSphere_d.png");
			oTextureResInfos.iTextureTarget = GL_TEXTURE_2D;
			FT_TEST(oResourceManager.GetTextureResManager()->Load(oTextureResInfos, xTextureResource) == FT_OK);
			FT_TEST(xCubeSphereDiffuse->Create(xTextureResource) == FT_OK);

			oTextureResInfos.oFilePath = Path("./Djobi/Assets/Textures/CubeSphere_am.png");
			oTextureResInfos.iTextureTarget = GL_TEXTURE_2D;
			FT_TEST(oResourceManager.GetTextureResManager()->Load(oTextureResInfos, xTextureResource) == FT_OK);
			FT_TEST(xCubeSphereAmbient->Create(xTextureResource) == FT_OK);

			oTextureResInfos.oFilePath = Path("./Djobi/Assets/Textures/CubeSphere_s.png");
			oTextureResInfos.iTextureTarget = GL_TEXTURE_2D;
			FT_TEST(oResourceManager.GetTextureResManager()->Load(oTextureResInfos, xTextureResource) == FT_OK);
			FT_TEST(xCubeSphereSpecular->Create(xTextureResource) == FT_OK);

			oTextureResInfos.oFilePath = Path("./Djobi/Assets/Textures/CubeSphere_e.png");
			oTextureResInfos.iTextureTarget = GL_TEXTURE_2D;
			FT_TEST(oResourceManager.GetTextureResManager()->Load(oTextureResInfos, xTextureResource) == FT_OK);
			FT_TEST(xCubeSphereEmissive->Create(xTextureResource) == FT_OK);
		}

		// Commentaire vu sur un site d'aide, à garder en tête:
		// Left-multiplying rot, will perform a rotation about the camera space X axis.
		// If you want a rotation about the world-space X axis, you need to right-multiply it.

		sf::Vector2i	vSfMousePosition = sf::Mouse::getPosition(*pWindow);
		Vector2			vMousePosition((float)vSfMousePosition.x, (float)vSfMousePosition.y);
		Vector2			vMouseMotion(0.f, 0.f);

		Vector3			vCamControllerPos = Vector3(3.f, 1.f, 1.f);
		Quaternion		qCamControllerRot = Quaternion(glm::inverse(glm::lookAt(vCamControllerPos, Vector3(0.f, 0.f, 0.5f), Vector3(0.f, 0.f, 1.f))));
		const float		fTranslationSpeed = 3.f;
		const float		fRotationSpeed = 0.3f;

		bool bRunning = true;
		while (bRunning)
		{
			//ProfilerBlockPrint oProfilerBlockLoop("Boucle de jeu");

			sf::Time oDeltaTime = oSFClock.restart();
			fDt = oDeltaTime.asSeconds();

			sf::Event oEvent;
			while (pWindow->pollEvent(oEvent))
			{
				if (oEvent.type == sf::Event::Closed)
				{
					bRunning = false;
				}
				else if (oEvent.type == sf::Event::KeyPressed)
				{
					if (oEvent.key.code == sf::Keyboard::Escape)
					{
						bRunning = false;
					}
				}
			}

			vSfMousePosition = sf::Mouse::getPosition(*pWindow);
			vMouseMotion = Vector2((float)vSfMousePosition.x, (float)vSfMousePosition.y) - vMousePosition;
			vMousePosition.x = (float)vSfMousePosition.x;
			vMousePosition.y = (float)vSfMousePosition.y;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				float fTranslationMotion = (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ? -fTranslationSpeed : fTranslationSpeed) * fDt;
				vCamControllerPos += qCamControllerRot * Vector3(0.f, 0.f, fTranslationMotion);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				float fTranslationMotion = (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ? -fTranslationSpeed : fTranslationSpeed) * fDt;
				vCamControllerPos += qCamControllerRot * Vector3(fTranslationMotion, 0.f, 0.f);
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				if (vMouseMotion.y * vMouseMotion.y > 0.1f)
					qCamControllerRot = glm::rotate(qCamControllerRot, -vMouseMotion.y * fDt * fRotationSpeed, Vector3(1.f, 0.f, 0.f));
				if (vMouseMotion.x * vMouseMotion.x > 0.1f)
					qCamControllerRot = glm::rotate(qCamControllerRot, -vMouseMotion.x * fDt * fRotationSpeed, Vector3(0.f, 1.f, 0.f));
			}

			//xCamera->mWorldTransform = glm::translate(Matrix44(1.f), vCamControllerPos) * glm::mat4_cast(qCamControllerRot);
			xCamera->mWorldTransform = glm::inverse(glm::lookAt(vCamControllerPos, vCamControllerPos + (qCamControllerRot * Vector3(0.f, 0.f, -1.f)), Vector3(0.f, 0.f, 1.f)));

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			{
				xCubeSphereModel->SetLocalTransform(glm::translate(xCubeSphereModel->GetLocalTransform(),
					Vector3(sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? fTranslationSpeed * fDt : 0.f,
							sf::Keyboard::isKeyPressed(sf::Keyboard::Z) ? fTranslationSpeed * fDt : 0.f,
							sf::Keyboard::isKeyPressed(sf::Keyboard::E) ? fTranslationSpeed * fDt : 0.f)));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				xCubeSphereModel->SetLocalTransform(glm::translate(xCubeSphereModel->GetLocalTransform(),
					Vector3(sf::Keyboard::isKeyPressed(sf::Keyboard::Q) ? -fTranslationSpeed * fDt : 0.f,
							sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? -fTranslationSpeed * fDt : 0.f,
							sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? -fTranslationSpeed * fDt : 0.f)));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) || sf::Keyboard::isKeyPressed(sf::Keyboard::F))
			{
				xCubeSphereModel->SetLocalTransform(glm::rotate(xCubeSphereModel->GetLocalTransform(),
												   (sf::Keyboard::isKeyPressed(sf::Keyboard::R) ? fRotationSpeed : -fRotationSpeed) * 3.f * fDt,
												   Vector3(1.f, 0.f, 0.f)));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::T) || sf::Keyboard::isKeyPressed(sf::Keyboard::G))
			{
				xCubeSphereModel->SetLocalTransform(glm::rotate(xCubeSphereModel->GetLocalTransform(),
												  (sf::Keyboard::isKeyPressed(sf::Keyboard::T) ? fRotationSpeed : -fRotationSpeed) * 3.f * fDt,
												  Vector3(0.f, 1.f, 0.f)));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y) || sf::Keyboard::isKeyPressed(sf::Keyboard::H))
			{
				xCubeSphereModel->SetLocalTransform(glm::rotate(xCubeSphereModel->GetLocalTransform(),
												   (sf::Keyboard::isKeyPressed(sf::Keyboard::Y) ? fRotationSpeed : -fRotationSpeed) * 3.f * fDt,
												   Vector3(0.f, 0.f, 1.f)));
			}
			xCubeSphereModel->Update();

			{
				//ProfilerBlockPrint oProfilerBlockRender("Boucle de rendu");

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);

				xCamera->MakeViewContext(&oViewContext);

				xBlinnPhongShader->Use();
				xBlinnPhongShader->SetUniform("mView", oViewContext.mView);
				xBlinnPhongShader->SetUniform("mProjection", oViewContext.mProjection);

				xBlinnPhongShader->SetUniform("oMaterial.oDiffuseTexture",		xCubeSphereDiffuse, 0);
				xBlinnPhongShader->SetUniform("oMaterial.bHasDiffuseTexture",	GL_TRUE);
				xBlinnPhongShader->SetUniform("oMaterial.oSpecularTexture",		xCubeSphereSpecular, 1);
				xBlinnPhongShader->SetUniform("oMaterial.bHasSpecularTexture",	GL_TRUE);
				xBlinnPhongShader->SetUniform("oMaterial.oEmissiveTexture",		xCubeSphereEmissive, 2);
				xBlinnPhongShader->SetUniform("oMaterial.bHasEmissiveTexture",	GL_TRUE);
				xBlinnPhongShader->SetUniform("oMaterial.fAmbient",				0.2f);
				xBlinnPhongShader->SetUniform("oMaterial.fShininess",			32.f);

				xBlinnPhongShader->SetUniform("vViewPosition",	Vector3(xCamera->mWorldTransform[3]));

				
				ModelNode::const_iterator itNode(xCubeSphereModel->m_xRootNode);
				while (*itNode != nullptr)
				{
					for (const Mesh* pMesh : itNode->m_oMeshes)
					{
						xTextureShader->SetUniform("mModel", xCubeSphereModel->GetWorldTransform() * itNode->GetWorldTransform());
						pMesh->Draw();
					}
					itNode.Next();
				}
				
				/*
				ModelNode::const_iterator itNode(xBombermanModel->m_xRootNode);
				while (*itNode != nullptr)
				{
					for (const Mesh* pMesh : itNode->m_oMeshes)
					{
						xTextureShader->SetUniform("mModel", xBombermanModel->GetWorldTransform() * itNode->GetWorldTransform());
						pMesh->Draw();
					}
					itNode.Next();
				}
				*/
				

				glDisable(GL_DEPTH_TEST);
				xColorShader->Use();
				xColorShader->SetUniform("mModel", Matrix44(1));
				xColorShader->SetUniform("mView", oViewContext.mView);
				xColorShader->SetUniform("mProjection", oViewContext.mProjection);
				xAxisMesh->Draw();
			}

			{
				//ProfilerBlockPrint oProfilerBlockFlip("Flip");
				pWindow->display();
			}
		}
	}

	FT_TEST(oResourceManager.Destroy() == FT_OK);

	delete pWindow;

	return 0;
}
