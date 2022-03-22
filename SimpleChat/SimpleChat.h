#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SimpleChat.h"

class SimpleChat : public QMainWindow
{
    Q_OBJECT

public:
    SimpleChat(QWidget *parent = Q_NULLPTR);

private:
    Ui::SimpleChatClass ui;
};
