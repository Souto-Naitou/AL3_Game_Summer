#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Lane.h"
#include "EnumClass.h"

class Note
{

public:
    Note();
    ~Note();

    void Initialize();
    void Update();
    void Draw3D(const ViewProjection& _viewProjection);

    /// <summary>
    /// レーン情報を取得
    /// </summary>
    /// <param name="_laneData">レーンデータ ポインタ</param>
    void SetLaneData(Lane::LaneData* _laneData);
    /// <summary>
    /// 左右どちらのレーンに配置するか
    /// </summary>
    /// <param name="_dir">方向</param>
    void SetBeginLane(Direction _dir);
    
    bool GetIsDead() { return isDead_; }

    Direction   GetBeginLane() { return beginLane_; }
    float       GetTimeLane() { return time_Lane_; }
    void        SetIsTapped() { isTapped_ = true; }
    void        SetUserSettingVelociT(float* _userSettingVel) { userSettingVelociT_ = _userSettingVel; };

private:
    // 自クラスが所有しているデータ
    WorldTransform      worldTransform_     = {};               // !< ワールド変換クラス
    Model*              pModel_             = nullptr;          // !< ノートモデル
    float               time_Lane_          = 0.0f;             // !< レーン上の媒介変数t
    bool                isDead_             = false;            // !< 死亡フラグ
    Direction           beginLane_          = Direction::Left;  // !< 始まるレーン
    unsigned int        modelTexture_       = 0u;               // !< モデルのテクスチャ
    Vector3             laneBegin_          = {};               // !< レーン始点
    Vector3             laneEnd_            = {};               // !< レーン終点
    bool                isTapped_           = false;            // !< タップされた

    // 別クラスから借りるデータ
    float*              userSettingVelociT_ = nullptr;          // !< ユーザーが設定した速度
    Lane::LaneData*     laneData_           = nullptr;          // !< レーン情報
};