
#if defined(_WIN32)
#	include <crtdbg.h>
#endif

#include <Output.hpp>
#include <OpenGL.hpp>
#include <Mesh.hpp>
#include <Model.hpp>
#include <Shader.hpp>
#include <ShaderProgram.hpp>
#include <Texture.hpp>
#include <Camera.hpp>
#include <Quaternion.hpp>

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
#endif

	using namespace ft;

	FT_COUT << "Binvenu sur DJOBI!" << std::endl;

	sf::ContextSettings oOpenGLContext;
	oOpenGLContext.depthBits		 = 24;
	oOpenGLContext.stencilBits		 = 8;
	oOpenGLContext.antialiasingLevel = 4;
	oOpenGLContext.majorVersion		 = 4;
	oOpenGLContext.minorVersion		 = 1;
	oOpenGLContext.attributeFlags	 = sf::ContextSettings::Core;
	sf::Window* pWindow = new sf::Window(sf::VideoMode(1080, 720), "Bomberman", sf::Style::Default, oOpenGLContext);
	{
		pWindow->setFramerateLimit(60);

		// Après l'initialisation d'un contexte OpenGL
		{
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

		SPtr<MeshResource>	xTorusResource = new MeshResource;
		{
			Assimp::Importer oAssimpImporter;
			const aiScene* pScene =
				oAssimpImporter.ReadFile(Path("./Djobi/Assets/Models/Torus.FBX").GetFullPath(),
//				oAssimpImporter.ReadFile(Path("./Djobi/Assets/Models/Bomberman.FBX").GetFullPath(),
				aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
			FT_ASSERT(pScene != nullptr);
			FT_ASSERT(!(pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE));
			FT_ASSERT(pScene->mNumMeshes > 0);

			FT_TEST(xTorusResource->MakeFromAssimpMesh(pScene->mMeshes[0]) == FT_OK);
		}

		SPtr<ModelResource> xBombermanResource = new ModelResource;
		{
			FT_TEST(xBombermanResource->LoadFromFile(Path("./Djobi/Assets/Models/Bomberman.FBX")) == FT_OK);

			ModelNodeResource::const_iterator	itNode(xBombermanResource->xRootNode);
			FT_ASSERT(*itNode != nullptr);
			FT_COUT << "------" << std::endl;
			while (*itNode != nullptr)
			{
				FT_COUT << (*itNode)->GetName() << std::endl;
				itNode.Next();
			}
			FT_COUT << "------" << std::endl;
		}
		SPtr<Model>	xBombermanModel = new Model;
		{
			Model::Desc oBombermanDesc;
			oBombermanDesc.pParent = nullptr;
			FT_TEST(xBombermanModel->Create(&oBombermanDesc, xBombermanResource) == FT_OK);
		}

		SPtr<Mesh>			xTorusMesh = new Mesh;
		FT_TEST(xTorusMesh->Create(xTorusResource) == FT_OK);
		Matrix44 mTransformTorus = glm::rotate(glm::scale(Matrix44(1), Vector3(0.03f)), glm::radians(45.f), glm::normalize(Vector3(0.7f, 0.3f, 0.1f)));

		SPtr<MeshResource>	xCubeResource = new MeshResource;
		FT_TEST(xCubeResource->MakePrimitiveCube(E_VERTEX_PROP_POSITION | E_VERTEX_PROP_UV) == FT_OK);
		SPtr<Mesh>			xCubeMesh = new Mesh;
		FT_TEST(xCubeMesh->Create(xCubeResource) == FT_OK);
		Matrix44 mTransformCube = glm::rotate(Matrix44(1), glm::radians(-55.f), glm::normalize(Vector3(0.7f, 0.3f, 0.1f)));

		SPtr<MeshResource>	xAxisResource = new MeshResource;
		FT_TEST(xAxisResource->MakePrimitiveMatrixAxis(E_VERTEX_PROP_POSITION | E_VERTEX_PROP_COLOR) == FT_OK);
		SPtr<Mesh>			xAxisMesh = new Mesh;
		FT_TEST(xAxisMesh->Create(xAxisResource) == FT_OK);

		ViewContext		oViewContext;
		SPtr<Camera>	xCamera = new Camera;
		Camera::Desc	oCameraDesc;
		oCameraDesc.eProjectionType = E_PROJECTION_PERSPECTIVE;
		oCameraDesc.fFov = glm::radians(60.f);
		oCameraDesc.fNear = 0.1f;
		oCameraDesc.fFar = 100.f;
		oCameraDesc.fWidth = (float)pWindow->getSize().x;
		oCameraDesc.fRatio = oCameraDesc.fWidth / (float)pWindow->getSize().y;
		FT_TEST(xCamera->Create(&oCameraDesc) == FT_OK);

		SPtr<Shader> xVsPositionUV = new Shader;
		FT_TEST(xVsPositionUV->Create(E_VERTEX_SHADER, Path("./Engine/Assets/Shaders/PositionUV.vs.glsl")) == FT_OK);
		SPtr<Shader> xVsPositionColor = new Shader;
		FT_TEST(xVsPositionColor->Create(E_VERTEX_SHADER, Path("./Engine/Assets/Shaders/PositionColor.vs.glsl")) == FT_OK);
		SPtr<Shader> xFsTexture = new Shader;
		FT_TEST(xFsTexture->Create(E_FRAGMENT_SHADER, Path("./Engine/Assets/Shaders/Texture.fs.glsl")) == FT_OK);
		SPtr<Shader> xFsColor = new Shader;
		FT_TEST(xFsColor->Create(E_FRAGMENT_SHADER, Path("./Engine/Assets/Shaders/Color.fs.glsl")) == FT_OK);

		SPtr<ShaderProgram> xTextureShader = new ShaderProgram;
		FT_TEST(xTextureShader->Create(xVsPositionUV, xFsTexture) == FT_OK);
		SPtr<ShaderProgram> xColorShader = new ShaderProgram;
		FT_TEST(xColorShader->Create(xVsPositionColor, xFsColor) == FT_OK);

		SPtr<Texture> xTextureTest = new Texture;
		FT_TEST(xTextureTest->Create(GL_TEXTURE_2D, Path("./Djobi/Assets/Textures/Purple.png")) == FT_OK);

		// Controles camera

		// Commentaire vu sur un site d'aide, à garder en tête:
		// Left-multiplying rot, will perform a rotation about the camera space X axis.
		// If you want a rotation about the world-space X axis, you need to right-multiply it.

		sf::Vector2i	vSfMousePosition = sf::Mouse::getPosition(*pWindow);
		Vector2			vMousePosition((float)vSfMousePosition.x, (float)vSfMousePosition.y);
		Vector2			vMouseMotion(0.f, 0.f);
		Vector3			vCamControllerPos = Vector3(3.f, 1.f, 1.f);
		Quaternion		qCamControllerRot = Quaternion(glm::inverse(glm::lookAt(vCamControllerPos, Vector3(0.f, 0.f, 0.5f), Vector3(0.f, 0.f, 1.f))));
		const float		fTranslationSpeed = 3.f;
		const float		fRotationSpeed = 0.2f;

		bool bRunning = true;
		while (bRunning)
		{
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






			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);

			xCamera->MakeViewContext(&oViewContext);

			xTextureShader->Use();
			xTextureShader->SetUniform("oTexture0", xTextureTest, 0);
			xTextureShader->SetUniform("mView", oViewContext.mView);
			xTextureShader->SetUniform("mProjection", oViewContext.mProjection);

			//xTextureShader->SetUniform("mModel", mTransformCube);
			//xCubeMesh->Draw();

			//xTextureShader->SetUniform("mModel", mTransformTorus);
			//xTorusMesh->Draw();

			ModelNode::const_iterator	itNode(xBombermanModel->m_xRootNode);
			while (*itNode != nullptr)
			{
				for (const Mesh* pMesh : itNode->m_oMeshes)
				{
					xTextureShader->SetUniform("mModel", itNode->GetWorldTransform());
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


			pWindow->display();
		}
	}

	delete pWindow;

	return 0;
}
