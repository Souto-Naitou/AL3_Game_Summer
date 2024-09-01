#include "Lane.h"
#include "Vector3/calc/vector3calc.h"
#include "JudgeTiming.h"

Lane::Lane()
{
	laneData_ = {};
	pPrimitiveDrawer_ = nullptr;
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
	laneData_.interval = 10.0f;
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
	laneData_.left[1].x = 0;
	laneData_.right[1].x = 0;

}

void Lane::Draw3D(const ViewProjection& _viewProjection)
{
	_viewProjection;
	pPrimitiveDrawer_->DrawLine3d(laneData_.left[0], laneData_.left[1], Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	pPrimitiveDrawer_->DrawLine3d(laneData_.right[0], laneData_.right[1], Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}
