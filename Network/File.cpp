#include "File.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>

namespace swl
{
	File::File() : filePath{}, fileName {}, data{}
	{}
	File::File(std::string path)
	{
		size_t length = 0;

		for (int i = path.size();; i--)
		{
			if (i == 0)
				break;
			else if (path[i] == '\\')
			{
				length--;
				break;
			}
			else
				length++;
		}

		this->fileName = path.substr(path.size() - length, length);
		this->filePath = path.substr(0, path.size() - length);
	}

	std::string& File::getFilePath()
	{
		return this->filePath;
	}

	std::string& File::getFileName()
	{
		return this->fileName;
	}

	uint32_t File::getDataSize()
	{
		return this->data.size();
	}

	char* File::getFileData()
	{
		return this->data.data();
	}

	void File::setFileData(const std::vector<char>& data)
	{
		this->data = data;
	}

	void File::setFileName(const std::string& name)
	{
		this->fileName = name;
	}

	bool File::readFile()
	{
		std::ifstream fileIn;
		fileIn.open((this->filePath + this->fileName), std::ios::binary);
		if (!fileIn.is_open())
			return true;
		int length;
		fileIn.seekg(0, std::ios::end);
		length = fileIn.tellg();
		fileIn.seekg(0);
		this->data.resize(length);
		fileIn.read(this->data.data(), length);
		fileIn.close();
		return false;
	}
	
	void File::saveFile()
	{
		std::ofstream fileOut;
		fileOut.open("2" + this->fileName, std::ios::binary);
		fileOut.write(this->data.data(), this->data.size());
		fileOut.close();
	}
}
