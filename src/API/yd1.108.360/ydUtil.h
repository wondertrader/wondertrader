#ifndef YD_UTIL_H
#define YD_UTIL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>

#if defined(WINDOWS) | defined(WIN32)
#pragma  warning(disable:4996)
#endif

#define START_HOUR 17

inline unsigned string2TimeID(const char *timeBuffer)
{
	if (*timeBuffer=='\0')
	{
		return 0;
	}
	const int timeAdjust='0'*(36000+3600+600+60+10+1);
	int tmp=(timeBuffer[0]*36000+timeBuffer[1]*3600+timeBuffer[3]*600+timeBuffer[4]*60+timeBuffer[6]*10+timeBuffer[7]);
	if (tmp>START_HOUR*3600+timeAdjust)
	{
		tmp-=START_HOUR*3600+timeAdjust;
	}
	else
	{
		tmp+=(24-START_HOUR)*3600-timeAdjust;
	}
	return tmp;
}

inline const char *timeID2String(unsigned timeID, char *buffer)
{
	if ((int)timeID<=0)
	{
		return "";
	}
	sprintf(buffer,"%02d:%02d:%02d",(timeID/3600+START_HOUR)%24,timeID/60%60,timeID%60);
	return buffer;
}

inline const char *timeStamp2String(unsigned timestamp,char *buffer)
{
	if (timestamp==0)
	{
		return "";
	}
	timeID2String(timestamp/1000,buffer);
	sprintf(buffer+8,".%03d",timestamp%1000);
	return buffer;
}

inline void dumpField(FILE *output,int value)
{
	fprintf(output,"%d ",value);
}

inline void dumpField(FILE *output,unsigned value)
{
	fprintf(output,"%u ",value);
}

inline void dumpField(FILE *output,short value)
{
	fprintf(output,"%d ",value);
}

inline void dumpField(FILE *output,unsigned short value)
{
	fprintf(output,"%u ",(unsigned)value);
}

inline void dumpField(FILE *output,char value)
{
	fprintf(output,"%d ",value);
}

inline void dumpField(FILE *output,unsigned char value)
{
	fprintf(output,"%u ",(unsigned)value);
}

inline void dumpField(FILE *output,unsigned long long value)
{
	fprintf(output,"%llu ",value);
}

inline void dumpField(FILE *output,long long value)
{
	fprintf(output,"%lld ",value);
}

inline void dumpField(FILE *output,bool value)
{
	fprintf(output,"%s ",(value?"true":"false"));
}

inline void dumpField(FILE *output,double value)
{
	char buffer[1000];
	if (value!=DBL_MAX)
	{
		sprintf(buffer,"%.12f",value);
		if (strchr(buffer,'.'))
		{
			char *p=buffer+strlen(buffer)-1;
			while ((p>=buffer)&&(*p=='0'))
			{
				*p='\0';
				p--;
			}
			if ((p>=buffer)&&(*p=='.'))
			{
				*p='\0';
			}
		}
		if (buffer[0]=='\0')
		{
			strcpy(buffer,"0");
		}
		else if (buffer[0]=='-')
		{
			if (buffer[1]=='\0')
			{
				strcpy(buffer,"0");
			}
			else if ((buffer[1]=='0')&&(buffer[2]=='\0'))
			{
				strcpy(buffer,"0");
			}
		}
	}
	else
	{
		buffer[0]='\0';
	}

	fprintf(output,"%s(",buffer);
	unsigned char *p=(unsigned char *)&value;
	for (unsigned i=0;i<sizeof(double);i++)
	{
		fprintf(output,"%02X",p[i]);
	}
	fprintf(output,") ");
}

inline void dumpField(FILE *output,const char *value)
{
	if (value!=NULL)
	{
		fprintf(output,"%s ",value);
	}
	else
	{
		fprintf(output," ");
	}
}

inline void dumpTimeField(FILE *output,int value)
{
	char buffer[16];
	fprintf(output,"%s ",timeID2String(value,buffer));
}

inline void dumpTimeStampField(FILE *output,int value)
{
	char buffer[16];
	fprintf(output,"%s ",timeStamp2String(value,buffer));
}

#endif
