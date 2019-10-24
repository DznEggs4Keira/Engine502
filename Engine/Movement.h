#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_

#include <DirectXMath.h>
#include <math.h>

using namespace DirectX;

class Movement
{
public:
	Movement();
	Movement(const Movement&);
	~Movement();

	void SetPosition() { m_position = XMFLOAT3(50.0f, 1.0f, 25.0f); }
	XMFLOAT3 GetRotation() { return m_rotation; }
	XMFLOAT3 GetPosition() { return m_position; }

	void SetFrameTime(float);

	void MoveForward(bool);
	void MoveBackward(bool);
	void MoveUpward(bool);
	void MoveDownward(bool);
	void RotateCamera(XMFLOAT3);
	void Rotate(const float, const float);

private:
	XMFLOAT3 m_position, m_rotation;

	float m_frameTime;

	float m_forwardSpeed, m_backwardSpeed;
	float m_upwardSpeed, m_downwardSpeed;
};
#endif
