#ifndef _ETPUPLOAD_H_
#define _ETPUPLOAD_H_

#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#define UPLOAD_FILE_AS  "while-uploading.txt"

class ETPUpload
{
public:
	ETPUpload();
	~ETPUpload();

	int Connect(const char* szServer, const char* szUsr, const char* szPwd);
	int Upload(const char* szFileName);
	void Disconnect();

protected:
	static size_t ReadCallback(void *ptr, size_t size, size_t nmemb, void *stream);

private:
	CURL *m_curl;
};

#endif
