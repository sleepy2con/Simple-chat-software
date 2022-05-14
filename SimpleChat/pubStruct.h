#ifndef PUBSTRUCT_H
#define PUBSTRUCT_H

#include <QString>
#include <QDateTime> 
//DataBaseManager* dbManager;			// 

struct ST_UserInfo
{
	QString sUserName;							// ����
	QString sPassword;							// ����
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



#endif

