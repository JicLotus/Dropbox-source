/*
 * Token.cpp
 *
 *  Created on: 1 de set. de 2015
 *      Author: eliana
 */

#include "Token.h"

string Token::GenerateRandomString(int size){
	string random;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < size; ++i) {
    	random += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return random;
}

Token::Token() {
	// TODO Auto-generated constructor stub

}

Token::~Token() {
	// TODO Auto-generated destructor stub
}

bool Token::IsGuidValid(string identifier, string guid){
	Token token;
	token.Load(identifier);
	string identifierGuid = token.GetGuid();
	return (identifierGuid == guid && !identifierGuid.empty() && !guid.empty());
}

string Token::GenerateGuid(){
	srand(time(NULL));
	string nGuid_field1 = GenerateRandomString(8);
	string nGuid_field2 = GenerateRandomString(4);
	string nGuid_field3 = GenerateRandomString(4);
	string nGuid_field4 = GenerateRandomString(4);
	string nGuid_field5 = GenerateRandomString(12);
	return nGuid_field1 + "-" + nGuid_field2 + "-" + nGuid_field3 + "-" + nGuid_field4 + "-" + nGuid_field5;
}

bool Token::Load(string identifier){
	guid = RocksdbHandler::Get("token_" + identifier, PATH_DB_USERS);
	return !guid.empty();
}

void Token::Save(string identifier, string newGuid){
	guid = newGuid;
	RocksdbHandler::Insert("token_" + identifier, newGuid, PATH_DB_USERS);
}

string Token::GetGuid(){
	return guid;
}
