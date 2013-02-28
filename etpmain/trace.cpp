#include <stdarg.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include "trace.h"
using namespace std;

PrintInfo::PrintInfo(const char *f/* = 0*/, int l/* = 0*/):m_file(f), m_line(l)
{
}

void PrintInfo::Print(const char *fmt, ...)
{
	va_list _va_list;
	char buf[1024] = {0};
//	sprintf(buf, "%s(%d):", m_file, m_line);
	if (fmt == NULL) 
		return;
	va_start(_va_list, fmt);
	vsprintf(buf+strlen(buf), fmt, _va_list);
	va_end(_va_list);
	FILE *fp = fopen("/home/logs/log", "a+");
	if(fp)
	{
		fputs(buf, fp);
		fclose(fp);
	}
	cout<<buf;
}
