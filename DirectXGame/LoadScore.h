#pragma once
#include <string>
#include "SheetMusic.h"

class LoadScore
{
public:
	LoadScore();
	~LoadScore();

	SheetMusic Load(std::string _fileName);

private:
	void Interpretation(std::stringstream& _file);

	const std::string directoryName = "sheet";
	SheetMusic sheetmusic_;
};