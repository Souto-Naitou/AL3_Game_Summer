#include "JudgeTiming.h"
#include "Lane.h"
#include "Note.h"

void JudgeTiming::Initialize()
{
    pPrimitiveDrawer_ = PrimitiveDrawer::GetInstance();
    pLaneEndPoint_[0] = &pLane_->GetLaneData()->left[1];
    pLaneEndPoint_[1] = &pLane_->GetLaneData()->right[1];
}

void JudgeTiming::Update()
{
}

void JudgeTiming::Draw3D(const ViewProjection& _viewProjection)
{
    _viewProjection;
    pPrimitiveDrawer_->DrawLine3d(*pLaneEndPoint_[0], *pLaneEndPoint_[1], { 1.0f, 1.0f, 1.0f, 1.0f });
}

HitResult JudgeTiming::Judge(Direction _laneDir)
{
    _laneDir;
    float preTimeLane = 0.0f;
    for (Note* note : *pNoteList_)
    {
        float currentTimeLane = note->GetTimeLane();

        preTimeLane = currentTimeLane;
    }
    return HitResult();
}
