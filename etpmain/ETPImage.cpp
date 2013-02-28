#include "ETPImage.h"
#include "ETPConfig.h"
#include "trace.h"
#include <libgen.h>
#include <list>
#include <cstring>
#include <cstdlib>
#include <cstdio>
using namespace std;

ETPImage::ETPImage()
{
}

ETPImage::~ETPImage()
{
}


bool ETPImage::SetImages(const vector<string>& images)
{
	m_images.clear();
	m_imgTop.clear();
	m_imgBottom.clear();
	m_information="";

	bool result = false;
	if(images.size()!=3)
	{
		PRINT(("[ERROR]Not enough photos, there must have three photos\n"));
		return result;
	}
	try 
	{
		vector<string>::const_iterator it = images.begin();
		for(; it != images.end(); ++it) 
		{
			PRINT(("[INFO]Reading image %s\n", (*it).c_str()));

			ETPImageInfo image_info;
			GetImageInfo(*it, image_info);
			m_images.push_back(image_info);

			Image img(image_info.fullname);
			AnnotateImage(img, image_info.text, "red");
			switch(image_info.index)
			{
			case 1:
				m_imgTop.push_back(img);
				break;
			case 2:
				if(m_imgBottom.size()>0)
					m_imgBottom.insert(m_imgBottom.begin(), img);
				else
					m_imgBottom.push_back(img);
				break;
			case 3:
				m_imgBottom.push_back(img);
				break;
			}
		}
		SetZoomArea(m_images[0].numberX, m_images[0].numberY, 1000, 1000);

		char buf[100] = {0};
		m_information+=" 路口地点：";
		m_information+=ETPConfig::GetCfg()->Cross().name;
		m_information+=" 路口号：";
		m_information+=ETPConfig::GetCfg()->Cross().serial;
		m_information+=" 抓拍时间：";
		m_information+=m_images[0].datetime;
		m_information+="\n 路口方向：";
		m_information+="如果没启用或启用多个如何判定？";
		m_information+=" 当前车道：";
		m_information+=m_images[0].lane+'0';
		m_information+=" 车牌：";
		m_information+=m_images[0].number;
		m_information+="\n 红灯时间：";
		m_information+="从哪知道红灯时间？";
		m_information+=" 车速：";
		snprintf(buf, sizeof(buf), "%d", m_images[0].speed);
		m_information+=buf;
		m_information+="km/h";
		m_information+=" 违章类型：";
		snprintf(buf, sizeof(buf), "%d", m_images[0].code);
		m_information+=buf;

		CreateBasename();

		result = true;
	}
	catch (Exception &error_)
	{
		PRINT(("[ERROR]Set image exception : %s\n", error_.what()));
		m_imgTop.clear();
		m_imgBottom.clear();
	}
	return result;
}

const string& ETPImage::GetBasename()
{
	return m_basename;
}

void ETPImage::GetImageInfo(const string& filename, ETPImageInfo& imageInfo)
{
	imageInfo.fullname=filename;
	char *tmp=strdup(filename.c_str());
	string image=basename(tmp);
	free(tmp);

	//去后缀名
	char buf[260] = {0};
	int dot = image.rfind('.');
	if(dot != string::npos) 
	{
		snprintf(buf,sizeof(buf),"%s",image.substr(0,dot).c_str());
	}
	else
	{
		snprintf(buf,sizeof(buf),"%s",image.c_str());
	}
//	PRINT(("[INFO]File name : %s\n", buf));
	
	int index = 0;
	char *token = strtok(buf, "_");
	while(token)
	{
		switch(index)
		{
		case 0:
			imageInfo.ip=token;
			break;
		case 1:
			imageInfo.datetime=token;
			if(imageInfo.datetime.length()==17)
			{
				imageInfo.datetime.insert(4,"/");
				imageInfo.datetime.insert(7,"/");
				imageInfo.datetime.insert(10," ");
				imageInfo.datetime.insert(13,":");
				imageInfo.datetime.insert(16,":");
				imageInfo.datetime.insert(19,".");
			}
			else
			{
				PRINT(("[ERROR]Datetime invalid format\n"));
			}
			break;
		case 2:
			imageInfo.lane=atoi(token);
			break;
		case 3:
			imageInfo.speed=atoi(token);
			break;
		case 4:
			imageInfo.code=atoi(token);
			break;
		case 5:
			imageInfo.number=token;
			break;
		case 6:
			if(strlen(token)==8)
			{
				string pos=token;
				imageInfo.numberX=atoi(pos.substr(0,4).c_str());
				imageInfo.numberY=atoi(pos.substr(4,4).c_str());
			}
			else
			{
				PRINT(("[ERROR]Position invalid format\n"));
			}
			break;
		case 7:
			imageInfo.index=atoi(token);
			break;
		}
		token = strtok(NULL, "_");
		++index;
	}

	// 生成叠加在图片上的文字
	imageInfo.text="\n\t时间：";
	imageInfo.text+=imageInfo.datetime;
	imageInfo.text+="\n\n";
	imageInfo.text+="\t红灯时间：";
	imageInfo.text+=imageInfo.index-1+'0';
}

bool ETPImage::SetZoomArea(int x, int y, int width, int height)
{
	bool result = false;
	try {
		if(m_imgTop.size()>0)
		{
			Image firstImg = m_imgTop.at(0);
			Geometry size = firstImg.size();
			size.aspect(true);
			firstImg.crop(Geometry(width, height, x, y));
			firstImg.zoom(size);
			m_imgTop.insert(m_imgTop.begin(), firstImg);
			result = true;
		}
	}
	catch (Exception &error_)
	{
		PRINT(("[ERROR]Zoom image exception : %s\n", error_.what()));
	}
	return result;
}

bool ETPImage::AppendImage(const string& filename)
{
	bool result = false;
	try {
		Image top;
		Image bottom;
		Image view;
		Image final;
		vector<Image> tmp;
		appendImages(&top, m_imgTop.begin(), m_imgTop.end());
		appendImages(&bottom, m_imgBottom.begin(), m_imgBottom.end());
		tmp.push_back(top);
		tmp.push_back(bottom);
		appendImages(&view, tmp.begin(), tmp.end(), true);
		Geometry size = view.size();

		// 生成信息图片
		tmp.clear();
		Geometry infoSize(size);
		infoSize.height(355);
		Image info(infoSize, Color("black"));
		AnnotateImage(info, m_information, "white");
		tmp.push_back(view);
		tmp.push_back(info);
		appendImages(&final, tmp.begin(), tmp.end(), true);

		//画出分割线
		list<Drawable> line_draw;
		line_draw.push_back(DrawableLine(0,size.height()/2,size.width(),size.height()/2));
		line_draw.push_back(DrawableLine(size.width()/2,0,size.width()/2,size.height()));
		line_draw.push_back(DrawableStrokeColor(Color("yellow")));
		line_draw.push_back(DrawableStrokeWidth(16.0));
		final.draw(line_draw);

		Geometry realSize(size);
		int image_size=atoi(ETPConfig::GetCfg()->Cross().image_size.c_str());
		switch(image_size)
		{
		case 0:
			PRINT(("[INFO]Image size 1024*800\n"));
			realSize.width(1024);
			realSize.height(800);
			break;
		case 1:
			PRINT(("[INFO]Image size 1280*1024\n"));
			realSize.width(1280);
			realSize.height(1024);
			break;
		case 2:
			PRINT(("[INFO]Image size 1920*1080\n"));
			realSize.width(1920);
			realSize.height(1080);
			break;
		case 3:
			PRINT(("[INFO]Image size 1600*1200\n"));
			realSize.width(1600);
			realSize.height(1200);
			break;
		case 4:
			PRINT(("[INFO]Image size 2048*1536\n"));
			realSize.width(2048);
			realSize.height(1536);
			break;
		case 5:
			PRINT(("[INFO]Image size 2592*1944\n"));
			realSize.width(2592);
			realSize.height(1944);
			break;
		}
		realSize.aspect(true);
		final.zoom(realSize);

		final.write(filename);
		result = true;
	}
	catch(Exception &error_)
	{
		PRINT(("[ERROR]Append image exception : %s\n", error_.what()));
	}
	return result;
}

void ETPImage::CreateIni(const string& filename)
{
	FILE *f = fopen(filename.c_str(), "w+");
	if(f)
	{
		char buf[256] = {0};
		string str="<dzjk>";
		str+="\n<wjm>";
		str+=m_basename; //文件名
		str+="</wjm>";
		str+="\n<wfsj>";
		str+=m_images[0].datetime; //违法时间，来自图片信息
		str+="</wfsj>";
		str+="\n<wfdd>";
		str+=ETPConfig::GetCfg()->Cross().code; //违法地点，来自配置(全国统一标准代码)
		str+="</wfdd>";
		str+="\n<wfxw>";
		snprintf(buf, sizeof(buf), "%d", m_images[0].code);
		str+=buf; //违法行为，来自图片信息(广东省标准代码)
		str+="</wfxw>";
		str+="\n<dlxzsd>";
		str+=ETPConfig::GetCfg()->Speed().speed_limit; //道路限制速度，来自配置
		str+="</dlxzsd>";
		snprintf(buf, sizeof(buf), "%d", m_images[0].speed);
		str+="\n<clxssd>";
		str+=buf; //道路行驶速度，来自图片信息
		str+="</clxssd>";
		str+="\n<hphm>";
		str+=m_images[0].number; //车牌号，来自图片信息
		str+="</hphm>";
		str+="\n<hpzl>";
		str+="蓝"; //车牌种类，来源不明(公安部标准代码)
		str+="</hpzl>";
		str+="\n<ssdw>";
		str+=ETPConfig::GetCfg()->Cross().serial; //执法单位，来自配置(东莞市交警支队统一标准代码)
		str+="</ssdw>";
		str+="\n<zqmj>";
		str+=ETPConfig::GetCfg()->Cross().op; //执勤民警，来自配置(东莞市交警支队统一标准代码)
		str+="</zqmj>";
		str+="\n</dzjk>";
		fputs(str.c_str(), f);
		fclose(f);
	}
}

bool ETPImage::AnnotateImage(Image& img, const string& text, const string& clr)
{
	bool result = false;
	try {
		Geometry size = img.size();
		list<Drawable> text_draw;
		text_draw.push_back(DrawableFont("/usr/share/fonts/truetype/arphic/gkai00mp.ttf"));
		text_draw.push_back(DrawableText(0, 0, text));
		text_draw.push_back(DrawableStrokeColor(Color(clr)));
		text_draw.push_back(DrawableStrokeWidth(5.0));
		text_draw.push_back(DrawableFillColor(Color(clr)));
		text_draw.push_back(DrawablePointSize(120.0));
		img.annotate(text,size,Magick::NorthWestGravity);
		img.draw(text_draw);
		result = true;
	}
	catch(Exception &error_)
	{
		PRINT(("[ERROR]Annotate image exception : %s", error_.what()));
	}
	return result;
}

void ETPImage::CreateBasename()
{
	m_basename="20130119-deviceid-serialnumber";
}
