#include "camera.h"
#include "Matrix4x4/calc/matrix4calc.h"
#include "define.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Initialize(const Vector3 _worldPosition, const Vector3 _rotate)
{
	worldTransform_.Initialize();
	worldTransform_.translation_ = _worldPosition;
	worldTransform_.rotation_ = _rotate;
	worldTransform_.matWorld_ = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, worldTransform_.rotation_, worldTransform_.translation_);
	viewProjection_.Initialize();
	viewProjection_.matProjection = MakePerspectiveFovMatrix(
		viewProjection_.fovAngleY, static_cast<float>(kScreenWidth) / static_cast<float>(kScreenHeight), viewProjection_.nearZ, viewProjection_.farZ
	);
}

void Camera::Update()
{
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	worldTransform_.UpdateMatrix();
}
