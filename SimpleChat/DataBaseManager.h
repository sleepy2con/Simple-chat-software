#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QString> 
#include <QSqlDatabase>
#include <QSqlQuery>

#include "pubStruct.h"



class DataBaseManager
{
public:
	DataBaseManager();
	~DataBaseManager();

	bool openDB();

	inline bool IsOpen() { return m_dataBase.isOpen(); }

	void Close() { if (m_dataBase.isOpen())m_dataBase.close(); }

	// 用户相关操作
	int QueryUser(QString UserCode, QString Password);
	bool AddUser(const ST_UserInfo& stUserInfo);
	bool UpdateUser(const ST_UserInfo& stUserInfo, bool bUpdatePassword = false);
	bool DelUser(const QString& sUserCode);



private:

	QSqlDatabase m_dataBase;
	QString m_sHost;
	int m_iPort;
	QString m_sDBName;
	QString m_sUserName;
	QString m_sPassword;

};

#endif