#ifndef _ETPIMAGE_H_
#define _ETPIMAGE_H_

#include <vector>
#include <string>
#include <Magick++.h>
using namespace std;
using namespace Magick;

class ETPImage
{
public:
	ETPImage();
	~ETPImage();

	bool SetImages(const vector<string>& images);
	const string& GetBasename();
	bool AppendImage(const string& filename);
	void CreateIni(const string& filename);

private:
	struct ETPImageInfo
	{
		int lane; //车道
		int speed; //车速
		int code; //违章代码
		int numberX; //车牌坐标X
		int numberY; //车票坐标Y
		int index; //图片序号
		string ip; //IP地址
		string text; //叠加在图片上的文字
		string datetime; //日期时间
		string number; //车牌号
		string fullname; //文件全路径名
	};
	void GetImageInfo(const string& filename, ETPImageInfo& imageInfo);
	bool SetZoomArea(int x, int y, int width, int height);
	bool AnnotateImage(Image& img, const string& text, const string& clr);
	void CreateBasename();
	vector<ETPImageInfo> m_images;
	vector<Image> m_imgTop;
	vector<Image> m_imgBottom;
	string m_information;
	string m_basename; //文件名(用于生成图片文件和配置文件的基础名)
};

#endif
