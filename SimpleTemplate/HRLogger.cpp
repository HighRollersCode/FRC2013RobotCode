/*
 * 
 *  HRLogger.h - simple, old fashioned logging system for our FRC robots which will create a new log file for each 'match'
 *  
 */

#include "HRLogger.h"
#include<stdio.h>
// Enable and disable ALL logging code by setting this to false.
static bool HRLogEnabled = true;

// Logging system variables:
char Filename[256];
int Logindex = 0;
bool Isfms = false;
bool Isinit = false;

void HRLogger::Init(bool fms)
{
	char rootname[256];

	bool file_exists = true;
	Isfms = fms;
	if(Isfms == true)
	{
		sprintf(rootname,"FMS_LOG_");
	}
	else
	{
		sprintf(rootname,"LOG_");
	}
	Logindex = 0;
	while(file_exists == true)
	{
		Logindex++;
		sprintf(Filename,"%s%03d.txt",rootname,Logindex);
		
		FILE *f = fopen(Filename,"r");
		file_exists = (f != NULL);
		if (file_exists) 
		{
			fclose(f);
		}
	
	}
	FILE *f = fopen(Filename,"w");
	if(f)
	{
		fclose(f);
		Isinit = true;
	}
	else
	{
		Isinit = false;
	}

}


void HRLogger::Log(const char * format,...)
{
	
	 //only delete group comments
	if (HRLogEnabled && Isinit)
	{
		/*
		char buffer[100000];
		va_list args;
		va_start (args, format);
		vsprintf (buffer,format, args);
		//perror (buffer);
		va_end (args);
		
	
		
	// write 'buffer' to the logfile
	//	FILE *f = fopen(Filename,"a");
	//	if(f)
	//	{
	//		fputs(buffer,f);
	//		fclose(f);
	//	}
		
		
		// print 'buffer' so we see it on the net console
	
		//printf("%s",buffer);
		printf("%s",buffer);
		*/
	};
}
