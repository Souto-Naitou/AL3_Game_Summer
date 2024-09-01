#include "LoadScore.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>

LoadScore::LoadScore()
{
	sheetmusic_ = {};

}

LoadScore::~LoadScore()
{
}

SheetMusic LoadScore::Load(std::string _fileName)
{
	sheetmusic_ = SheetMusic();
	std::stringstream sheetmusicFile;
	std::ifstream file;

	std::string directory = "Resources/" + directoryName + "/" + _fileName;
	file.open(directory);
	assert(file.is_open());

	sheetmusicFile << file.rdbuf();
	
	file.close();

	Interpretation(sheetmusicFile);

	return sheetmusic_;
}

void LoadScore::Interpretation(std::stringstream& _file)
{
	// 1行分
	std::string line;

	std::string tag; // タグ

	// 取り出しループ
	while (std::getline(_file, line))
	{
		std::string content;
		bool isTag = false;
		bool isEndTag = false;
		char preChar = '\0';
		for (char c : line)
		{
			if (c == '<') isTag = true;
			else if (c == '>') isTag = false;
			else if (c == '/' && preChar == '<') isEndTag = true;
			else if (isTag && !isEndTag) tag += c;
			else if (!isEndTag && c != 32)
			{
				// コンテンツ部なら
				content += c;
			}

			// 現在の文字をコピー
			preChar = c;
		}

		std::stringstream content_stream(content);

		if (tag == "title") sheetmusic_.title = content;
		else if (tag == "source") sheetmusic_.sourcePath = content;
		else if (tag == "bpm") content_stream >> sheetmusic_.bpm;
		else if (tag == "offset") content_stream >> sheetmusic_.offset;
		else if (tag == "length") content_stream >> sheetmusic_.length;
		else if (tag == "data" && content.size())
		{
			sheetmusic_.sheetData.push_back({});

			std::string word;
			while (std::getline(content_stream, word, ','))
			{
				std::stringstream word_stream(word);
				std::string LR = {};
				unsigned int noteLen = 0u;
				word_stream >> noteLen;
				word_stream >> LR;
				sheetmusic_.sheetData.back().push_back({ LR, noteLen });
			};

		}
		if (isEndTag) tag.clear();
	}

}
