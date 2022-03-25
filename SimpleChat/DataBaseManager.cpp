#include "DataBaseManager.h"
#include <QSqlerror>
void DataBaseManager::Init(const QString& sHost, const int& iPort, const QString& sDBName, const QString& sUserName, const QString& sPassword)
{
	m_sHost = sHost;
	m_iPort = iPort;
	m_sDBName = sDBName;
	m_sUserName = sUserName;
	m_sPassword = sPassword;
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
		qDebug()<<"opendb failed";
		qDebug()<< m_dataBase.lastError().text();
		return false;
	}
	return true;
}

