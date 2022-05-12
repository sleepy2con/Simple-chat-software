#include "DataBaseManager.h"
#include <QSqlerror>
#include <QDateTime> 
DataBaseManager::DataBaseManager()
{
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

int DataBaseManager::QueryUser(QString UserCode, QString Password)
{
	if (!IsOpen())
	{
		qDebug()<<"数据库未打开";
		return DBIsNotOpen;
	}

	QSqlQuery query(m_dataBase);

	// 判断数据库中是否已存在记录
	QString sSql = "select count(*) from user where username=? and password=?";
	query.prepare(sSql);
	query.bindValue(0, UserCode);
	query.bindValue(1, Password);

	if (!query.exec())
	{
		qDebug() << "sqlquery.exe执行失败";
		m_dataBase.close();
		return QueryExecFailed;
	}

	int iCount = 0;
	if (query.next())
	{
		iCount = query.value(0).toInt();
	}

	if (iCount <= 0)
	{
		return QueryCountLessThan0;
	}
	m_dataBase.close();
	return LoginSuccess;
}

bool DataBaseManager::AddUser(const ST_UserInfo& stUserInfo)
{
	if (!IsOpen())
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

