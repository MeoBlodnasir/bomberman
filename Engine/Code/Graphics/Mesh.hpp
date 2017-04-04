#pragma once

#include "Core/Handled.hpp"
#include "Math/Matrix44.hpp"
#include "Graphics/MeshResource.hpp"
#include "Graphics/RenderTechnique.hpp"

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
