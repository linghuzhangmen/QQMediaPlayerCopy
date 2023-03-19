#include "CVideoWidgetTopWidget.h"
#include <QEvent>
#include <QMouseEvent>
#include <string>
#include <iostream>

using namespace std;




CVideoWidgetTopWidget::CVideoWidgetTopWidget(QWidget* p)
	:QWidget(p)
{
	setAttribute(Qt::WA_StyledBackground);
	this->setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::SubWindow);
	setAutoFillBackground(true);
	setMouseTracking(true);

	m_pOpenFileButton = new COpenFileButton(this);
	m_pOpenrRightlistButton = new QPushButton(this);

	m_pOpenrRightlistButton->setText(u8"");
	m_pOpenrRightlistButton->setFixedSize(36, 80);

	//Í¼Æ¬µÄÆ«ÒÆ 80 / 2 - 32 / 2 = 40 - 16 = 24
	m_pOpenrRightlistButton->setStyleSheet("QPushButton{background-color:rgb(54,54,54);background-image:url(:/videoWidget/resources/videoWidget/left_arrow.svg); \
		background-position:center; \
		padding-top: 24px; \
		background-repeat: no-repeat; \
		border:none;} \
		QPushButton:hover{background-image:url(:/videoWidget/resources/videoWidget/left_arrow_hover.svg);border:none;} \
		QPushButton:pressed{background-image:url(:/videoWidget/resources/videoWidget/left_arrow.svg);border:none;}");

	connect(m_pOpenFileButton, &COpenFileButton::sig_openfile, this, &CVideoWidgetTopWidget::sig_OpenFile);
	connect(m_pOpenFileButton, &COpenFileButton::sig_openUrl, this, &CVideoWidgetTopWidget::sig_OpenUrl);
	connect(m_pOpenrRightlistButton, &QPushButton::clicked, this, &CVideoWidgetTopWidget::sig_OpenPlaylist);
}

CVideoWidgetTopWidget::~CVideoWidgetTopWidget()
{

}

void CVideoWidgetTopWidget::showRightBtn(bool flag)
{
	flag ? m_pOpenrRightlistButton->show() : m_pOpenrRightlistButton->hide();
}

void CVideoWidgetTopWidget::showOpenFileBtn(bool flag)
{
	flag ? m_pOpenFileButton->show() : m_pOpenFileButton->hide();
}

void CVideoWidgetTopWidget::resizeEvent(QResizeEvent* event)
{
	if (m_pOpenFileButton && m_pOpenrRightlistButton)
	{
		int x1 = this->width() / 2 - m_pOpenFileButton->width() / 2;
		int y1 = this->height() / 2 - m_pOpenFileButton->height() / 2;
		m_pOpenFileButton->move(x1, y1);

		int x2 = this->width() - m_pOpenrRightlistButton->width();
		int y2 = this->height() / 2 - m_pOpenrRightlistButton->height() / 2;
		m_pOpenrRightlistButton->move(x2, y2);
	}
}



