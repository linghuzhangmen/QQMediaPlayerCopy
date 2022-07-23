#include "CVolumeButton.h"
#include <QMouseEvent>
#include <QStylePainter>
#include <QStyleOptionButton>
#include <iostream>
#include <QThread>

using namespace std;

CVolumeButton::CVolumeButton(QWidget* parent)
	: QPushButton(parent)
{
	setFixedSize(32, 32);

	setStyleSheet("QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/audio_open.svg);border:none;}"
		"QPushButton:hover{background-image:url(:/playCtrlBar/resources/playctrlBar/audio_open_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/audio_open.svg);border:none;}");
}

CVolumeButton::~CVolumeButton()
{
}

void CVolumeButton::paintEvent(QPaintEvent*)
{
	QStylePainter p(this);
	QStyleOptionButton option;
	initStyleOption(&option);
	p.drawControl(QStyle::CE_PushButton, option);
}

void CVolumeButton::enterEvent(QEnterEvent* event)
{
	if (!m_pVolumeSliderDlg)
		m_pVolumeSliderDlg = new CVolumeSliderDialog(this);

	QPoint p1 = this->mapToGlobal(QPoint(0, 0));  //声音按钮左上角相对于桌面的绝对位置
	QRect rect1 = this->rect();
	QRect rect2 = m_pVolumeSliderDlg->rect();     //rect包含标题栏，去掉标题栏后height不变

	int x = p1.x() + (rect1.width() - rect2.width()) / 2;
	int y = p1.y() - rect2.height() - 5;
	m_pVolumeSliderDlg->move(x, y);   //move是相对于桌面原点的位置

	m_pVolumeSliderDlg->show();
	startTimer(250);

	//setStyleSheet("QPushButton{border-image:url(:/ButtonPopup/picture/volume.png)}");

	connect(m_pVolumeSliderDlg, &CVolumeSliderDialog::sig_SliderValueChanged, [=](int value) {
		emit sig_VolumeValue(value);
		});
}


//void CVolumeButton::leaveEvent(QEvent* event)
//{
//	根据鼠标的位置判断音量调节窗口是否消失
//	//QPoint p1 = QCursor::pos();   //绝对位置
//
//	//cout << "QCursor x= " << p1.x() << " y = " << p1.y() << endl;
//
//	//if (m_pVolumeSliderDlg)
//	//{
//	//	QRect rect1 = this->rect();  //按钮矩形
//	//	QRect rect2 = m_pVolumeSliderDlg->rect();
//	//	QRect rect3 = m_pVolumeSliderDlg->geometry();
//
//	//	QPoint p2 = this->mapToGlobal(QPoint(0, 0));   //声音按钮左上角相对于桌面的绝对位置
//
//	//	//已知：音量框宽40 > 按钮宽30
//	//	QRect area(rect3.left(), rect3.top(), rect2.width(), p2.y() + rect1.height() - rect3.top()); //左上宽高
//
//	//	cout << "p1 x = " << p1.x() << " y = " << p1.y() << endl;
//
//	//	if (!area.contains(p1))
//	//	{
//	//		m_pVolumeSliderDlg->hide();
//	//	}
//	//}
//}

void CVolumeButton::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_isMute = !m_isMute;
		if (m_isMute)
		{
			if (m_pVolumeSliderDlg)
				m_pVolumeSliderDlg->setSliderValue(0);

			//setStyleSheet("QPushButton{border-image:url(:/ButtonPopup/picture/mute.png)}");
		}
		else
		{
			if (m_pVolumeSliderDlg)
				m_pVolumeSliderDlg->setSliderValue(50);

			//setStyleSheet("QPushButton{border-image:url(:/ButtonPopup/picture/volume.png)}");
		}
	}
}


/**
 * @brief 用定时器模拟leaveEvent，直接在leaveEvent里让m_pVolumeSliderDlg消失，效果不太好，用鼠标移动事件也不太好，定时器是比较好的做法
 */
void CVolumeButton::timerEvent(QTimerEvent* event)
{
	if ((m_pVolumeSliderDlg != nullptr) && (m_pVolumeSliderDlg->isVisible()))
	{
		QPoint p1 = QCursor::pos();   //鼠标绝对位置
		if (m_pVolumeSliderDlg)
		{
			QRect rect1 = this->rect();  //按钮矩形
			QRect rect2 = m_pVolumeSliderDlg->rect();
			QRect rect3 = m_pVolumeSliderDlg->geometry();

			QPoint p2 = this->mapToGlobal(QPoint(0, 0));   //声音按钮左上角相对于桌面的绝对位置

			//已知：音量框宽40 > 按钮宽30
			QRect area(rect3.left(), rect3.top(), rect2.width(), p2.y() + rect1.height() - rect3.top()); //左上宽高

			if (!area.contains(p1))
			{
				m_pVolumeSliderDlg->hide();
			}
		}
	}
	else
	{
		killTimer(event->timerId());
	}
}
