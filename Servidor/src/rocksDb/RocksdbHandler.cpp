/*
 * RocksdbHandler.cpp
 *
 *  Created on: 26 de ago. de 2015
 *      Author: eliana
 */

#include "RocksdbHandler.h"

void RocksdbHandler::openDB(rocksdb::DB **db, string path) {
	rocksdb::Options options;
	options.create_if_missing = true;
	rocksdb::DB::Open(options, path, db);
}


void RocksdbHandler::Insert(string key, string value, string path){
	rocksdb::DB *db;
	openDB(&db,path);
	db->Put(rocksdb::WriteOptions(), key, value);
	delete db;
}

string RocksdbHandler::Get(string key, string path){
	rocksdb::DB *db;
	openDB(&db,path);
	string value;
	rocksdb::Status s = db->Get(rocksdb::ReadOptions(), key, &value);
	if (s.IsNotFound()) {
		delete db;
		return "";
	}
	if (s.ok()){
		delete db;
		return value;
	}
	delete db;
	return "";
}

void RocksdbHandler::Delete(string key, string path){
	rocksdb::DB *db;
	openDB(&db,path);
	db->Delete(rocksdb::WriteOptions(), key);
	delete db;
}
