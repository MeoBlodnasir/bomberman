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

		virtual ErrorCode	Create(const SPtr<MeshResource>& xResource);
		virtual ErrorCode	Destroy() override;

		virtual void		Draw() const;

	protected:

		SPtr<MeshResource>	m_xResource;

	private:

		Mesh(const Mesh&) FT_DELETED;
		Mesh& operator = (const Mesh&) FT_DELETED;
	};
}
