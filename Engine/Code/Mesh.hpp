#pragma once

#include "Handled.hpp"
#include "MeshResource.hpp"
#include "RenderTechnique.hpp"
#include "Matrix44.hpp"

namespace ft
{
	// fw
	struct RenderContext;

	class Mesh : public Handled, public CountableSPtr
	{
	public:

				Mesh();
		virtual ~Mesh();

		virtual ErrorCode		Create(const SPtr<MeshResource>& xResource);
		virtual ErrorCode		Destroy() override;

		virtual	void			Render(const RenderContext& oRenderContext, const Matrix44& mModel) const;
		virtual	void			Draw() const;

		RenderTechnique&		GetRenderTechnique()					{ return m_oRenderTechnique; }

	protected:

		SPtr<MeshResource>		m_xResource;
		RenderTechnique			m_oRenderTechnique;

	private:

		Mesh(const Mesh&) FT_DELETED;
		Mesh& operator = (const Mesh&) FT_DELETED;
	};
}
