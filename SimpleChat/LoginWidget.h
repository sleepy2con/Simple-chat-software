#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QDialog>
#include "ui_LoginWidget.h"

class Baseframe;

class LoginWidget : public QDialog
{
	Q_OBJECT

public:
	LoginWidget(QWidget *parent = Q_NULLPTR);
	~LoginWidget();

public slots:


private:
	Ui::LoginWidget m_ui;
	Baseframe* m_baseframe;
};

#endif