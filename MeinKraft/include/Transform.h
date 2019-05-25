#pragma once
#include "MathLib.h"
#include "Vec3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

using namespace vecs;
using namespace matrix;

class GameObject;

class Transform final
{
#pragma region Non-Static Attributes
private:
	matrix::Matrix4x4	m_global;
	vecs::Vec3			m_position;
	vecs::Vec3			m_rotation;
	vecs::Vec3			m_scale;
	GameObject&			m_gameObject;
	bool				m_isDirty;
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
public:
	Transform() = delete;
	Transform(GameObject& gameObject);
	virtual ~Transform();

	virtual void Update() {}

private:
	void BuildGlobal();
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
public:
	inline GameObject&		GetGameObject()						{ return m_gameObject; }
	inline void				SetPosition (const Vec3& position)	{ m_position = position; m_isDirty = true; }
	inline void				SetRotation	(const Vec3& rotation)	{ m_rotation = rotation; m_isDirty = true; }
	inline void				SetScale	(const Vec3& scale)		{ m_scale = scale; m_isDirty = true; }
	inline const Vec3&		GetPosition()	const				{ return m_position; }
	inline const Vec3&		GetRotation()	const				{ return m_rotation; }
	inline const Vec3&		GetScale ()		const				{ return m_scale; }
	inline const Vec3		GetForward()						{ return mathlib::Normalize(vecs::Vec3(rotation::CreateRotationQuaternion(m_rotation).AsMatrix() * Vec4::Forward())); }
	inline const Vec3		GetUp()								{ return mathlib::Normalize(vecs::Vec3(rotation::CreateRotationQuaternion(m_rotation).AsMatrix() * Vec4::Up())); }
	inline const Vec3		GetRight()							{ return mathlib::Normalize(vecs::Vec3(rotation::CreateRotationQuaternion(m_rotation).AsMatrix() * Vec4::Right())); }
	inline void				AddPosition(const Vec3& position)	{ m_position += position; m_isDirty = true; }
	inline void				AddRotation(const Vec3& rotation)	{ m_rotation += rotation; m_isDirty = true; }
	inline void				AddScale   (const Vec3& scale)		{ m_scale += scale; m_isDirty = true; }
	const Matrix4x4&		GetGlobal();
#pragma endregion
};