//============================================================================
// Name        : Servidor.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstring>
#include <cstdio>
#include "rocksDb/RocksDb.h"
#include "mongoose/MongooseHandler.h"
#include "jsoncpp/json.h"
#include "rocksDb/RocksdbHandler.h"
#include "log/Log.h"
#include "entities/FileContent.h"

using namespace std;

int main(int argc, char** argv) {

	MongooseHandler* ms = new MongooseHandler();
	ms->waitConnection();



	return 0;
}
