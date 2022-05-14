#include "Baseframe.h"

#include <windows.h>        //ע��ͷ�ļ�
#include <windowsx.h>


Baseframe::Baseframe(QWidget *parent)
	: QWidget(parent), m_iWinType(0)
{
	m_ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
}

Baseframe::~Baseframe()
{
}
void Baseframe::on_btn_close_clicked()
{
    emit(closeAppSignals());
    this->close();
}
void Baseframe::on_btn_min_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}
void Baseframe::on_btn_zoom_clicked()
{
}
void Baseframe::addWidget2Content(QWidget* ptr)
{
	m_ui.bodyLayout->addWidget(ptr);
}

void Baseframe::setWindowType(int wType)
{
    m_iWinType = wType;
    switch (wType)
    {
    case defaultType:
        break;
    case hideBtnZoom:
        m_ui.btn_zoom->hide();
        break;
    default:
        break;
    }

}

void Baseframe::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bDrag = true;
        //������ĳ�ʼλ��
        mouseStartPoint = event->globalPos();
        //mouseStartPoint = event->pos();
        //��ô��ڵĳ�ʼλ��
        windowTopLeftPoint = this->frameGeometry().topLeft();
    }
}

void Baseframe::mouseMoveEvent(QMouseEvent* event)
{
    if (m_bDrag)
    {
        //�������ƶ��ľ���
        QPoint distance = event->globalPos() - mouseStartPoint;
        //QPoint distance = event->pos() - mouseStartPoint;
        //�ı䴰�ڵ�λ��
        this->move(windowTopLeftPoint + distance);
    }
}

void Baseframe::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bDrag = false;
    }
}
