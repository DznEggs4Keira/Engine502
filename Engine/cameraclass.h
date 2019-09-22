
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

// INCLUDES 

#include "d3dclass.h"

// MY CLASS INCLUDES 

#include "inputclass.h"

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void RotateCamera(XMFLOAT3);
	void MoveCamera(XMFLOAT3);

	void Render();
	XMMATRIX GetViewMatrix();

	void RenderReflection(float);
	XMMATRIX GetReflectionViewMatrix();

private:

	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMMATRIX m_viewMatrix;
	XMMATRIX rotationMatrix;
	XMMATRIX m_reflectionViewMatrix;
};

#endif