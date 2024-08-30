#include "RhythmGame.h"
#include "GameScene.h"
#include "LoadScore.h"
#include "Lane.h"
#include "Note.h"
#include "Skydome.h"
#include "JudgeTiming.h"

#include <Model.h>
#include <Audio.h>
#include <profileapi.h>
#include <cassert>
#include <Sprite.h>
#include <TextureManager.h>
#include <Windows.h>
#include <format>

RhythmGame::~RhythmGame()
{
    for (Note* note : pNoteList_) delete note;

    delete pLane_;
}

void RhythmGame::Initialize()
{
    /// インスタンス
    pInput_ = Input::GetInstance();
    pLane_ = new Lane();
    pLoadScore_ = new LoadScore();
    pAudio_ = Audio::GetInstance();
    pJudgeTiming_ = new JudgeTiming();

    /// セッター
    pJudgeTiming_->SetLane(pLane_);
    pJudgeTiming_->SetNoteList(&pNoteList_);

    /// 初期化処理
    pLane_->Initialize();
    pJudgeTiming_->Initialize();
    

    /// 背景画像の読み込み
    hBackgound_ = TextureManager::Load("tenkyurs/sky_sphere.png");
    /// 背景画像のスプライトを作成
    pSpriteBackGround_ = Sprite::Create(hBackgound_, {0,0});

    /// 各SEの読み込み
    hCagw_ = pAudio_->LoadWave("se/cagw.wav");
    hKick_ = pAudio_->LoadWave("se/kick.wav");
    hSnare_ = pAudio_->LoadWave("se/snare.wav");

    /// 楽譜データのロード
    sheetpath_ = "im_here.xml";
    sheetmusic_ = pLoadScore_->Load(sheetpath_);

    /// QPFの使用可否を取得
    bool qpfCompatible = QueryPerformanceFrequency(&mFreq_);
    assert(qpfCompatible && "QueryPerformanceFrequencyが非対応です");
    QueryPerformanceCounter(&mStart_);
    LARGE_INTEGER mNow;
    QueryPerformanceCounter(&mNow);
    elapsedTime_ = static_cast<double>(mNow.QuadPart - mStart_.QuadPart) / static_cast<double>(mFreq_.QuadPart);

    /// メインソングのロード(別スレッドにて)
    /* 楽譜データに楽曲パスが含まれているため楽譜データの読み込みより早く実行してはいけない */
    thread_loading_ = std::thread(&RhythmGame::LoadingThreadProcess, this);

    /// デバッグで使用するデータを登録
    pDebugOperationData_->pElapsedTime = &elapsedTime_;
    pDebugOperationData_->pNoteListSize = &notelistSize_;
    pDebugOperationData_->pSheetMusic = &sheetmusic_;
    pDebugOperationData_->pEnableMetronome = &enableMetronome_;
    pDebugOperationData_->pSheetPath = &sheetpath_;
    pDebugOperationData_->pIsReqReload = &requestReload_;
    pDebugOperationData_->pIsOpenPopupModal = &isOpenPopupModal_;
    pDebugOperationData_->pVolume = &musicVolume_;
    pDebugOperationData_->pIsChangeVolume = &isChangeMusicVolume_;
    pDebugOperationData_->pCountMeasure = &countMeasure_;
    pDebugOperationData_->pHitCount = &hitCount_;

    musicVolume_ = 0.05f;
    playTimingSec_ = 3.02;

    /* 0.005 ~ 0.020 */
    userSettingVelociT_ = 0.005f;

    /// 一小節目のデータを予め解読
    for (auto& sheet : sheetmusic_.sheetData)
        for (auto& symbols : sheet)
        {
            // {L,4} , {R,4} みたいに入ってく
            sheetDataQueue_.push(symbols);
        }
    
    nextNoteSymbol_ = sheetDataQueue_.front();
    sheetDataQueue_.pop();
}

void RhythmGame::Update()
{
    // 現在のカウント
    LARGE_INTEGER mNow;
    QueryPerformanceCounter(&mNow);
    // 経過時間計算
    elapsedTime_ =
        static_cast<double>(mNow.QuadPart - mStart_.QuadPart) / static_cast<double>(mFreq_.QuadPart);

    pLane_->Update();
    if (!isOpenPopupModal_)
    {
        if (pInput_->TriggerKey(DIK_F))
        {
            pAudio_->PlayWave(hKick_, false, 0.3f);
            Judge(Direction::Left);
        }
        if (pInput_->TriggerKey(DIK_J))
        {
            pAudio_->PlayWave(hKick_, false, 0.3f);
            Judge(Direction::Left);
        }
        if (pInput_->TriggerKey(DIK_D))
        {
            pAudio_->PlayWave(hSnare_, false, 0.3f);
            Judge(Direction::Right);
        }
        if (pInput_->TriggerKey(DIK_K))
        {
            pAudio_->PlayWave(hSnare_, false, 0.3f);
            Judge(Direction::Right);
        }

        if (pInput_->TriggerKey(DIK_H) || requestReload_)
        {
            HotReloadInitialize();
            requestReload_ = false;
        }
    }

    /// ボリューム設定
    if (isChangeMusicVolume_)
        pAudio_->SetVolume(playHandle_Song_, musicVolume_);

    /// ロードが終わったら初期化処理をし、スレッド終了を待機
    if (isEndLoad_)
    {
        playHandle_Song_ = pAudio_->PlayWave(hSong_, false, musicVolume_);
        playTimingSec_ = elapsedTime_;
        countBeat_ = 0u;
        countMeasure_ = 0u;
        countMeasureShifted_ = 0u;
        thread_loading_.join();
        isEndLoad_ = false;
    }

    // リズムゲーム更新
    UpdateRhythmGame();

    /// Update
    for (Note* note : pNoteList_)
    {
        note->Update();
    }

    /// Destroy
    pNoteList_.remove_if([](Note* _note)
        {
            return _note->GetIsDead();
        }
    );

    notelistSize_ = pNoteList_.size();
    pJudgeTiming_->Update();
}

void RhythmGame::DrawSpriteBackGround()
{
    pSpriteBackGround_->Draw();
}

void RhythmGame::Draw3D(const ViewProjection& _viewProjection)
{

    for (Note* note : pNoteList_)
    {
        note->Draw3D(_viewProjection);
    }
    // note->Draw3Dより先にpLane_->Draw3Dを走らせるとエラーでます
    pLane_->Draw3D(_viewProjection);
    pJudgeTiming_->Draw3D(_viewProjection);
}

void RhythmGame::MakeNote(Direction _beginLane)
{
    pNoteList_.push_back(new Note());
    Note* backNote = pNoteList_.back();
    backNote->Initialize();
    backNote->SetBeginLane(_beginLane);
    backNote->SetLaneData(pLane_->GetLaneData());
    backNote->SetUserSettingVelociT(&userSettingVelociT_);
    return;
}

void RhythmGame::UpdateRhythmGame()
{
    // 再生中チェック
    if (pAudio_->IsPlaying(playHandle_Song_)) isPlaying_ = true;
    else isPlaying_ = false;

    secNextBeat_ = 60.0 / static_cast<double>(sheetmusic_.bpm);

    if (!isPlaying_) enableMetronome_ = false;

    /// 譜面データを読みながらノートを作成
    MakeNoteFromSheet();
    
    // 4分インターバル
    if (elapsedTime_ - playTimingSec_ - sheetmusic_.offset * 0.001 >= secNextBeat_ * countBeat_)
    {
        if (countBeat_ % 4 == 0)
        {
            if (enableMetronome_)
                pAudio_->PlayWave(hCagw_, false, 0.5f);
            countMeasure_++; // 4/4で一小節インクリメント
        }
        // メトロノームクリック音
        if (enableMetronome_)
            pAudio_->PlayWave(hCagw_, false, 0.5f);
        countBeat_++;
    }

}

void RhythmGame::HotReloadInitialize()
{
    pAudio_->StopWave(playHandle_Song_);
    sheetmusic_ = pLoadScore_->Load(sheetpath_);
    
    thread_loading_ = std::thread(&RhythmGame::LoadingThreadProcess, this);

    // 初期化する
    sheetDataQueue_ = std::queue<std::pair<std::string, unsigned int>>();
    // 一小節目のデータを予め解読
    for (auto& sheet : sheetmusic_.sheetData)
        for (auto& symbols : sheet)
        {
            // {L,4} , {R,4} みたいに入ってく
            sheetDataQueue_.push(symbols);
        }

    nextNoteSymbol_ = sheetDataQueue_.front();
    sheetDataQueue_.pop();
}

void RhythmGame::MakeNoteFromSheet()
{
    if (nextNoteSymbol_.first.size() == 0) return;
    double shiftTime = 1.0 / static_cast<double>(userSettingVelociT_) / 60.0;
    double nBeatTime = secNextBeat_ * countBeatShifted_ - shiftTime;
    
    // 4分
    if (elapsedTime_ - playTimingSec_ - sheetmusic_.offset * 0.001 >= nBeatTime &&
        nextNoteSymbol_.second == 4u)
    {
        if (countBeatShifted_ % 4 == 0)
        {
            countMeasureShifted_++;
        }
        if (nextNoteSymbol_.first == "L")
        {
            MakeNote(Direction::Left);
            GetNextNoteSymbol();
        }
        else if (nextNoteSymbol_.first == "R")
        {
            MakeNote(Direction::Right);
            GetNextNoteSymbol();
        }
        else if (nextNoteSymbol_.first == "LR" || nextNoteSymbol_.first == "RL")
        {
            MakeNote(Direction::Left);
            MakeNote(Direction::Right);
            GetNextNoteSymbol();
        }
        if (elapsedTime_ - secPreBeat_ < secNextBeat_)
        {
            OutputDebugStringA(std::format("### INCORRECT TIMING ### [Time = {}]\n", elapsedTime_).c_str());
        }
        secPreBeat_ = elapsedTime_;
        countBeatShifted_++;
    }
}

void RhythmGame::GetNextNoteSymbol()
{
    if (sheetDataQueue_.size() > 0)
    {
        nextNoteSymbol_ = sheetDataQueue_.front();
        sheetDataQueue_.pop();
    }
    else
    {
        nextNoteSymbol_ = std::pair<std::string, unsigned int>();
    }

}

void RhythmGame::LoadingThreadProcess()
{
    timeBeginPeriod(1);

    const double minWaitTime = 3.0;
    double loadBeginTime = elapsedTime_;
    
    isEndLoad_ = false;
    hSong_ = pAudio_->LoadWave(sheetmusic_.sourcePath);
    
    double stopThreadTime = minWaitTime - (elapsedTime_ - loadBeginTime);
    OutputDebugStringA(std::format("stopThreadTime = {}\n", stopThreadTime).c_str());
    if (stopThreadTime > 0)
    {
        Sleep(static_cast<DWORD>(stopThreadTime * 1000));
    }

    isEndLoad_ = true;
    timeEndPeriod(1);
}

void RhythmGame::Judge(Direction _dir)
{
    HitResult hitResult = {};
    Note* note = pJudgeTiming_->Judge(_dir, hitResult);
    
    if (!note) return;

    switch (hitResult)
    {
    case HitResult::Perfect:
        hitCount_.perfect++;
        break;
    case HitResult::Great:
        hitCount_.great++;
        break;
    case HitResult::Bad:
        hitCount_.bad++;
        break;
    default:
        break;
    }
    note->SetIsDead();
}
