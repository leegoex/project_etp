#include <sqlite3.h>
#include <unistd.h>
#include <stdio.h>
#include "DBConnection.h"

#ifndef NULL
#define NULL 0
#endif

CDBConnection::CDBConnection(void) : m_pConn(NULL)
{
}

CDBConnection::~CDBConnection()
{
	Close();
}

int CDBConnection::Open(const char *szDb)
{
	if(!szDb || m_pConn)
		return -1;
	if(access(szDb, 0) == -1) {
		printf("The database file is not exist!\n");
		//数据库文件不存在
		return -1;
	}
	//如果文件不存在，sqlite3_open会自动创建该文件
	int nResult = sqlite3_open(szDb, &m_pConn);
	if(nResult != SQLITE_OK) {
		printf("Open database failed:%d\n", nResult);
		return -1;
	}
	return 0;
}

void CDBConnection::Close()
{
	if(m_pConn) {
		sqlite3_close(m_pConn);
		m_pConn = NULL;
	}
}

int CDBConnection::IsOpen()
{
	return (m_pConn!=NULL);
}

int CDBConnection::ExecuteSql(const char *szSql)
{
	if(!m_pConn || !szSql)
		return -1;
	char *szErrMsg = 0;
	if(sqlite3_exec(m_pConn, szSql, 0, 0, &szErrMsg) != SQLITE_OK) {
		printf("Execute sql failed:%s\n", szErrMsg);
		sqlite3_free(szErrMsg);
		return -1;
	}
	return 0;
}

sqlite3_stmt* CDBConnection::QuerySql(const char *szSql)
{
	if(!m_pConn || !szSql)
		return NULL;
	sqlite3_stmt *pStmt;
	int nResult = sqlite3_prepare(m_pConn, szSql, -1, &pStmt, 0);
	if(nResult != SQLITE_OK) {
		printf("Query sql failed:%d\n", nResult);
		return NULL;
	}
	return pStmt;
}

CDBRecordSet::CDBRecordSet(sqlite3_stmt* pRes) : m_pRes(pRes)
{
}

CDBRecordSet::~CDBRecordSet()
{
	if(m_pRes) 
		sqlite3_finalize(m_pRes);
}

int CDBRecordSet::Next()
{
	if(!m_pRes)
		return 0;
	if(sqlite3_step(m_pRes) != SQLITE_ROW)
		return 0;
	return 1;
}

int CDBRecordSet::GetValue(int nCol, char *szBuf, int nLen)
{
	if(!m_pRes || nCol<0)
		return -1;
	const char* pData = (const char*)sqlite3_column_text(m_pRes, nCol);
	if(szBuf) {
		snprintf(szBuf, nLen, "%s", pData);
	}
	return 0;
}

int CDBRecordSet::GetValue(int nCol, string& sResult)
{
	if(!m_pRes || nCol<0)
		return -1;
	const char* pData = (const char*)sqlite3_column_text(m_pRes, nCol);
	sResult = pData;
	return 0;
}
