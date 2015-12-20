/*
 * File.h
 *
 *  Created on: 9 de set. de 2015
 *      Author: kevin
 */

#ifndef ENTITIES_FILECONTENT_H_
#define ENTITIES_FILECONTENT_H_

#include <string>
#include "../jsoncpp/json.h"
#include "../rocksDb/RocksdbHandler.h"
#include "../log/Log.h"

using namespace std;

class FileContent {
private:
	string identifier;
	string content;
public:
	FileContent();
	virtual ~FileContent();

	bool Load(string identifier);
	bool Save();
	void Generate(Json::Value root);
	string GetJson();
	void SetIdentifier(string identifier);
	string GetIdentifier();
	string GetContent();
	void SetContent(string content);
};

#endif /* ENTITIES_FILECONTENT_H_ */
