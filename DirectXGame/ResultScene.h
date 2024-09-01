#pragma once
#include "Sprite.h"
#include "input/Input.h"
#include "GameScene.h"
#include <Audio.h>
#include "Vector2.h"
#include <vector>

class ResultScene
{
public:

    void Initialize();
    void Update();
    void DrawSpriteFront();
    void SetGameScene(GameScene* _gameScene) { pGameScene_ = _gameScene; };
    void SetHitCount(HitCount _hitCount) { hitCount_ = _hitCount; }

private:
    const unsigned int  kDuration_resultString   = 120u;
    const unsigned int  kDuration_perfect        = 80u;
    const unsigned int  kDuration_great          = 100u;
    const unsigned int  kDuration_bad            = 120u;


    unsigned int        handle_resultString     = 0u;
    unsigned int        handle_songname         = 0u;
    unsigned int        handle_perfect          = 0u;
    unsigned int        handle_great            = 0u;
    unsigned int        handle_bad              = 0u;
    unsigned int        handle_white            = 0u;
    unsigned int        handle_numbers          = 0u;

    unsigned int        countFrame              = 0u;

    Sprite*             sprite_resultString;
    Sprite*             sprite_songname;
    Sprite*             sprite_perfect;
    Sprite*             sprite_great;
    Sprite*             sprite_bad;
    Sprite*             sprite_white;
    Sprite*             sprite_numbers;

    Input*              pInput                  = nullptr;
    GameScene*          pGameScene_              = nullptr;
    Audio*              pAudio                  = nullptr;

    Vector2             position_resultString_  = {};
    Vector2             position_songname_      = {};
    Vector2             position_perfect_       = {};
    Vector2             position_great_         = {};
    Vector2             position_bad_           = {};
    Vector2             position_hitPerfect_    = {};
    Vector2             position_hitGreat_      = {};
    Vector2             position_hitBad_        = {};

    Vector2             numLeftTopPosition_[10] = {};

    float               time_resultString_      = 0.0f;
    float               time_perfect_           = 0.0f;
    float               time_great_             = 0.0f;
    float               time_bad_               = 0.0f;
    float               easeTime_resultString_  = 0.0f;
    float               easeTime_perfect_       = 0.0f;
    float               easeTime_great_         = 0.0f;
    float               easeTime_bad_           = 0.0f;

    HitCount            hitCount_               = {};        // ヒット回数 (RhythmGameSceneからコピーす)

    std::vector<unsigned int> cut_perfect_      = {};
    std::vector<unsigned int> cut_great_        = {};
    std::vector<unsigned int> cut_bad_          = {};
    std::vector<Sprite*> sprites_numPerfect_    = {};
    std::vector<Sprite*> sprites_numGreat_      = {};
    std::vector<Sprite*> sprites_numBad_        = {};

    std::vector<unsigned int> CutNumber(unsigned int _number);
};