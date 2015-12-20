/*
 * RocksDb.cpp
 *
 *  Created on: 24/8/2015
 *      Author: lotus
 */

#include "RocksDb.h"

RocksDb::RocksDb() {
}

void RocksDb::openDataBase(){
	 options.create_if_missing = true;
	 rocksdb::Status status = rocksdb::DB::Open(options, "/tmp/testdb/", &db);
}

RocksDb::~RocksDb() {
}

