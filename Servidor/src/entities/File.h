/*
 * File.h
 *
 *  Created on: 9 de set. de 2015
 *      Author: kevin
 */

#ifndef ENTITIES_FILE_H_
#define ENTITIES_FILE_H_

#include <string>
#include <time.h>
#include <vector>
#include "../jsoncpp/json.h"
#include "../rocksDb/RocksdbHandler.h"
#include "../log/Log.h"

using namespace std;

class FileVersion {
public:
	int version;
	string fileContentIdentifier;
	time_t date;
};


class File {
private:
	int identifier;
	string name;
	string extension;
	int type;
	int size;
	vector<string> tags;
	bool deleted;
	string lastModifiedBy;
	time_t lastModifiedOn;
	int lastVersion;
	vector<FileVersion*> versions;
	string owner;
public:
	File();
	virtual ~File();

	bool Load(int identifier);
	void Generate(Json::Value root);
	string GetJson();
	string GetTagsStr();
	string GetDeletedStr();
	string GetVersionsStr();
	void SetIdentifier(int identifier);
	int GetIdentifier();
	void SetType(int type);
	int GetType();
	bool Save();
	int CreateId();
	bool IsDeleted();
	void SetDeleted(bool deleted);
	string GetExtension();
	void SetExtension(const string extension);
	time_t GetLastModifiedOn();
	void SetLastModifiedOn(time_t lastModifiedOn);
	string GetLastModifiedBy();
	void SetLastModifiedBy(string lastModifiedBy);
	string GetName();
	void SetName(const string name);
	string GetOwner();
	void SetOwner(string owner);
	int GetSize();
	void SetSize(int size);
	int GetLastVersion();
	void SetLastVersion(int lastVersion);
	void AddVersion(FileVersion* fileVersion);
	FileVersion* GetVersion(int version);
};

#endif /* ENTITIES_FILE_H_ */
