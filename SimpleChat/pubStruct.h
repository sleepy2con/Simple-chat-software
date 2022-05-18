#ifndef PUBSTRUCT_H
#define PUBSTRUCT_H
#include <QLayout>
#include <QString>
#include <QDateTime> 
#include <QNetworkInterface>
#include <QList>
#include <QPair>

//DataBaseManager* dbManager;			// 
void clearWidgets(QLayout* layout);
QString getLocalIp();


struct ST_LoginInfo
{
	QString sUserName;							// 姓名
	QString sPassword;							// 密码
};

struct ST_UserInfo
{
	int id;
	QString sUserName;							// 姓名
	QString sPassword;							// 密码
	QDateTime createTime;
	bool bifOnLine;
	QString slocalIp;
};

struct ST_friendInfo
{
	int id;	// 账号
	QString sUserName;							// 姓名
	QDateTime addTime;		// 添加好友时间。
	bool bifOnLine;
	QString sIp;
};

struct ST_udpContentHeader
{
	int id;
};

// sql对应的状态码
enum
{
	Success,
	Failed,

	QueryExecFailed,

	LoginSuccess,
	DBIsNotOpen,
	QueryCountLessThan0,

	AddUserSuccess,
	WannaAddUserButThereHasBeen,

	// 添加好友。
	CannotFindThidUser,
	AlreadyHaveThisRelation,
	AddFriSuccess,

	// 群
	CreateGroupSuccess,
	insertGroupRelationSuccess,

	updateChatGroupMembersInfoSuccess,

	AlreadyHaveThisGroupRelation,
	CouldFindTheGroup,
};
enum ChatMsgType { ChatMsg, OnLine, OffLine, SfileName, RefFile };//消息类型

enum MsgType{GroupType,BetweenUsers};

struct stUdpContentHeader
{
	quint32 senderId;
	quint32 receiverId;
	quint32 msgType;
	friend QDataStream& operator<<(QDataStream& out,const stUdpContentHeader&);
	friend QDataStream& operator>>(QDataStream& in, stUdpContentHeader&);
};

struct stChatGroup
{
	int id;
	int iOwnId;
	QMap<QString,int> members;		// 用来存储群组成员
	QString sNickName;
	QDateTime addTime;
	//stChatGroup& operator=(const stChatGroup& data);
};

struct stGroupRelation
{
	int id;
	int igroupId;
	int memberId;
	QDateTime addTime;
	QDateTime exitTime;
};

// 
class CurUserData
{
public:
	static int iPubPort4Udp;
	static ST_UserInfo curUserInfo;
	static ST_friendInfo curChosenUser;

};

#endif

