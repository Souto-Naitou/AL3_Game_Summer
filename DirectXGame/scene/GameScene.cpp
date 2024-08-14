#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include "ImGuiWindow.h"

GameScene::GameScene()
{

}

GameScene::~GameScene() 
{
	delete pLane_;
	delete pDebugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	viewProjection_.Initialize();
	worldTransform_.Initialize();
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
	pLane_ = new Lane();
	pLane_->Initialize();
	pCamera = new Camera();
	pCamera->Initialize({ 0.0f, -3.0f, 3.4f }, { -2.7f, 0.0f, 0.0f });
	pDebugCamera_ = new DebugCamera(1280, 720);
	imguiWindow_ = new ImGuiWindow();
	imguiWindow_->SetDebugOperationData(&debugOperationData_);
	debugOperationData_.pCameraRotation = &pCamera->worldTransform_.rotation_;
	debugOperationData_.pCameraTranslation = &pCamera->worldTransform_.translation_;
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

	pLane_->Update();
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

	pLane_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
