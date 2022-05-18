#include "AddDialog.h"
#include <QButtonGroup>
#include <QMessageBox>
#include "pubStruct.h"
#include "SimpleChat.h"


AddDialog::AddDialog(QWidget *parent)
	: QDialog(parent), m_baseframe(new Baseframe(parent)), m_addType(0)
{
	ui.setupUi(this);
	m_baseframe->addWidget2Content(this);
	m_baseframe->show();
	m_baseframe->setWindowType(1);
	QButtonGroup* box = new QButtonGroup;
		//ui.btn_addfri->setDown(true);
	ui.btn_addfri->setChecked(true);
	ui.btn_addgroup->setChecked(false);
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
	int iStateNum;
	// 当添加fri
	if (ui.btn_addfri->isChecked())
	{
		//extern int iCurUserId;
		iStateNum = tempDBPtr->AddFri(QString::number(SimpleChat::iCurUserId), ui.le_add->text());
		emit AddFriResponse(iStateNum);
	}
	else if (ui.btn_addgroup->isChecked())
	{
		stGroupRelation tempData;
		tempData.igroupId = ui.le_add->text().toInt();
		tempData.memberId = CurUserData::curUserInfo.id;
		// 当添加group
		iStateNum = tempDBPtr->insertChatGroupRelation(tempData);
		emit AddFriResponse(iStateNum);
	}

}

void AddDialog::on_btn_addgroup_clicked()
{
}
