#pragma once

#include "ViewProjection.h"
#include "input/Input.h"
#include "EnumClass.h"
#include "SheetMusic.h"
#include "ImGuiWindow.h"
#include "define.h"

#include <list>
#include <queue>
#include <thread>

class GameScene;
class LoadScore;
class Lane;
class Note;
class Audio;
class Skydome;
class Model;
class Sprite;
class JudgeTiming;

/// <summary>
/// リズムゲーム
/// </summary>
class RhythmGame
{
    /* < About class >
    ** レーンやノーツなどのリズムゲームオブジェクトを所有するクラス
    */

public:

    ~RhythmGame();

    void Initialize();
    void Update();
    void DrawSprite();
    void DrawSpriteBackGround();
    void Draw3D(const ViewProjection& _viewProjection);

    void SetGameScene(GameScene* _gameScene) { pGameScene_ = _gameScene; }
    void SetDebugOperationData(ImGuiWindow::DebugOperationData* _dod) { pDebugOperationData_ = _dod; }
    //float GetUserSettingVelociT() { return userSettingVelociT_; }

private:

    Lane*               pLane_                  = nullptr;      // !< レーン
    std::list<Note*>    pNoteList_              = {};           // !< ノートリスト
    Audio*              pAudio_                 = nullptr;      // !< オーディオ
    Input*              pInput_                 = nullptr;      // !< インプットオブジェクト
    GameScene*          pGameScene_             = nullptr;      // !< ゲームシーンポインタ（借りてくる）
    LoadScore*          pLoadScore_             = nullptr;      // !< 譜面ローダー
    JudgeTiming*        pJudgeTiming_           = nullptr;      // !< タイミング判定クラス
    Sprite*             pSpriteBackGround_      = nullptr;      // !< 背景スプライト
    std::string         sheetpath_              = {};           // !< 譜面パス
    SheetMusic          sheetmusic_             = {};           // !< 譜面
    uint32_t            hCagw_                  = 0u;           // !< cagwheel handle
    uint32_t            hSong_                  = 0u;           // !< song handle
    uint32_t            playHandle_Song_        = 0u;           // !< 再生ハンドル
    uint32_t            hKick_                  = 0u;           // !< キック
    uint32_t            hSnare_                 = 0u;           // !< スネア
    uint32_t            hBackgound_             = 0u;           // !< 背景スプライト
    LARGE_INTEGER       mFreq_                  = {};           // !< 1秒間のカウント数
    LARGE_INTEGER       mStart_                 = {};           // !< スタート時のカウント数
    double              playTimingSec_          = {};           // !< 再生が始まったタイミング
    double              elapsedTime_            = {};           // !< 経過時間
    double              elapsedBeat_            = {};           // !< 拍をうってからの経過時間
    size_t              notelistSize_           = {};           // !< ノートリストのサイズ (デバッグ用)
    bool                enableMetronome_        = true;         // !< メトロノームが有効か
    bool                isPlaying_              = false;        // !< 再生中か
    bool                requestReload_          = false;        // !< リロードが要求されたか
    bool                isOpenPopupModal_       = false;        // !< ポップアップが開いているかどうか
    bool                isChangeMusicVolume_    = {};           // !< ボリューム変更検出用フラグ
    unsigned int        countBeat_              = 0u;           // !< 拍回数
    unsigned int        countBeatShifted_       = 0u;           // !< シフト後の拍カウント
    unsigned int        countMeasure_           = 0u;           // !< 小節数カウント
    unsigned int        countMeasureShifted_    = 0u;           // !< シフト後の小節数カウント
    float               musicVolume_            = {};           // !< 音楽ボリューム
    double              secNextBeat_            = 0.0;          // !< 拍と拍の間隔
    std::thread         thread_loading_         = {};           // !< 読み込み用スレッド
    bool                isEndLoad_              = false;        // !< 読み込み完了したか
    float               userSettingVelociT_     = 0.0f;         // !< ユーザーが設定した速度t
    HitCount            hitCount_               = {};           // !< ヒット回数カウント

    ImGuiWindow::DebugOperationData* pDebugOperationData_ = nullptr; // !< デバッグ操作データ (借りてくる)
    std::queue<std::pair<std::string, unsigned int>> sheetDataQueue_ = {}; // !< 譜面データキュー
    std::pair<std::string, unsigned int> nextNoteSymbol_ = {}; // !< 次のノート記号 (sheetキューから取ってくる)

    double              secPreBeat_             = 0.0;          // !< 1つ前の拍の時間（デバッグ用）

    /// 関数
    void MakeNote(Direction _beginLane);
    void UpdateRhythmGame();
    void HotReloadInitialize();
    void MakeNoteFromSheet();
    void GetNextNoteSymbol();
    void LoadingThreadProcess();
    void Judge(Direction _dir);
};