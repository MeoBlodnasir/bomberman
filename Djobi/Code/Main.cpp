
#if defined(_WIN32)
#	include <crtdbg.h>
#endif

#include <Output.hpp>
#include <OpenGL.hpp>
#include <Mesh.hpp>
#include <Shader.hpp>
#include <ShaderProgram.hpp>
#include <Texture.hpp>
#include <Camera.hpp>

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>


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

#if defined(_WIN32)
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
#endif

		glViewport(0, 0, (GLsizei)pWindow->getSize().x, (GLsizei)pWindow->getSize().y);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.05f, 0.05f, 0.1f, 1.0f);

		sf::Clock oSFClock;
		oSFClock.restart();
		float fDt;

		SPtr<MeshResource>	xCubeResource = new MeshResource;
		FT_TEST(xCubeResource->MakePrimitiveCube(E_VERTEX_PROP_POSITION | E_VERTEX_PROP_UV) == FT_OK);
		SPtr<Mesh>			xCubeMesh = new Mesh;
		FT_TEST(xCubeMesh->Create(xCubeResource) == FT_OK);
		Matrix44 mTransformCube = glm::rotate(Matrix44(1), glm::radians(-55.f), glm::normalize(Vector3(0.7f, 0.3f, 0.1f)));

		SPtr<Camera> xCamera = new Camera;
		Camera::Desc oCameraDesc;
		oCameraDesc.eProjectionType = E_PROJECTION_PERSPECTIVE;
		oCameraDesc.fFov = glm::radians(60.f);
		oCameraDesc.fNear = 0.1f;
		oCameraDesc.fFar = 100.f;
		oCameraDesc.fWidth = (float)pWindow->getSize().x;
		oCameraDesc.fRatio = oCameraDesc.fWidth / (float)pWindow->getSize().y;
		FT_TEST(xCamera->Create(&oCameraDesc) == FT_OK);
		xCamera->mWorldTransform = glm::translate(Matrix44(1), Vector3(0.f, 0.f, -3.f));
		ViewContext oViewContext;

		SPtr<Shader> xVsPositionUV = new Shader;
		FT_TEST(xVsPositionUV->Create(E_VERTEX_SHADER, Path("./Engine/Assets/Shaders/PositionUV.vs.glsl")) == FT_OK);
		SPtr<Shader> xFsTexture = new Shader;
		FT_TEST(xFsTexture->Create(E_FRAGMENT_SHADER, Path("./Engine/Assets/Shaders/Texture.fs.glsl")) == FT_OK);

		SPtr<ShaderProgram> xTextureShader = new ShaderProgram;
		FT_TEST(xTextureShader->Create(xVsPositionUV, xFsTexture) == FT_OK);

		SPtr<Texture> xTextureTest = new Texture;
		FT_TEST(xTextureTest->Create(GL_TEXTURE_2D, Path("./Djobi/Assets/Textures/Test.png")) == FT_OK);

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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				mTransformCube = glm::translate(mTransformCube, Vector3(0.f, fDt, 0.f));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				mTransformCube = glm::translate(mTransformCube, Vector3(0.f, -fDt, 0.f));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				mTransformCube = glm::translate(mTransformCube, Vector3(-fDt, 0.f, 0.f));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				mTransformCube = glm::translate(mTransformCube, Vector3(fDt, 0.f, 0.f));
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			xCamera->MakeViewContext(&oViewContext);

			xTextureShader->Use();
			xTextureShader->SetUniform("oTexture0", xTextureTest, 0);
			xTextureShader->SetUniform("mModel", mTransformCube);
			xTextureShader->SetUniform("mView", oViewContext.mView);
			xTextureShader->SetUniform("mProjection", oViewContext.mProjection);
			xCubeMesh->Draw();

			pWindow->display();
		}
	}

	delete pWindow;

	return 0;
}
