#pragma once
#include "math.h"

class FrustumCulling
{
public:
	FrustumCulling(const Matrix4f& viewProj)
	{
		update(viewProj);
	}

	void update(const Matrix4f& viewProj)
	{
		viewProj.calcClipPlanes(m_leftClipPlane,
								m_rightClipPlane,
								m_bottomClipPlane,
								m_topClipPlane,
								m_nearClipPlane,
								m_farClipPlane);
	}

	bool isPointInsideViewFrustum(const Vector3f& p) const
	{
		Vector4f p4D(p, 1.0f);

		bool inside =
			(m_leftClipPlane.dot(p4D) >= 0) &&
			(m_rightClipPlane.dot(p4D) <= 0) &&
			//  (m_topClipPlane.dot(p4D)    <= 0) &&
			//  (m_bottomClipPlane.dot(p4D) >= 0) &&
			(m_nearClipPlane.dot(p4D) >= 0) &&
			(m_farClipPlane.dot(p4D) <= 0);

		return inside;
	}

private:

	Vector4f m_leftClipPlane;
	Vector4f m_rightClipPlane;
	Vector4f m_bottomClipPlane;
	Vector4f m_topClipPlane;
	Vector4f m_nearClipPlane;
	Vector4f m_farClipPlane;
};
