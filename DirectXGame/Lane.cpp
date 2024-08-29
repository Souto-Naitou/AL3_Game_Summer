#include "Lane.h"
#include "Vector3/calc/vector3calc.h"

Lane::Lane()
{
	laneData_ = {};
	pPrimitiveDrawer_ = nullptr;
	for (auto& perf : perfectRange_) perf = {};
	for (auto& grea : greatRange_) grea = {};
}
Lane::~Lane()
{

}

void Lane::Initialize()
{
	// ワールド変換 初期化
	worldTransform_.Initialize();

	// laneDataの初期化
	laneData_.direction = Vector3(0.0f, 0.0f, -1.0f); // -z方向を前に
	laneData_.interval = 5.0f;
	laneData_.length = 40.0f;

	// モデル生成
	//pModel_ = Model::CreateFromOBJ("", true);

	// プリミティブドロワー取得
	pPrimitiveDrawer_ = PrimitiveDrawer::GetInstance();
}

void Lane::Update()
{
	/// レーン位置計算
	laneData_.left[0].x = -1.0f * laneData_.interval * 0.5f;
	laneData_.right[0].x = 1.0f * laneData_.interval * 0.5f;
	laneData_.left[1] = laneData_.left[0];
	laneData_.right[1] = laneData_.right[0];
	laneData_.left[1] = Add(laneData_.left[0], Multiply(laneData_.length, laneData_.direction));
	laneData_.right[1] = Add(laneData_.right[0], Multiply(laneData_.length, laneData_.direction));


	/// デバッグ用
	float time = 0.99f;
	perfectRange_[0] = (1.0f - time) * laneData_.left[0] + time * laneData_.left[1];
	perfectRange_[1] = (1.0f - time) * laneData_.right[0] + time * laneData_.right[1];
	time = 1.01f;
	perfectRange_[2] = (1.0f - time) * laneData_.left[0] + time * laneData_.left[1];
	perfectRange_[3] = (1.0f - time) * laneData_.right[0] + time * laneData_.right[1];
}

void Lane::Draw3D(const ViewProjection& _viewProjection)
{
	_viewProjection;
	pPrimitiveDrawer_->DrawLine3d(laneData_.left[0], laneData_.left[1], Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	pPrimitiveDrawer_->DrawLine3d(laneData_.right[0], laneData_.right[1], Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	/// デバッグ用
	pPrimitiveDrawer_->DrawLine3d(perfectRange_[0], perfectRange_[1], Vector4(0.2f, 0.2f, 1.0f, 1.0f));
	pPrimitiveDrawer_->DrawLine3d(perfectRange_[2], perfectRange_[3], Vector4(0.2f, 0.2f, 1.0f, 1.0f));
}
