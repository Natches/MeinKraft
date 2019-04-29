#include "Camera.h"
#include "INISettings.h"
#include "GLFW\glfw3.h"
#include "MathLib.h"
#include "Inputs.h"
#include "GameTime.h"
#include "INISettings.h"
#include "Ray.h"

Camera::Camera(Entity* const target, const Vec3& offset)
	: Entity(entity::E_ENTITY_TYPE::CAMERA), m_target(target), m_offset(offset)
{
	m_scrollSpeed		= INISettings::cameraAngularSpeed;
	m_speed				= INISettings::playerMovementSpeed;
	m_horizontalAngle	= 0;
	m_verticalAngle		= 0;
	m_speedModifier		= 1;
	m_jumpSpeed			= 100.f;
}

void Camera::Update()
{
	//Get Angles
	m_horizontalAngle	+= (INISettings::windowWidth/2.f - Inputs::GetMousePosition().x) * m_scrollSpeed * GameTime::Instance()->GetDeltaTime();
	m_verticalAngle		-= (INISettings::windowHeight/2.f - Inputs::GetMousePosition().y) * m_scrollSpeed * GameTime::Instance()->GetDeltaTime();
	
	//Compute Angle
	m_verticalAngle		= mathlib::Clamp(-89.f, 89.f, m_verticalAngle);
	m_horizontalAngle	= (float)fmod(m_horizontalAngle, 360.f);

	//Set Rotation	
	m_transform.SetRotation({ m_verticalAngle, m_horizontalAngle, 0.f });

	NormalizeAccelerationWithSpeed();

	m_speedModifier = 1.f;
}

#pragma region View matrix
matrix::Matrix4x4 Camera::LookAt(const vecs::Vec3& eye, const vecs::Vec3& at, const vecs::Vec3& up)
{
	m_eye = eye;
	m_at = at;
	m_up = up;

	BuildView();
	return m_view;
}

void Camera::BuildView()
{
	vecs::Vec3 zc = mathlib::Normalize(m_eye - m_at);
	vecs::Vec3 xc = mathlib::Normalize(mathlib::Cross(m_up, zc));
	vecs::Vec3 yc = mathlib::Cross(zc, xc);

	m_view[0][0] = xc.x;
	m_view[1][0] = xc.y;
	m_view[2][0] = xc.z;
	m_view[3][0] = mathlib::Dot(-m_eye, xc);

	m_view[0][1] = yc.x;
	m_view[1][1] = yc.y;
	m_view[2][1] = yc.z;
	m_view[3][1] = mathlib::Dot(-m_eye, yc);

	m_view[0][2] = zc.x;
	m_view[1][2] = zc.y;
	m_view[2][2] = zc.z;
	m_view[3][2] = mathlib::Dot(-m_eye, zc);
}
#pragma endregion

#pragma region Perspective Projection
matrix::Matrix4x4 Camera::Perspective(const float fov, const float aspRatio, const float near, const float far)
{
	m_fov		= fov;
	m_aspRatio	= aspRatio;
	m_near		= near;
	m_far		= far;
	m_inv_fsubn = 1 / (m_far - m_near);

	BuildPerspective();
	return m_prsp;
}

void Camera::BuildPerspective()
{
	float tangent = mathlib::tan((m_fov / 2.f));

	m_prsp[0][0] = 1.f / (tangent * m_aspRatio);
	m_prsp[0][1] = 0.f;
	m_prsp[0][2] = 0.f;
	m_prsp[0][3] = 0.f;

	m_prsp[1][0] = 0.f;
	m_prsp[1][1] = 1.f / tangent;
	m_prsp[1][2] = 0.f;
	m_prsp[1][3] = 0.f;

	m_prsp[2][0] = 0.f;
	m_prsp[2][1] = 0.f;
	m_prsp[2][2] = -(m_far + m_near) * m_inv_fsubn;
	m_prsp[2][3] = -1.f;

	m_prsp[3][0] = 0.f;
	m_prsp[3][1] = 0.f;
	m_prsp[3][2] = -2.f * m_far * m_near * m_inv_fsubn;
	m_prsp[3][3] = 0.f;
}
#pragma endregion

#pragma region Orthographic Projection
matrix::Matrix4x4 Camera::Orthographic(const float width, const float height, const float near, const float far)
{
	m_orthoR = width * 0.5f;
	m_orthoH = height * 0.5f;
	m_near = near;
	m_far = far;
	m_inv_fsubn = 1 / (m_far - m_near);

	BuildOrthographic();
	return m_ortho;
}

void Camera::BuildOrthographic()
{
	m_ortho[0][0] = 1.f / m_orthoR;
	m_ortho[0][1] = 0.f;
	m_ortho[0][2] = 0.f;
	m_ortho[0][3] = 0.f;

	m_ortho[1][0] = 0.f;
	m_ortho[1][1] = 1.f / m_orthoH;
	m_ortho[1][2] = 0.f;
	m_ortho[1][3] = 0.f;

	m_ortho[2][0] = 0.f;
	m_ortho[2][1] = 0.f;
	m_ortho[2][2] = -2 * m_inv_fsubn;
	m_ortho[2][3] = 0.f;

	m_ortho[3][0] = 0.f;
	m_ortho[3][1] = 0.f;
	m_ortho[3][2] = -(m_far + m_near) * m_inv_fsubn;
	m_ortho[3][3] = 1.f;
}
#pragma endregion

void Camera::ApplyAccelerationToTarget()
{
	if (m_target)
	{
		m_target->SetAcceleration(m_acceleration);
		m_acceleration = vecs::Vec3::Zero();
	}
}

void Camera::ApplyTransformationToTarget()
{
	if (m_target)
		m_target->GetTransform().SetPosition(m_transform.GetPosition() - m_offset);
}

void Camera::CenterToTargetWithOffset()
{
	m_transform.SetPosition(m_target->GetTransform().GetPosition() + m_offset);
}

void Camera::Clean()
{

}

Camera::~Camera()
{
	Clean();
}

void Camera::MoveForward()
{
	m_acceleration += vecs::Vec3(m_transform.GetForward().x, 0.f, m_transform.GetForward().z);
}

void Camera::MoveBackward()
{
	m_acceleration += -vecs::Vec3(m_transform.GetForward().x, 0.f, m_transform.GetForward().z);
}

void Camera::StrafeLeft()
{
	m_acceleration += vecs::Vec3(m_transform.GetRight().x, 0.f, m_transform.GetRight().z);
}

void Camera::StrafeRight()
{
	m_acceleration += -vecs::Vec3(m_transform.GetRight().x, 0.f, m_transform.GetRight().z);
}

void Camera::MoveUp()
{
	m_acceleration += Vec3::Up();
}

void Camera::MoveDown()
{
	m_acceleration += -Vec3::Up();
}

void Camera::Sprint()
{
	m_speedModifier = 2.f;
}

void Camera::Jump(world::World* world)
{
	if (m_target->IsGrounded(world))
	{
		m_target->AddVerticalVelocity(vecs::Vec3::Up() * m_jumpSpeed);
	}
}