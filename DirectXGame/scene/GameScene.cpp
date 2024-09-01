#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include "ImGuiWindow.h"
#include "TitleScene.h"
#include "define.h"
#include <Windows.h>
#include "ResultScene.h"

GameScene::GameScene()
{
	timeBeginPeriod(1);
}

GameScene::~GameScene() 
{
	delete pDebugCamera_;
	timeEndPeriod(1);
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	viewProjection_.Initialize();
	worldTransform_.Initialize();
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
	pCamera = new Camera();
	pCamera->Initialize({ 0.0f, 3.4f, -43.5f }, { 0.47f, 0.0f, 0.0f });
	//pCamera->Initialize({ 0.0f, 13.8f, -50.45f }, { 0.85f, 0.0f, 0.0f });
	pDebugCamera_ = new DebugCamera(kScreenWidth, kScreenHeight);
	imguiWindow_ = new ImGuiWindow();
	imguiWindow_->SetDebugOperationData(&debugOperationData_);

	debugOperationData_.pCameraRotation = &pCamera->worldTransform_.rotation_;
	debugOperationData_.pCameraTranslation = &pCamera->worldTransform_.translation_;

	//currentScene_ = Scenes::Title;
	//pTitleScene_ = new TitleScene();
	//pTitleScene_->Initialize();
	//pTitleScene_->SetGameScene(this);
	//pRhythmGame_ = new RhythmGame();
	//pRhythmGame_->SetGameScene(this);
	//pRhythmGame_->SetDebugOperationData(&debugOperationData_);
	//pRhythmGame_->Initialize();
	pResultScene_ = new ResultScene();
	pResultScene_->SetGameScene(this);
	pResultScene_->Initialize();
}

void GameScene::Update()
{
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_O))
	{
		enableDebugCamera = enableDebugCamera ? false : true;
	}
#endif // _DEBUG

	imguiWindow_->DebugWindowDraw();

	pDebugCamera_->Update();
	pCamera->Update();

	switch (currentScene_)
	{
	case Scenes::Title:
		pTitleScene_->Update();
		break;
	case Scenes::Select:
		break;
	case Scenes::RhythmGame:
		pRhythmGame_->Update();
		break;
	case Scenes::Result:
		pResultScene_->Update();
		break;
	default:
		break;
	}
	

	if (enableDebugCamera)
	{
		viewProjection_.matView = pDebugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = pDebugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
	else
	{
		viewProjection_.matView = pCamera->GetViewProjection().matView;
		viewProjection_.matProjection = pCamera->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	switch (currentScene_)
	{
	case Scenes::Title:
		break;
	case Scenes::Select:
		break;
	case Scenes::RhythmGame:
		pRhythmGame_->DrawSpriteBackGround();
		break;
	case Scenes::Result:
		break;
	default:
		break;
	}

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	switch (currentScene_)
	{
	case Scenes::Title:
		break;
	case Scenes::Select:
		break;
	case Scenes::RhythmGame:
		pRhythmGame_->Draw3D(viewProjection_);
		break;
	case Scenes::Result:
		break;
	default:
		break;
	}


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	switch (currentScene_)
	{
	case Scenes::Title:
		pTitleScene_->DrawSpriteFront();
		break;
	case Scenes::Select:
		break;
	case Scenes::RhythmGame:
		break;
	case Scenes::Result:
		pResultScene_->DrawSpriteFront();
		break;
	default:
		break;
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::ChangeScene()
{
	// 早期リターン
	if (currentScene_ == reserveScene_) return;
	currentScene_ = reserveScene_;
	
	switch (currentScene_)
	{
	case Scenes::Title:
		if (pTitleScene_)
		{
			delete pTitleScene_;
			pTitleScene_ = nullptr;
		}
		pTitleScene_ = new TitleScene();
		pTitleScene_->Initialize();
		pTitleScene_->SetGameScene(this);
		break;


	case Scenes::Select:
		break;


	case Scenes::RhythmGame:
		if (pRhythmGame_)
		{
			delete pRhythmGame_;
			pRhythmGame_ = nullptr;
		}
		pRhythmGame_ = new RhythmGame();
		pRhythmGame_->SetDebugOperationData(&debugOperationData_);
		pRhythmGame_->SetGameScene(this);
		pRhythmGame_->Initialize();
		break;


	case Scenes::Result:
		if (pResultScene_)
		{
			delete pResultScene_;
			pResultScene_ = nullptr;
		}
		pResultScene_ = new ResultScene();
		pResultScene_->Initialize();
		pResultScene_->SetGameScene(this);
		pResultScene_->SetHitCount(pRhythmGame_->GetHitCount());
		break;
	}
}

void GameScene::ReserveSceneChange(Scenes _scene)
{
	reserveScene_ = _scene;
}
