#include "ETPUpload.h"
#include "trace.h"
#include <libgen.h>
#include <stdlib.h>

ETPUpload::ETPUpload() : m_curl(NULL)
{
	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);
}

ETPUpload::~ETPUpload()
{
	Disconnect();
	curl_global_cleanup();
}

int ETPUpload::Connect(const char* szServer, const char* szUsr, const char* szPwd)
{
	int result = -1;

	/* get a curl handle */
	m_curl = curl_easy_init();
	if(m_curl) 
	{
		/* we want to use our own read function */
		curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, ReadCallback);

		/* enable uploading */
		curl_easy_setopt(m_curl, CURLOPT_UPLOAD, 1L);

		/* specify target */
		/* The path part of an FTP request specifies the file to retrieve and from what directory */
		string sRemoteUrl = szServer;
		sRemoteUrl += UPLOAD_FILE_AS;
		curl_easy_setopt(m_curl, CURLOPT_URL, sRemoteUrl.c_str());
		curl_easy_setopt(m_curl, CURLOPT_USERNAME, szUsr);
		curl_easy_setopt(m_curl, CURLOPT_PASSWORD, szPwd);

		result = 0;
	}
	return result;
}

int ETPUpload::Upload(const char* szFileName)
{
	CURLcode res;
	struct curl_slist *headerlist = NULL;
	char szTemp[260] = {0};
	string sUploadFile = "RNFR ";
	string sRenameFile = "RNTO ";
	snprintf(szTemp, sizeof(szTemp), "%s", szFileName);
	sUploadFile += UPLOAD_FILE_AS;
	sRenameFile += basename(szTemp);
	
	/* build a list of commands to pass to libcurl */
	headerlist = curl_slist_append(headerlist, sUploadFile.c_str());
	headerlist = curl_slist_append(headerlist, sRenameFile.c_str());
	/* pass in that last of FTP commands to run after the transfer */
	curl_easy_setopt(m_curl, CURLOPT_POSTQUOTE, headerlist);

	FILE *hd_src;
	struct stat file_info;
	curl_off_t fsize;

	/* get the file size of the local file */
	if(stat(szFileName, &file_info)) 
	{
		PRINT(("[ERROR]Could't open '%s' : %s\n", szFileName, strerror(errno)));
		return -1;
	}
	fsize = (curl_off_t)file_info.st_size;
	
	PRINT(("[INFO]Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.\n", fsize));

	/* get a FILE * of the same file */
	hd_src = fopen(szFileName, "rb");

	/* now specify which file to upload */
	curl_easy_setopt(m_curl, CURLOPT_READDATA, hd_src);

	/* Set the size of the file to upload (optional).  If you give a *_LARGE
	 * option you MUST make sure that the type of the passed-in argument is a
	 * curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
	 * make sure that to pass in a type 'long' argument. */
	curl_easy_setopt(m_curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)fsize);
	
	/* Now run off and do what you've been told! */
	res = curl_easy_perform(m_curl);
	/* Check for errors */
	if(res != CURLE_OK)
	{
		PRINT(("[ERROR]curl_easy_perform() failed: %s\n", curl_easy_strerror(res)));
	}

	/* clean up the FTP commands list */
	curl_slist_free_all (headerlist);
	
	fclose(hd_src); /* close the local file */

	return 0;
}

void ETPUpload::Disconnect()
{
	if(m_curl) 
	{
		/* always cleanup */
		curl_easy_cleanup(m_curl);
		m_curl = NULL;
	}
}

size_t ETPUpload::ReadCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	curl_off_t nread;
	/* in real-world cases, this would probably get this data differently
	 * as this fread() stuff is exactly what the library already would do
	 * by default internally */
	size_t retcode = fread(ptr, size, nmemb, (FILE*)stream);

	nread = (curl_off_t)retcode;
	
//	fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
//			" bytes from file\n", nread);
	return retcode;
}
