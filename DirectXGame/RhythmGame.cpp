#include "RhythmGame.h"
#include "GameScene.h"
#include "LoadScore.h"
#include "Lane.h"
#include "Note.h"
#include "Skydome.h"
#include <Model.h>
#include <Audio.h>
#include <profileapi.h>
#include <cassert>
#include <Sprite.h>
#include <TextureManager.h>

RhythmGame::~RhythmGame()
{
    for (Note* note : pNoteList_) delete note;

    delete pLane_;
}

void RhythmGame::Initialize()
{
    // インスタンスを取得
    pInput_ = Input::GetInstance();
    pLane_ = new Lane();
    pLane_->Initialize();
    pLoadScore_ = new LoadScore();
    pAudio_ = Audio::GetInstance();
    pSkydome_ = new Skydome();
    pModelSkydome_ = Model::CreateFromOBJ("tenkyurs", true);
    pSkydome_->Initialize(pModelSkydome_);
    hBackgound_ = TextureManager::Load("tenkyurs/sky_sphere.png");
    pSpriteBackGround_ = Sprite::Create(hBackgound_, {0,0});
    hCagw_ = pAudio_->LoadWave("se/cagw.wav");
    hKick_ = pAudio_->LoadWave("se/kick.wav");
    hSnare_ = pAudio_->LoadWave("se/snare.wav");
    sheetpath_ = "im_here.xml";
    sheetmusic_ = pLoadScore_->Load(sheetpath_);
    thread_loading_ = std::thread(&RhythmGame::LoadingThreadProcess, this);
    bool qpfCompatible = QueryPerformanceFrequency(&mFreq_);
    assert(qpfCompatible && "QueryPerformanceFrequencyが非対応です");
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

    elapsedTime_ = -100.0;
    musicVolume_ = 0.05f;

    userSettingVelociT_ = 0.01f;

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

void RhythmGame::Update()
{
    pLane_->Update();
    if (!isOpenPopupModal_)
    {
        if (pInput_->TriggerKey(DIK_F) || pInput_->TriggerKey(DIK_J))
        {
            pAudio_->PlayWave(hKick_, false, 0.3f);
        }
        if (pInput_->TriggerKey(DIK_D) || pInput_->TriggerKey(DIK_K))
        {
            pAudio_->PlayWave(hSnare_, false, 0.3f);
        }
        if (pInput_->TriggerKey(DIK_H) || requestReload_)
        {
            HotReloadInitialize();
            requestReload_ = false;
        }
    }

    // ボリューム設定
    if (isChangeMusicVolume_)
        pAudio_->SetVolume(playHandle_Song_, musicVolume_);

    // ロードが終わったら初期化処理をし、スレッド終了を待機
    if (isEndLoad_)
    {
        playHandle_Song_ = pAudio_->PlayWave(hSong_, false, musicVolume_);
        QueryPerformanceCounter(&mStart_);
        countBeat_ = 0u;
        countBeatShifted_ = 0u;
        countMeasure_ = 0u;
        countMeasureShifted_ = 0u;
        thread_loading_.join();
        isEndLoad_ = false;
    }

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
}

void RhythmGame::MakeNote(Direction _beginLane)
{
    pNoteList_.push_back(new Note());
    Note* backNote = pNoteList_.back();
    backNote->Initialize();
    backNote->SetBeginLane(_beginLane);
    backNote->SetLaneData(pLane_->GetLaneData());
    return;
}

void RhythmGame::UpdateRhythmGame()
{
    // 再生中チェック
    if (pAudio_->IsPlaying(playHandle_Song_)) isPlaying_ = true;
    else isPlaying_ = false;

    if (!isPlaying_) enableMetronome_ = false;
    else
    {
        secNextBeat_ = 60.0 / static_cast<double>(sheetmusic_.bpm);

        // 現在のカウント
        LARGE_INTEGER mNow;
        QueryPerformanceCounter(&mNow);
        // 経過時間計算
        elapsedTime_ =
            static_cast<double>(mNow.QuadPart - mStart_.QuadPart) / static_cast<double>(mFreq_.QuadPart);
        elapsedTime_ -= static_cast<double>(sheetmusic_.offset) * 0.001;
    }

    /// 譜面データを読みながらノートを作成
    MakeNoteFromSheet();
    
    // 4分インターバル
    if (elapsedTime_ >= secNextBeat_ * countBeat_)
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
    elapsedTime_ = -100.0;
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
    // 4分
    if (elapsedTime_ >= (secNextBeat_ * countBeatShifted_) - shiftTime && nextNoteSymbol_.second == 4u)
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
    isEndLoad_ = false;
    hSong_ = pAudio_->LoadWave(sheetmusic_.sourcePath);
    isEndLoad_ = true;
}
