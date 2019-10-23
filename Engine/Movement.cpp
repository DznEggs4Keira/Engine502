#include "Movement.h"

Movement::Movement()
{
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_frameTime = 0.0f;

	m_forwardSpeed = 0.0f;
	m_backwardSpeed = 0.0f;
	m_upwardSpeed = 0.0f;
	m_downwardSpeed = 0.0f;

}

Movement::Movement(const Movement& other)
{
}

Movement::~Movement()
{
}

void Movement::SetFrameTime(float time)
{
	m_frameTime = time;
}

void Movement::MoveForward(bool keydown)
{
	float radians;

	// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_forwardSpeed += m_frameTime * 0.001f;

		if (m_forwardSpeed > (m_frameTime * 0.03f))
		{
			m_forwardSpeed = m_frameTime * 0.03f;
		}
	}

	else
	{
		m_forwardSpeed -= m_frameTime * 0.0007f;

		if (m_forwardSpeed < 0.0f)
		{
			m_forwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = m_rotation.y * 0.0174532925f;

	// Update the position.
	m_position.x += sinf(radians) * m_forwardSpeed;
	m_position.z += cosf(radians) * m_forwardSpeed;
}
void Movement::MoveBackward(bool keydown)
{
	float radians;

	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_backwardSpeed += m_frameTime * 0.001f;

		if (m_backwardSpeed > (m_frameTime * 0.03f))
		{
			m_backwardSpeed = m_frameTime * 0.03f;
		}
	}

	else
	{
		m_backwardSpeed -= m_frameTime * 0.0007f;

		if (m_backwardSpeed < 0.0f)
		{
			m_backwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = m_rotation.y * 0.0174532925f;

	// Update the position.
	m_position.x -= sinf(radians) * m_backwardSpeed;
	m_position.z -= cosf(radians) * m_backwardSpeed;
}

void Movement::RotateCamera(XMFLOAT3 rotation)
{
	m_rotation.x += rotation.y;
	m_rotation.y += rotation.x;
	m_rotation.z += rotation.z;

	//check if the rotation is greater than 0 then add 360 and if the angle exceeds 360 then reduce. FOR THE X AXIS
	if (m_rotation.x < 0.f)
		m_rotation.x = +360.0f;
	else if (m_rotation.x > 360.0f)
		m_rotation.x -= 360.0f;

	//check if the rotation is greater than 0 then add 360 and if the angle exceeds 360 then reduce. FOR THE Y AXIS
	if (m_rotation.y < 0.0f)
		m_rotation.y += 360.0f;
	else if (m_rotation.y > 360.0f)
		m_rotation.y -= 360.0f;

}

void Movement::MoveUpward(bool keydown)
{
	// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_upwardSpeed += m_frameTime * 0.003f;

		if (m_upwardSpeed > (m_frameTime * 0.03f))
		{
			m_upwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_upwardSpeed -= m_frameTime * 0.002f;

		if (m_upwardSpeed < 0.0f)
		{
			m_upwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_position.y += m_upwardSpeed;
}
void Movement::MoveDownward(bool keydown)
{
	// Update the downward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_downwardSpeed += m_frameTime * 0.003f;

		if (m_downwardSpeed > (m_frameTime * 0.03f))
		{
			m_downwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_downwardSpeed -= m_frameTime * 0.002f;

		if (m_downwardSpeed < 0.0f)
		{
			m_downwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_position.y -= m_downwardSpeed;
}