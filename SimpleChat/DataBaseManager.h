#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QString> 
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>

#include "pubStruct.h"


class DataBaseManager
{
public:
	DataBaseManager();
	~DataBaseManager(); 

	bool openDB();

	inline bool IsOpen() { return m_dataBase.isOpen(); }

	void Close() { if (m_dataBase.isOpen())m_dataBase.close(); }

	bool Exec(const QString& sSql);

	// 用户相关操作
	int QueryUser(const QString& UserCode, const QString& Password);		// 用于登录验证
	int AddUser(const ST_LoginInfo& stUserInfo);
	int UpdateUserIp(int id,const QString& ip);

	// 
	bool UpdateUser(const ST_LoginInfo& stUserInfo, bool bUpdatePassword = false);
	bool DelUser(const QString& sUserCode);

	// 查找添加好友
	int AddFri(const QString& myid,const QString& userid);

	// 获取好友列表
	int getFriendsList(int id,QList<ST_friendInfo>& friInfoList);

	int changeOnlineStatus(int id, bool b = true);

	int getUserInfo(int id, ST_UserInfo& userInfo);

	// 群聊相关操作
	int CreateChatGroup(const stChatGroup& chatName);

private:

	QSqlDatabase m_dataBase;
	QString m_sHost;
	int m_iPort;
	QString m_sDBName;
	QString m_sUserName;
	QString m_sPassword;

};

#endif