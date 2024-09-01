#pragma once
#include "Vector3.h"
#include "SheetMusic.h"
#include <string>
#include "define.h"


class ImGuiWindow
{
public:

    struct DebugOperationData
    {
        Vector3*        pCameraRotation;
        Vector3*        pCameraTranslation;
        size_t*         pNoteListSize;
        double*         pElapsedTime;
        SheetMusic*     pSheetMusic;
        bool*           pEnableMetronome;
        std::string*    pSheetPath;
        bool*           pIsReqReload;
        bool*           pIsOpenPopupModal;
        float*          pVolume;
        bool*           pIsChangeVolume;
        unsigned int*   pCountMeasure;
        HitCount*       pHitCount;
        double*         pElapsedTimeShifted;
        double*         pFramelate;
        BeatCount*      pBeatCount;
    };

    ImGuiWindow() {};
    ~ImGuiWindow() {};

    void SetDebugOperationData(DebugOperationData* _debugOperationData) { pDebugOperationData_ = _debugOperationData; };
    void DebugWindowDraw();
    

private:
    void Window_Camera();
    void Window_RhythmGame();
    DebugOperationData* pDebugOperationData_ = nullptr;
    char textBuffer[64];
};
