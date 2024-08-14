#include "WorldTransform.h"
#include "Matrix4x4/calc/matrix4calc.h"

void WorldTransform::UpdateMatrix()
{
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	if (parent_)
	{
		matWorld_ = matWorld_ * parent_->matWorld_;
	}

	TransferMatrix();
}