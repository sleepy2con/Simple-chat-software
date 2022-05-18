#pragma once

#include <QWidget>
#include "ui_userWidget.h"
#include "pubStruct.h"
class userWidget : public QWidget
{
	Q_OBJECT

public:
	userWidget(QWidget *parent = Q_NULLPTR);
	~userWidget();
	void setData(const ST_friendInfo& tempData);
	void setGroupData(const stChatGroup& tempData);
	ST_friendInfo getData() { return m_thisGuyInfo; }
protected:
	virtual void mouseReleaseEvent(QMouseEvent* ev);
signals:
	void clicked();
private:
	Ui::userWidget ui;
	ST_friendInfo m_thisGuyInfo;
	stChatGroup m_thisGroupInfo;
};
