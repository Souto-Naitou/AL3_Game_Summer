#include "Note.h"
#include <cassert>
#include "TextureManager.h"
#include "Vector3/calc/vector3calc.h"
#include <format>

Note::Note()
{
}

Note::~Note()
{
    delete pModel_;
}

void Note::Initialize()
{
    worldTransform_.Initialize();
    pModel_ = Model::Create();
    modelTexture_ = TextureManager::Load("white1x1.png");
    worldTransform_.scale_ = { 0.5f, 0.1f, 0.1f };
}

void Note::Update()
{
    /// 早期リターン
    if (!laneData_)
    {
        assert(0 && "レーンデータが初期化されていません。");
        return;
    }

    /// 処理
    if (countUpdate_ == 0u) // Updateが最初に呼び出されたときのみ実行
    {
        if (beginLane_ == Direction::Left)
        {
            // 左レーンの始点を代入
            laneBegin_ = laneData_->left[0];
            laneEnd_ = laneData_->left[1];
            worldTransform_.translation_ = laneBegin_;
        }
        else if (beginLane_ == Direction::Right)
        {
            // 右レーンの始点を代入
            laneBegin_ = laneData_->right[0];
            laneEnd_ = laneData_->right[1];
            worldTransform_.translation_ = laneBegin_;
        }
        else
        {
            assert(0 && "beginLane_の値が不正です。");
            return;
        }
        OutputDebugStringA(std::format("first t = {}\n", time_Lane_).c_str());
    }
    
    // tに従って移動する
    worldTransform_.translation_ = (1.0f - static_cast<float>(time_Lane_)) * laneBegin_ + static_cast<float>(time_Lane_) * laneEnd_;

    if (time_Lane_ >= 1.5f) isDead_ = true;

    time_Lane_ += *userSettingVelociT_;
    worldTransform_.UpdateMatrix();
    countUpdate_++;
}

void Note::Draw3D(const ViewProjection& _viewProjection)
{
    // ノートをレーンに沿って描画
    pModel_->Draw(worldTransform_, _viewProjection, modelTexture_);
}

void Note::SetLaneData(Lane::LaneData* _laneData)
{
    laneData_ = _laneData;
}

void Note::SetBeginLane(Direction _dir)
{
    beginLane_ = _dir;
}
