#include "JudgeTiming.h"
#include "Note.h"
#include <format>

const float JudgeTiming::kPerfectRange_[2] = { 0.98f, 1.02f };
const float JudgeTiming::kGreatRange_[2] = { 0.96f, 1.04f };

void JudgeTiming::Initialize()
{
    pPrimitiveDrawer_ = PrimitiveDrawer::GetInstance();
    pLaneData_ = pLane_->GetLaneData();
    pLaneEndPoint_[0] = &pLaneData_->left[1];
    pLaneEndPoint_[1] = &pLaneData_->right[1];
}

void JudgeTiming::Update()
{

    /* perfect */
    perfectRange_[0] = (1.0f - JudgeTiming::kPerfectRange_[0]) * pLaneData_->left[0] + JudgeTiming::kPerfectRange_[0] * pLaneData_->left[1];
    perfectRange_[1] = (1.0f - JudgeTiming::kPerfectRange_[0]) * pLaneData_->right[0] + JudgeTiming::kPerfectRange_[0] * pLaneData_->right[1];
    perfectRange_[2] = (1.0f - JudgeTiming::kPerfectRange_[1]) * pLaneData_->left[0] + JudgeTiming::kPerfectRange_[1] * pLaneData_->left[1];
    perfectRange_[3] = (1.0f - JudgeTiming::kPerfectRange_[1]) * pLaneData_->right[0] + JudgeTiming::kPerfectRange_[1] * pLaneData_->right[1];
    /* great */
    greatRange_[0] = (1.0f - JudgeTiming::kGreatRange_[0]) * pLaneData_->left[0] + JudgeTiming::kGreatRange_[0] * pLaneData_->left[1];
    greatRange_[1] = (1.0f - JudgeTiming::kGreatRange_[0]) * pLaneData_->right[0] + JudgeTiming::kGreatRange_[0] * pLaneData_->right[1];
    greatRange_[2] = (1.0f - JudgeTiming::kGreatRange_[1]) * pLaneData_->left[0] + JudgeTiming::kGreatRange_[1] * pLaneData_->left[1];
    greatRange_[3] = (1.0f - JudgeTiming::kGreatRange_[1]) * pLaneData_->right[0] + JudgeTiming::kGreatRange_[1] * pLaneData_->right[1];
}

void JudgeTiming::Draw3D(const ViewProjection& _viewProjection)
{
    _viewProjection;
    pPrimitiveDrawer_->DrawLine3d(*pLaneEndPoint_[0], *pLaneEndPoint_[1], { 1.0f, 1.0f, 1.0f, 1.0f });
    /// デバッグ用
    if (false)
    {
        pPrimitiveDrawer_->DrawLine3d(perfectRange_[0], perfectRange_[1], Vector4(0.2f, 0.2f, 1.0f, 1.0f));
        pPrimitiveDrawer_->DrawLine3d(perfectRange_[2], perfectRange_[3], Vector4(0.2f, 0.2f, 1.0f, 1.0f));
        pPrimitiveDrawer_->DrawLine3d(greatRange_[0], greatRange_[1], Vector4(0.2f, 1.0f, 0.2f, 1.0f));
        pPrimitiveDrawer_->DrawLine3d(greatRange_[2], greatRange_[3], Vector4(0.2f, 1.0f, 0.2f, 1.0f));
    }
}

Note* JudgeTiming::Judge(Direction _laneDir, HitResult& _rtnHitResult)
{
    Note* rtnNote = nullptr;
    HitResult rtnHitResult = {};
    _laneDir;
    double maxTimeLane = 0.0f;
    for (Note* note : *pNoteList_)
    {
        if (note->GetBeginLane() != _laneDir) continue;
        double currentTimeLane = note->GetTimeLane();

        // greatの判定
        if (currentTimeLane >= kGreatRange_[0] && currentTimeLane <= kGreatRange_[1])
        {
            // perfectの判定
            if (currentTimeLane >= kPerfectRange_[0] && currentTimeLane <= kPerfectRange_[1])
            {
                if (currentTimeLane >= maxTimeLane)
                {
                    maxTimeLane = currentTimeLane;
                    rtnNote = note;
                    rtnHitResult = HitResult::Perfect;
                }
            }
            else
            {
                // グレート
                if (currentTimeLane >= maxTimeLane)
                {
                    maxTimeLane = currentTimeLane;
                    rtnNote = note;
                    rtnHitResult = HitResult::Great;
                }
            }
        }
    }
    OutputDebugStringA(std::format("maxTimeLane = {}\n", maxTimeLane).c_str());
    _rtnHitResult = rtnHitResult;
    return rtnNote;
}
