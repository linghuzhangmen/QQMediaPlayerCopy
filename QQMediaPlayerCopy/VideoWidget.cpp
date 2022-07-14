/*

该界面使用move布局

视频播放另外用一个widget，是为了消除控件的raise,lower闪烁问题

*/

#include "VideoWidget.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenu>

#define TIME_SLIDER_HEIGHT   20

VideoWidget::VideoWidget(QWidget *p):
	QWidget(p)
{
	setAttribute(Qt::WA_StyledBackground);  // 禁止父窗口样式影响子控件样式

	//设置渐变色
	this->setStyleSheet("QWidget{\
                       background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #09115C, stop:1 #000000 );\
                        font-family:Microsoft YaHei\
                         }");

	QString openfile_button_qss = "QPushButton::menu-indicator:open{"
		"image:url(:/titleBar/resources/titleBar/down_arrow.svg);"
		"subcontrol-position:right center;"
		"subcontrol-origin:padding;border:none;}"
		"QPushButton::menu-indicator:closed{"
		"image:url(:/titleBar/resources/titleBar/up_arrow.svg);"
		"subcontrol-position:right center;"
		"subcontrol-origin:padding;border:none;}";

	setMinimumSize(800, 450);

	m_pVideoPlayWidget = new QWidget(this);
	m_pVideoPlayWidget->resize(this->width(), this->height());
	m_pVideoPlayWidget->lower();

	m_pOpenFileButton = new COpenFileButton(this);
	m_pOpenrRightlistButton = new QPushButton(this);

	m_pOpenrRightlistButton->setText(u8"");
	m_pOpenrRightlistButton->setFixedSize(36, 80);

	//图片的偏移 80 / 2 - 32 / 2 = 40 - 16 = 24
	m_pOpenrRightlistButton->setStyleSheet("QPushButton{background-color:rgb(54,54,54);background-image:url(:/videoWidget/resources/videoWidget/left_arrow.svg); \
		background-position:center; \
		padding-top: 24px; \
		padding-down: 24px; \
		background-repeat: no-repeat; \
		border:none;} \
		QPushButton:hover{background-image:url(:/videoWidget/resources/videoWidget/left_arrow_hover.svg);border:none;} \
		QPushButton:pressed{background-image:url(:/videoWidget/resources/videoWidget/left_arrow.svg);border:none;}");

	m_pTimeSlider = new QSlider(this);
	m_pTimeSlider->setOrientation(Qt::Horizontal);
	m_pTimeSlider->setFixedHeight(TIME_SLIDER_HEIGHT);
	m_pTimeSlider->hide();   //默认隐藏
	m_pTimeSlider->lower();

	connect(m_pOpenFileButton, &COpenFileButton::sig_openfile, this, &VideoWidget::sig_OpenFile);
	connect(m_pOpenFileButton, &COpenFileButton::sig_openFloder, this, &VideoWidget::sig_OpenFloder);
	connect(m_pOpenrRightlistButton, &QPushButton::clicked, this, &VideoWidget::sig_OpenPlaylist);
}

VideoWidget::~VideoWidget()
{
}

void VideoWidget::ShowOpenFileButton(bool show)
{
	show ? m_pOpenFileButton->show() : m_pOpenFileButton->hide();
}

void VideoWidget::setPlay(bool play)
{
	m_isPlay = play;

	if (play)
	{
		m_pOpenFileButton->hide();
		m_pOpenrRightlistButton->hide();
	}
	else
	{
		m_pOpenFileButton->show();
		m_pOpenrRightlistButton->show();
	}
}

QWidget* VideoWidget::playWidget() const
{
	return m_pVideoPlayWidget;
}

void VideoWidget::setTimeSliderRange(int maxValue)
{
	m_pTimeSlider->setRange(0, maxValue);
}

void VideoWidget::setTimeSliderPos(int pos)
{
	m_pTimeSlider->setValue(pos);
}

//隐藏控件
void VideoWidget::hideCtrl(bool hide)
{
	if (hide)
	{
		m_pOpenFileButton->hide();
		m_pOpenrRightlistButton->hide();
		m_pTimeSlider->hide();
	}
	else
	{
		m_pOpenFileButton->show();
		m_pOpenrRightlistButton->show();
		m_pTimeSlider->show();
	}
}

void VideoWidget::resizeEvent(QResizeEvent* event)
{
	if (m_pOpenFileButton && m_pOpenrRightlistButton && m_pTimeSlider)
	{
		m_pVideoPlayWidget->move(0, 0);
		m_pVideoPlayWidget->resize(this->width(), this->height());

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

#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
void VideoWidget::enterEvent(QEnterEvent* event)
#else
void VideoWidget::enterEvent(QEvent* event)
#endif
{
	if (m_isPlay)
	{
		m_pTimeSlider->raise();
		m_pTimeSlider->show();

		m_pOpenrRightlistButton->raise();
		m_pOpenrRightlistButton->show();
	}
}

void VideoWidget::leaveEvent(QEvent* event)
{
	if (m_isPlay)
	{
		m_pTimeSlider->lower();
		m_pTimeSlider->hide();

		m_pOpenrRightlistButton->lower();
		m_pOpenrRightlistButton->hide();
	}
}

void VideoWidget::paintEvent(QPaintEvent* event)
{
	
}
