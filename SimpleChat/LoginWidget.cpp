#include "LoginWidget.h"
#include "Baseframe.h"

LoginWidget::LoginWidget(QWidget *parent)
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
