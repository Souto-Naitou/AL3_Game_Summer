#include "TitleScene.h"
#include "TextureManager.h"
#include "Vector4.h"


void TitleScene::Initialize()
{
	handle_titleScene = TextureManager::Load("TitleScene/titleScene.png");
	handle_title = TextureManager::Load("TitleScene/title.png");
	handle_white = TextureManager::Load("white1x1.png");
	
	sprite_titleScene = Sprite::Create(handle_titleScene, Vector2(0, 0));
	sprite_titleScene->SetSize({ 297, 50 });
	sprite_title = Sprite::Create(handle_title, Vector2(640, 200), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f));
	sprite_white = Sprite::Create(handle_white, Vector2(0, 0));
	sprite_white->SetSize({ 1280,720 });
	pInput = Input::GetInstance();
}

void TitleScene::Update()
{
	if (pInput->TriggerKey(DIK_P))
	{
		gameScene_->ReserveSceneChange(Scenes::RhythmGame);
	}
}

void TitleScene::DrawSpriteFront()
{
	sprite_white->Draw();
	sprite_titleScene->Draw();
	sprite_title->Draw();
}
