
#include "Graphics/Camera.hpp"

#include "Math/Vector3.hpp"
#include "Graphics/Renderer.hpp"

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
		FT_TEST_RETURN(SceneNode::Create(dynamic_cast<const SceneNode::Desc*>(pDesc)) == FT_OK, FT_FAIL);

		m_eProjectionType = pDesc->eProjectionType;
		m_fFov	 = pDesc->fFov;
		m_fNear	 = pDesc->fNear;
		m_fFar	 = pDesc->fFar;
		m_fWidth = pDesc->fWidth;
		m_fRatio = pDesc->fRatio;

		return FT_OK;
	}

	ErrorCode	Camera::Destroy()
	{
		return SceneNode::Destroy();
	}

	void		Camera::MakeViewContext(ViewContext* pOutViewContext) const
	{
		FT_ASSERT(pOutViewContext != nullptr);

		pOutViewContext->vViewPosition = Vector3(GetWorldTransform()[3]);
		pOutViewContext->mView = glm::inverse(GetWorldTransform());

		if (m_eProjectionType == E_PROJECTION_PERSPECTIVE)
			pOutViewContext->mProjection = glm::perspective(m_fFov, m_fRatio, m_fNear, m_fFar);
		else if (m_eProjectionType == E_PROJECTION_ORTHOGRAPHIC)
			pOutViewContext->mProjection = glm::ortho(-m_fWidth * 0.5f, m_fWidth * 0.5f, -m_fWidth / m_fRatio * 0.5f, m_fWidth / m_fRatio * 0.5f, m_fNear, m_fFar);
	}
}
