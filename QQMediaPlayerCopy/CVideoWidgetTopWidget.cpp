#include "CVideoWidgetTopWidget.h"
#include <QEvent>
#include <QMouseEvent>
#include <string>
#include <iostream>

using namespace std;

#define TIME_SLIDER_HEIGHT   20


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

	m_pTimeSlider = new CTimeSlider(this);

	QString slider_qss = "QSlider{background:transparent; \
		border-style: outset; \
		border-style: outset;  \
			border-radius: 10px;} \
		QSlider::groove:horizontal{ \
			height: 12px; \
			background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4); \
			margin: 2px 0} \
		QSlider::handle:horizontal { \
			background: QRadialGradient(cx:0, cy:0, radius: 1, fx:0.5, fy:0.5, stop:0 white, stop:1 green); \
			width: 16px; \
			height: 16px; \
			margin: -5px 6px -5px 6px; \
			border-radius:11px;  \
		border: 3px solid #ffffff;}";

	m_pTimeSlider->setStyleSheet(slider_qss);

	connect(m_pOpenFileButton, &COpenFileButton::sig_openfile, this, &CVideoWidgetTopWidget::sig_OpenFile);
	connect(m_pOpenFileButton, &COpenFileButton::sig_openFloder, this, &CVideoWidgetTopWidget::sig_OpenFloder);
	connect(m_pOpenrRightlistButton, &QPushButton::clicked, this, &CVideoWidgetTopWidget::sig_OpenPlaylist);

	connect(m_pTimeSlider, &QSlider::sliderMoved, this, &CVideoWidgetTopWidget::onSliderMoved);
}

CVideoWidgetTopWidget::~CVideoWidgetTopWidget()
{

}

void CVideoWidgetTopWidget::onSliderMoved(int position)
{
	emit sig_SliderMove(position);
}

void CVideoWidgetTopWidget::setTimeSliderRange(qint64 value)
{
	m_pTimeSlider->setRange(0, value);
}

void CVideoWidgetTopWidget::setSliderValue(qint64 value)
{
	m_pTimeSlider->setValue(value);
}

void CVideoWidgetTopWidget::showRightBtn(bool flag)
{
	flag ? m_pOpenrRightlistButton->show() : m_pOpenrRightlistButton->hide();
}

void CVideoWidgetTopWidget::showOpenFileBtn(bool flag)
{
	flag ? m_pOpenFileButton->show() : m_pOpenFileButton->hide();
}

void CVideoWidgetTopWidget::showSlider(bool flag)
{
	m_pTimeSlider->show();
}

void CVideoWidgetTopWidget::resizeEvent(QResizeEvent* event)
{
	if (m_pOpenFileButton && m_pOpenrRightlistButton && m_pTimeSlider)
	{
		int x1 = this->width() / 2 - m_pOpenFileButton->width() / 2;
		int y1 = this->height() / 2 - m_pOpenFileButton->height() / 2;
		m_pOpenFileButton->move(x1, y1);

		int x2 = this->width() - m_pOpenrRightlistButton->width();
		int y2 = this->height() / 2 - m_pOpenrRightlistButton->height() / 2;
		m_pOpenrRightlistButton->move(x2, y2);

		int x3 = 0;
		int y3 = this->height() - TIME_SLIDER_HEIGHT;
		m_pTimeSlider->move(x3, y3);
		m_pTimeSlider->resize(this->width(), TIME_SLIDER_HEIGHT);
	}
}



