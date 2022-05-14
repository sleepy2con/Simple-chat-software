#ifndef PUBSTRUCT_H
#define PUBSTRUCT_H
#include <QLayout>
#include <QString>
#include <QDateTime> 
//DataBaseManager* dbManager;			// 
void clearWidgets(QLayout* layout);

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
};

struct ST_friendInfo
{
	int id;	// �˺�
	QString sUserName;							// ����
	QDateTime addTime;		// ��Ӻ���ʱ�䡣
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

// 
class CurUserData
{
public:
	static ST_UserInfo curUserInfo;
};

#endif

