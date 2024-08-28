#pragma once
#include "Sprite.h"
#include "input/Input.h"
#include "GameScene.h"

class TitleScene
{
public:

    void Initialize();
    void Update();
    void DrawSpriteFront();
    void SetGameScene(GameScene* _gameScene) { gameScene_ = _gameScene; };

private:

    unsigned int handle_titleScene = 0u;
    unsigned int handle_title = 0u;
    unsigned int handle_white = 0u;

    Sprite* sprite_titleScene;
    Sprite* sprite_title;
    Sprite* sprite_white;

    Input* pInput = nullptr;
    GameScene* gameScene_ = nullptr;

};