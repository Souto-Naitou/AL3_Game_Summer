#include "ResultScene.h"
#include <TextureManager.h>
#include "Phill.h"

void ResultScene::Initialize()
{
	pInput = Input::GetInstance();
	pAudio = Audio::GetInstance();
    handle_resultString = TextureManager::Load("result/result.png");
    handle_songname = TextureManager::Load("result/songname.png");
    handle_perfect = TextureManager::Load("result/perfect.png");
    handle_great = TextureManager::Load("result/great.png");
    handle_bad = TextureManager::Load("result/bad.png");
    handle_white = TextureManager::Load("white1x1.png");
    handle_numbers = TextureManager::Load("result/numbers.png");
    sprite_resultString = Sprite::Create(handle_resultString, position_resultString_);
    sprite_songname = Sprite::Create(handle_songname, position_resultString_);
    sprite_perfect = Sprite::Create(handle_perfect, position_resultString_);
    sprite_great = Sprite::Create(handle_great, position_resultString_);
    sprite_bad = Sprite::Create(handle_bad, position_resultString_);
    sprite_white = Sprite::Create(handle_white, { 0,0 });
    position_resultString_ = { 237.0f - 1280.0f, 137.0f };
    position_songname_ = { 664.0f + 1280.0f, 196.0f };
    position_perfect_ = { 244.0f, 348.0f + 720.0f };
    position_great_ = { 272.0f, 404.0f + 720.0f };
    position_bad_ = { 304.0f, 470.0f + 720.0f };
    for (int i = 0; i < 10; i++)
    {
        numLeftTopPosition_[i].x = 21.2f * i;
    }
    hitCount_.perfect = 120;
    hitCount_.great = 345;
    hitCount_.bad = 6789;
}

void ResultScene::Update()
{
    const Vector2 kPosition_resultString_ = { 237, 137 };
    const Vector2 kPosition_songname_ = { 664, 196 };
    const Vector2 kPosition_perfect_ = { 244, 348 };
    const Vector2 kPosition_great_ = { 272, 404 };
    const Vector2 kPosition_bad_ = { 304, 470 };
    const Vector2 kPosition_hitPerfect = { 410, kPosition_perfect_.y };
    const Vector2 kPosition_hitGreat = { 410, kPosition_great_.y };
    const Vector2 kPosition_hitBad = { 410, kPosition_bad_.y };

    if (pInput->TriggerKey(DIK_SPACE))
    {
        pGameScene_->ReserveSceneChange(Scenes::Title);
    }

    if (countFrame == 0)
    {
        cut_perfect_ = CutNumber(hitCount_.perfect);
        cut_great_ = CutNumber(hitCount_.great);
        cut_bad_ = CutNumber(hitCount_.bad);

        for (int i = 0; i < cut_perfect_.size(); i++)
        {
            sprites_numPerfect_.push_back(Sprite::Create(handle_numbers, {}));
            sprites_numPerfect_.back()->SetPosition({ kPosition_hitPerfect.x + 21.2f * i , kPosition_hitPerfect.y + 720.0f });
            sprites_numPerfect_.back()->SetSize({ 21.2f, 24.0f });
            sprites_numPerfect_.back()->SetTextureRect(numLeftTopPosition_[cut_perfect_[i]], { 21.2f, 24.0f });
            position_hitPerfect_.y = kPosition_hitPerfect.y + 720.0f;
        }
        for (int i = 0; i < cut_great_.size(); i++)
        {
            sprites_numGreat_.push_back(Sprite::Create(handle_numbers, {}));
            sprites_numGreat_.back()->SetPosition({ kPosition_hitGreat.x + 21.2f * i , kPosition_hitGreat.y + 720.0f });
            sprites_numGreat_.back()->SetSize({ 21.2f, 24.0f });
            sprites_numGreat_.back()->SetTextureRect(numLeftTopPosition_[cut_great_[i]], { 21.2f, 24.0f });
            position_hitGreat_.y = kPosition_hitGreat.y + 720.0f;
        }
        for (int i = 0; i < cut_bad_.size(); i++)
        {
            sprites_numBad_.push_back(Sprite::Create(handle_numbers, {}));
            sprites_numBad_.back()->SetPosition({ kPosition_hitBad.x + 21.2f * i , kPosition_hitBad.y + 720.0f });
            sprites_numBad_.back()->SetSize({ 21.2f, 24.0f });
            sprites_numBad_.back()->SetTextureRect(numLeftTopPosition_[cut_bad_[i]], { 21.2f, 24.0f });
            position_hitBad_.y = kPosition_hitBad.y + 720.0f;
        }
    }

    /// リザルト
    if (countFrame >= 0 && countFrame <= kDuration_resultString)
    {
        time_resultString_ = static_cast<float>(countFrame) / static_cast<float>(kDuration_resultString);
        easeTime_resultString_ = Phill::EaseOutQuart(time_resultString_);
        position_resultString_ = Vector2(237.0f - 1280.0f, 137.0f) * (1.0f - easeTime_resultString_) + kPosition_resultString_ * easeTime_resultString_;
        position_songname_ = Vector2(664.0f + 1280.0f, 196.0f) * (1.0f - easeTime_resultString_) + kPosition_songname_ * easeTime_resultString_;
    }
    /// perfect
    if (countFrame >= (kDuration_resultString - 80u) &&
        countFrame <= kDuration_perfect + (kDuration_resultString - 80u))
    {
        time_perfect_ = static_cast<float>(countFrame - (kDuration_resultString - 80u)) / kDuration_perfect;
        easeTime_perfect_ = Phill::EaseOutQuart(time_perfect_);
        position_perfect_ = Vector2(244.0f, 348.0f + 720.0f) * (1.0f - easeTime_perfect_) + kPosition_perfect_ * easeTime_perfect_;
        position_hitPerfect_ = Vector2(410.0f, 348.0f + 720.0f) * (1.0f - easeTime_perfect_) + kPosition_hitPerfect * easeTime_perfect_;
    }
    /// great
    if (countFrame >= (kDuration_resultString - 80u) &&
        countFrame <= kDuration_great + (kDuration_resultString - 80u))
    {
        time_great_ = static_cast<float>(countFrame - (kDuration_resultString - 80u)) / kDuration_great;
        easeTime_great_ = Phill::EaseOutQuart(time_great_);
        position_great_ = Vector2(272.0f, 404.0f + 720.0f) * (1.0f - easeTime_great_) + kPosition_great_ * easeTime_great_;
        position_hitGreat_ = Vector2(410.0f, 404.0f + 720.0f) * (1.0f - easeTime_great_) + kPosition_hitGreat * easeTime_great_;
    }
    /// bad
    if (countFrame >= (kDuration_resultString - 80u) &&
        countFrame <= kDuration_bad + (kDuration_resultString - 80u))
    {
        time_bad_ = static_cast<float>(countFrame - (kDuration_resultString - 80u)) / kDuration_bad;
        easeTime_bad_ = Phill::EaseOutQuart(time_bad_);
        position_bad_ = Vector2(304.0f, 470.0f + 720.0f) * (1.0f - easeTime_bad_) + kPosition_bad_ * easeTime_bad_;
        position_hitBad_ = Vector2(410.0f, 470.0f + 720.0f) * (1.0f - easeTime_bad_) + kPosition_hitBad * easeTime_bad_;
    }

    sprite_resultString->SetPosition(position_resultString_);
    sprite_songname->SetPosition(position_songname_);
    sprite_perfect->SetPosition(position_perfect_);
    sprite_great->SetPosition(position_great_);
    sprite_bad->SetPosition(position_bad_);
    sprite_white->SetSize(Vector2(1280, 720));
    for (Sprite* numPer : sprites_numPerfect_) numPer->SetPosition({ numPer->GetPosition().x, position_hitPerfect_.y });
    for (Sprite* numGre : sprites_numGreat_) numGre->SetPosition({ numGre->GetPosition().x, position_hitGreat_.y });
    for (Sprite* numBad : sprites_numBad_) numBad->SetPosition({ numBad->GetPosition().x, position_hitBad_.y });

    countFrame++;
}

void ResultScene::DrawSpriteFront()
{
    sprite_white->Draw();
    sprite_resultString->Draw();
    sprite_songname->Draw();
    sprite_perfect->Draw();
    sprite_great->Draw();
    sprite_bad->Draw();
    for (Sprite* numPer : sprites_numPerfect_) numPer->Draw();
    for (Sprite* numGre : sprites_numGreat_) numGre->Draw();
    for (Sprite* numBad : sprites_numBad_) numBad->Draw();
}

std::vector<unsigned int> ResultScene::CutNumber(unsigned int _number)
{
    std::vector<unsigned int> result;

    bool isProcess = false;
    while (_number || !isProcess)
    {
        unsigned int singleDigit = 0u;
        singleDigit = _number % 10u;
        _number -= singleDigit;
        _number /= 10u;
        result.insert(result.begin(), singleDigit);
        isProcess = true;
    }

    return result;
}
