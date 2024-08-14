#pragma once
#include "Vector3.h"


class ImGuiWindow
{
public:

	struct DebugOperationData
	{
		Vector3* pCameraRotation;
		Vector3* pCameraTranslation;
	};

	ImGuiWindow() {};
	~ImGuiWindow() {};

	void SetDebugOperationData(DebugOperationData* _debugOperationData) { pDebugOperationData_ = _debugOperationData; };
	void DebugWindowDraw();
	

private:
	void Window_Camera();
	DebugOperationData* pDebugOperationData_ = nullptr;
};
