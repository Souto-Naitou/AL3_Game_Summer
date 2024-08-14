#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Note
{
public:

	void Initialize();
	void Update();
	void Draw(const ViewProjection& _viewProjection);

private:
	WorldTransform worldTransform_;
	Model* pModel_;

};