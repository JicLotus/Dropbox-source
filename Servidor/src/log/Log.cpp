/*
 * Log.cpp
 *
 *  Created on: 28 de ago. de 2015
 *      Author: eliana
 */

#include "Log.h"

int Log::TYPE_ERROR = 1;
int Log::TYPE_WARNING = 2;
int Log::TYPE_INFORMATION = 3;
int Log::TYPE_DEBUG = 4;
int Log::TYPE_TRACE = 5;

string* stringToArrayOfStrings(string str, int countWords){
	string* arrayOfStrings = new string[countWords];
	int i = 0;
	stringstream ssin(str);
	while (ssin.good() && i < 5){
		ssin >> arrayOfStrings[i];
		++i;
	}
	return arrayOfStrings;
}

void getDateAndTime(string &date, string &time){
	std::time_t result = std::time(NULL);
	string dateAndTime = std::asctime(std::localtime(&result));
	string* arrayDateTime = stringToArrayOfStrings(dateAndTime, 5);
	date = arrayDateTime[0] + " " + arrayDateTime[1] + " " + arrayDateTime[2] + " " + arrayDateTime[4];
	time = arrayDateTime[3];
	delete[] arrayDateTime;
}

Log::Log() {

}

Log::~Log() {

}

void Log::insert(int typeLog, string message){
	if(correspondsToLog(typeLog)){
		string date, time;
		getDateAndTime(date, time);
		writeLog(date, time, typeLog, message);
	}
}

bool Log::correspondsToLog(int typeLog){
	ifstream logFile;
	logFile.open("server.config", ios::in | ios::binary);
	string line;
	int nLine = 1;
	while (getline(logFile, line)){
		if(nLine == typeLog && line.find("true") != string::npos){
			logFile.close();
			return true;
		}
		nLine++;
	}
	logFile.close();
	return false;
}

void Log::writeLog(string date, string time, int typeLog, string message){
	ofstream logFile;
	logFile.open("server.log", ios::out | ios::app | ios::binary);
	logFile << date << " " << " " << time << " " << typeLog << " " << message << endl;
	logFile.close();
}


