
#include <Core/Output.hpp>
#include <Core/ProfilerBlock.hpp>
#include <Math/Quaternion.hpp>
#include <Resource/ResourceManager.hpp>
#include <Graphics/OpenGL.hpp>
#include <Graphics/Mesh.hpp>
#include <Graphics/Model.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/ShaderProgram.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/Color.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Renderer.hpp>

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>

#include <glm/gtx/matrix_decompose.hpp>


int		TestsRender()
{
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

		FT_TEST(oResourceManager.LoadDefaultResources() == FT_OK);

		Renderer oRenderer;
		FT_TEST(oRenderer.Create(oResourceManager) == FT_OK);

		SPtr<SceneNode>	xSceneRoot = new SceneNode;
		SceneNode::Desc oSceneRootDesc;
		oSceneRootDesc.pParent = nullptr;
		FT_TEST(xSceneRoot->Create(&oSceneRootDesc) == FT_OK);
		
		SPtr<Model>	xBombermanModel = new Model;
		{
			ProfilerBlockPrint oProfilerBlock("Chargement Bomberman.FBX");

			ModelResourceInfos oModelResourceInfos;
			SPtr<ModelResource> xModelResource;
			Model::Desc oModelDesc;

			oModelResourceInfos.oFilePath = Path("./Djobi/Assets/Models/Bomberman.FBX");
			FT_TEST(oResourceManager.GetModelResourceManager()->Load(oModelResourceInfos, xModelResource) == FT_OK);
			oModelDesc.pParent = xSceneRoot.Ptr();
			FT_TEST(xBombermanModel->Create(&oModelDesc, xModelResource) == FT_OK);
		}
		

		SPtr<Model>	xCubeSphereModel = new Model;
		{
			ProfilerBlockPrint oProfilerBlock("Chargement CubeSphere.FBX");
			
			ModelResourceInfos oModelResourceInfos;
			SPtr<ModelResource> xModelResource;
			Model::Desc oModelDesc;
			
			oModelResourceInfos.oFilePath = Path("./Djobi/Assets/Models/CubeSphere.FBX");
			FT_TEST(oResourceManager.GetModelResourceManager()->Load(oModelResourceInfos, xModelResource) == FT_OK);
			oModelDesc.pParent = xSceneRoot.Ptr();
			FT_TEST(xCubeSphereModel->Create(&oModelDesc, xModelResource) == FT_OK);
		}
		

		SPtr<Mesh> xAxisMesh = new Mesh;
		{
			MeshResourceInfos oMeshResourceInfos;
			SPtr<MeshResource> xMeshResource;

			oMeshResourceInfos.eSource = E_PRIMITIVE_AXIS;
			oMeshResourceInfos.iVertexProperties = E_VERTEX_PROP_POSITION | E_VERTEX_PROP_COLOR;
			FT_TEST(oResourceManager.GetMeshResourceManager()->Load(oMeshResourceInfos, xMeshResource) == FT_OK);
			FT_TEST(xAxisMesh->Create(xMeshResource) == FT_OK);
		}


		SPtr<Camera>	xCamera = new Camera;
		Camera::Desc	oCameraDesc;
		ViewContext		oViewContext;
		oCameraDesc.pParent = xSceneRoot.Ptr();
		oCameraDesc.eProjectionType = E_PROJECTION_PERSPECTIVE;
		oCameraDesc.fFov = glm::radians(60.f);
		oCameraDesc.fNear = 0.1f;
		oCameraDesc.fFar = 100.f;
		oCameraDesc.fWidth = (float)pWindow->getSize().x;
		oCameraDesc.fRatio = oCameraDesc.fWidth / (float)pWindow->getSize().y;
		FT_TEST(xCamera->Create(&oCameraDesc) == FT_OK);
		oRenderer.SetActive3DCamera(xCamera);

		SPtr<ShaderProgram> xTextureShaderProgram	 = new ShaderProgram;
		SPtr<ShaderProgram> xColorShaderProgram		 = new ShaderProgram;
		SPtr<ShaderProgram> xColorValueShaderProgram = new ShaderProgram;
		SPtr<ShaderProgram> xBlinnPhongShaderProgram = new ShaderProgram;
		{
			ProfilerBlockPrint oProfilerBlock("Chargement Shaders");

			ShaderProgramResourceInfos	oShaderProgramResInfos;
			SPtr<ShaderProgramResource>	xShaderProgramResource;

			oShaderProgramResInfos.iShaderTypesFlags = SHADER_TYPE_FLAG(E_VERTEX_SHADER) | SHADER_TYPE_FLAG(E_FRAGMENT_SHADER);

			oShaderProgramResInfos.sName = "TextureShader";
			FT_TEST(oShaderProgramResInfos.oVertexShaderFilePath.Set("./Engine/Assets/Shaders/PositionUV.vs.glsl"));
			FT_TEST(oShaderProgramResInfos.oFragmentShaderFilePath.Set("./Engine/Assets/Shaders/Texture.fs.glsl"));
			FT_TEST(oResourceManager.GetShaderProgramResourceManager()->Load(oShaderProgramResInfos, xShaderProgramResource) == FT_OK);
			FT_TEST(xTextureShaderProgram->Create(xShaderProgramResource) == FT_OK);

			oShaderProgramResInfos.sName = "ColorShader";
			FT_TEST(oShaderProgramResInfos.oVertexShaderFilePath.Set("./Engine/Assets/Shaders/PositionColor.vs.glsl"));
			FT_TEST(oShaderProgramResInfos.oFragmentShaderFilePath.Set("./Engine/Assets/Shaders/Color.fs.glsl"));
			FT_TEST(oResourceManager.GetShaderProgramResourceManager()->Load(oShaderProgramResInfos, xShaderProgramResource) == FT_OK);
			FT_TEST(xColorShaderProgram->Create(xShaderProgramResource) == FT_OK);

			oShaderProgramResInfos.sName = "ColorValueShader";
			FT_TEST(oShaderProgramResInfos.oVertexShaderFilePath.Set("./Engine/Assets/Shaders/Position.vs.glsl"));
			FT_TEST(oShaderProgramResInfos.oFragmentShaderFilePath.Set("./Engine/Assets/Shaders/ColorValue.fs.glsl"));
			FT_TEST(oResourceManager.GetShaderProgramResourceManager()->Load(oShaderProgramResInfos, xShaderProgramResource) == FT_OK);
			FT_TEST(xColorValueShaderProgram->Create(xShaderProgramResource) == FT_OK);

			oShaderProgramResInfos.sName = "BlinnPhongShader";
			FT_TEST(oShaderProgramResInfos.oVertexShaderFilePath.Set("./Engine/Assets/Shaders/BlinnPhong.vs.glsl"));
			FT_TEST(oShaderProgramResInfos.oFragmentShaderFilePath.Set("./Engine/Assets/Shaders/BlinnPhong.fs.glsl"));
			FT_TEST(oResourceManager.GetShaderProgramResourceManager()->Load(oShaderProgramResInfos, xShaderProgramResource) == FT_OK);
			FT_TEST(xBlinnPhongShaderProgram->Create(xShaderProgramResource) == FT_OK);
		}
		
		SPtr<Texture> xCubeSphereDiffuse	= new Texture;
		SPtr<Texture> xCubeSphereAmbient	= new Texture;
		SPtr<Texture> xCubeSphereSpecular	= new Texture;
		SPtr<Texture> xCubeSphereEmissive	= new Texture;
		{
			ProfilerBlockPrint oProfilerBlock("Chargement Textures");

			SPtr<TextureResource> xTextureResource;
			TextureResourceInfos oTextureResInfos;

			oTextureResInfos.oFilePath = Path("./Djobi/Assets/Textures/CubeSphere_d.png");
			oTextureResInfos.eTextureTarget = E_TEXTURE_2D;
			FT_TEST(oResourceManager.GetTextureResourceManager()->Load(oTextureResInfos, xTextureResource) == FT_OK);
			FT_TEST(xCubeSphereDiffuse->Create(xTextureResource) == FT_OK);

			oTextureResInfos.oFilePath = Path("./Djobi/Assets/Textures/CubeSphere_am.png");
			oTextureResInfos.eTextureTarget = E_TEXTURE_2D;
			FT_TEST(oResourceManager.GetTextureResourceManager()->Load(oTextureResInfos, xTextureResource) == FT_OK);
			FT_TEST(xCubeSphereAmbient->Create(xTextureResource) == FT_OK);

			oTextureResInfos.oFilePath = Path("./Djobi/Assets/Textures/CubeSphere_s.png");
			oTextureResInfos.eTextureTarget = E_TEXTURE_2D;
			FT_TEST(oResourceManager.GetTextureResourceManager()->Load(oTextureResInfos, xTextureResource) == FT_OK);
			FT_TEST(xCubeSphereSpecular->Create(xTextureResource) == FT_OK);

			oTextureResInfos.oFilePath = Path("./Djobi/Assets/Textures/CubeSphere_e.png");
			oTextureResInfos.eTextureTarget = E_TEXTURE_2D;
			FT_TEST(oResourceManager.GetTextureResourceManager()->Load(oTextureResInfos, xTextureResource) == FT_OK);
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
			xCamera->SetLocalTransform(glm::inverse(glm::lookAt(vCamControllerPos, vCamControllerPos + (qCamControllerRot * Vector3(0.f, 0.f, -1.f)), Vector3(0.f, 0.f, 1.f))));

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

			SceneNode::UpdateHierarchy(xSceneRoot);

			{
				//ProfilerBlockPrint oProfilerBlockRender("Boucle de rendu");

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);

				
				oRenderer.RenderScene(xSceneRoot.Ptr());
				

				//glDisable(GL_DEPTH_TEST);
				//xColorShaderProgram->Use();
				//xColorShaderProgram->SetUniform("mModel", Matrix44(1));
				//xColorShaderProgram->SetUniform("mView", oViewContext.mView);
				//xColorShaderProgram->SetUniform("mProjection", oViewContext.mProjection);
				//xAxisMesh->Draw();
			}

			{
				//ProfilerBlockPrint oProfilerBlockFlip("Flip");
				pWindow->display();
			}
		}

		xSceneRoot = nullptr;
		FT_TEST(oRenderer.Destroy() == FT_OK);
	}

	FT_TEST(oResourceManager.Destroy() == FT_OK);

	delete pWindow;

	return 0;
}
