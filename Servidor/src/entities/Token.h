/*
 * Token.h
 *
 *  Created on: 1 de set. de 2015
 *      Author: eliana
 */

#ifndef ENTITIES_TOKEN_H_
#define ENTITIES_TOKEN_H_

#include <string>
#include <stdlib.h>
#include <time.h>
#include "../rocksDb/RocksdbHandler.h"

using namespace std;

class Token {
private:
	string GenerateRandomString(int size);
	string guid;
public:
	Token();
	virtual ~Token();
	static bool IsGuidValid(string identifier, string guid);
	string GenerateGuid();
	bool Load(string identifier);
	void Save(string identifier, string newGuid);

	string GetGuid();

};

#endif /* ENTITIES_TOKEN_H_ */
