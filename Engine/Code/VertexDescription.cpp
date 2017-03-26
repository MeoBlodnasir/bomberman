
#include "VertexDescription.hpp"

namespace ft
{
	const uint32	VertexDescription::s_iElementCount[FT_VERTEX_LOCATION_COUNT] =
	{
		3, // FT_VERTEX_LOCATION_POSITION
		2, // FT_VERTEX_LOCATION_UV
		4  // FT_VERTEX_LOCATION_COLOR
	};

	VertexDescription::VertexDescription()
		: m_iProperties(E_VERTEX_PROP_NONE)
		, m_iVertexSize((uint32)-1)
	{
	}

	VertexDescription::VertexDescription(const VertexDescription& oDesc)
		: m_iProperties(oDesc.m_iProperties)
		, m_iVertexSize(oDesc.m_iVertexSize)
	{
	}

	VertexDescription::~VertexDescription()
	{
	}

	VertexDescription&	VertexDescription::operator = (const VertexDescription& oDesc)
	{
		m_iProperties = oDesc.m_iProperties;
		m_iVertexSize = oDesc.m_iVertexSize;
		return *this;
	}

	ErrorCode	VertexDescription::Create(uint32 iProperties)
	{
		uint32	iVertexSize = 0;

		for (uint32 i = 0; i < FT_VERTEX_LOCATION_COUNT; ++i)
		{
			if (iProperties & (1 << i))
				iVertexSize += GetPropertySize(static_cast<EVertexProperty>(1 << i));
		}

		if (iVertexSize == 0)
			return FT_FAIL;

		m_iProperties = iProperties;
		m_iVertexSize = iVertexSize;

		return FT_OK;
	}

	bool	VertexDescription::IsValid() const
	{
		return m_iVertexSize != (uint32)-1 && m_iVertexSize > 0;
	}

	uint32	VertexDescription::GetPropertyStride(EVertexProperty eProperty) const
	{
		FT_ASSERT((m_iProperties & (uint32)eProperty) > 0);

		uint32 iStride = 0;

		if ((m_iProperties & (uint32)eProperty) > 0)
		{
			for (uint32 i = 0; (1u << i) < (uint32)eProperty; ++i)
				if (m_iProperties & (1 << i))
					iStride += GetPropertySize(static_cast<EVertexProperty>(1 << i));
		}

		return iStride;
	}

	uint32	VertexDescription::GetPropertySize(EVertexProperty eProperty)
	{
		uint32 iRet;

		switch (eProperty)
		{
		case E_VERTEX_PROP_POSITION:	{ iRet = s_iElementCount[FT_VERTEX_LOCATION_POSITION]	* sizeof(float32);	break; }
		case E_VERTEX_PROP_UV:			{ iRet = s_iElementCount[FT_VERTEX_LOCATION_UV]			* sizeof(float32);	break; }
		case E_VERTEX_PROP_COLOR:		{ iRet = s_iElementCount[FT_VERTEX_LOCATION_COLOR]		* sizeof(float32);	break; }
		default:						{ iRet = (uint32)-1; break; }
		}

		return iRet;
	}
}
