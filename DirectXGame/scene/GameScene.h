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
#include "Note.h"
#include "RhythmGame.h"
#include "EnumClass.h"



#include <list>

class TitleScene;


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

    /// <summary>
    /// シーンを変えるフェーズ
    /// </summary>
    void ChangeScene();

private: // メンバ変数
    DirectXCommon*      dxCommon_           = nullptr;
    Input*              input_              = nullptr;
    Audio*              audio_              = nullptr;
    ViewProjection      viewProjection_;
    WorldTransform      worldTransform_;

    Camera*             pCamera             = nullptr;
    DebugCamera*        pDebugCamera_       = nullptr;
    ImGuiWindow*        imguiWindow_        = nullptr;
    RhythmGame*         pRhythmGame_        = nullptr;
    TitleScene*         pTitleScene_        = nullptr;

    Scenes              reserveScene_       = Scenes::Title;
    Scenes              currentScene_       = Scenes::Title;
    ImGuiWindow::DebugOperationData debugOperationData_ = {};

    bool                enableDebugCamera   = false;

public:
    /// <summary> 
    /// シーンチェンジを予約する
    /// </summary>
    void ReserveSceneChange(Scenes _scene);

    /// <summary>
    /// ゲームシーン用
    /// </summary>
};
