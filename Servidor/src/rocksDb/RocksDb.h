/*
 * RocksDb.h
 *
 *  Created on: 24/8/2015
 *      Author: lotus
 */

#ifndef ROCKSDB_ROCKSDB_H_
#define ROCKSDB_ROCKSDB_H_
#include "rocksdb/db.h"

class RocksDb {
public:
	RocksDb();
	void openDataBase();
	virtual ~RocksDb();
private:
	rocksdb::DB* db;
	rocksdb::Options options;
};

#endif /* ROCKSDB_ROCKSDB_H_ */
