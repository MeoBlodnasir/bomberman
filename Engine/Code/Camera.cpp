
#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace ft
{
	Camera::Camera()
		: m_eProjectionType(E_PROJECTION_PERSPECTIVE)
		, m_fFov(0.f)
		, m_fNear(0.f)
		, m_fFar(0.f)
		, m_fWidth(0.f)
		, m_fRatio(0.f)
	{
	}

	Camera::~Camera()
	{
		FT_TEST(Destroy() == FT_OK);
	}

	ErrorCode	Camera::Create(const Camera::Desc* pDesc)
	{
		const Camera::Desc& oDesc = *pDesc;
		m_eProjectionType = oDesc.eProjectionType;
		m_fFov	 = oDesc.fFov;
		m_fNear	 = oDesc.fNear;
		m_fFar	 = oDesc.fFar;
		m_fWidth = oDesc.fWidth;
		m_fRatio = oDesc.fRatio;

		return FT_OK;
	}

	ErrorCode	Camera::Destroy()
	{
		return FT_OK;
	}

	void		Camera::MakeViewContext(ViewContext* pOutViewContext) const
	{
		FT_ASSERT(pOutViewContext != nullptr);
		pOutViewContext->mView = mWorldTransform;
		if (m_eProjectionType == E_PROJECTION_PERSPECTIVE)
			pOutViewContext->mProjection = glm::perspective(m_fFov, m_fRatio, m_fNear, m_fFar);
		else if (m_eProjectionType == E_PROJECTION_ORTHOGRAPHIC)
			pOutViewContext->mProjection = glm::ortho(-m_fWidth * 0.5f, m_fWidth * 0.5f, -m_fWidth / m_fRatio * 0.5f, m_fWidth / m_fRatio * 0.5f, m_fNear, m_fFar);
	}
}
