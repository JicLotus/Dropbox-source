/*
 * RocksdbHandler.h
 *
 *  Created on: 26 de ago. de 2015
 *      Author: eliana
 */

#ifndef ROCKSDB_ROCKSDBHANDLER_H_
#define ROCKSDB_ROCKSDBHANDLER_H_

#include "rocksdb/db.h"

using namespace std;

const string PATH_DB_USERS = "testdbusers";
const string PATH_DB_FILES = "testdbfiles";

class RocksdbHandler {
public:
	RocksdbHandler();
	virtual ~RocksdbHandler();
	static void Insert(string key, string value, string path);
	static string Get(string key, string path);
	static void Delete(string key, string path);
private:
	static void openDB(rocksdb::DB **db, string path);
};

#endif /* ROCKSDB_ROCKSDBHANDLER_H_ */
