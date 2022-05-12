#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QDialog>
#include "ui_LoginWidget.h"
#include "DataBaseManager.h"
class Baseframe;

class LoginWidget : public QDialog
{
	Q_OBJECT

public:
	LoginWidget(QWidget* parent = Q_NULLPTR);
	~LoginWidget();
	void setDBPtr(DataBaseManager* ptr) { tempDBPtr = ptr;}
public slots:

	void on_btn_login_clicked();
	void on_btn_reg_clicked();
	void on_btn_comeback_clicked() { m_ui.stackedWidget->setCurrentIndex(0); }
	void on_btn_gotoReg_clicked() { m_ui.stackedWidget->setCurrentIndex(1); };

signals:
	void loginResponse(int iStateNum);
private:
	Ui::LoginWidget m_ui;
	Baseframe* m_baseframe;
	DataBaseManager* tempDBPtr;

};

#endif