#ifndef _ETPDETECTIMAGE_H_
#define _ETPDETECTIMAGE_H_

#include <string>
#include <vector>
#include <pthread.h>
using namespace std;

typedef int (*ImageDetected)(const vector<string>& images);

class ETPDetectImage
{
public:
	ETPDetectImage(const string& imageDir);
	~ETPDetectImage();

	int Run(ImageDetected pfnCb);
	void Stop();

private:
	void TrashDetected();
	static void* DetectImage(void *arg);
	void Execute();

private:
	bool m_status;
	ImageDetected m_pfnCb;
	string m_imageDir;
	vector<string> m_images;
	pthread_t m_thread;
	bool m_exit;
};

#endif
