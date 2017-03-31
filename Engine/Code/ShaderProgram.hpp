#pragma once

#include "Handled.hpp"
#include "Resource.hpp"
#include "Path.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix33.hpp"
#include "Matrix44.hpp"

namespace ft
{
	// fw
	class ShaderProgramResource;
	class Texture;

	class ShaderProgram : public CountableSPtr
	{
	public:

		ShaderProgram();
		virtual ~ShaderProgram();

		ErrorCode	Create(const SPtr<ShaderProgramResource>& xResource);
		ErrorCode	Destroy();

		void		Use() const;

		// Wrappers glUniform*
		void	SetUniform(const char* csName, float32 f) const;
		void	SetUniform(const char* csName, int32 i) const;
		void	SetUniform(const char* csName, const float32* p, uint32 iCount = 1) const;
		void	SetUniform(const char* csName, const int32* p, uint32 iCount = 1) const;
		void	SetUniform(const char* csName, const Vector2& v) const;
		void	SetUniform(const char* csName, const Vector3& v) const;
		void	SetUniform(const char* csName, const Vector4& v) const;
		void	SetUniform(const char* csName, const Matrix33& m) const;
		void	SetUniform(const char* csName, const Matrix44& m) const;
		void	SetUniform(const char* csName, const SPtr<Texture>& xTexture, uint32 iTextureUnit) const;

	private:

		SPtr<ShaderProgramResource>	m_xResource;

		ShaderProgram(const ShaderProgram&) FT_DELETED;
		ShaderProgram& operator = (const ShaderProgram&) FT_DELETED;
	};

	// fw
	template <typename TResourceType> class SpecificResourceManager;

	struct ShaderProgramResourceInfos
	{
		uint32	iShaderTypesFlags;
		Path	oVertexShaderFilePath;
		Path	oFragmentShaderFilePath;
	};

	// Ressource de programme lié
	class ShaderProgramResource : public Handled, public Resource<ShaderProgramResourceInfos>
	{
	public:

		virtual ~ShaderProgramResource();

		virtual bool	IsLoadedAndValid() const override;

	protected:

		ShaderProgramResource();

		ShaderProgramResourceInfos	m_oResourceInfos;

	private:
		friend SpecificResourceManager<ShaderProgramResource>;

		virtual ErrorCode	Load(ResourceManager& oResourceManager, const ShaderProgramResourceInfos& oInfos) override;
		virtual ErrorCode	Unload() override;
	};
}

// Nécessaire pour SpecificResourceManager
namespace std
{
	template <>
	struct hash<ft::ShaderProgramResourceInfos>
	{
		size_t	operator ()	(const ft::ShaderProgramResourceInfos& oObj) const
		{
			return hash<std::string>()(oObj.oVertexShaderFilePath.GetFullPath()
									+ oObj.oFragmentShaderFilePath.GetFullPath());
		}
	};
}