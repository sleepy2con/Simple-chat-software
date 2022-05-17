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
	void setData(const stChatGroup& data);

public slots:
	void on_btn_createChatGroup_clicked();
	void setDbPtr(DataBaseManager* ptr) { dbPtr = ptr; }
signals:
	void giveResponse(int iStateNum);
private:
	Ui::CreateChatGroup ui;
	DataBaseManager* dbPtr;
	stChatGroup m_data;
};
