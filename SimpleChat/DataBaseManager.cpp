﻿#include <QSqlerror>
#include <QDateTime> 
#include "DataBaseManager.h"
#include "SimpleChat.h"

DataBaseManager::DataBaseManager()
{
	//qDebug() << QSqlDatabase::drivers();	
	// 读取数据库配置文件
	m_sHost = "43.138.67.149";	// 
	m_iPort = 3306;
	m_sDBName = "simplechatdb";
	m_sUserName = "root";
	m_sPassword = "lz19990405";

}
DataBaseManager::~DataBaseManager()
{

}


bool DataBaseManager::openDB()
{
	if (m_dataBase.isOpen())
	{
		return true;
	}
	m_dataBase = QSqlDatabase::addDatabase("QMYSQL");

	m_dataBase.setHostName(m_sHost);
	m_dataBase.setPort(m_iPort);
	m_dataBase.setDatabaseName(m_sDBName);
	m_dataBase.setUserName(m_sUserName);
	m_dataBase.setPassword(m_sPassword);

	if (!m_dataBase.open())
	{
		qDebug() << "opendb failed";
		qDebug() << m_dataBase.lastError().text();
		return false;
	}
	return true;
}

int DataBaseManager::QueryUser(const QString& UserCode, const QString& Password)
{
	if (!openDB())
	{
		qDebug()<<"数据库未打开";
		return DBIsNotOpen;
	}

	QSqlQuery query(m_dataBase);

	// 判断数据库中是否已存在记录
	QString sSql = "select id from user where username=? and password=?";
	query.prepare(sSql);
	query.bindValue(0, UserCode);
	query.bindValue(1, Password);

	if (!query.exec())
	{
		qDebug() << "sqlquery.exe执行失败";
		m_dataBase.close();
		return QueryExecFailed;
	}

	int id = 0;
	if (query.next())
	{
		id = query.value(0).toInt();
	}

	if (id <= 0)
	{
		return QueryCountLessThan0;
	}
	m_dataBase.close();
	SimpleChat::iCurUserId = id;

	return LoginSuccess;
}

int DataBaseManager::AddUser(const ST_UserInfo& stUserInfo)
{
	if (!openDB())
	{
		qDebug() << "数据库未打开";
		return DBIsNotOpen;
	}

	QSqlQuery query(m_dataBase);

	// 判断数据库中是否已存在记录
	QString sSql = "select count(*) from user where username=?";
	query.prepare(sSql);
	query.bindValue(0, stUserInfo.sUserName);
	if (!query.exec())
	{
		return QueryExecFailed;
	}

	int iCount = 0;
	if (query.next())
	{
		iCount = query.value(0).toInt();
	}

	if (iCount > 0)
	{
		return WannaAddUserButThereHasBeen;
	}

	sSql = "insert into user(username,password,createtime) VALUES(:UserName,:Password,:CreateTime)";
	query.prepare(sSql);
	query.bindValue(":UserName", stUserInfo.sUserName);
	query.bindValue(":Password", stUserInfo.sPassword);
	query.bindValue(":CreateTime",QDateTime::currentDateTime());

	if (!query.exec())
	{
		m_dataBase.close();
		return QueryExecFailed;
	}
	m_dataBase.close();
	return AddUserSuccess;
}

bool DataBaseManager::UpdateUser(const ST_UserInfo& stUserInfo, bool bUpdatePassword)
{
	return 0;
}

bool DataBaseManager::DelUser(const QString& sUserCode)
{
	return 0;
}

int DataBaseManager::AddFri(const QString& myid,const QString& userid)
{

	if (!openDB())
	{
		qDebug() << "数据库未打开";
		return DBIsNotOpen;
	}

	QSqlQuery query(m_dataBase);

	// 判断数据库中是否有该用户。
	QString sSql = "select id from user where id=?";
	query.prepare(sSql);
	query.bindValue(0, userid);
	if (!query.exec())
	{
		return QueryExecFailed;
	}
	int iCunt = 0;
	if (query.next())
	{
		iCunt = query.value(0).toInt();
	}
	// 如果无法找到该用户
	if (iCunt == 0)
	{
		return CannotFindThidUser;
	}

	// 查询是否有此好友关系
	sSql = "select count(*) from friend_relation where userid=? and friend_id=? or userid=? and friend_id=?";		
	query.prepare(sSql);
	query.bindValue(0, myid);
	query.bindValue(1, userid);
	query.bindValue(2, userid);
	query.bindValue(3, myid);
	if (!query.exec())
	{
		return QueryExecFailed;
	}
	int iCount = 0;
	if (query.next())
	{
		iCount = 0;
		iCount = query.value(0).toInt();
	}
	if (iCount > 0)				// 如果结果大于0，最大可能为2，则意味着有该好友了
	{
		return AlreadyHaveThisRelation;
	}


	// 否则，插入这段关系
	sSql = "insert into friend_relation(userid,friend_id,addFriTime) VALUES(:MyID,:FriendID,:CreateTime)";
	query.prepare(sSql);
	query.bindValue(":MyID", myid);
	query.bindValue(":FriendID", userid);
	query.bindValue(":CreateTime", QDateTime::currentDateTime());

	if (!query.exec())
	{
		m_dataBase.close();
		return QueryExecFailed;
	}
	m_dataBase.close();
	return AddFriSuccess;
}

int DataBaseManager::changeOnlineStatus(int id, bool b)
{
	if (!openDB())
	{
		qDebug() << "数据库未打开";
		return DBIsNotOpen;
	}

	QSqlQuery query(m_dataBase);

	// 判断数据库中是否已存在记录
	QString sSql = "update user set ifOnline=? where id=?";

	query.prepare(sSql);
	query.bindValue(0,(b?1:0));
	query.bindValue(1, id);
	if (!query.exec())
	{
		qDebug() << "sqlquery.exe执行失败";
		m_dataBase.close();
		return QueryExecFailed;
	}
	return Success;
}


bool DataBaseManager::Exec(const QString& sSql)
{
	if (!openDB())
	{
		return false;
	}
	QSqlQuery query = m_dataBase.exec(sSql);
	return query.isValid();
}

int DataBaseManager::getFriendsList(int id, QList<ST_friendInfo>& friInfoList)
{
	if (!openDB())
	{
		qDebug() << "数据库未打开";
		return DBIsNotOpen;
	}

	QSqlQuery query(m_dataBase);

	// 判断数据库中是否已存在记录
	QString sSql = "select user.id,user.username,friend_relation.addFriTime from user \
		inner join friend_relation\
		on user.id = friend_relation.userid where friend_id=?\
		UNION\
		select user.id, user.username, friend_relation.addFriTime from user\
		inner join friend_relation\
		on user.id = friend_relation.friend_id where userid = ?";

	query.prepare(sSql);
	query.bindValue(0, id); 
	query.bindValue(1, id);
	if (!query.exec())
	{
		qDebug() << "sqlquery.exe执行失败";
		m_dataBase.close();
		return QueryExecFailed;
	}
	while (query.next())
	{
		ST_friendInfo stTempData;
		stTempData.id = query.value(0).toInt();
		stTempData.sUserName = query.value(1).toString();
		stTempData.addTime = QDateTime::fromString(query.value(2).toString(), "yyyy-MM-dd hh:mm:ss");
		friInfoList.append(stTempData);
	}
	return Success;
}