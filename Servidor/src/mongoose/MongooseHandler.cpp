/*
 * Mongoose.cpp
 *
 *  Created on: 24/8/2015
 *      Author: lotus
 */

#include "MongooseHandler.h"

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
	  switch (ev) {
	    case MG_AUTH: return MG_TRUE;
	    case MG_REQUEST:
	    {
	    	string uri = std::string(conn->uri);
	    	string method = std::string(conn->request_method);
	    	string content = std::string(conn->content);

	    	if (uri.find("/users/auth") != std::string::npos && method == "GET"){

	    		char identifier[100], md5[100];

	    		mg_get_var(conn, "identifier", identifier, sizeof(identifier));
	    		mg_get_var(conn, "md5", md5, sizeof(md5));

	    		UserHandler userHandler;
	    		string response = userHandler.Auth(identifier, md5);
	    		mg_printf_data(conn, response.c_str());
	    		return MG_TRUE;
	    	}

	    	if (uri.find("/users/") != std::string::npos && method == "GET"){

	    		char guid[100];

	    		mg_get_var(conn, "token", guid, sizeof(guid));


	    		UserHandler userHandler;
	    		string response = userHandler.Get(uri.substr(7, uri.length()), string(guid));
	    		mg_printf_data(conn, response.c_str());
	    		return MG_TRUE;
	    	}

	    	if (uri.find("/users") != std::string::npos && method == "POST"){
	    		UserHandler userHandler;
	    		string response = userHandler.Insert(content);
	    		mg_printf_data(conn, response.c_str());
	    		return MG_TRUE;
	    	}

	    	if (uri.find("/users/") != std::string::npos && method == "PUT"){
	    		UserHandler userHandler;
	    		string response = userHandler.Update(uri.substr(7, uri.length()), content);
	    		mg_printf_data(conn, response.c_str());
	    		return MG_TRUE;
	    	}

	    	if (uri.find("/users/") != std::string::npos && method == "DELETE"){

	    		char guid[100];

	    		mg_get_var(conn, "token", guid, sizeof(guid));

	    		UserHandler userHandler;
	    		string response = userHandler.Delete(uri.substr(7, uri.length()), string(guid));
	    		mg_printf_data(conn, response.c_str());
	    		return MG_TRUE;
	    	}

			if (uri.find("/files") != std::string::npos && method == "POST"){
				FileHandler fileHandler;
				string response = fileHandler.Insert(content);
				mg_printf_data(conn,response.c_str());
		    	return MG_TRUE;
		    }

			if (uri.find("/metafiles/share/") != std::string::npos && method == "POST"){
				FileHandler fileHandler;
				string response = fileHandler.AddSharedFiles(uri.substr(17, uri.length()), content);
				mg_printf_data(conn,response.c_str());
		    	return MG_TRUE;
		    }

			if (uri.find("/metafiles/share/") != std::string::npos && method == "DELETE"){

	    		char userIdentifier[100];
	    		mg_get_var(conn, "user", userIdentifier, sizeof(userIdentifier));

				FileHandler fileHandler;
				string response = fileHandler.DeleteSharedFiles(uri.substr(17, uri.length()), userIdentifier);
				mg_printf_data(conn,response.c_str());
		    	return MG_TRUE;
		    }

			if (uri.find("/metafiles/") != std::string::npos && method == "GET"){
	    		char guid[100];

	    		mg_get_var(conn, "token", guid, sizeof(guid));
				FileHandler fileHandler;
				string response = fileHandler.GetMetadata(uri.substr(11, uri.length()), guid);
				mg_printf_data(conn,response.c_str());
		    	return MG_TRUE;
		    }

			if (uri.find("/metafiles/") != std::string::npos && method == "DELETE"){
	    		char guid[100];

	    		mg_get_var(conn, "token", guid, sizeof(guid));
				FileHandler fileHandler;
				string response = fileHandler.Delete(uri.substr(11, uri.length()), guid);
				mg_printf_data(conn,response.c_str());
		    	return MG_TRUE;
		    }

		if (uri.find("/files/") != std::string::npos && method == "GET"){
			string response;
			char guid[100];
			mg_get_var(conn, "token", guid, sizeof(guid));

			FileHandler fileHandler;
			if(uri.find("/", 7) != -1)
			{
				string id = uri.substr(7, uri.find("/", 7) - 1);
				string version = uri.substr(uri.find("/", 7) + 1, uri.length());

				response = fileHandler.GetDocument(id, guid, std::stoi(version));
			}
			else
				response = fileHandler.GetDocument(uri.substr(7, uri.length()), guid);


			mg_send_data(conn,response.c_str(),strlen(response.c_str()));


		    	return MG_TRUE;
		    }

		    if (uri.find("/metafiles/") != std::string::npos && method == "PUT"){
				FileHandler fileHandler;
				string response = fileHandler.Update(uri.substr(11, uri.length()), content);
				mg_printf_data(conn,response.c_str());
	    		return MG_TRUE;
	    	}

	    	return MG_FALSE;
	    }
	    default: return MG_FALSE;
	  }
}

MongooseHandler::MongooseHandler() {
	server = mg_create_server(NULL, ev_handler);
	mg_set_option(server, "listening_port", "8080");
}

void MongooseHandler::waitConnection()
{
	printf("Mongoose iniciado en puerto: %s\n", mg_get_option(server, "listening_port"));
	for (;;) {
	  mg_poll_server(server, 1000);
	}
	this->~MongooseHandler();
}


MongooseHandler::~MongooseHandler() {
	// Cleanup, and free server instance
	mg_destroy_server(&server);
}

