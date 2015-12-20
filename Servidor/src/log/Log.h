/*
 * Log.h
 *
 *  Created on: 28 de ago. de 2015
 *      Author: eliana
 */

#ifndef LOG_LOG_H_
#define LOG_LOG_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <string>

using namespace std;

class Log {
public:
	Log();
	virtual ~Log();
	static void insert(int typeLog, string message);
	static bool correspondsToLog(int typeLog);

	static int TYPE_ERROR;
	static int TYPE_WARNING;
	static int TYPE_INFORMATION;
	static int TYPE_DEBUG;
	static int TYPE_TRACE;

private:
	static void writeLog(string date, string time, int typeLog, string message);

};

#endif /* LOG_LOG_H_ */
