#ifndef SIMPLECHAT_H
#define SIMPLECHAT_H

#include <QtWidgets/QMainWindow>
#include "ui_SimpleChat.h"
#include "DataBaseManager.h"
class LoginWidget;
class QFileSystemWatcher;

class SimpleChat : public QMainWindow
{
	Q_OBJECT

public:
	SimpleChat(QWidget* parent = Q_NULLPTR);

	void autoLoadQssFile(const QString& strQssPath);

private:

	void initConnect();

	void ResponseByDifferentStateNum(int iStateNum);

private:
	Ui::SimpleChatClass m_ui;
	LoginWidget* m_pLoginWidget;
	QFileSystemWatcher* m_fileWatcher;
	DataBaseManager* m_dbManager;
};
#endif // !SIMPLECHAT_H