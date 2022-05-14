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
private:
	Ui::userWidget ui;
};
