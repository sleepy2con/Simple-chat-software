#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SimpleChat.h"

class LoginWidget;
class QFileSystemWatcher;

class SimpleChat : public QMainWindow
{
    Q_OBJECT

public:
    SimpleChat(QWidget *parent = Q_NULLPTR);

    void autoLoadQssFile(const QString& strQssPath);

private:
    Ui::SimpleChatClass m_ui;
    LoginWidget* m_pLoginWidget;
    QFileSystemWatcher* m_fileWatcher;
};
