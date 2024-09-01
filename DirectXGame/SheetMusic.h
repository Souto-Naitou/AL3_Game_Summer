#pragma once
#include <vector>
#include <string>

struct SheetMusic
{
	std::string title;
	std::string sourcePath;
	unsigned int bpm;
	unsigned int offset; // !< オフセット ミリ秒
	unsigned int length;
	std::vector<std::vector<std::pair<std::string, unsigned int>>> sheetData;
};