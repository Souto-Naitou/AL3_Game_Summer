#pragma once

#include "EnumClass.h"
#include "Vector3.h"
#include "Lane.h"

#include <ViewProjection.h>
#include <PrimitiveDrawer.h>
#include <list>

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
    /// タイミングを判定する
    /// </summary>
    /// <param name="_laneDir">どちらのレーンか</param>
    /// <param name="_rtnHitResult">ヒット判定結果</param>
    /// <returns>判定対象があった場合、対象Noteのポインタを返す。なかった場合、nullptrを返す。</returns>
    Note* Judge(Direction _laneDir, HitResult& _rtnHitResult);

    void SetLane(Lane* _pLane) { pLane_ = _pLane; }
    void SetNoteList(std::list<Note*>* _pNoteList) { pNoteList_ = _pNoteList; }

    static const float         kPerfectRange_[2];       // Perfect の範囲
    static const float         kGreatRange_[2];         // Great の範囲

private:

    PrimitiveDrawer*    pPrimitiveDrawer_   = nullptr;              // !< プリミティブドロワー
    Lane*               pLane_              = nullptr;              // !< レーン (リズムゲームシーンから借りてくる)
    Vector3*            pLaneEndPoint_[2]   = {};                   // !< 左右レーンの終点
    std::list<Note*>*   pNoteList_          = {};                   // !< ノートリストポインタ (リズムゲームシーンから借りてくる)
    Lane::LaneData*     pLaneData_          = nullptr;              // !< レーンデータ
    Vector3             perfectRange_[4]    = {};                   // !< perfect の範囲
    Vector3             greatRange_[4]      = {};                   // !< great の範囲
};