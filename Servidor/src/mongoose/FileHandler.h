/*
 * FileHandler.h
 *
 *  Created on: 9 de set. de 2015
 *      Author: kevin
 */

#ifndef MONGOOSE_FILEHANDLER_H_
#define MONGOOSE_FILEHANDLER_H_

#include <string>
#include "../entities/File.h"
#include "../entities/FileContent.h"
#include "../entities/Token.h"
#include "../entities/User.h"
#include "../log/Log.h"

using namespace std;

class FileHandler {

public:
	FileHandler();
	virtual ~FileHandler();

	string GetMetadata(string identifier, string guid);
	string GetDocument(string identifier, string guid);
	string GetDocument(string identifier, string guid, int version);
	string Insert(string json);
	string Update(string identifier, string json);
	string AddSharedFiles(string identifier, string json);
	string DeleteSharedFiles(string fileIdentifier, string userIdentifier);
	string Delete(string identifier, string guid);

};

#endif /* MONGOOSE_FILEHANDLER_H_ */
