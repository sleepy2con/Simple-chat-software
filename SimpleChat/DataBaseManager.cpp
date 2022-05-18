#include <QSqlerror>
#include <QDateTime> 
#include "DataBaseManager.h"
#include "SimpleChat.h"
#include <QMessageBox>
#include <QJsonObject>
#include <QPair>
#include <QList>

DataBaseManager::DataBaseManager()
{
	//qDebug() << QSqlDatabase::drivers();	
	// 读取数据库配置文件
	//m_sHost = "43.138.67.149";	// 云服务器
	m_sHost = "192.168.171.1";
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
		QMessageBox::warning(0, "opendb faile", m_dataBase.lastError().text());
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
	changeOnlineStatus(SimpleChat::iCurUserId);
	UpdateUserIp(id,getLocalIp());
	return LoginSuccess;
}

int DataBaseManager::UpdateUserIp(int id,const QString& ip)
{
	if (!openDB())
	{
		qDebug() << "数据库未打开";
		return DBIsNotOpen;
	}

	QSqlQuery query(m_dataBase);

	// 判断数据库中是否已存在记录
	QString sSql = "update user set ipAddress=? where id=?";

	query.prepare(sSql);
	query.bindValue(0, ip);
	query.bindValue(1, id);
	if (!query.exec())
	{
		qDebug() << "sqlquery.exe执行失败";
		m_dataBase.close();
		return QueryExecFailed;
		return QueryExecFailed;
	}
	return Success;
}

int DataBaseManager::AddUser(const ST_LoginInfo& stUserInfo)
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

	sSql = "insert into user(username,password,createtime,ipAddress) VALUES(:UserName,:Password,:CreateTime,:ipAddress)";
	query.prepare(sSql);
	query.bindValue(":UserName", stUserInfo.sUserName);
	query.bindValue(":Password", stUserInfo.sPassword);
	query.bindValue(":CreateTime",QDateTime::currentDateTime());
	query.bindValue(":ipAddress",getLocalIp());
	if (!query.exec())
	{
		m_dataBase.close();
		return QueryExecFailed;
	}
	m_dataBase.close();
	return AddUserSuccess;
}

bool DataBaseManager::UpdateUser(const ST_LoginInfo& stUserInfo, bool bUpdatePassword)
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

int DataBaseManager::GetAllMyChatGroupInfo(int id, QList<stChatGroup>& groupInfoList)
{
	if (!openDB())
	{
		qDebug() << "数据库未打开";
		return DBIsNotOpen;
	}

	QSqlQuery query(m_dataBase);

	// 判断数据库中是否已存在记录
	QString sSql = "select chatgroup.nickName,chatgroup.members,chatgroup.GroupOwnerID,\
		chatgroup.id, chatgroup_relation.addTime from chatgroup\
		inner join chatgroup_relation\
		on chatgroup.id = chatgroup_relation.group_id where user_id = ?";

	query.prepare(sSql);
	query.bindValue(0, id);
	if (!query.exec())
	{
		qDebug() << "sqlquery.exe执行失败";
		m_dataBase.close();
		return QueryExecFailed;
	}
	groupInfoList.clear();
	while (query.next())
	{
		stChatGroup stTempData;

		QMap<QString,QVariant> members;		// 用来存储群组成员


		stTempData.sNickName = query.value(0).toString();
		QString tempMemData = query.value(1).toString();

		QVariant tempVariant(query.value(2).toString());
		members = tempVariant.toMap();
		for (auto i = members.begin(); i != members.end(); i++)
		{
			QMap<QString, int> tempNode;
			tempNode.insert(i.key(), i.value().toInt());
			stTempData.members.insert(tempNode);
		}

		stTempData.iOwnId = query.value(2).toInt();
		stTempData.id = query.value(3).toInt();
		stTempData.addTime = QDateTime::fromString(query.value(4).toString(), "yyyy-MM-dd hh:mm:ss");;
		groupInfoList.append(stTempData);
	}
	return Success;
}

int DataBaseManager::upDateChatGroupMemberInfo(int id)
{
	if (!openDB())
	{
		qDebug() << "数据库未打开";
		return DBIsNotOpen;
	}

	QSqlQuery query(m_dataBase);

	// 判断数据库中是否已存在记录
	QString sSql = "SELECT user.id,user.username FROM user \
		inner join chatgroup_relation on user.id = chatgroup_relation.user_id\
	where chatgroup_relation.group_id = ?";

	query.prepare(sSql);
	query.bindValue(0, id);

	if (!query.exec())
	{
		qDebug() << "sqlquery.exe执行失败";
		m_dataBase.close();
		return QueryExecFailed;
	}

	// 转换QString
	QMap<QString, QVariant> groupMembers;
	while (query.next())
	{
		groupMembers.insert(query.value(1).toString(), query.value(0).toInt());
	}
	QVariant tempVar(groupMembers);
	QString strMem = tempVar.toString();

	sSql = "update chatgroup set members = ? where id = ?";
	query.prepare(sSql);
	query.bindValue(0, strMem);
	query.bindValue(1, id);
	if (!query.exec())
	{
		qDebug() << "sqlquery.exe执行失败";
		m_dataBase.close();
		return QueryExecFailed;
	}

	return updateChatGroupMembersInfoSuccess;
}

int DataBaseManager::insertChatGroupRelation(const stGroupRelation& stData)
{
	if (!openDB())
	{
		qDebug() << "数据库未打开";
		return DBIsNotOpen;
	}

	// 判断是否有群聊
	QSqlQuery query(m_dataBase);
	QString sSql = "select count(*) from chatgroup where id=?";
	query.prepare(sSql);
	query.bindValue(0, stData.igroupId);
	if (!query.exec())
	{
		m_dataBase.close();
		return QueryExecFailed;
	}
	// select count肯定有值，值为0或者大于0
	query.next();
	if (query.value(0).toInt() == 0)
	{
		return CouldNotFindTheGroup;		// 无法找到群聊
	}

	// 判断是否已经加入该群聊了
	sSql = "select count(*) from chatgroup_relation where group_id=? and user_id =?";
	query.prepare(sSql);
	query.bindValue(0, stData.igroupId);
	query.bindValue(1, stData.memberId);
	if (!query.exec())
	{
		m_dataBase.close();
		return QueryExecFailed;
	}
	query.next();
	if (query.value(0).toInt() != 0)
	{
		return AlreadyHaveThisGroupRelation;		// 
	}

	// 加入群聊
	sSql = "insert into chatgroup_relation(group_id,user_id,addTime) VALUES(?,?,?)";

	query.prepare(sSql);
	query.bindValue(0, stData.igroupId);
	query.bindValue(1, stData.memberId);
	query.bindValue(2, QDateTime::currentDateTime());

	if (!query.exec())
	{
		m_dataBase.close();
		return QueryExecFailed;
	}
	m_dataBase.close();

	// 每次有人加入群后都添加信息到members里
	if (upDateChatGroupMemberInfo(stData.igroupId) != updateChatGroupMembersInfoSuccess)
	{
		return QueryExecFailed;
	}

	return insertGroupRelationSuccess;
}

int DataBaseManager::CreateChatGroup(const stChatGroup& stdata)
{
	if (!openDB())
	{
		qDebug() << "数据库未打开";
		return DBIsNotOpen;
	}

	QSqlQuery query(m_dataBase);
	QString sSql = "insert into chatgroup(nickname,GroupOwnerID) VALUES(?,?)";		// 已创建触发器

	query.prepare(sSql);
	query.bindValue(0, stdata.sNickName);
	query.bindValue(1, stdata.iOwnId);

	if (!query.exec())
	{
		m_dataBase.close();
		return QueryExecFailed;
	}
	m_dataBase.close();

	return CreateGroupSuccess;
}

int DataBaseManager::getUserInfo(int id,ST_UserInfo& stTempData)
{

	if (!openDB())
	{
		qDebug() << "数据库未打开";
		return DBIsNotOpen;
	}

	QSqlQuery query(m_dataBase);

	// 判断数据库中是否已存在记录
	QString sSql = "select * from user where id=?";

	query.prepare(sSql);
	query.bindValue(0, id);
	if (!query.exec())
	{
		qDebug() << "sqlquery.exe执行失败";
		m_dataBase.close();
		return QueryExecFailed;
	}

	while (query.next())
	{
		stTempData.id = query.value(0).toInt();
		stTempData.sUserName = query.value(1).toString();
		stTempData.sPassword = query.value(2).toString();
		stTempData.createTime = QDateTime::fromString(query.value(3).toString(), "yyyy-MM-dd hh:mm:ss");
		stTempData.bifOnLine = query.value(4).toInt();
		stTempData.slocalIp = query.value(5).toString();
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
	QString sSql = "select user.id,user.username,friend_relation.addFriTime,user.ifOnline,user.ipAddress from user \
		inner join friend_relation\
		on user.id = friend_relation.userid where friend_id=?\
		UNION\
		select user.id, user.username, friend_relation.addFriTime,user.ifOnline,user.ipAddress from user\
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
		stTempData.bifOnLine = query.value(3).toInt();
		stTempData.sIp = query.value(4).toString();
		friInfoList.append(stTempData);
	}
	return Success;
}