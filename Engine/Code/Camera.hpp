#pragma once

#include "Core.hpp"
#include "ErrorCode.hpp"
#include "StrongPointer.hpp"
#include "Matrix44.hpp"
#include "SceneNode.hpp"

namespace ft
{
	// fw
	struct ViewContext;

	enum EProjectionType
	{
		E_PROJECTION_PERSPECTIVE,
		E_PROJECTION_ORTHOGRAPHIC
	};

	class Camera : public SceneNode
	{
	public:

		struct Desc : public SceneNode::Desc
		{
			Desc()
				: SceneNode::Desc()
				, eProjectionType(E_PROJECTION_ORTHOGRAPHIC)
				, fFov(0.f)
				, fNear(0.f)
				, fFar(0.f)
				, fWidth(0.f)
				, fRatio(0.f)
			{}

			EProjectionType	eProjectionType;
			float			fFov;
			float			fNear;
			float			fFar;
			float			fWidth;
			float			fRatio;
		};

		Camera();
		virtual ~Camera();

		virtual	ErrorCode		Create(const Camera::Desc* pDesc);
		virtual	ErrorCode		Destroy();

		inline	EProjectionType	GetProjectionType() const					{ return m_eProjectionType; }
		inline	float			GetFieldOfView() const						{ return m_fFov; }
		inline	float			GetNearPlaneDistance() const				{ return m_fNear; }
		inline	float			GetFarPlaneDistance() const					{ return m_fFar; }
		inline	float			GetWidth() const							{ return m_fWidth; }
		inline	float			GetHeight() const							{ return m_fWidth / m_fRatio; }
		inline	float			GetRatio() const							{ return m_fRatio; }

		inline	void			SetProjectionType(EProjectionType eType)	{ m_eProjectionType = eType; }
		inline	void			SetFieldOfView(float fFov)					{ m_fFov = fFov; }
		inline	void			SetPlaneDistances(float fNear, float fFar)	{ m_fNear = fNear; m_fFar = fFar; }
		inline	void			SetDimensions(float fWidth, float fRatio)	{ m_fWidth = fWidth; m_fRatio = fRatio; }

				void			MakeViewContext(ViewContext* pOutViewContext) const;

	protected:

		EProjectionType		m_eProjectionType;
		float				m_fFov;
		float				m_fNear;
		float				m_fFar;
		float				m_fWidth;
		float				m_fRatio;

	private:

		Camera(const Camera&) FT_DELETED;
		Camera& operator = (const Camera&) FT_DELETED;
	};
}
