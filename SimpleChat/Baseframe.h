#pragma once

#include <QWidget>
#include "ui_Baseframe.h"
#include <QMouseEvent>
enum
{
	defaultType = 0,
	hideBtnZoom,
};


class Baseframe : public QWidget
{
	Q_OBJECT

public:
	Baseframe(QWidget *parent = Q_NULLPTR);
	~Baseframe();
	void addWidget2Content(QWidget* ptr = nullptr);
	void setWindowType(int wType = 0);

protected:
	//ÍÏ×§´°¿Ú
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);


public slots:
	void on_btn_close_clicked();


private:
	Ui::Baseframe m_ui;

	bool        m_bDrag;
	QPoint      mouseStartPoint;
	QPoint      windowTopLeftPoint;
};
