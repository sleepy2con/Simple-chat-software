#include "pubStruct.h"

void clearWidgets(QLayout* layout)
{
	if (!layout)
		return;
	while (auto item = layout->takeAt(0)) {
		delete item->widget();
		clearWidgets(item->layout());
	}
}

ST_UserInfo CurUserData::curUserInfo = { 0,"","",QDateTime::currentDateTime(),false};