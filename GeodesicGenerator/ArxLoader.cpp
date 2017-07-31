#include "ArxLoader.h"


ArxLoader::ArxLoader(const char * arx_path) {
	arx = std::ifstream(arx_path, std::ios::in | std::ios::binary);

	if(!arx.is_open()) {
		throw std::runtime_error("Failed to open file: " + std::string(arx_path));
	}

	arx.read(header, 0x10);
	// Check file is arx archive
	if(header[0] != 'A' || header[1] != 'R' || header[2] != 'X') {
		throw std::runtime_error("Invalid archive: " + std::string(arx_path));
	}

	char major = header[3];
	char minor = header[4];
	char rev = header[5];

	if(major != 0 || minor != 0 || rev != 0) {
		throw std::runtime_error("Unsupported archive version");
	}

	unsigned baseDirLoc = header[6];
	unsigned baseDirSize = header[0xA];
}

ArxLoader::~ArxLoader() {
	arx.close();
}
