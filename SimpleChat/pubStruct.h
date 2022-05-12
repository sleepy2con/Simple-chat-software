#ifndef PUBSTRUCT_H
#define PUBSTRUCT_H
#include <QString>

struct ST_UserInfo
{
	QString sUserName;							// 姓名
	QString sPassword;							// 密码
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
	

};

#endif

