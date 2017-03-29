
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

int		main()
{
#if defined(_WIN32)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//long	iBreakAlloc = -1;
	//_CrtSetBreakAlloc(iBreakAlloc);
#endif

	using namespace ft;

	FT_COUT << "Bienvenu sur DJOBI!" << std::endl;

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
		glClearColor(0.05f, 0.05f, 0.1f, 1.0f);

		sf::Clock oSFClock;
		oSFClock.restart();
		float fDt;


		ResourceManager	oResourceManager;
		FT_TEST(oResourceManager.Create() == FT_OK);

		SPtr<Model>	xBombermanModel = new Model;
		{
			ProfilerBlockPrint oProfilerBlock("Chargement Bomberman.FBX");

			SPtr<ModelResource> xBombermanResource = new ModelResource;
			Model::Desc oBombermanDesc;
			FT_TEST(xBombermanResource->LoadFromFile(Path("./Djobi/Assets/Models/Bomberman.FBX")) == FT_OK);
			oBombermanDesc.pParent = nullptr;
			FT_TEST(xBombermanModel->Create(&oBombermanDesc, xBombermanResource) == FT_OK);
		}

		SPtr<MeshResource>	xAxisResource = new MeshResource;
		FT_TEST(xAxisResource->MakePrimitiveMatrixAxis(E_VERTEX_PROP_POSITION | E_VERTEX_PROP_COLOR) == FT_OK);
		SPtr<Mesh>			xAxisMesh = new Mesh;
		FT_TEST(xAxisMesh->Create(xAxisResource) == FT_OK);

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
		SPtr<ShaderProgram> xTextureShader	= new ShaderProgram;
		SPtr<ShaderProgram> xColorShader	= new ShaderProgram;
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
		}
		
		TextureResourceInfos oTextureResInfos;
		SPtr<Texture>	xTextureTest	= new Texture;
		SPtr<Texture>	xTexturePurple	= new Texture;
		{
			ProfilerBlockPrint oProfilerBlock("Chargement Textures");

			SPtr<TextureResource>	xTextureResource;
			oTextureResInfos.oFilePath = Path("./Djobi/Assets/Textures/Purple.png");
			oTextureResInfos.iTextureTarget = GL_TEXTURE_2D;
			FT_TEST(oResourceManager.GetTextureResManager()->Load(oTextureResInfos, xTextureResource) == FT_OK);
			FT_TEST(xTextureTest->Create(xTextureResource) == FT_OK);

			oTextureResInfos.oFilePath = Path("./Djobi/Assets/Textures/Purple.png");
			oTextureResInfos.iTextureTarget = GL_TEXTURE_2D;
			FT_TEST(oResourceManager.GetTextureResManager()->Load(oTextureResInfos, xTextureResource) == FT_OK);
			FT_TEST(xTexturePurple->Create(xTextureResource) == FT_OK);
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
					qCamControllerRot = glm::rotate(qCamControllerRot, vMouseMotion.y * fDt * fRotationSpeed, Vector3(1.f, 0.f, 0.f));
				if (vMouseMotion.x * vMouseMotion.x > 0.1f)
					qCamControllerRot = glm::rotate(qCamControllerRot, vMouseMotion.x * fDt * fRotationSpeed, Vector3(0.f, 1.f, 0.f));
			}

			//xCamera->mWorldTransform = glm::translate(Matrix44(1.f), vCamControllerPos) * glm::mat4_cast(qCamControllerRot);
			xCamera->mWorldTransform = glm::inverse(glm::lookAt(vCamControllerPos, vCamControllerPos + (qCamControllerRot * Vector3(0.f, 0.f, -1.f)), Vector3(0.f, 0.f, 1.f)));

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			{
				xBombermanModel->SetLocalTransform(glm::translate(xBombermanModel->GetLocalTransform(),
					Vector3(sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? fTranslationSpeed * fDt : 0.f,
							sf::Keyboard::isKeyPressed(sf::Keyboard::Z) ? fTranslationSpeed * fDt : 0.f,
							sf::Keyboard::isKeyPressed(sf::Keyboard::E) ? fTranslationSpeed * fDt : 0.f)));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				xBombermanModel->SetLocalTransform(glm::translate(xBombermanModel->GetLocalTransform(),
					Vector3(sf::Keyboard::isKeyPressed(sf::Keyboard::Q) ? -fTranslationSpeed * fDt : 0.f,
							sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? -fTranslationSpeed * fDt : 0.f,
							sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? -fTranslationSpeed * fDt : 0.f)));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) || sf::Keyboard::isKeyPressed(sf::Keyboard::F))
			{
				xBombermanModel->SetLocalTransform(glm::rotate(xBombermanModel->GetLocalTransform(),
												   (sf::Keyboard::isKeyPressed(sf::Keyboard::R) ? fRotationSpeed : -fRotationSpeed) * 3.f * fDt,
												   Vector3(1.f, 0.f, 0.f)));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::T) || sf::Keyboard::isKeyPressed(sf::Keyboard::G))
			{
				xBombermanModel->SetLocalTransform(glm::rotate(xBombermanModel->GetLocalTransform(),
												  (sf::Keyboard::isKeyPressed(sf::Keyboard::T) ? fRotationSpeed : -fRotationSpeed) * 3.f * fDt,
												  Vector3(0.f, 1.f, 0.f)));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y) || sf::Keyboard::isKeyPressed(sf::Keyboard::H))
			{
				xBombermanModel->SetLocalTransform(glm::rotate(xBombermanModel->GetLocalTransform(),
												   (sf::Keyboard::isKeyPressed(sf::Keyboard::Y) ? fRotationSpeed : -fRotationSpeed) * 3.f * fDt,
												   Vector3(0.f, 0.f, 1.f)));
			}
			xBombermanModel->Update();

			{
				//ProfilerBlockPrint oProfilerBlockRender("Boucle de rendu");

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);

				xCamera->MakeViewContext(&oViewContext);

				xTextureShader->Use();
				xTextureShader->SetUniform("oDiffuseTexture", xTextureTest, 0);
				xTextureShader->SetUniform("mView", oViewContext.mView);
				xTextureShader->SetUniform("mProjection", oViewContext.mProjection);

				ModelNode::const_iterator	itNode(xBombermanModel->m_xRootNode);
				while (*itNode != nullptr)
				{
					for (const Mesh* pMesh : itNode->m_oMeshes)
					{
						xTextureShader->SetUniform("mModel", xBombermanModel->GetWorldTransform() * itNode->GetWorldTransform());
						pMesh->Draw();
					}
					itNode.Next();
				}

				glDisable(GL_DEPTH_TEST);
				xColorShader->Use();
				xTextureShader->SetUniform("mModel", Matrix44(1));
				xTextureShader->SetUniform("mView", oViewContext.mView);
				xTextureShader->SetUniform("mProjection", oViewContext.mProjection);
				xAxisMesh->Draw();
			}

			{
				//ProfilerBlockPrint oProfilerBlockFlip("Flip");
				pWindow->display();
			}
		}

		FT_TEST(oResourceManager.Destroy() == FT_OK);
	}

	delete pWindow;

	return 0;
}
