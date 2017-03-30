#pragma once

#include "Core.hpp"
#include "Types.hpp"
#include "ErrorCode.hpp"
#include "VertexLocation.h"

namespace ft
{
	enum EVertexProperty
	{
		E_VERTEX_PROP_NONE		= 0,
		E_VERTEX_PROP_POSITION	= (1 << FT_VERTEX_LOCATION_POSITION),
		E_VERTEX_PROP_NORMAL	= (1 << FT_VERTEX_LOCATION_NORMAL),
		E_VERTEX_PROP_UV		= (1 << FT_VERTEX_LOCATION_UV),
		E_VERTEX_PROP_COLOR		= (1 << FT_VERTEX_LOCATION_COLOR),

		E_VERTEX_PROP_MAX		= (1 << FT_VERTEX_LOCATION_COUNT)
	};

	class VertexDescription
	{
	public:

		static const uint32	s_iElementCount[FT_VERTEX_LOCATION_COUNT];

		VertexDescription();
		VertexDescription(const VertexDescription& oDesc);
		~VertexDescription();

		VertexDescription&	operator = (const VertexDescription& oDesc);

				ErrorCode	Create(uint32 iProperties);

				bool		IsValid() const;

		inline	uint32		GetProperties() const					{ return m_iProperties; }
		inline	uint32		GetVertexSize() const					{ return m_iVertexSize; }
		inline	uint32		GetVertexElementCount() const			{ return m_iVertexSize / sizeof(float32); }

				uint32		GetPropertyStride(EVertexProperty eProperty) const;

		static	uint32		GetPropertySize(EVertexProperty eProperty);

	private:

		uint32		m_iProperties;
		uint32		m_iVertexSize;
	};
}
