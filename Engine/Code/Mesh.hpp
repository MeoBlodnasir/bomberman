#pragma once

#include "Handled.hpp"
#include "MeshResource.hpp"

namespace ft
{
	class Mesh : public Handled, public CountableSPtr
	{
	public:

				Mesh();
		virtual ~Mesh();

		virtual ErrorCode	Create(const SPtr<MeshResource>& xMeshResource);
		virtual ErrorCode	Destroy() override;

		virtual bool		IsValid() const;

		virtual void		Draw() const;

	protected:

		SPtr<MeshResource>	m_xMeshResource;

		uint32	m_iVbo;
		uint32	m_iEbo;
		uint32	m_iVertexCount;
		uint32	m_iVertexToDrawCount;

	private:

		Mesh(const Mesh&) FT_DELETED;
		Mesh& operator = (const Mesh&) FT_DELETED;
	};
}
