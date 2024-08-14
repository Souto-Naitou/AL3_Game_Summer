#include "camera.h"
#include "Matrix4x4/calc/matrix4calc.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Initialize(const Vector3 _worldPosition, const Vector3 _rotate)
{
	worldTransform_.translation_ = _worldPosition;
	worldTransform_.rotation_ = _rotate;
	viewProjection_.Initialize();
}

void Camera::Update()
{
	worldTransform_.matWorld_ = MakeAffineMatrix({1.0f, 1.0f, 1.0f}, worldTransform_.rotation_, worldTransform_.translation_);
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
}
