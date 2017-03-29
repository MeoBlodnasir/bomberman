
#include "ShaderResource.hpp"

namespace ft
{
	ShaderFileResource::ShaderFileResource()
	{
	}

	ShaderFileResource::~ShaderFileResource()
	{
	}

	bool	ShaderFileResource::IsLoadedAndValid() const 
	{
		return !m_oResourceInfos.oFilePath.IsEmpty() && m_iHandle != 0;
	}

	ShaderResource::ShaderResource()
	{
	}

	ShaderResource::~ShaderResource()
	{
	}

	bool	ShaderResource::IsLoadedAndValid() const 
	{
		return m_iHandle != 0;
	}
}
