#include "QQMediaPlayerCopy.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <string>
#include <iostream>
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>
#include <QStringListModel>
#include <QScreen>
#include <Windows.h>
#include <QApplication>

using namespace std;

BOOL CALLBACK EnumVLC(HWND hwnd, LPARAM lParam)
{
	TCHAR szTitle[1024];

	int nLen = GetWindowText(hwnd, szTitle, 1024);

	if (nLen > 0)
	{
		EnableWindow(hwnd, FALSE);
		KillTimer(NULL, 1);
	}

	return TRUE;
}

void CALLBACK TimeProc(HWND hwnd, UINT msg, UINT_PTR id, DWORD time)
{
	HWND vlcHwnd = FindWindowEx(NULL, NULL, NULL, L"vlcmain");

	if (vlcHwnd)
	{
		EnumChildWindows(vlcHwnd, EnumVLC, NULL);
	}
}

//��ֹ��������
string qstr2str(const QString& qstr)
{
	QByteArray cdata = qstr.toLocal8Bit();
	return string(cdata);
}

QQMediaPlayerCopy::QQMediaPlayerCopy(QWidget* parent)
	: QWidget(parent)
{
	this->setWindowTitle("vlcmain");
	setMouseTracking(true);

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	setAttribute(Qt::WA_Hover);

	m_pVlc.reset(new CVlcKits());

	int nRet = m_pVlc->initVLC();

	switch (nRet)
	{
	case -1:
		QMessageBox::information(this, tr("Warn"),
			tr("libvlc_new failed"));
		exit(EXIT_FAILURE);

	case -2:
		QMessageBox::information(this, tr("Warn"),
			tr("libvlc_media_player_new failed"));
		exit(EXIT_FAILURE);
	}

	connect(m_pVlc.get(), &CVlcKits::sig_UpdateTimeText, this, &QQMediaPlayerCopy::OnUpdateTimeText);
	connect(m_pVlc.get(), &CVlcKits::sig_TimeSliderPos, this, &QQMediaPlayerCopy::OnUpdateTimeSlider);

	QVBoxLayout* pVLay = new QVBoxLayout(this);
	pVLay->setSpacing(0);

	m_pTitleBar = new CTitleBar(this);
	m_pVideoWidget = new VideoWidget(this);
	m_pPlayCtrlBar = new CPlayCtrlBar(this);
	m_pPlaylistWidget = new CPlayListWidget(this);

	pVLay->addWidget(m_pTitleBar);

	QHBoxLayout* pHLay = new QHBoxLayout;
	pHLay->addWidget(m_pVideoWidget);
	pHLay->addWidget(m_pPlaylistWidget);
	pVLay->addLayout(pHLay);

	pVLay->addWidget(m_pPlayCtrlBar);
	pVLay->setContentsMargins(0, 0, 0, 0);

	m_pPlaylistWidget->hide();  //�����б�Ĭ������
	this->resize(800, 600);

	connect(m_pTitleBar, &CTitleBar::sig_close, this, &QQMediaPlayerCopy::On_Close);
	connect(m_pTitleBar, &CTitleBar::sig_ShowFullFcreen, this, &QQMediaPlayerCopy::On_ShowFullScreen);
	connect(m_pTitleBar, &CTitleBar::sig_showMiniMode, this, &QQMediaPlayerCopy::On_ShowMiniMode);
	connect(m_pTitleBar, &CTitleBar::sig_openfile, this, &QQMediaPlayerCopy::on_openFile);
	connect(m_pVideoWidget, &VideoWidget::sig_OpenFile, this, &QQMediaPlayerCopy::on_openFile);
	connect(m_pVideoWidget, &VideoWidget::sig_OpenPlaylist, this, &QQMediaPlayerCopy::On_openRightPlaylist);
	connect(m_pPlayCtrlBar, &CPlayCtrlBar::sig_fullScreen, this, &QQMediaPlayerCopy::On_ShowFullScreen);
	connect(m_pPlayCtrlBar, &CPlayCtrlBar::sig_playRate, this, &QQMediaPlayerCopy::OnSetPlayRate);
	connect(m_pPlaylistWidget, &CPlayListWidget::sig_doubleClickFileName,
		this,
		&QQMediaPlayerCopy::OnPlay);
}

void QQMediaPlayerCopy::resizeEvent(QResizeEvent* event)
{
	if (!this->isMaximized() && !this->isFullScreen())
	{
		QScreen* pScreen = qApp->primaryScreen();
		QRect appRect = pScreen->availableGeometry();
		int sw = appRect.width();
		int sh = appRect.height();

		int thisW = this->width();
		int thisH = this->height();

		//�����ĳЩ�����г�ͻ����ʱ���ƶ�����Ļ����
		//this->move((sw - thisW) / 2, (sh - thisH) / 2);
	}
}

void QQMediaPlayerCopy::On_Close()
{
	if (m_isPlay)
	{
		QMessageBox::StandardButton rb = QMessageBox::warning(this, "warn",
			"Do you want to end player?",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		if (rb == QMessageBox::Yes)
		{

			close();
		}
	}
	else
	{
		close();
	}
}

void QQMediaPlayerCopy::on_openFile(const QStringList& fileList)
{
	if (fileList.isEmpty())
		return;

	// ��������ģ��
	m_pStringListModel = new QStringListModel(this);

	// Ϊģ������StringList���ᵼ��StringList������
	m_pStringListModel->setStringList(fileList);

	// ΪlistView��������ģ��
	m_pPlaylistWidget->setPlayList(m_pStringListModel);

	if (0 != m_pVlc->play(fileList, (void*)(m_pVideoWidget->winId())))
	{
		QMessageBox::information(this, tr("Warn"), tr("can't play this video file"));
		return;
	}

	SetTimer(NULL, 1, 300, TimeProc);
	m_pVideoWidget->setPlayStatus(true);
	m_pVideoWidget->showTopWidgetOpenfilesButton(false);
	QString fileName = fileList[0];
	int index1 = fileName.lastIndexOf('/');
	
	fileName = fileName.mid(index1 + 1);
	m_pTitleBar->setFileNameLabelText(fileName);
	m_isPlay = true;

	m_pPlayCtrlBar->showTimeLabel(true);
	
	m_pTimer = new QTimer(this);
	m_pTimer->start(3000);
	connect(m_pTimer, &QTimer::timeout, this, &QQMediaPlayerCopy::On_timer);
}

void QQMediaPlayerCopy::On_openRightPlaylist()
{
	if (m_pPlaylistWidget->isHidden())
	{
		m_pPlaylistWidget->show();
	}
	else
	{
		m_pPlaylistWidget->hide();
	}
}

void QQMediaPlayerCopy::On_timer()
{
	if (m_isPlay)
	{
		if (mouse_last_moved_pos_ == QCursor::pos())
		{
			QPoint p1 = m_pVideoWidget->mapToGlobal(QPoint(0, 0));
			QRect rect = m_pVideoWidget->rect();

			QRect rect2(p1.x(), p1.y(), rect.width(), rect.height());
			QPoint p2 = QCursor::pos();

			if (m_isPlay)
			{
				if (rect2.contains(p2))
				{
					m_pVideoWidget->showTopWidget(false);
					this->setCursor(Qt::BlankCursor);   //�������
					m_pTimer->stop();
				}
				else
				{
					this->setCursor(Qt::ArrowCursor);  //��ʾ�������
				}
			}
		}
		else
		{
			this->setCursor(Qt::ArrowCursor);  //��ʾ�������
		}
	}
}

void QQMediaPlayerCopy::OnUpdateTimeText(const QString& str)
{
	m_pPlayCtrlBar->setCurPlayTime(str);
}

void QQMediaPlayerCopy::OnUpdateTimeSlider(const int& value)
{
	m_pVideoWidget->setTimeSliderPos(value);
}

// ���ٲ���
void QQMediaPlayerCopy::OnSetPlayRate(double rate)
{
	m_pVlc->setPlayRate(rate);
}

void QQMediaPlayerCopy::OnPlay(const QString& fileName)
{
	QStringList fileList;
	fileList << fileName;

	if (0 != m_pVlc->play(fileList, (void*)(m_pVideoWidget->winId())))
	{
		QMessageBox::information(this, tr("Warn"), tr("can't play this video file"));
		return;
	}
}

void QQMediaPlayerCopy::On_ShowFullScreen()
{
	this->showFullScreen();
	m_pTitleBar->hide();
	m_pPlayCtrlBar->hide();
	m_pPlaylistWidget->hide();
}

void QQMediaPlayerCopy::On_ShowMiniMode()
{
	if (m_isPlay)
	{
		m_isMiniMode = true;
		m_pTitleBar->hide();
		m_pPlayCtrlBar->hide();
		m_pPlaylistWidget->hide();
		this->update();
	}
} 

void QQMediaPlayerCopy::mouseDoubleClickEvent(QMouseEvent* event)
{
	QPoint p1 = m_pVideoWidget->mapToGlobal(QPoint(0, 0));
	QRect rect = m_pVideoWidget->rect();

	QRect rect2(p1.x(), p1.y(), rect.width(), rect.height());
	QPoint p2 = QCursor::pos();

	if (rect2.contains(p2))
	{
		m_isMiniMode = false;  // �˳�����ģʽ

		if (this->isFullScreen())
		{
			this->showNormal();
			m_pTitleBar->show();
			m_pPlayCtrlBar->show();
		}
		else
		{
			this->showFullScreen();
			m_pTitleBar->hide();
			m_pPlayCtrlBar->hide();
			m_pPlaylistWidget->hide();
		}
	}
}

void QQMediaPlayerCopy::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		if (this->isFullScreen())
		{
			this->showNormal();
			m_isMiniMode = false;  // �˳�����ģʽ
			m_pTitleBar->show();
			m_pPlayCtrlBar->show();
		}
	}
}

void QQMediaPlayerCopy::mouseMoveEvent(QMouseEvent* event)
{
	mouse_moved_ = true;
	mouse_last_moved_pos_ = QCursor::pos();

	if (m_pTimer && !m_pTimer->isActive())
		m_pTimer->start(3000);

	QPoint p1 = m_pVideoWidget->mapToGlobal(QPoint(0, 0));
	QRect rect = m_pVideoWidget->rect();

	QRect rect2(p1.x(), p1.y(), rect.width(), rect.height());
	QPoint p2 = QCursor::pos();

	if (m_isPlay)
	{
		if (rect2.contains(p2))
		{
			m_pVideoWidget->showTopWidget(true);
		}
		else
		{
			m_pVideoWidget->showTopWidget(false);
		}
	}

	/*********************************��������*********************************/
	//���״̬ʱ��������
	if (isMaximized())
	{
		return;
	}
	//����λ�����������ʽ
	SetMouseCursor(event->pos().x(), event->pos().y());

	if ((event->buttons() == Qt::LeftButton) && left_button_pressed_)
	{
		QPoint point_offset = event->globalPos() - last_position_;
		if (mouse_press_region_ == kMousePositionMid)
		{
			if (m_isMiniMode)
			{
				// ����ģʽ���ƶ�����
				setCursor(Qt::ArrowCursor);
				move(point_offset + last_point_);
			}
		}
		else
		{
			QRect rect = geometry();
			switch (mouse_press_region_)
			{
			case kMousePositionLeftTop:
				rect.setTopLeft(rect.topLeft() + point_offset);
				break;
			case kMousePositionTop:
				rect.setTop(rect.top() + point_offset.y());
				break;
			case kMousePositionRightTop:
				rect.setTopRight(rect.topRight() + point_offset);
				break;
			case kMousePositionRight:
				rect.setRight(rect.right() + point_offset.x());
				break;
			case kMousePositionRightButtom:
				rect.setBottomRight(rect.bottomRight() + point_offset);
				break;
			case kMousePositionButtom:
				rect.setBottom(rect.bottom() + point_offset.y());
				break;
			case kMousePositionLeftButtom:
				rect.setBottomLeft(rect.bottomLeft() + point_offset);
				break;
			case kMousePositionLeft:
				rect.setLeft(rect.left() + point_offset.x());
				break;
			default:
				break;
			}
			setGeometry(rect);
			last_position_ = event->globalPos();
		}
	}
	/******************************************************************/
}

void QQMediaPlayerCopy::mouseReleaseEvent(QMouseEvent* event)
{
	left_button_pressed_ = false;
}

void QQMediaPlayerCopy::mousePressEvent(QMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton) {
		left_button_pressed_ = true;
		last_point_ = pos();
		last_position_ = event->globalPos();
		mouse_press_region_ = GetMouseRegion(event->pos().x(), event->pos().y());
	}
}

void QQMediaPlayerCopy::SetMouseCursor(int x, int y)
{
	Qt::CursorShape cursor;
	int region = GetMouseRegion(x, y);
	switch (region)
	{
	case kMousePositionLeftTop:
	case kMousePositionRightButtom:
		cursor = Qt::SizeFDiagCursor; break;
	case kMousePositionRightTop:
	case kMousePositionLeftButtom:
		cursor = Qt::SizeBDiagCursor; break;
	case kMousePositionLeft:
	case kMousePositionRight:
		cursor = Qt::SizeHorCursor; break;
	case kMousePositionTop:
	case kMousePositionButtom:
		cursor = Qt::SizeVerCursor; break;
	case kMousePositionMid:
		cursor = Qt::ArrowCursor; break;
	default:
		break;
	}
	setCursor(cursor);
}

int QQMediaPlayerCopy::GetMouseRegion(int x, int y)
{
	int region_x = 0, region_y = 0;
	if (x < kMouseRegionLeft)
	{
		region_x = 1;
	}
	else if (x > (width() - kMouseRegionRight))
	{
		region_x = 3;
	}
	else
	{
		region_x = 2;
	}
	if (y < kMouseRegionTop)
	{
		region_y = 1;
	}
	else if (y > (height() - kMouseRegionButtom))
	{
		region_y = 3;
	}
	else
	{
		region_y = 2;
	}
	return region_y * 10 + region_x;
}