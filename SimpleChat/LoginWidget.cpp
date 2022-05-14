#include "LoginWidget.h"

#include <QMessageBox>
#include "pubStruct.h"

LoginWidget::LoginWidget(QWidget* parent)
	: QDialog(parent)
	, m_baseframe(new Baseframe(parent))
{
	m_ui.setupUi(this);
	m_baseframe->addWidget2Content(this);
	m_baseframe->show();
	m_baseframe->setWindowType(1);

	m_ui.stackedWidget->setCurrentIndex(0);



}

LoginWidget::~LoginWidget()
{
}

void LoginWidget::on_btn_login_clicked()
{
	if (m_ui.le_username->text().isEmpty() || m_ui.le_password->text().isEmpty())
	{
		QMessageBox::warning(0, "错误", tr("用户名或密码不能为空"));
		return;
	}

	int iStateNum = tempDBPtr->QueryUser(m_ui.le_username->text(), m_ui.le_password->text());
	emit loginResponse(iStateNum);
}

void LoginWidget::on_btn_reg_clicked()
{
	if (m_ui.le_username_2->text().isEmpty() || m_ui.le_password_1->text().isEmpty())
	{
		QMessageBox::warning(0, "错误", tr("用户名或密码不能为空"));
		return;
	}
	if (m_ui.le_password_1->text() != m_ui.le_confirm->text())
	{
		QMessageBox::warning(0, "错误", tr("两次密码不一致"));
		return;
	}
	ST_LoginInfo tempData = { m_ui.le_username_2->text(), m_ui.le_password_1->text()};

	int iStateNum = tempDBPtr->AddUser(tempData);
	emit loginResponse(iStateNum);
}
