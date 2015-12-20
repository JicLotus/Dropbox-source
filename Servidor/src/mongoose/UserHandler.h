/*
 * UserHandler.h
 *
 *  Created on: Aug 30, 2015
 *      Author: pablo
 */

#ifndef MONGOOSE_USERHANDLER_H_
#define MONGOOSE_USERHANDLER_H_

#include "../entities/User.h"
#include "../entities/Token.h"
#include "../log/Log.h"

using namespace std;

class UserHandler {

private:
	int GetConfigQuotaLimitMB();
public:
	UserHandler();
	virtual ~UserHandler();

	string Insert(string json);
	string Insert_ValidJsonChecked(User user);
	string Insert_UsernameInJsonChecked(User user);
	string Update(string identifier, string json);
	string Update_ValidJsonChecked(string identifier, Json::Value rootToken, string json);
	string Update_TokenReceivedChecked(string identifier, Json::Value rootToken, string json);
	string Update_ExistentUserChecked(string identifier, Json::Value rootToken, string json, User user);
	string Get(string identifier, string guid);
	string Get_ValidUserChecked(User user, string guid);
	string Delete(string identifier, string guid);
	string Delete_ExistentUserChecked(User user, string guid);
	string Auth(string identifier, string md5);

};

#endif /* MONGOOSE_USERHANDLER_H_ */
