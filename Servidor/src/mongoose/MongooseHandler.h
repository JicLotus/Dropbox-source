/*
 * Mongoose.h
 *
 *  Created on: 24/8/2015
 *      Author: lotus
 */

#ifndef MONGOOSE_MONGOOSE_H_
#define MONGOOSE_MONGOOSE_H_

#include <string>
#include "UserHandler.h"
#include "FileHandler.h"
#include "mongoose.h"

using namespace std;

class MongooseHandler {
public:
	MongooseHandler();
	void waitConnection();
	virtual ~MongooseHandler();

private:
	mg_server *server;
	string a;
};

#endif /* MONGOOSE_MONGOOSE_H_ */

static int ev_handler(struct mg_connection *conn, enum mg_event ev);
