#include "SimpleChat.h"
#include "LoginWidget.h"
SimpleChat::SimpleChat(QWidget *parent)
    : QMainWindow(parent)
    , m_pLoginWidget(new LoginWidget(0))
{
    m_ui.setupUi(this);
    this->hide();
    m_pLoginWidget->show();

}
