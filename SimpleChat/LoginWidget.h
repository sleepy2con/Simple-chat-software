#pragma once

#include <QDialog>
#include "ui_LoginWidget.h"

class Baseframe;

class LoginWidget : public QDialog
{
	Q_OBJECT

public:
	LoginWidget(QWidget *parent = Q_NULLPTR);
	~LoginWidget();

private:
	Ui::LoginWidget ui;
	Baseframe* m_baseframe;
};
