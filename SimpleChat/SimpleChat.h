#ifndef SIMPLECHAT_H
#define SIMPLECHAT_H
#include <QUdpSocket>
#include <QtWidgets/QMainWindow>
#include "ui_SimpleChat.h"
#include "DataBaseManager.h"
class LoginWidget;
class QFileSystemWatcher;
class Baseframe;

class SimpleChat : public QMainWindow
{
	Q_OBJECT

public:
	SimpleChat(QWidget* parent = Q_NULLPTR);
	~SimpleChat();
	void autoLoadQssFile(const QString& strQssPath);

	// 全局变量
	static int iCurUserId;			// 当前用户ID,0表示未登录

public slots:
	
	void on_btn_add_clicked();
	// 发送消息
	void on_btn_send_clicked();

	void on_btn_createGroup_clicked();
private:

	void initConnect();
	void initFriendList();	//	刷新显示好友列表
	void initChatGroupList();

	void initUdpSocket();

	void ResponseByDifferentStateNum(int iStateNum);

private:
	Ui::MainWindow m_ui;
	LoginWidget* m_pLoginWidget;
	QFileSystemWatcher* m_fileWatcher;
	DataBaseManager* m_dbManager;
	Baseframe* m_baseframe;

	// 网络通信
	QUdpSocket* myUdpSocket;
	int myUdpPort;
	
	QTimer* m_timer;

};
#endif // !SIMPLECHAT_H