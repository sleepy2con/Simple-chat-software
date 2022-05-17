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
QString getLocalIp()
{
	QList<QHostAddress> addrlist = QNetworkInterface::allAddresses();
	foreach(QHostAddress addr, addrlist)
	{
		if (addr.protocol() == QAbstractSocket::IPv4Protocol) return addr.toString();
	}
	return 0;
}
ST_UserInfo CurUserData::curUserInfo = { 0,"","",QDateTime::currentDateTime(),false,""};
ST_friendInfo CurUserData::curChosenUser = {};
int CurUserData::iPubPort4Udp = 23333;

QDataStream& operator<<(QDataStream& out, const stUdpContentHeader& st) { out << st.senderId << st.receiverId << st.msgType; return out; }
QDataStream& operator>>(QDataStream& in, stUdpContentHeader& st) { in >> st.senderId >> st.receiverId >> st.msgType; return in; }

//stChatGroup& stChatGroup::operator=(const stChatGroup& data)
//{
//	this->id = data.id;
//}