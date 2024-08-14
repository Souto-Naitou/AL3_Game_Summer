#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

class Camera
{
public:
	Camera();
	~Camera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3 _worldPosition, const Vector3 _rotate);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ビュープロジェクションの取得
	/// </summary>
	/// <returns>ViewProjection リファレンス</returns>
	ViewProjection& GetViewProjection() { return viewProjection_; }

	WorldTransform worldTransform_;
private:
	ViewProjection viewProjection_;
};