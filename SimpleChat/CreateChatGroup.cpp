#include "CreateChatGroup.h"
#include <QMessageBox>

CreateChatGroup::CreateChatGroup(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

CreateChatGroup::~CreateChatGroup()
{
}

void CreateChatGroup::setData(const stChatGroup& data)
{
	m_data = data;
}

void CreateChatGroup::on_btn_createChatGroup_clicked()
{
	if (ui.le_groupName->text().isEmpty())
	{
		QMessageBox::warning(0, tr("警告"), tr("群聊名称不能为空"));
		return;
	}
	m_data.sNickName = ui.le_groupName->text();
	int iStateNum = dbPtr->CreateChatGroup(m_data);
	emit giveResponse(iStateNum);
}
