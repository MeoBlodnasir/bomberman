#pragma once

#include "Handled.hpp"
#include "Resource.hpp"
#include "Path.hpp"
#include "ShaderType.hpp"

// Shader compilé individuellement

namespace ft
{
	// fw
	class ShaderResource;

	class Shader : public CountableSPtr
	{
	public:

				Shader();
		virtual ~Shader();

		ErrorCode		Create(const SPtr<ShaderResource>& xResource);
		ErrorCode		Destroy();

	protected:

		SPtr<ShaderResource>	m_xResource;

	private:

		Shader(const Shader&) FT_DELETED;
		Shader& operator = (const Shader&) FT_DELETED;
	};


	// fw
	template <typename TResourceType> class SpecificResourceManager;

	struct ShaderResourceInfos
	{
		Path			oFilePath;
		EShaderType		eShaderType;

		ShaderResourceInfos()
			: eShaderType(E_INVALID_SHADER_TYPE)
		{}
	};

	class ShaderResource : public Handled, public Resource<ShaderResourceInfos>
	{
	public:

		virtual ~ShaderResource();

		virtual bool	IsLoadedAndValid() const override;

	protected:

		ShaderResource();

		ShaderResourceInfos		m_oResourceInfos;

	private:
		friend SpecificResourceManager<ShaderResource>;

		virtual ErrorCode	Load(ResourceManager& oResourceManager, const ShaderResourceInfos& oInfos) override;
		virtual ErrorCode	Unload() override;
	};
}

// Nécessaire pour SpecificResourceManager
namespace std
{
	template <>
	struct hash<ft::ShaderResourceInfos>
	{
		size_t	operator ()	(const ft::ShaderResourceInfos& oObj) const
		{
			return hash<std::string>()(oObj.oFilePath.GetFullPath());
		}
	};
}
