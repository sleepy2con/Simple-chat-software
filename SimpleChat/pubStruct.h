#ifndef PUBSTRUCT_H
#define PUBSTRUCT_H
#include <QLayout>
#include <QString>
#include <QDateTime> 
//DataBaseManager* dbManager;			// 
void clearWidgets(QLayout* layout);

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
};

struct ST_friendInfo
{
	int id;	// 账号
	QString sUserName;							// 姓名
	QDateTime addTime;		// 添加好友时间。
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
};

// 
class CurUserData
{
public:
	static ST_UserInfo curUserInfo;
};

#endif

