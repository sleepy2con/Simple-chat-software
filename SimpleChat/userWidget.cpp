#include "userWidget.h"

userWidget::userWidget(QWidget *parent)
	: QWidget(parent)
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
}

void userWidget::mouseReleaseEvent(QMouseEvent* ev) {
	emit clicked();
}