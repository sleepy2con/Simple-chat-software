#ifndef ADDDIALOG_H
#define ADDDIALOG_H


#include <QDialog>
#include "ui_AddDialog.h"
#include "Baseframe.h"
#include "DataBaseManager.h"
class AddDialog : public QDialog
{
	Q_OBJECT

public:
	AddDialog(QWidget *parent = Q_NULLPTR);
	~AddDialog();
	

public slots:
	void on_btn_add_clicked();
	void on_btn_addgroup_clicked();
signals:
	void AddFriResponse(int iStateNum);

private:
	Ui::AddDialog ui;

	Baseframe* m_baseframe;
	DataBaseManager* tempDBPtr;
public:
	void hide() { m_baseframe->hide(); }
	void show() { m_baseframe->show(); }
	void setDBPtr(DataBaseManager* ptr) { tempDBPtr = ptr; }
};


#endif