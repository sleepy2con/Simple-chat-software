#pragma once

#include <QWidget>
#include "ui_Baseframe.h"

class Baseframe : public QWidget
{
	Q_OBJECT

public:
	Baseframe(QWidget *parent = Q_NULLPTR);
	~Baseframe();

private:
	Ui::Baseframe ui;
};
