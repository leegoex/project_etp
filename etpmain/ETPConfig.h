#ifndef _ETPCONFIG_H_
#define _ETPCONFIG_H_

#include <string>
#include "ETPDef.h"
using namespace std;

class CDBConnection;
class ETPConfig
{
public:
	~ETPConfig();

	static ETPConfig* GetCfg();

	const Crossing& Cross();
	const MeasureSpeed& Speed();
	const FTPSetting& FTP();
	const ServerUpload& Upload();
	const ClearSetting& Clear();

	void UpdateCfg();

private:
	static ETPConfig* m_instance;

private:
	ETPConfig();
	ETPConfig(const ETPConfig& rhs);
	ETPConfig& operator=(const ETPConfig& rhs);

	CDBConnection* m_db;
	Crossing m_crossing;
	MeasureSpeed m_measurespeed;
	FTPSetting m_ftpsetting;
	ServerUpload m_serverupload;
	ClearSetting m_clearsetting;
};

#endif
