#pragma once

#include "EnumClass.h"
#include "Vector3.h"

#include <ViewProjection.h>
#include <PrimitiveDrawer.h>
#include <list>

class Lane;
class Note;

class JudgeTiming
{
public:

    JudgeTiming() {};
    ~JudgeTiming() {};

    void Initialize();
    void Update();
    void Draw3D(const ViewProjection& _viewProjection);

    /// <summary>
    /// タイミングを判定します
    /// </summary>
    /// <param name="_laneDir"></param>
    HitResult Judge(Direction _laneDir);

    void SetLane(Lane* _pLane) { pLane_ = _pLane; }
    void SetNoteList(std::list<Note*>* _pNoteList) { pNoteList_ = _pNoteList; }

private:

    PrimitiveDrawer*    pPrimitiveDrawer_   = nullptr;  // !< プリミティブドロワー
    Lane*               pLane_              = nullptr;  // !< レーン (リズムゲームシーンから借りてくる)
    Vector3*            pLaneEndPoint_[2]   = {};       // !< 左右レーンの終点
    std::list<Note*>*   pNoteList_          = {};       // !< ノートリストポインタ (リズムゲームシーンから借りてくる)
};