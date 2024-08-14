#include "ImGuiWindow.h"
#include "ImGuiManager.h"

void ImGuiWindow::DebugWindowDraw()
{
	Window_Camera();
}

void ImGuiWindow::Window_Camera()
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::SeparatorText("Camera");
		ImGui::DragFloat3("Rotate", &pDebugOperationData_->pCameraRotation->x, 0.01f);
		ImGui::DragFloat3("Translate", &pDebugOperationData_->pCameraTranslation->x, 0.01f);

		ImGui::End();
	}
}
