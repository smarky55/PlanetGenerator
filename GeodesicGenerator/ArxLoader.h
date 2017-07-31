#pragma once

#include <iostream>
#include <fstream>

class ArxLoader {
	std::ifstream arx;
	char header[0x10];
public:
	ArxLoader(const char * arx_path);
	~ArxLoader();

	void getFile(const char * path, const char * data, unsigned * size);
};

