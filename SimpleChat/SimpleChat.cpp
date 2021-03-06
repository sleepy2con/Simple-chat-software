#include <QFileSystemWatcher>
#include <QFile>
#include <QDebug> 
#include <QMessageBox>
#include <QScreen> 
#include <QTimer>
#include <QNetworkInterface>

#include "pubStruct.h"

#include "AddDialog.h"
#include "baseframe.h"
#include "SimpleChat.h"
#include "LoginWidget.h"
#include "userWidget.h"
#include "CreateChatGroup.h"


int SimpleChat::iCurUserId = 0;

SimpleChat::SimpleChat(QWidget* parent)
	: QMainWindow(parent)
	, m_pLoginWidget(new LoginWidget(0))
	, m_dbManager(new DataBaseManager())
	, m_baseframe(new Baseframe(parent))
	, m_timer(new QTimer(this))
{
	m_baseframe->addWidget2Content(this);
	m_baseframe->setWindowType(MainWindow);
	m_ui.setupUi(this);
	//this->hide();
	m_baseframe->hide();
	// 初始化数据库
	m_dbManager->openDB();

	m_pLoginWidget->show();
	m_pLoginWidget->setDBPtr(m_dbManager);


	initConnect();
	initUdpSocket();

	QRect screenRect = QGuiApplication::primaryScreen()->geometry();
	//获取设备像素比
	double devicePixelRatio = QGuiApplication::primaryScreen()->devicePixelRatio();
	int screenW = screenRect.width();
	int screenH = screenRect.height();
	m_baseframe->setFixedSize(screenW * 0.5, screenH * 0.5);

	// 初始化时未点击任何用户，令发送按钮不可用。
	m_ui.btn_send->setEnabled(false);

	// 暂时隐藏的功能控件
	m_ui.btn_chat->hide();
	m_ui.btn_relation->hide();
}

SimpleChat::~SimpleChat()
{
	//extern int iCurUserId;
	//iCurUserId = 0;
}

void SimpleChat::autoLoadQssFile(const QString& strQssPath)
{
	QFile* qssFile = new QFile(strQssPath);
	if (!qssFile->open(QFile::ReadOnly))
	{
		qDebug() << "file open error";
		return;
	}
	qApp->setStyleSheet(qssFile->readAll());
	qssFile->close();
}

void SimpleChat::on_btn_add_clicked()
{
	AddDialog* tempDialog = new AddDialog(0);
	tempDialog->setDBPtr(m_dbManager);
	tempDialog->show();
	connect(tempDialog, &AddDialog::AddFriResponse, [=](int iStateNum) {
		ResponseByDifferentStateNum(iStateNum);
		initFriendList();
		});

}

void SimpleChat::on_btn_send_clicked()
{

	quint16 sendport = quint16(CurUserData::iPubPort4Udp);  //获取目标端口
	// 首先判断是群聊还是好友聊天，虽然是单对单，但是选择了广播
	if (CurUserData::chatType)
	{
		QString sendip = CurUserData::curChosenUser.sIp;   //获取目标ip
		

		if (sendip.isEmpty())
		{
			QMessageBox::warning(0, tr("警告"), tr("对方用户未上线过，无法查询到对方IP\n暂无法联系，请以后再联系。"));
			return;
		}

		QByteArray sendData;
		QDataStream write(&sendData, QIODevice::WriteOnly);
		QString tempStr = m_ui.le_sendMsg->text();

		stUdpContentHeader tempData = { CurUserData::curUserInfo.id ,CurUserData::curChosenUser.id,BetweenUsers };
		write << tempData << tempStr;
		//if(myUdpSocket->writeDatagram(sendData, sendData.length(), QHostAddress(sendip), quint16(sendport)) == -1)  //往 IP+prot上 发生数据)
		if (myUdpSocket->writeDatagram(sendData, sendData.length(), QHostAddress::Broadcast, quint16(sendport)) == -1)
		{
			QMessageBox::warning(0, tr("错误"), tr("发送信息失败"));
			return;
		}

		QString curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"); //将时间转换成字符串格式（按格式）
		m_ui.plainTextEdit->appendHtml("<pre style='text-align:right;color:rgb(0,128,64);'>[自己]" + curTime + "< / pre>");
		m_ui.plainTextEdit->appendPlainText(m_ui.le_sendMsg->text()); //添加文本
		m_ui.plainTextEdit->scrollBarWidgets(Qt::AlignBottom);
	}
	// 群聊聊天，仍然是选择广播；
	else
	{
		QByteArray sendData;
		QDataStream write(&sendData, QIODevice::WriteOnly);
		QString tempStr = m_ui.le_sendMsg->text();

		stUdpContentHeader tempData = { CurUserData::curUserInfo.id ,CurUserData::curChatGroup.id,GroupType };	// 发送者自己的ID，群聊的ID，发送类型；
		write << tempData << tempStr;
		//if(myUdpSocket->writeDatagram(sendData, sendData.length(), QHostAddress(sendip), quint16(sendport)) == -1)  //往 IP+prot上 发生数据)
		if (myUdpSocket->writeDatagram(sendData, sendData.length(), QHostAddress::Broadcast, quint16(sendport)) == -1)
		{
			QMessageBox::warning(0, tr("错误"), tr("发送信息失败"));
			return;
		}

		QString curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"); //将时间转换成字符串格式（按格式）
		m_ui.plainTextEdit->appendHtml("<pre style='text-align:right;color:rgb(0,128,64);'>[自己]" + curTime + "< / pre>");
		m_ui.plainTextEdit->appendPlainText(m_ui.le_sendMsg->text()); //添加文本
		m_ui.plainTextEdit->scrollBarWidgets(Qt::AlignBottom);

	}

}

void SimpleChat::on_btn_createGroup_clicked()
{
	CreateChatGroup* tempDialog = new CreateChatGroup(this);

	stChatGroup tempData;
	tempData.iOwnId = CurUserData::curUserInfo.id;
	tempDialog->setData(tempData);
	tempDialog->setDbPtr(m_dbManager);
	tempDialog->show();
	connect(tempDialog, &CreateChatGroup::giveResponse, [=](int iStateNum) {
		ResponseByDifferentStateNum(iStateNum);
		});
	//connect(tempDialog)
}


void SimpleChat::initUdpSocket()
{
	myUdpSocket = new QUdpSocket(this);
	myUdpPort = CurUserData::iPubPort4Udp;
	//myUdpSocket->bind(myUdpPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
	if (!myUdpSocket->bind(myUdpPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint))
	{
		QMessageBox::warning(0, tr("错误"), tr("初始化绑定端口失败,请重启软件"));
		return;
	}
	
	connect(myUdpSocket, &QUdpSocket::readyRead, [=]() {

		while (myUdpSocket->hasPendingDatagrams())
		{
			QByteArray   datagram;
			QHostAddress    peerAddr;
			quint16 peerPort;

			datagram.resize(myUdpSocket->pendingDatagramSize());
			myUdpSocket->readDatagram(datagram.data(), datagram.size(), &peerAddr, &peerPort);

			QDataStream read(&datagram, QIODeviceBase::ReadOnly);
			QString tempStr;
			stUdpContentHeader tempData;
			read >> tempData >> tempStr;
			// 如果是好友聊天的话。其他消息都过滤
			if (tempData.msgType ==BetweenUsers&& CurUserData::chatType == true)
			{
				// 判断接受者是否是自己。
				if (tempData.receiverId != CurUserData::curUserInfo.id)
				{
					return;
				}
				if (tempData.senderId != CurUserData::curChosenUser.id)
				{
					QMessageBox::warning(0, tr("消息提示"), tr("您有来自账号") + QString::number(tempData.senderId) + tr("的消息，消息不会保存仅仅给您个提示"));
					return;
				}

				QString curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"); //将时间转换成字符串格式（按格式）
				m_ui.plainTextEdit->appendHtml("<pre style='text-align:right;color:rgb(0,0,252);'>[" + CurUserData::curChosenUser.sUserName + "]" + curTime + "< / pre>");
				//m_ui.plainTextEdit->appendPlainText("[" + CurUserData::curChosenUser.sUserName + "]" + curTime + ":");
				m_ui.plainTextEdit->appendPlainText(tempStr); //添加文本
				m_ui.plainTextEdit->scrollBarWidgets(Qt::AlignBottom);      //滚轮自动移动到末端    
			}
			else if(tempData.msgType == GroupType&& CurUserData::chatType == false)
			{
				bool yesItHas = 0;
				// 判断接受信息的群聊是否是自己拥有群聊，
				for (auto i = CurUserData::thoseGroupsWhatIAmIn.begin(); i != CurUserData::thoseGroupsWhatIAmIn.end(); i++)
				{
					if (tempData.receiverId == i->id)
					{
						yesItHas = true;
						break;
					}
				}
				if (!yesItHas){ return;}
				// 判断接受信息的群聊当前是否是当前已经打开群聊
				if (tempData.receiverId != CurUserData::curChatGroup.id)
				{
					//QMessageBox::warning(0, tr("消息提示"), tr("您有来自群聊") +					// 有其他群聊消息，但不做提示了。
					//QString::number(CurUserData::curChatGroup.id) + tr("的消息，消息不会保存仅仅给您个提示"));	
					return;
				}
				// 判断发送者是否是自己，如果是自己，不做任何操作
				if (tempData.senderId == CurUserData::curUserInfo.id)
				{
					return;
				}
				// 如过自己所在群聊有消息并且自己已打开当前聊天窗口
				QString curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"); //将时间转换成字符串格式（按格式）
				m_ui.plainTextEdit->appendHtml("<pre style='text-align:right;color:rgb(0,0,252);'>[" + QString::number(tempData.senderId) + "]" + curTime + "< / pre>");
				//m_ui.plainTextEdit->appendPlainText("[" + QString::number(tempData.senderId) + "]" + curTime + ":");
				m_ui.plainTextEdit->appendPlainText(tempStr); //添加文本
				m_ui.plainTextEdit->scrollBarWidgets(Qt::AlignBottom);      //滚轮自动移动到末端    
			}
		}
		});

}


void SimpleChat::initConnect()
{
	initChatGroupList();
	// 监视Qss文件当发生变动时自动重新加载QSS

	m_fileWatcher = new QFileSystemWatcher();
	QString strQssPath = QApplication::applicationDirPath() + "/qss/style.qss";;
	m_fileWatcher->addPath(strQssPath);
	autoLoadQssFile(strQssPath);
	connect(m_fileWatcher, &QFileSystemWatcher::fileChanged,
		[=]() {
			autoLoadQssFile(strQssPath);
		});

	connect(m_pLoginWidget, &LoginWidget::loginResponse, [=](int iStateNum) {
		ResponseByDifferentStateNum(iStateNum);
		});

	// 留着做一些事情、
	connect(m_baseframe, &Baseframe::closeAppSignals, [=]() {
		m_dbManager->changeOnlineStatus(CurUserData::curUserInfo.id, false);	// 当时MainWindow时，设置下线。
		m_baseframe->setWindowType(defaultType);
		m_baseframe->close();
		});
	m_timer->start(10000);
	// 每隔十秒重新刷新好友状态。
	connect(m_timer, &QTimer::timeout, [=]() {
		initFriendList();
		initChatGroupList();
		});
	m_ui.tabWidget->setCurrentIndex(0);
}

void SimpleChat::initFriendList()
{
	// clear;
	clearWidget(m_ui.friListLayout);

	QList<ST_friendInfo> tempData;
	int iStateNum = m_dbManager->getFriendsList(SimpleChat::iCurUserId, tempData);
	if (iStateNum == Success)
	{
		for (int i = 0; i < tempData.size(); i++)
		{
			userWidget* tempWidget = new userWidget(0);
			connect(tempWidget, &userWidget::clicked, [=](bool bType) {
				// 点击了某用户后，此时才可发送信息。caiji
				CurUserData::chatType = bType;
				CurUserData::curChosenUser = tempWidget->getData();
				m_ui.lb_name->setText(CurUserData::curChosenUser.sUserName + "(" 
					+ QString::number(CurUserData::curChosenUser.id) 
					+ ")    ip:" 
					+ CurUserData::curChosenUser.sIp
					+"状态:"+ (CurUserData::curChosenUser.bifOnLine?"在线":"离线") + (CurUserData::chatType? "    好友" :"    群聊"));
				m_ui.btn_send->setEnabled(true);
				m_ui.plainTextEdit->clear();
				});

			m_ui.friListLayout->addWidget(tempWidget);
			tempWidget->setData(tempData[i]);
			if (tempData[i].bifOnLine)
			{
				tempWidget->setOnline();
			}
		}
	}
	//QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	//m_ui.friListLayout->addItem(verticalSpacer);

}

void SimpleChat::initChatGroupList()
{
	clearWidget(m_ui.chatGroupLayout);

	int iState = m_dbManager->GetAllMyChatGroupInfo(CurUserData::curUserInfo.id, CurUserData::thoseGroupsWhatIAmIn);
	if (iState == Success)
	{
		for (int i = 0; i < CurUserData::thoseGroupsWhatIAmIn.size(); i++)
		{
			userWidget* tempWidget = new userWidget(0);
			connect(tempWidget, &userWidget::clicked, [=](bool bType) {
				CurUserData::chatType = bType;
				// 点击了某用户后，此时才可发送信息。caiji
				CurUserData::curChatGroup = tempWidget->getGroupData();
				m_ui.lb_name->setText(CurUserData::curChatGroup.sNickName + "("
					+ QString::number(CurUserData::curChatGroup.id)+ (CurUserData::chatType ? "    好友" : "    群聊"));

				m_ui.btn_send->setEnabled(true);
				m_ui.plainTextEdit->clear();
				});
			m_ui.chatGroupLayout->addWidget(tempWidget);
			tempWidget->setGroupData(CurUserData::thoseGroupsWhatIAmIn[i]);
		}
	}
}

void SimpleChat::ResponseByDifferentStateNum(int iStateNum)
{
	//QMessageBox::question(0, "", QString::number(iStateNum));	// 用来测试
	switch (iStateNum)
	{
	case LoginSuccess:		// 登录成功
	{
		m_pLoginWidget->hide();
		m_baseframe->show();
		initFriendList();

		m_dbManager->getUserInfo(SimpleChat::iCurUserId, CurUserData::curUserInfo);
		m_ui.btn_headPic->setText(CurUserData::curUserInfo.sUserName);
		initChatGroupList();
		break;
	}
	case DBIsNotOpen:
	{
		qDebug() << tr("数据库未打开");
		break;
	}
	case QueryExecFailed:
	{
		qDebug() << tr("数据库语句执行失败");
		QMessageBox::warning(0, tr("错误"), tr("数据库语句执行失败"));
		break;
	}
	case QueryCountLessThan0:
	{
		QMessageBox::warning(0, tr("错误"), tr("错误的用户名或密码"));
		break;
	}
	case AddUserSuccess:
	{
		QMessageBox::warning(0, tr("成功"), tr("添加用户成功"));
		break;
	}
	case WannaAddUserButThereHasBeen:
	{
		QMessageBox::warning(0, tr("失败"), tr("已经有改用户了"));
		break;
	}

	// 添加好友状态返回码判断
	case CannotFindThidUser:
	{
		QMessageBox::warning(0, tr("提示"), tr("该账号未注册"));
		break;
	}
	case AlreadyHaveThisRelation:
	{
		QMessageBox::warning(0, tr("添加失败"), tr("已经有该好友了"));
		break;
	}
	case AddFriSuccess:
	{
		QMessageBox::information(0, tr("成功"), tr("添加好友成功"));
		break;
	}

	case CreateGroupSuccess:
		QMessageBox::information(0, tr("成功"), tr("创建群聊成功"));
		break;

	case insertGroupRelationSuccess:
		QMessageBox::information(0, tr("成功"), tr("加入群聊成功"));
		break;
	
	case AlreadyHaveThisGroupRelation:
		QMessageBox::warning(0, tr("警告"), tr("已经加入过群聊了"));
		break;
	case CouldNotFindTheGroup:
		QMessageBox::warning(0, tr("警告"), tr("无法找到该群聊"));
		break;

	default:
		break;
	}

}

void SimpleChat::clearWidget(QVBoxLayout* layout)
{
	if (layout)
	{
		//clearWidgets(templayout);
		int iTemp = layout->count();
		while (layout->count())
		{
			iTemp = layout->count();
			QWidget* p = layout->itemAt(0)->widget();
			p->setParent(NULL);
			layout->removeWidget(p);
			delete p; // 清除内存
		}
	}
}

