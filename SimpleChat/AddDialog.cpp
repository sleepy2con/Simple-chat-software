#include "AddDialog.h"
#include <QButtonGroup>
#include <QMessageBox>
#include "pubStruct.h"
#include "SimpleChat.h"


AddDialog::AddDialog(QWidget *parent)
	: QDialog(parent), m_baseframe(new Baseframe(parent))
{
	ui.setupUi(this);
	m_baseframe->addWidget2Content(this);
	m_baseframe->show();
	m_baseframe->setWindowType(1);
	QButtonGroup* box = new QButtonGroup;
	// 设置是否互斥
	box->setExclusive(true);
	// 将需要互斥的按钮全部添加到 QButtonGroup 中
	box->addButton(ui.btn_addfri);
	box->addButton(ui.btn_addgroup);

}

AddDialog::~AddDialog()
{
}

void AddDialog::on_btn_add_clicked()
{
	if (ui.le_add->text().isEmpty())
	{
		QMessageBox::warning(0, tr("警告"), tr("账号不能为空"));
		return;
	}
	//extern int iCurUserId;
	int iStateNum = tempDBPtr->AddFri(QString::number(SimpleChat::iCurUserId), ui.le_add->text());
	emit AddFriResponse(iStateNum);
}

void AddDialog::on_btn_addgroup_clicked()
{
}
