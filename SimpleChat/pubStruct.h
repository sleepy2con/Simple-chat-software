#ifndef PUBSTRUCT_H
#define PUBSTRUCT_H
#include <QString>

struct ST_UserInfo
{
	QString sUserName;							// ����
	QString sPassword;							// ����
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
	

};

#endif

