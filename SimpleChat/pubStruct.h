#ifndef PUBSTRUCT_H
#define PUBSTRUCT_H
#include <QLayout>
#include <QString>
#include <QDateTime> 
#include <QNetworkInterface>
//DataBaseManager* dbManager;			// 
void clearWidgets(QLayout* layout);
QString getLocalIp();


struct ST_LoginInfo
{
	QString sUserName;							// ����
	QString sPassword;							// ����
};

struct ST_UserInfo
{
	int id;
	QString sUserName;							// ����
	QString sPassword;							// ����
	QDateTime createTime;
	bool bifOnLine;
	QString slocalIp;
};

struct ST_friendInfo
{
	int id;	// �˺�
	QString sUserName;							// ����
	QDateTime addTime;		// ��Ӻ���ʱ�䡣
	bool bifOnLine;
	QString sIp;
};

struct ST_udpContentHeader
{
	int id;
};

// sql��Ӧ��״̬��
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

	// ��Ӻ��ѡ�
	CannotFindThidUser,
	AlreadyHaveThisRelation,
	AddFriSuccess,
};
enum ChatMsgType { ChatMsg, OnLine, OffLine, SfileName, RefFile };//��Ϣ����
// 
class CurUserData
{
public:
	static int iPubPort4Udp;
	static ST_UserInfo curUserInfo;
	static ST_friendInfo curChosenUser;
};

#endif

