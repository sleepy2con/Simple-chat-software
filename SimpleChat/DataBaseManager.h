#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QString> 
#include <QSqlDatabase>
#include <QSqlQuery>

class DataBaseManager
{
public:
	DataBaseManager();
	~DataBaseManager();
	void Init(const QString& sHost, const int& iPort, const QString& sDBName,
		const QString& sUserName, const QString& sPassword);
	
	bool openDB();

	inline bool IsOpen(){return m_dataBase.isOpen();}

	inline void Close()
	{
		if (m_dataBase.isOpen())
		{
			m_dataBase.close();
		}
	}

	bool Exec(const QString& sSql)
	{
		if (!openDB())
		{
			return false;
		}
		QSqlQuery query = m_dataBase.exec(sSql);
		return query.isValid();
	}

private:

	QSqlDatabase m_dataBase;
	QString m_sHost;
	int m_iPort;
	QString m_sDBName;
	QString m_sUserName;
	QString m_sPassword;

};

#endif