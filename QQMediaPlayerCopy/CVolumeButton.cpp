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

void CVolumeButton::enterEvent(QEvent* event)
{
	if (!m_pVolumeSliderDlg)
		m_pVolumeSliderDlg = new CVolumeSliderDialog(this);

	QPoint p1 = this->mapToGlobal(QPoint(0, 0));  //������ť���Ͻ����������ľ���λ��
	QRect rect1 = this->rect();
	QRect rect2 = m_pVolumeSliderDlg->rect();     //rect������������ȥ����������height����

	int x = p1.x() + (rect1.width() - rect2.width()) / 2;
	int y = p1.y() - rect2.height() - 5;
	m_pVolumeSliderDlg->move(x, y);   //move�����������ԭ���λ��

	m_pVolumeSliderDlg->show();
	startTimer(250);

	//setStyleSheet("QPushButton{border-image:url(:/ButtonPopup/picture/volume.png)}");

	connect(m_pVolumeSliderDlg, &CVolumeSliderDialog::sig_SliderValueChanged, [=](int value) {
		emit sig_VolumeValue(value);
		});
}


//void CVolumeButton::leaveEvent(QEvent* event)
//{
//	��������λ���ж��������ڴ����Ƿ���ʧ
//	//QPoint p1 = QCursor::pos();   //����λ��
//
//	//cout << "QCursor x= " << p1.x() << " y = " << p1.y() << endl;
//
//	//if (m_pVolumeSliderDlg)
//	//{
//	//	QRect rect1 = this->rect();  //��ť����
//	//	QRect rect2 = m_pVolumeSliderDlg->rect();
//	//	QRect rect3 = m_pVolumeSliderDlg->geometry();
//
//	//	QPoint p2 = this->mapToGlobal(QPoint(0, 0));   //������ť���Ͻ����������ľ���λ��
//
//	//	//��֪���������40 > ��ť��30
//	//	QRect area(rect3.left(), rect3.top(), rect2.width(), p2.y() + rect1.height() - rect3.top()); //���Ͽ��
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
 * @brief �ö�ʱ��ģ��leaveEvent��ֱ����leaveEvent����m_pVolumeSliderDlg��ʧ��Ч����̫�ã�������ƶ��¼�Ҳ��̫�ã���ʱ���ǱȽϺõ�����
 */
void CVolumeButton::timerEvent(QTimerEvent* event)
{
	if ((m_pVolumeSliderDlg != nullptr) && (m_pVolumeSliderDlg->isVisible()))
	{
		QPoint p1 = QCursor::pos();   //������λ��
		if (m_pVolumeSliderDlg)
		{
			QRect rect1 = this->rect();  //��ť����
			QRect rect2 = m_pVolumeSliderDlg->rect();
			QRect rect3 = m_pVolumeSliderDlg->geometry();

			QPoint p2 = this->mapToGlobal(QPoint(0, 0));   //������ť���Ͻ����������ľ���λ��

			//��֪���������40 > ��ť��30
			QRect area(rect3.left(), rect3.top(), rect2.width(), p2.y() + rect1.height() - rect3.top()); //���Ͽ��

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
