/**
 * @file DBConnection.h
 * @brief Sqlite数据库封装
 * @author Hoolg
 * @version 1.0
 * @date 2012-07-31
 */
#ifndef __DBCONNECTION_H__
#define __DBCONNECTION_H__

#include <string>
using namespace std;

/**
 * @brief 前置声明，避免包含头文件
 */
struct sqlite3;
struct sqlite3_stmt;

/**
 * @brief 数据库管理类
 */
class CDBConnection
{
public:
	CDBConnection(void);
	~CDBConnection(void);

public:
	/**
	 * @brief 打开数据库
	 * @param szDb 数据库文件名
	 * @return 0表示成功，-1表示失败
	 */
	int Open(const char *szDb);

	/**
	 * @brief 关闭数据库
	 */
	void Close();

	/**
	 * @brief 判断数据库是否已打开
	 * @return 0表示未打开，1表示已打开
	 */
	int IsOpen();

	/**
	 * @brief 用于执行不需要返回结果集的语句
	 * @param szSql SQL语句字符串
	 * @return 0表示执行成功，-1表示执行失败
	 */
	int ExecuteSql(const char *szSql);

	/**
	 * @brief 用于执行查询语句，返回的结果可以由CDBRecordSet处理
	 * @param szSql SQL语句字符串
	 * @return 返回查询的结果，返回的结果可以由CDBRecordSet处理
	 */
	sqlite3_stmt* QuerySql(const char *szSql);

private:
	/**
	 * @brief 数据库指针
	 */
	sqlite3* m_pConn;
};


/**
 * @brief 数据集操作类
 */
class CDBRecordSet 
{
public:
	CDBRecordSet(sqlite3_stmt* );
	~CDBRecordSet();

public:
	/**
	 * @brief 查询数据集中的下一条记录
	 * @return 0表示无记录，1表示还有记录
	 */
	int Next();

	/**
	 * @brief 根据结果集中的列序号获取结果
	 * @param nCol 结果集列序号（以0开始）
	 * @param szBuf 结果缓存
	 * @param nLen 结果缓存长度
	 * @return 0表示成功，-1表示失败
	 */
	int GetValue(int nCol, char *szBuf, int nLen);

	/**
	 * @brief 根据结果集中的列序号获取结果
	 * @param nCol 结果集列序号（以0开始）
	 * @param sResult 结果缓存
	 * @return 0表示成功，-1表示失败
	 */
	int GetValue(int nCol, string& sResult);

private:
	/**
	 * @brief 结果集类禁止使用拷贝构造函数和赋值函数 
	 */
	CDBRecordSet(const CDBRecordSet& rhs);
	CDBRecordSet& operator=(CDBRecordSet& rhs);

private:
	/**
	 * @brief 数据集结果指针
	 */
	sqlite3_stmt* m_pRes;
};

#endif
