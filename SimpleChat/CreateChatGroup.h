#pragma once

#include <QDialog>
#include "ui_CreateChatGroup.h"
#include "DataBaseManager.h"
class CreateChatGroup : public QDialog
{
	Q_OBJECT

public:
	CreateChatGroup(QWidget *parent = Q_NULLPTR);
	~CreateChatGroup();

public slots:
	void on_btn_createChatGroup_clicked();
	void setDbPtr(DataBaseManager* ptr) { dbPtr = ptr; }
private:
	Ui::CreateChatGroup ui;
	DataBaseManager* dbPtr;
};
