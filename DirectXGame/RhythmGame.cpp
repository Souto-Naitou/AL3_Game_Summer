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

    pAudio_->StopWave(playHandle_Song_);
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
  
    /// ロード
    hKeyF_ = TextureManager::Load("rhythm/F.png");
    hKeyJ_ = TextureManager::Load("rhythm/J.png");
    sprite_F_ = Sprite::Create(hKeyF_, { 426,600 }, { 1,1,1,1 }, { 0.5f,0.5f });
    sprite_J_ = Sprite::Create(hKeyJ_, { 853,600 }, { 1,1,1,1 }, { 0.5f,0.5f });

    /// 背景画像の読み込み
    hBackgound_ = TextureManager::Load("tenkyurs/sky_sphere.png");
    /// 背景画像のスプライトを作成
    pSpriteBackGround_ = Sprite::Create(hBackgound_, {0,0});

    /// 各SEの読み込み
    hCagw_ = pAudio_->LoadWave("se/cagw.wav");
    hKick_ = pAudio_->LoadWave("se/kick.wav");
    hSnare_ = pAudio_->LoadWave("se/snare.wav");
    hPiNigo_ = pAudio_->LoadWave("se/pinigo.wav");

    /// 楽譜データのロード
    sheetpath_ = "Boukyousei_Signal.xml";
    sheetmusic_ = pLoadScore_->Load(sheetpath_);

    /// QPFの使用可否を取得
    bool qpfCompatible = QueryPerformanceFrequency(&mFreq_);
    qpfCompatible;
    assert(qpfCompatible && "QueryPerformanceFrequencyが非対応です");
    QueryPerformanceCounter(&mStart_);
    LARGE_INTEGER mNow;
    QueryPerformanceCounter(&mNow);
    elapsedTime_ = static_cast<double>(mNow.QuadPart - mStart_.QuadPart) / static_cast<double>(mFreq_.QuadPart);

    /// メインソングのロード(別スレッドにて)
    /* 楽譜データに楽曲パスが含まれているため楽譜データの読み込みより早く実行してはいけない */
    thread_loading_ = std::thread(&RhythmGame::LoadingThreadProcess, this);

    musicVolume_ = 0.3f;
    playTimingSec_ = 5.02;

    /* 0.005 ~ 0.020 */
    userSettingVelociT_ = 0.008;

    /// データを予め解読
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
    CalculateElapsedTime();
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
            pAudio_->PlayWave(hSnare_, false, 0.3f);
            Judge(Direction::Right);
        }
        if (pInput_->TriggerKey(DIK_D))
        {
            pAudio_->PlayWave(hKick_, false, 0.3f);
            Judge(Direction::Left);
        }
        if (pInput_->TriggerKey(DIK_K))
        {
            pAudio_->PlayWave(hSnare_, false, 0.3f);
            Judge(Direction::Right);
        }
    }

    /// フレームレート計算
    if (elapsedTime_ - elapsedFrameCount_ >= 2.0)
    {
        fps_ = frameCount_ * 1.0 / (elapsedTime_ - elapsedFrameCount_);

        frameCount_ = 0;
        elapsedFrameCount_ = elapsedTime_;
    }
    frameCount_++;

    /// ボリューム設定
    if (isChangeMusicVolume_)
        pAudio_->SetVolume(playHandle_Song_, musicVolume_);

    /// ロードが終わったら初期化処理をし、スレッド終了を待機
    if (isEndLoad_)
    {
        playHandle_Song_ = pAudio_->PlayWave(hSong_, false, musicVolume_);
        isPlaying_ = true;
        playTimingSec_ = CalculateElapsedTime();
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
        bool ispi = note->GetIsPi();
        if (!ispi)
        {
            if (note->GetTimeLane() >= 1.0f)
            {
                note->SetIsPi();
            }
        }
        if (note->GetIsDead() && note->GetTimeLane() >= 1.45f)
        {
            hitCount_.bad++;
        }
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
    sprite_F_->Draw();
    sprite_J_->Draw();
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

void RhythmGame::MakeNote(Direction _beginLane, double _startT)
{
    pNoteList_.push_back(new Note());
    Note* backNote = pNoteList_.back();
    backNote->Initialize();
    backNote->SetBeginLane(_beginLane);
    backNote->SetLaneData(pLane_->GetLaneData());
    backNote->SetUserSettingVelociT(&userSettingVelociT_);
    backNote->SetTimeLane(_startT);
    return;
}

void RhythmGame::UpdateRhythmGame()
{
    // 再生中チェック
    if (elapsedTime_ - playTimingSec_ >= sheetmusic_.length * 0.001) 
    {
        pGameScene_->ReserveSceneChange(Scenes::Result);
    }
    

    beatDuration_.quarter   = 60.0 / static_cast<double>(sheetmusic_.bpm);
    beatDuration_.eighth    = 60.0 / static_cast<double>(sheetmusic_.bpm) * 0.5;
    beatDuration_.sixteenth = 60.0 / static_cast<double>(sheetmusic_.bpm) * 0.25;

    if (!isPlaying_) enableMetronome_ = false;

    /// 譜面データを読みながらノートを作成
    MakeNoteFromSheet();
    
    // 4分インターバル
    if (CalculateElapsedTime() - playTimingSec_ - sheetmusic_.offset * 0.001 >= beatDuration_.quarter * countBeat_)
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
    double shiftTime        = 1.0 / userSettingVelociT_ / fps_; // 3.33333
    double quarterTime      = beatDuration_.quarter     *   beatCount_.quarterCount   -     shiftTime;
    double eighthTime       = beatDuration_.eighth      *   beatCount_.eighthCount    -     shiftTime;
    double sixteenthTime    = beatDuration_.sixteenth   *   beatCount_.sixteenthCount -     shiftTime;
    double errorTime        = 0;
    double startFixedErrorT = 0;
    elapsedTimeShifted_ = CalculateElapsedTime() - playTimingSec_ - sheetmusic_.offset * 0.001;

    /// 4分
    if (elapsedTimeShifted_ >= quarterTime && nextNoteSymbol_.second == 4u)
    {
        errorTime = elapsedTimeShifted_ - quarterTime;
        startFixedErrorT = errorTime / shiftTime;
        if (beatCount_.quarterCount % 4 == 0)
        {
            countMeasureShifted_++;
        }
        if (nextNoteSymbol_.first == "L")
        {
            elapsedMakeNote = elapsedTime_;
            MakeNote(Direction::Left, startFixedErrorT);
            GetNextNoteSymbol();
        }
        else if (nextNoteSymbol_.first == "R")
        {
            MakeNote(Direction::Right, startFixedErrorT);
            GetNextNoteSymbol();
        }
        else if (nextNoteSymbol_.first == "LR" || nextNoteSymbol_.first == "RL")
        {
            MakeNote(Direction::Left, startFixedErrorT);
            MakeNote(Direction::Right, startFixedErrorT);
            GetNextNoteSymbol();
        }
        else if (nextNoteSymbol_.first == "_")
        {
            GetNextNoteSymbol();
        }
        beatCount_.quarterCount++;
        beatCount_.eighthCount += 2;
        beatCount_.sixteenthCount += 4;
    }
    /// 8分
    else if (elapsedTimeShifted_ >= eighthTime && nextNoteSymbol_.second == 8u)
    {
        errorTime = elapsedTimeShifted_ - eighthTime;
        startFixedErrorT = errorTime / shiftTime;

        JudgeSymbolAndMakeNote(startFixedErrorT);

        beatCount_.eighthCount++;
        beatCount_.sixteenthCount += 2;
        if (beatCount_.eighthCount % 2 == 0) beatCount_.quarterCount++;
    }
    /// 16分
    else if (elapsedTimeShifted_ >= sixteenthTime && nextNoteSymbol_.second == 16u)
    {
        errorTime = elapsedTimeShifted_ - sixteenthTime;
        startFixedErrorT = errorTime / shiftTime;

        JudgeSymbolAndMakeNote(startFixedErrorT);

        beatCount_.sixteenthCount++;
        if (beatCount_.sixteenthCount % 4 == 0) beatCount_.quarterCount++;
        if (beatCount_.sixteenthCount % 2 == 0) beatCount_.eighthCount++;
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

    const double minWaitTime = 5.0;
    double loadBeginTime = CalculateElapsedTime();
    
    isEndLoad_ = false;
    hSong_ = pAudio_->LoadWave(sheetmusic_.sourcePath);
    
    double stopThreadTime = minWaitTime - (CalculateElapsedTime() - loadBeginTime);
    OutputDebugStringA(std::format("stopThreadTime = {}\n", stopThreadTime).c_str());
    if (stopThreadTime > 0)
    {
        Sleep(static_cast<DWORD>(stopThreadTime * 1000));
    }

    OutputDebugStringA(std::format("Thread Elapsed Time = {}\n", CalculateElapsedTime() - loadBeginTime).c_str());

    isEndLoad_ = true;
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

double RhythmGame::CalculateElapsedTime()
{
    // 現在のカウント
    LARGE_INTEGER mNow;
    QueryPerformanceCounter(&mNow);
    elapsedTime_ =
        static_cast<double>(mNow.QuadPart - mStart_.QuadPart) / static_cast<double>(mFreq_.QuadPart);
    return elapsedTime_;
}

void RhythmGame::JudgeSymbolAndMakeNote(double _startFixedErrorT)
{
    if (nextNoteSymbol_.first == "L")
    {
        MakeNote(Direction::Left, _startFixedErrorT);
        GetNextNoteSymbol();
    }
    else if (nextNoteSymbol_.first == "R")
    {
        MakeNote(Direction::Right, _startFixedErrorT);
        GetNextNoteSymbol();
    }
    else if (nextNoteSymbol_.first == "LR" || nextNoteSymbol_.first == "RL")
    {
        MakeNote(Direction::Left, _startFixedErrorT);
        MakeNote(Direction::Right, _startFixedErrorT);
        GetNextNoteSymbol();
    }
    else if (nextNoteSymbol_.first == "_")
    {
        GetNextNoteSymbol();
    }
}
