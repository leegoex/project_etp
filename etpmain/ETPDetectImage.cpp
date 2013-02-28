#include "ETPDetectImage.h"
#include "trace.h"
#include <dirent.h>
#include <unistd.h>
using namespace std;

ETPDetectImage::ETPDetectImage(const string& imageDir):
	m_imageDir(imageDir),m_exit(false),m_pfnCb(NULL),m_status(false)
{
}

ETPDetectImage::~ETPDetectImage()
{
	Stop();
}

void* ETPDetectImage::DetectImage(void *arg)
{
	ETPDetectImage* pThis = (ETPDetectImage*)arg;
	if(pThis != NULL) 
	{
		pThis->Execute();
	}
	return (void*)0;
}

void ETPDetectImage::Execute()
{
	while(!m_exit) 
	{
		usleep(1000*1000);
		if(m_imageDir.empty())
		{
			PRINT(("[ERROR]Image directory is unknown\n"));
			return;
		}
		PRINT(("[INFO]Detecting images...\n"));
		DIR* dirp;
		struct dirent* direntp;
		dirp = opendir(m_imageDir.c_str());
		if(dirp!=NULL) 
		{
			m_images.clear();
			for(;;)
			{
				direntp = readdir(dirp);
				if(direntp==NULL) 
					break;
				if(direntp->d_type == DT_DIR) 
					continue;
				string temp = m_imageDir + "/";
				temp += direntp->d_name;
				m_images.push_back(temp);
				if(m_images.size()==3)
				{
					if(m_pfnCb) 
					{
						m_pfnCb(m_images);
					}
					m_images.clear();
					break;
				}
			}
			if(m_images.size()<3) 
			{
				PRINT(("[INFO]Not enough images\n"));
				m_images.clear();
			}
			closedir(dirp);
		}
	}
}

int ETPDetectImage::Run(ImageDetected pfnCb)
{
//	PRINT(("[INFO]Begin to run images detecting\n"));
	m_exit = false;
	int result = pthread_create(&m_thread,NULL,DetectImage,(void*)this);
	if(result!=0) 
	{
		PRINT(("[ERROR]Create detect image thread failed\n"));
		return -1;
	}
	m_pfnCb = pfnCb;
	m_status = true;
//	PRINT(("[INFO]Run images detecting succeed\n"));
	return 0;
}

void ETPDetectImage::Stop()
{
	if(m_status) 
	{
		m_exit = true;
		pthread_join(m_thread, NULL);
		m_images.clear();
		m_status = false;
	}
}

void ETPDetectImage::TrashDetected()
{
	if(m_images.size() > 0)
	{
		//删除还是转移到其他盘
	}
}
