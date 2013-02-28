#ifndef _TRACE_H_
#define _TRACE_H_

#include <time.h>
#include <iostream>
#include <sys/time.h>
using namespace std;

class PrintInfo
{
public:
	PrintInfo(const char *f = 0, int l = 0);
	void Print(const char *fmt, ...);
	const char *m_file;
	const int m_line;
};

#define PRINT(X)\
	do {\
	PrintInfo __Log(__FILE__, __LINE__);\
	__Log.Print X;\
	} while(0);

class Timepiece
{
public:
	Timepiece(char * pBuf) 
	{
		gettimeofday(&tvs, NULL);
		m_pBuf = pBuf;
	}
	~Timepiece()
	{
		gettimeofday(&tve, NULL);
		cout<<m_pBuf<<" Time elapsed:"<<(tve.tv_sec-tvs.tv_sec)*1000.0+(tve.tv_usec-tvs.tv_usec)/1000.0<<"ms\n";
	}
private:
	struct timeval tvs;
	struct timeval tve;
	char * m_pBuf;
};

#endif
