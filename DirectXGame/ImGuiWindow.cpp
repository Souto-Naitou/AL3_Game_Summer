#include "ImGuiWindow.h"
#include "ImGuiManager.h"

void ImGuiWindow::DebugWindowDraw()
{
    Window_Camera();
    Window_RhythmGame();
}

void ImGuiWindow::Window_Camera()
{
    ImGui::Begin("Camera");

    ImGui::PushID("CAMERA");

    ImGui::SeparatorText("Camera");
    ImGui::DragFloat3("Rotate", &pDebugOperationData_->pCameraRotation->x, 0.01f);
    ImGui::DragFloat3("Translate", &pDebugOperationData_->pCameraTranslation->x, 0.01f);

    ImGui::PopID();

    ImGui::End();
    
}

void ImGuiWindow::Window_RhythmGame()
{
    if (pDebugOperationData_->pElapsedTime      &&
        pDebugOperationData_->pNoteListSize     &&
        pDebugOperationData_->pSheetMusic       &&
        pDebugOperationData_->pEnableMetronome  &&
        pDebugOperationData_->pSheetPath        &&
        pDebugOperationData_->pIsReqReload      &&
        pDebugOperationData_->pVolume           &&
        pDebugOperationData_->pIsChangeVolume   &&
        pDebugOperationData_->pCountMeasure)
    {
        ImGui::Begin("RhythmGame");
        ImGui::PushID("RHYTHMGAME");

        ImGui::SeparatorText("Game Debug Info");
        ImGui::Text("Elapsed Time : %lf", *pDebugOperationData_->pElapsedTime);
        ImGui::Text("NoteList Size : %d", *pDebugOperationData_->pNoteListSize);
        ImGui::Text("Measure : %u", *pDebugOperationData_->pCountMeasure);

        ImGui::SeparatorText("Game Setting");
        if (ImGui::Button("Look SheetMusicData"))
            ImGui::OpenPopup("SheetMusicData");
        if (ImGui::BeginPopup("SheetMusicData"))
        {
            SheetMusic* pSheetMusic = pDebugOperationData_->pSheetMusic;
            if (!pSheetMusic)
            {
                ImGui::EndPopup();
                ImGui::End();
                return;
            }

            ImGui::SeparatorText("Sheet Data");
            
            ImGui::Text("Title : %s", pSheetMusic->title.c_str());
            ImGui::Text("BPM : %u", pSheetMusic->bpm);
            ImGui::Text("Offset : %u", pSheetMusic->offset);
            ImGui::EndPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Change sheet"))
        {
            ImGui::OpenPopup("ChangeSheetData");
            strcpy_s(textBuffer, pDebugOperationData_->pSheetPath->c_str());
        }
        if (ImGui::BeginPopupModal("ChangeSheetData", nullptr, ImGuiWindowFlags_NoResize))
        {
            ImGui::SetWindowSize(ImVec2(700, 80));
            ImGui::InputText("Path", textBuffer, 64);
            *pDebugOperationData_->pSheetPath = textBuffer;
            *pDebugOperationData_->pIsOpenPopupModal = true;
            if (ImGui::Button("Apply"))
            {
                *pDebugOperationData_->pIsReqReload = true;
                ImGui::CloseCurrentPopup();
                *pDebugOperationData_->pIsOpenPopupModal = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
                *pDebugOperationData_->pIsOpenPopupModal = false;
            }
            ImGui::EndPopup();
        }

        ImGui::Checkbox("Metronome", pDebugOperationData_->pEnableMetronome);

        *pDebugOperationData_->pIsChangeVolume = ImGui::SliderFloat("Music Volume", pDebugOperationData_->pVolume, 0.0f, 1.0f, "%.2f");

        ImGui::PopID();
        ImGui::End();
    }
}
