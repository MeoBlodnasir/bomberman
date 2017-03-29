#pragma once

#include "ErrorCode.hpp"
#include "Resource.hpp"
#include "Handled.hpp"
#include "Path.hpp"
#include "ShaderType.hpp"

namespace ft
{
	struct ShaderFileResourceInfos
	{
		EShaderType	eShaderType;
		Path		oFilePath;

		ShaderFileResourceInfos()
			: eShaderType(E_INVALID_SHADER_TYPE)
		{}
	};

	// Ressource de shader compilé individuellement
	class ShaderFileResource : public Handled, public Resource<ShaderFileResourceInfos>
	{
	public:

		virtual ~ShaderFileResource();

		virtual bool	IsLoadedAndValid() const override;
		EShaderType		GetShaderType() const				{ return m_oResourceInfos.eShaderType; }
		const Path&		GetFilePath() const					{ return m_oResourceInfos.oFilePath; }

	protected:

		friend class ShaderFileResourceManager;

		ShaderFileResource();

		ShaderFileResourceInfos		m_oResourceInfos;
	};

	struct ShaderResourceInfos
	{
		uint32	iShaderTypesFlags;
		Path	oVertexShaderFilePath;
		Path	oFragmentShaderFilePath;
	};

	// Ressource de programme lié
	class ShaderResource : public Handled, public Resource<ShaderResourceInfos>
	{
	public:

		virtual ~ShaderResource();

		virtual bool	IsLoadedAndValid() const override;

	protected:

		friend class ShaderResourceManager;

		ShaderResource();

		ShaderResourceInfos		m_oResourceInfos;
	};
}

// Nécessaire pour SpecificResourceManager
namespace std
{
	template <>
	struct hash<ft::ShaderFileResourceInfos>
	{
		size_t	operator ()	(const ft::ShaderFileResourceInfos& oObj) const
		{
			return hash<std::string>()(oObj.oFilePath.GetFullPath());
		}
	};

	template <>
	struct hash<ft::ShaderResourceInfos>
	{
		size_t	operator ()	(const ft::ShaderResourceInfos& oObj) const
		{
			return hash<std::string>()(
				oObj.oVertexShaderFilePath.GetFullPath()
				+ oObj.oFragmentShaderFilePath.GetFullPath());
		}
	};
}
