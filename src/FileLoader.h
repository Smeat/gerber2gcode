/*
 * FileLoader.h
 *
 *  Created on: 26.06.2013
 *      Author: smeat
 */

#ifndef FILELOADER_H_
#define FILELOADER_H_

#include <fstream>

#include "Util.h"

class FileLoader{
protected:
	std::ifstream _fileStream;
public:
	FileLoader(){}
	virtual ~FileLoader(){}

	/**
	 * Open file.
	 */
	virtual bool openFile(const char* file);
	virtual bool closeFile();
	virtual bool generateGeometry() = 0;
};

inline bool FileLoader::openFile(const char* file) {
	_fileStream.open(file, std::ios::in);
	return _fileStream;
}

inline bool FileLoader::closeFile() {
	_fileStream.close();
	return true;
}

#endif /* FILELOADER_H_ */
