/*
 * User.h
 *
 *  Created on: Aug 30, 2015
 *      Author: pablo
 */

#ifndef ENTITIES_USER_H_
#define ENTITIES_USER_H_

#include "../jsoncpp/json.h"
#include "../rocksDb/RocksdbHandler.h"
#include "File.h"
#include <set>
#include <list>

using namespace std;

class UserContent
{
public:
	int identifier;
	int type;
	std::list<UserContent*> content;
};

class User {

private:
	string identifier;
	string name;
	string lastName;
	string md5;
	int quotaMB;
	int quotaLimitMB;
	string email;
	string avatar;
	string lastLatitude;
	string lastLongitude;
	bool isActive;
	std::list<UserContent*> ownFiles;
	std::list<int> sharedFiles;

	string GetJsonByUserContentList(std::list<UserContent*> list, bool full);
	string GetJsonBySharedFilesList(bool full);
	UserContent* GetUserContentByFather(std::list<UserContent*> list, File* father);
	void SetSharedFilesByJson(Json::Value root);
public:
	User();
	virtual ~User();

	bool Load(string identifier);
	bool Save();
	string GetJson(bool full);
	bool SetAllAttributes(string json, std::set<string> ignoredAttributes);
	void SetOwnFilesByJson(Json::Value root, UserContent* father);
	bool AddOwnFile(File* father, File* file);
	bool DeleteOwnFile(File* file);
	bool AddSharedFile(File* file);
	bool DeleteSharedFile(File* file);

	string GetIdentifier();
	void SetIdentifier(string identifier);
	string GetName();
	void SetName(string name);
	string GetLastName();
	void SetLastName(string lastname);
	string GetMD5();
	void SetMD5(string md5);
	int GetQuotaMB();
	void SetQuotaMB(int quotaMB);
	int GetQuotaLimitMB();
	void SetQuotaLimitMB(int quotaLimitMB);
	string GetEmail();
	void SetEmail(string name);
	string GetAvatar();
	void SetAvatar(string avatar);
	string GetLastLatitude();
	void SetLastLatitude(string lastLatitude);
	string GetLastLongitude();
	void SetLastLongitude(string lastLongitude);
	bool GetIsActive();
	void SetIsActive(bool isActive);
};



#endif /* ENTITIES_USER_H_ */
