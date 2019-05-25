#pragma once
#include "Entity.h"
#include "Matrix4x4.h"
#include "Vec3.h"
#include "AABBCollider.h"

struct GLFWwindow;

class Camera final : public Entity
{
public:
	Camera(Entity* const target, const Vec3& offset);
	virtual void Update();
	~Camera();

	matrix::Matrix4x4 LookAt		(const vecs::Vec3& eye, const vecs::Vec3& at, const vecs::Vec3& up);
	matrix::Matrix4x4 Perspective	(const float fov, const float aspRatio, const float near, const float far);
	matrix::Matrix4x4 Orthographic	(const float width, const float height, const float near, const float far);

	inline void RebuildView()			{ BuildView(); }
	inline void RebuildPerspective()	{ BuildPerspective(); }
	inline void RebuildOrthographic()	{ BuildOrthographic(); }

	void ApplyAccelerationToTarget();
	void ApplyTransformationToTarget();
	void CenterToTargetWithOffset();
	void Clean();

	virtual void Jump(world::World* world);

	void MoveForward();
	void MoveBackward();
	void StrafeLeft();
	void StrafeRight();
	void MoveUp();
	void MoveDown();
	void Sprint();

#pragma region Getters/Setters
	inline Entity* GetTarget() { return m_target; }

	inline const	vecs::Vec3&			GetPosition()				const	{ return m_eye; }
	inline const	matrix::Matrix4x4&	GetPerspective()			const	{ return m_prsp; }
	inline const	matrix::Matrix4x4&	GetOrthographic()			const	{ return m_ortho; }
	inline const	matrix::Matrix4x4&	GetView()					const	{ return m_view; }
	inline const	vecs::Vec3&			GetUp()						const	{ return m_up; }

	inline float	GetFOV()										const	{ return m_fov; }
	
	inline void		SetFOV					(const float fov)				{ if (m_fov >= 0) m_fov = fov; }
	inline void		SetPosition				(const vecs::Vec3& newPos)		{ m_eye = newPos; }
	inline void		SetLocalPosition		(const vecs::Vec3& newPos)		{ m_transform.SetPosition(newPos); }
#pragma endregion
	
private:
	void BuildView();
	void BuildPerspective();
	void BuildOrthographic();

private:
	Entity*					m_target;			//GameObject the camera will follow
	vecs::Vec3				m_offset;			//Offset position from the target
	vecs::Vec3				m_direction;        //At - Eye
	float					m_horizontalAngle;
	float					m_verticalAngle;

	matrix::Matrix4x4 		m_view;				// View matrix
	matrix::Matrix4x4 		m_prsp; 			// Perspective matrix
	matrix::Matrix4x4		m_ortho;			// Orthographic matrix

	vecs::Vec3 				m_eye; 				// Position
	vecs::Vec3 				m_at;				// Target
	vecs::Vec3 				m_up;				// Gives camera rotation

	float 					m_fov;				// Field of view
	float					m_aspRatio;			// Aspect ratio
	float 					m_near; 			// Z of near plane
	float 					m_far;				// Z of far plane
	float					m_inv_fsubn;		// (far - near)^-1
									
	// Used for orthographic projection exclusively
	float					m_orthoR;			// 'right'	(prism width / 2)
	float					m_orthoH;			// 'top'	(prism height / 2)

	float					m_scrollSpeed;
};
