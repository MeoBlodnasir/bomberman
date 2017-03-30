#pragma once

#include "Handled.hpp"
#include "MeshData.hpp"

namespace ft
{
	// A considérer comme une ressource GPU ?
	// Dans le sens où il ne faudrait pas la charger sur le GPU plusieurs fois,
	// ni la décharger tant qu'elle est utilisée.
	// Faire la même distinction qu'entre Image et Texture.
	class Mesh : public Handled, public CountableSPtr
	{
	public:

				Mesh();
		virtual ~Mesh();

		virtual ErrorCode	Create(const SPtr<MeshData>& xMeshData);
		virtual ErrorCode	Destroy() override;

		virtual bool		IsValid() const;

		virtual void		Draw() const;

	protected:

		SPtr<MeshData>	m_xMeshData;

		uint32	m_iVbo;
		uint32	m_iEbo;

	private:

		Mesh(const Mesh&) FT_DELETED;
		Mesh& operator = (const Mesh&) FT_DELETED;
	};
}
