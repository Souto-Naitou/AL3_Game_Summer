#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Lane.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "ImGuiWindow.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
    /// <summary>
    /// コンストクラタ
    /// </summary>
    GameScene();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~GameScene();

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();

    /// <summary>
    /// 毎フレーム処理
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

private: // メンバ変数
    DirectXCommon*  dxCommon_ = nullptr;
    Input*          input_ = nullptr;
    Audio*          audio_ = nullptr;
    ViewProjection  viewProjection_;
    WorldTransform  worldTransform_;
    Lane*           pLane_ = nullptr;
    Camera*         pCamera = nullptr;
    DebugCamera*    pDebugCamera_ = nullptr;
    ImGuiWindow*    imguiWindow_ = nullptr;
    ImGuiWindow::DebugOperationData debugOperationData_ = {};

    bool            enableDebugCamera = false;

    /// <summary>
    /// ゲームシーン用
    /// </summary>
};
