#include <iostream> 
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "ETPImage.h"
#include "ETPDetectImage.h"
#include "ETPConfig.h"
#include "ETPUpload.h"
#include "trace.h"
using namespace std; 

int DetectImageCallback(const vector<string>& images)
{
//	PRINT(("[INFO]Images size : %d\n", images.size()));
	ETPImage image;
	PRINT(("[INFO]Reading images...\n"));
	image.SetImages(images);
	string name = image.GetBasename();
	name.insert(0, "/home/");
	PRINT(("[INFO]Appending images...\n"));
	image.AppendImage(name+".jpg");
	PRINT(("[INFO]Create ini file...\n"));
	image.CreateIni(name+".ini");

	if(ETPConfig::GetCfg()->Upload().enable_upload == "checked")
	{
		PRINT(("[INFO]Uploading image via TCP...\n"));
	}

	if(ETPConfig::GetCfg()->FTP().enable_ftp == "checked")
	{
		if(ETPConfig::GetCfg()->FTP().enable_time == "checked")
		{
			time_t t;
			struct tm* a;
			time(&t);
			a = localtime(&t);
			int timeBegin = atoi(ETPConfig::GetCfg()->FTP().time_start.c_str());
			int timeEnd = atoi(ETPConfig::GetCfg()->FTP().time_finished.c_str());
			if(a->tm_hour<timeBegin || a->tm_hour>timeEnd)
			{
				PRINT(("[INFO]Uploading time invalid\n"));
				return 0;
			}
		}

		string sFTPServer = "ftp://";
		string sFTPUser = ETPConfig::GetCfg()->FTP().username;
		string sFTPPwd = ETPConfig::GetCfg()->FTP().password;
		sFTPServer += ETPConfig::GetCfg()->FTP().remote;
		sFTPServer += ":";
		sFTPServer += ETPConfig::GetCfg()->FTP().port;
		sFTPServer += "/";

		PRINT(("[INFO]Uploading image via FTP...\n"));
		ETPUpload upload;
		upload.Connect(sFTPServer.c_str(), sFTPUser.c_str(), sFTPPwd.c_str());
		string upfile = name+".jpg";
		upload.Upload(upfile.c_str());
		upfile = name+".ini";
		upload.Upload(upfile.c_str());
		upload.Disconnect();
	}

	return 0;
}

int main(int argc,char **argv) 
{ 
	InitializeMagick(*argv);

	// 读取配置
	ETPConfig::GetCfg()->UpdateCfg();

	// 检测图片
  	ETPDetectImage detectImage("/home/testimages");
  	if(detectImage.Run(DetectImageCallback)!=0) 
	{
		return -1;
  	}


	while(1)
	{
		usleep(1000);
	}

	detectImage.Stop();
	
	return 0; 
}
