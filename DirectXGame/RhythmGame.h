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
    void DrawSpriteBackGround();
    void Draw3D(const ViewProjection& _viewProjection);

    void SetGameScene(GameScene* _gameScene) { pGameScene_ = _gameScene; }
    HitCount GetHitCount() { return hitCount_; }

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
    uint32_t            hPiNigo_                = 0u;           // !< 濁ったピッ
    uint32_t            hBackgound_             = 0u;           // !< 背景スプライト
    uint32_t            hKeyF_                  = 0u;           // !< Fキーハンドル
    uint32_t            hKeyJ_                  = 0u;           // !< Dキーハンドル
    Sprite*             sprite_F_               = nullptr;
    Sprite*             sprite_J_               = nullptr;
    LARGE_INTEGER       mFreq_                  = {};           // !< 1秒間のカウント数
    LARGE_INTEGER       mStart_                 = {};           // !< スタート時のカウント数
    double              playTimingSec_          = {};           // !< 再生が始まったタイミング
    double              elapsedTime_            = {};           // !< 経過時間
    double              elapsedTimeShifted_     = {};           // !< いろいろシフトした経過時間
    double              elapsedMakeNote         = {};           // !< 拍をうってからの経過時間
    double              elapsedFrameCount_      = {};           // !< フレームレート用経過時間
    size_t              notelistSize_           = {};           // !< ノートリストのサイズ (デバッグ用)
    bool                enableMetronome_        = true;         // !< メトロノームが有効か
    bool                isPlaying_              = false;        // !< 再生中か
    bool                requestReload_          = false;        // !< リロードが要求されたか
    bool                isOpenPopupModal_       = false;        // !< ポップアップが開いているかどうか
    bool                isChangeMusicVolume_    = {};           // !< ボリューム変更検出用フラグ
    bool                isEndLoad_              = false;        // !< 読み込み完了したか
    unsigned int        countBeat_              = 0u;           // !< 拍回数
    BeatCount           beatCount_              = {};           // !< 拍回数カウンタ
    unsigned int        countMeasure_           = 0u;           // !< 小節数カウント
    unsigned int        countMeasureShifted_    = 0u;           // !< シフト後の小節数カウント
    float               musicVolume_            = {};           // !< 音楽ボリューム
    BeatDuration        beatDuration_           = {};           // !< 長さの定義
    std::thread         thread_loading_         = {};           // !< 読み込み用スレッド
    double              userSettingVelociT_     = 0.0f;         // !< ユーザーが設定した速度t
    HitCount            hitCount_               = {};           // !< ヒット回数カウント
    unsigned int        frameCount_             = 0u;           // !< フレーム数
    double              deltaTime_              = 0;            // !< デルタ時間
    double              fps_                    = 57;           // !< フレームレート

    std::queue<std::pair<std::string, unsigned int>> sheetDataQueue_ = {}; // !< 譜面データキュー
    std::pair<std::string, unsigned int> nextNoteSymbol_ = {}; // !< 次のノート記号 (sheetキューから取ってくる)

    /// 関数
    void MakeNote(Direction _beginLane, double _startT);
    void UpdateRhythmGame();
    void HotReloadInitialize();
    void MakeNoteFromSheet();
    void GetNextNoteSymbol();
    void LoadingThreadProcess();
    void Judge(Direction _dir);
    double CalculateElapsedTime();
    void JudgeSymbolAndMakeNote(double _errorTime);
};