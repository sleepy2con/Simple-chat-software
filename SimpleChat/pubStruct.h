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

// 全局变量
class CurUserData
{
public:
	static int iPubPort4Udp;
	static ST_UserInfo curUserInfo;			//登录后储存自己信息
	static ST_friendInfo curChosenUser;		// 单对单聊天时储存对面信息
	static stChatGroup curChatGroup;		// 群聊时储存当前群组的临时信息；
	static bool chatType;		// 当前聊天类型，是群聊还是单对单，1-user,0-group;
	static QList<stChatGroup> thoseGroupsWhatIAmIn;	// 自己加入的群聊id List
};

#endif

