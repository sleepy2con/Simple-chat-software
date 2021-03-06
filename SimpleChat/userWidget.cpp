#include "userWidget.h"

userWidget::userWidget(QWidget *parent)
	: QWidget(parent), whatIam(1)
{
	ui.setupUi(this);
	setProperty("class", "userWidget");
	setFixedHeight(40);
}

userWidget::~userWidget()
{
}

void userWidget::setData(const ST_friendInfo& tempData)
{
	m_thisGuyInfo = tempData;
	ui.lb_headPic->setText(tempData.sUserName[0]);
	ui.lb_username->setText(tempData.sUserName + " (" +QString::number(tempData.id) + ")");
	whatIam = true;
}

void userWidget::setGroupData(const stChatGroup& tempData)
{
	m_thisGroupInfo = tempData;
	ui.lb_headPic->setText(tempData.sNickName[0]);
	ui.lb_username->setText(tempData.sNickName + " (" + QString::number(tempData.id) + ")");
	whatIam = false;
}

void userWidget::setOnline()
{
	this->setStyleSheet("QLabel{background-color:rgb(61,128,109);color:white;border:1px solid black;}");
}

void userWidget::mouseReleaseEvent(QMouseEvent* ev) {
	emit clicked(whatIam);
}