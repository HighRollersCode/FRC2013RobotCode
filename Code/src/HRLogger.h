/*
 * 
 *  HRLogger.h - simple, old fashioned logging system for our FRC robots which will create a new log file for each 'match'
 *  
 */

#ifndef HRLOGGER_H
#define HRLOGGER_H


class HRLogger
{
public:
	static void Init(bool fms);
	static void Log(const char * format,...);
};

#endif //HRLOGGER_H
