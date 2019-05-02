
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

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void RotateCamera(D3DXVECTOR3);
	void MoveCamera(D3DXVECTOR3);

	void Render();
	void GetViewMatrix(D3DXMATRIX&);

	void RenderReflection(float);
	void GetReflectionViewMatrix(D3DXMATRIX&);

private:

	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX rotationMatrix;
	D3DXMATRIX m_reflectionViewMatrix;
};

#endif