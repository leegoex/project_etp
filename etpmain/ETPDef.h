#ifndef _ETPDEF_H_
#define _ETPDEF_H_

#include <string>
using namespace std;

typedef struct Crossing
{
	string serial;
	string code;
	string name;
	string kilometer;
	string meter;
	string op;
	string name_format;
	string image_size;
	string image_quality;
}CROSSING,*LPCROSSING;

typedef struct MeasureSpeed
{
	string speed_limit;
	string speed_starting;
	string speed_delta;
	string lane_same;
	string enable_direction1;
	string direction1;
	string d1_lane1_delta;
	string d1_lane2_delta;
	string d1_lane3_delta;
	string d1_lane4_delta;
	string enable_direction2;
	string direction2;
	string d2_lane1_delta;
	string d2_lane2_delta;
	string d2_lane3_delta;
	string d2_lane4_delta;
}MEASURESPEED,*LPMEASURESPEED;

typedef struct FTPSetting
{
	string enable_ftp;
	string remote;
	string port;
	string enable_time;
	string time_start;
	string time_finished;
	string username;
	string password;
}FTPSETTING,*LPFTPSETTING;

typedef struct ServerUpload
{
	string enable_upload;
	string serverip;
	string serverport;
}SERVERUPLOAD,*LPSERVERUPLOAD;

typedef struct ClearSetting
{
	string elapse;
	string enable_usb;
}CLEARSETTING,*LPCLEARSETTING;

#endif
