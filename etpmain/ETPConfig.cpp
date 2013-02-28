#include "ETPConfig.h"
#include "DBConnection.h"
#include "trace.h"

ETPConfig* ETPConfig::m_instance = NULL;

ETPConfig* ETPConfig::GetCfg()
{
	if(m_instance==NULL) {
		m_instance = new ETPConfig();
	}
	return m_instance;
}

ETPConfig::ETPConfig():m_db(NULL)
{
	m_db = new CDBConnection();
	m_db->Open("/home/project_etp/etpconfig/etpconfig.db");
}

ETPConfig::~ETPConfig()
{
	if(m_db)
	{
		m_db->Close();
		delete m_db;
	}
}

const Crossing& ETPConfig::Cross()
{
	return m_crossing;
}

const MeasureSpeed& ETPConfig::Speed()
{
	return m_measurespeed;
}

const FTPSetting& ETPConfig::FTP()
{
	return m_ftpsetting;
}

const ServerUpload& ETPConfig::Upload()
{
	return m_serverupload;
}

const ClearSetting& ETPConfig::Clear()
{
	return m_clearsetting;
}

void ETPConfig::UpdateCfg()
{
	if(m_db->IsOpen() == 0)
		return;
	string sql = "select serial,code,name,kilometer,meter,operator,name_format,image_size,image_quality from etpapp_crossing";
	{
		CDBRecordSet rs(m_db->QuerySql(sql.c_str()));
		if(rs.Next()) 
		{
			rs.GetValue(0, m_crossing.serial);
			rs.GetValue(1, m_crossing.code);
			rs.GetValue(2, m_crossing.name);
			rs.GetValue(3, m_crossing.kilometer);
			rs.GetValue(4, m_crossing.meter);
			rs.GetValue(5, m_crossing.op);
			rs.GetValue(6, m_crossing.name_format);
			rs.GetValue(7, m_crossing.image_size);
			rs.GetValue(8, m_crossing.image_quality);
		}
		else
		{
			PRINT(("[ERROR]No crossing setting exist\n"));
		}
//		PRINT(("[INFO]Update crossing setting finished\n"));
	}
	sql = "select speed_limit,speed_starting,speed_delta,lane_same,enable_direction1,direction1,d1_lane1_delta,d1_lane2_delta,d1_lane3_delta,d1_lane4_delta,enable_direction2,direction2,d2_lane1_delta,d2_lane2_delta,d2_lane3_delta,d2_lane4_delta from etpapp_measurespeed";
	{
		CDBRecordSet rs(m_db->QuerySql(sql.c_str()));
		if(rs.Next()) 
		{
			rs.GetValue(0, m_measurespeed.speed_limit);
			rs.GetValue(1, m_measurespeed.speed_starting);
			rs.GetValue(2, m_measurespeed.speed_delta);
			rs.GetValue(3, m_measurespeed.lane_same);
			rs.GetValue(4, m_measurespeed.enable_direction1);
			rs.GetValue(5, m_measurespeed.direction1);
			rs.GetValue(6, m_measurespeed.d1_lane1_delta);
			rs.GetValue(7, m_measurespeed.d1_lane2_delta);
			rs.GetValue(8, m_measurespeed.d1_lane3_delta);
			rs.GetValue(9, m_measurespeed.d1_lane4_delta);
			rs.GetValue(10, m_measurespeed.enable_direction2);
			rs.GetValue(11, m_measurespeed.direction2);
			rs.GetValue(12, m_measurespeed.d2_lane1_delta);
			rs.GetValue(13, m_measurespeed.d2_lane2_delta);
			rs.GetValue(14, m_measurespeed.d2_lane3_delta);
			rs.GetValue(15, m_measurespeed.d2_lane4_delta);
		}
		else
		{
			PRINT(("[ERROR]No measure speed setting exist\n"));
		}
//		PRINT(("[INFO]Update measure speed setting finished\n"));
	}
	sql = "select enable_ftp,remote,port,enable_time,time_start,time_finished,username,password from etpapp_ftpsetting";
	{
		CDBRecordSet rs(m_db->QuerySql(sql.c_str()));
		if(rs.Next()) 
		{
			rs.GetValue(0, m_ftpsetting.enable_ftp);
			rs.GetValue(1, m_ftpsetting.remote);
			rs.GetValue(2, m_ftpsetting.port);
			rs.GetValue(3, m_ftpsetting.enable_time);
			rs.GetValue(4, m_ftpsetting.time_start);
			rs.GetValue(5, m_ftpsetting.time_finished);
			rs.GetValue(6, m_ftpsetting.username);
			rs.GetValue(7, m_ftpsetting.password);
		}
		else
		{
			PRINT(("[ERROR]No ftp setting exist\n"));
		}
//		PRINT(("[INFO]Update ftp setting finished\n"));
	}
	sql = "select enable_upload,serverip,serverport from etpapp_serverupload";
	{
		CDBRecordSet rs(m_db->QuerySql(sql.c_str()));
		if(rs.Next()) 
		{
			rs.GetValue(0, m_serverupload.enable_upload);
			rs.GetValue(1, m_serverupload.serverip);
			rs.GetValue(2, m_serverupload.serverport);
		}
		else
		{
			PRINT(("[ERROR]No server upload setting exist\n"));
		}
//		PRINT(("[INFO]Update server upload setting finished\n"));
	}
	sql = "select elapse,enable_usb from etpapp_clearsetting";
	{
		CDBRecordSet rs(m_db->QuerySql(sql.c_str()));
		if(rs.Next()) 
		{
			rs.GetValue(0, m_clearsetting.elapse);
			rs.GetValue(1, m_clearsetting.enable_usb);
		}
		else
		{
			PRINT(("[ERROR]No clear setting exist\n"));
		}
//		PRINT(("[INFO]Update clear setting finished\n"));
	}
}
