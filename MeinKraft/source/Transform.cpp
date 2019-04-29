#include "Transform.h"
#include "Quaternion.h"
#include "GameObject.h"

Transform::Transform(GameObject& gameObject)
	: m_gameObject(gameObject)
{
	m_isDirty = true;
	m_scale = { 1.f, 1.f, 1.f };
}

Transform::~Transform()
{

}

void Transform::BuildGlobal()
{
	m_isDirty = false;
	m_global =	CreatePositionMatrix(m_position) *
				rotation::CreateRotationQuaternion(m_rotation).AsMatrix() *
				CreateScaleMatrix(m_scale);
}

const Matrix4x4& Transform::GetGlobal()
{
	if (m_isDirty) { BuildGlobal(); }
	return m_global;
}