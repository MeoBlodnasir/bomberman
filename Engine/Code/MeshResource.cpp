
#include "MeshResource.hpp"

#include "ResourceManager.hpp"
#include "OpenGL.hpp"
#include "AssimpConversions.hpp"
#include "MaterialResource.hpp"
#include "ShaderProgram.hpp"

#include <assimp/scene.h>
#include <assimp/mesh.h>

namespace ft
{
	MeshResource::MeshResource()
		: m_iPrimitiveType(GL_TRIANGLES)
		, m_iVerticeCount(0)
		, m_iVertexToDrawCount(0)
		, m_iVbo(0)
		, m_iEbo(0)
		, m_xMaterialResource(nullptr)
		, m_xShaderProgramResource(nullptr)
	{
	}

	MeshResource::~MeshResource()
	{
		FT_ASSERT(!IsLoadedAndValid());
	}

	bool	 MeshResource::IsLoadedAndValid() const
	{
		return m_oVertexDescription.IsValid()
			&& m_iVerticeCount > 0
			&& m_iVertexToDrawCount > 0
			&& m_iVbo != 0
			&& IsHandled();
	}

	ErrorCode	MeshResource::Load(ResourceManager& oResourceManager, const MeshResourceInfos& oInfos)
	{
		// Initialise les données
		switch (oInfos.eSource)
		{
		case E_PRIMITIVE_QUAD:	{ FT_TEST_RETURN(LoadPrimitiveQuad(oResourceManager, oInfos.iVertexProperties) == FT_OK, FT_FAIL); break; }
		case E_PRIMITIVE_CUBE:	{ FT_TEST_RETURN(LoadPrimitiveCube(oResourceManager, oInfos.iVertexProperties) == FT_OK, FT_FAIL); break; }
		case E_PRIMITIVE_AXIS:	{ FT_TEST_RETURN(LoadPrimitiveMatrixAxis(oResourceManager, oInfos.iVertexProperties) == FT_OK, FT_FAIL); break; }
		case E_ASSIMP_MESH:		{ FT_TEST_RETURN(LoadAssimpMesh(oResourceManager, oInfos) == FT_OK, FT_FAIL); break; }
		default:				{ return FT_FAIL; }
		}

		ShaderProgramResourceInfos oShaderProgramResourceInfos;
		oShaderProgramResourceInfos.sName = "BlinnPhongShader";
		FT_TEST(oResourceManager.GetShaderProgramResourceManager()->Get(oShaderProgramResourceInfos, m_xShaderProgramResource) == FT_OK);

		// Chargement en mémoire vidéo
		FT_GL_ASSERT( glGenVertexArrays(1, &m_iHandle) );
		FT_GL_ASSERT( glBindVertexArray(m_iHandle) );

		if (m_oIndice.size() > 0)
		{
			FT_GL_ASSERT( glGenBuffers(1, &m_iEbo) );
			FT_GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iEbo) );
			FT_GL_ASSERT( glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_oIndice.size() * sizeof(GLuint), (GLuint*)m_oIndice.data(), GL_STATIC_DRAW) );
		}

		FT_GL_ASSERT( glGenBuffers(1, &m_iVbo) );
		FT_GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, m_iVbo) );
		FT_GL_ASSERT( glBufferData(GL_ARRAY_BUFFER, m_oVerticeData.size() * sizeof(GLfloat), (GLfloat*)m_oVerticeData.data(), GL_STATIC_DRAW) );

		uint32 iProperties = m_oVertexDescription.GetProperties();
		uint32 iVertexSize = m_oVertexDescription.GetVertexSize();
		uint32 iPropertyElementCount = 0;
		uint64 iCurrentStride = 0;
		for (uint32 i = 0; i < FT_VERTEX_LOCATION_COUNT; ++i)
		{
			if (iProperties & VERTEX_PROPERTY_FROM_LOCATION(i))
			{
				FT_ASSERT(iCurrentStride == m_oVertexDescription.GetPropertyStride((EVertexProperty)VERTEX_PROPERTY_FROM_LOCATION(i)));
				iPropertyElementCount = VertexDescription::s_iElementCount[i];
				FT_GL_ASSERT( glVertexAttribPointer(i, iPropertyElementCount, GL_FLOAT, GL_FALSE, iVertexSize, (GLvoid*)iCurrentStride) );
				FT_GL_ASSERT( glEnableVertexAttribArray(i) );
				iCurrentStride += iPropertyElementCount * sizeof(GLfloat);
			}
		}

		FT_GL_ASSERT( glBindVertexArray(0) );

		// Libération de mémoire
		m_oIndice.clear();
		m_oVerticeData.clear();

		return FT_OK;
	}

	ErrorCode	MeshResource::Unload()
	{
		FT_GL_ASSERT( glDeleteBuffers(1, &m_iVbo) );
		FT_GL_ASSERT( glDeleteBuffers(1, &m_iEbo) );
		FT_GL_ASSERT( glDeleteVertexArrays(1, &m_iHandle) );
		m_iVbo = 0;
		m_iEbo = 0;

		m_xMaterialResource = nullptr;
		m_xShaderProgramResource = nullptr;

		return Handled::Destroy();
	}

	// Voir comment utiliser les propriétés pour assigner efficacement les valeurs correspondantes.
	// Pour le moment, flemme.

	ErrorCode	MeshResource::LoadPrimitiveQuad(ResourceManager& oResourceManager, uint32 /*iVertexProperties*/)
	{
		const float32 oVert[] =
		{
			// pos				   uv
			 1.0f,  1.0f,  0.0f,   0.0f, 0.0f,
			 1.0f, -1.0f,  0.0f,   0.0f, 1.0f,
			-1.0f,  1.0f,  0.0f,   1.0f, 0.0f,
			-1.0f, -1.0f,  0.0f,   1.0f, 1.0f
		};

		const uint32 oId[] =
		{
			0, 1, 2, 1, 2, 3
		};

		m_oVerticeData.assign((const float32*)oVert, (const float32*)((uint8*)oVert + sizeof(oVert)));
		m_oIndice.assign((const uint32*)oId, (const uint32*)((uint8*)oId + sizeof(oId)));

		FT_TEST(m_oVertexDescription.Create(E_VERTEX_PROP_POSITION | E_VERTEX_PROP_UV) == FT_OK);
		m_iPrimitiveType = GL_TRIANGLES;
		m_iVerticeCount = m_oVerticeData.size() / m_oVertexDescription.GetVertexElementCount();
		m_iVertexToDrawCount = m_oIndice.size();

		FT_TEST(oResourceManager.GetMaterialResourceManager()->GetDefault(m_xMaterialResource) == FT_OK);
		FT_TEST(oResourceManager.GetShaderProgramResourceManager()->GetDefault(m_xShaderProgramResource) == FT_OK);

		return FT_OK;
	}

	ErrorCode	MeshResource::LoadPrimitiveCube(ResourceManager& oResourceManager, uint32 /*iVertexProperties*/)
	{
		const float32 oVert[] =
		{
			// pos				   uv
			 0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,   0.0f, 1.0f,

			 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,   1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,   0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,   0.0f, 1.0f,

			-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,   0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,   1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,   0.0f, 1.0f,

			-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,   1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,   0.0f, 1.0f,

			 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,   0.0f, 1.0f
		};

		const uint32 oId[] =
		{
			 0,  1,  2,  0,  2,  3,
			 4,  5,  6,  4,  6,  7,
			 8,  9, 10,  8, 10, 11,
			12, 13, 14, 12, 14, 15,
			16, 17, 18, 16, 18, 19,
			20, 21, 22, 20, 22, 23
		};

		m_oVerticeData.assign((float32*)oVert, (float32*)((uint8*)(oVert) + sizeof(oVert)));
		m_oIndice.assign((uint32*)oId, (uint32*)((uint8*)(oId) + sizeof(oId)));

		FT_TEST(m_oVertexDescription.Create(E_VERTEX_PROP_POSITION | E_VERTEX_PROP_UV) == FT_OK);
		m_iPrimitiveType = GL_TRIANGLES;
		m_iVerticeCount = m_oVerticeData.size() / m_oVertexDescription.GetVertexElementCount();
		m_iVertexToDrawCount = m_oIndice.size();

		FT_TEST(oResourceManager.GetMaterialResourceManager()->GetDefault(m_xMaterialResource) == FT_OK);
		FT_TEST(oResourceManager.GetShaderProgramResourceManager()->GetDefault(m_xShaderProgramResource) == FT_OK);

		return FT_OK;
	}

	ErrorCode	MeshResource::LoadPrimitiveMatrixAxis(ResourceManager& oResourceManager, uint32 /*iVertexProperties*/)
	{
		const float32 oVert[] =
		{
			// pos				color
			0.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 1.0f,

			0.0f, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f, 1.0f,

			0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f, 1.0f,
		};

		m_oVerticeData.assign((float32*)oVert, (float32*)((uint8*)(oVert) + sizeof(oVert)));
		m_oIndice.clear();

		FT_TEST(m_oVertexDescription.Create(E_VERTEX_PROP_POSITION | E_VERTEX_PROP_COLOR) == FT_OK);
		m_iPrimitiveType = GL_LINES;
		m_iVerticeCount = m_oVerticeData.size() / m_oVertexDescription.GetVertexElementCount();
		m_iVertexToDrawCount = m_iVerticeCount;

		m_xMaterialResource = nullptr;

		ShaderProgramResourceInfos	oShaderProgramResInfos;
		oShaderProgramResInfos.sName = "ColorShader";
		FT_TEST(oShaderProgramResInfos.oVertexShaderFilePath.Set("./Engine/Assets/Shaders/Position.vs.glsl"));
		FT_TEST(oShaderProgramResInfos.oFragmentShaderFilePath.Set("./Engine/Assets/Shaders/Color.fs.glsl"));
		FT_TEST(oResourceManager.GetShaderProgramResourceManager()->Load(oShaderProgramResInfos, m_xShaderProgramResource) == FT_OK);

		return FT_OK;
	}

	ErrorCode	MeshResource::LoadAssimpMesh(ResourceManager& oResourceManager, const MeshResourceInfos& oInfos)
	{
		const aiMesh* pMesh = oInfos.pAiMesh;

		// S'il n'y a rien à récupérer, on sort
		if (pMesh == nullptr || pMesh->mNumVertices == 0)
			return FT_FAIL;

		// Récupération du type de primitive à laquelle correspondent les données
		switch (pMesh->mPrimitiveTypes)
		{
		case aiPrimitiveType_POINT:		{ m_iPrimitiveType = GL_POINTS; break; }
		case aiPrimitiveType_LINE:		{ m_iPrimitiveType = GL_LINES; break; }
		case aiPrimitiveType_TRIANGLE:	{ m_iPrimitiveType = GL_TRIANGLES; break; }
		default:						{ return FT_FAIL; }
		}

		// Récupération des propriétés de vertex disponibles
		uint32 iVertexProperties = E_VERTEX_PROP_POSITION;
		if (pMesh->HasNormals())
			iVertexProperties |= E_VERTEX_PROP_NORMAL;
		if (pMesh->HasTextureCoords(0))
			iVertexProperties |= E_VERTEX_PROP_UV;
		FT_TEST(m_oVertexDescription.Create(iVertexProperties) == FT_OK);

		// Récupération des données de vertex et d'indices
		// Réinitialisation, puis allocation de la mémoire nécessaire
		m_oVerticeData.clear();
		m_oIndice.clear();
		m_oVerticeData.reserve(pMesh->mNumVertices * m_oVertexDescription.GetVertexSize());
		m_oIndice.reserve(pMesh->mNumFaces * 3);

		// Vertice
		for (uint32 i = 0, iCount = pMesh->mNumVertices; i < iCount; ++i)
		{
			const aiVector3D* pPos = pMesh->mVertices + i;
			m_oVerticeData.push_back(pPos->x);
			m_oVerticeData.push_back(pPos->y);
			m_oVerticeData.push_back(pPos->z);

			if (iVertexProperties & E_VERTEX_PROP_NORMAL)
			{
				const aiVector3D* pNormal = pMesh->mNormals + i;
				m_oVerticeData.push_back(pNormal->x);
				m_oVerticeData.push_back(pNormal->y);
				m_oVerticeData.push_back(pNormal->z);
			}

			if (iVertexProperties & E_VERTEX_PROP_UV)
			{
				const aiVector3D* pTexCoord = pMesh->mTextureCoords[0] + i;
				m_oVerticeData.push_back(pTexCoord->x);
				m_oVerticeData.push_back(pTexCoord->y);
			}
		}

		// Indices
		for (uint32 i = 0, iCount = pMesh->mNumFaces; i < iCount; ++i)
		{
			const aiFace& oFace = pMesh->mFaces[i];
			FT_ASSERT(oFace.mNumIndices == 3);
			m_oIndice.push_back(oFace.mIndices[0]);
			m_oIndice.push_back(oFace.mIndices[1]);
			m_oIndice.push_back(oFace.mIndices[2]);
		}

		// Infos supplémentaires
		m_iVerticeCount = m_oVerticeData.size() / m_oVertexDescription.GetVertexElementCount();
		if (m_oIndice.size() > 0)
			m_iVertexToDrawCount = m_oIndice.size();
		else
			m_iVertexToDrawCount = m_iVerticeCount;

		// Matériau
		if (oInfos.pAiMaterial != nullptr)
		{
			MaterialResourceInfos	oMaterialResourceInfos;
			aiString				oAiString;

			if (oInfos.pAiMaterial->Get(AI_MATKEY_NAME, oAiString) == AI_SUCCESS)
			{
				oMaterialResourceInfos.sName = oAiString.C_Str();
				oMaterialResourceInfos.eSource = E_ASSIMP_MATERIAL;
				oMaterialResourceInfos.oLocalPath = oInfos.oLocalPath;
				oMaterialResourceInfos.pAiMaterial = oInfos.pAiMaterial;
				if (oResourceManager.GetMaterialResourceManager()->Load(oMaterialResourceInfos, m_xMaterialResource) != FT_OK)
				{
					FT_FAILED_ASSERTION("Chargement materiau");
					FT_TEST_RETURN(oResourceManager.GetMaterialResourceManager()->GetDefault(m_xMaterialResource) == FT_OK, FT_FAIL);
				}
			}
		}
		else
		{
			FT_TEST_RETURN(oResourceManager.GetMaterialResourceManager()->GetDefault(m_xMaterialResource) == FT_OK, FT_FAIL);
		}

		return FT_OK;
	}
}
