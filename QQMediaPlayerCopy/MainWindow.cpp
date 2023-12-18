#include "MainWindow.h"
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

const wchar_t* MAIN_TITLE_TEXT = L"qt_media_plyaer_ex";

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
	HWND vlcHwnd = FindWindowEx(NULL, NULL, NULL, MAIN_TITLE_TEXT);

	if (vlcHwnd)
	{
		EnumChildWindows(vlcHwnd, EnumVLC, NULL);
	}
}

CMainWindow::CMainWindow(QWidget* parent)
	: CFrameLessWidgetBase(parent)
{
	QString main_title = QString::fromStdWString(MAIN_TITLE_TEXT);
	this->setWindowTitle(main_title);
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

	connect(m_pVlc.get(), &CVlcKits::sig_UpdateTimeText, this, &CMainWindow::OnUpdateTimeText);
	connect(m_pVlc.get(), &CVlcKits::sig_TimeSliderPos, this, &CMainWindow::OnUpdateTimeSlider);

	InitUI();
}

void CMainWindow::InitUI()
{
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

	m_pPlaylistWidget->hide();  //播放列表默认隐藏
	this->resize(830, 640);

	connect(m_pTitleBar, &CTitleBar::sig_close, this, &CMainWindow::On_Close);
	connect(m_pTitleBar, &CTitleBar::sig_ShowFullFcreen, this, &CMainWindow::On_ShowFullScreen);
	connect(m_pTitleBar, &CTitleBar::sig_showMiniMode, this, &CMainWindow::On_ShowMiniMode);
	connect(m_pTitleBar, &CTitleBar::sig_openfile, this, &CMainWindow::on_openFile);
	connect(m_pTitleBar, &CTitleBar::sig_openUrl, this, &CMainWindow::on_openUrl);
	connect(m_pVideoWidget, &VideoWidget::sig_OpenFile, this, &CMainWindow::on_openFile);
	connect(m_pVideoWidget, &VideoWidget::sig_OpenUrl, this, &CMainWindow::on_openUrl);
	connect(m_pVideoWidget, &VideoWidget::sig_OpenPlaylist, this, &CMainWindow::On_openRightPlaylist);
	connect(m_pPlayCtrlBar, &CPlayCtrlBar::sig_fullScreen, this, &CMainWindow::On_ShowFullScreen);
	connect(m_pPlayCtrlBar, &CPlayCtrlBar::sig_playRate, this, &CMainWindow::OnSetPlayRate);

	connect(m_pPlaylistWidget, &CPlayListWidget::sig_doubleClickFileName,
		this,
		&CMainWindow::OnPlay);
}

void CMainWindow::resizeEvent(QResizeEvent* event)
{
	if (!this->isMaximized() && !this->isFullScreen())
	{
		QScreen* pScreen = qApp->primaryScreen();
		QRect appRect = pScreen->availableGeometry();
		int sw = appRect.width();
		int sh = appRect.height();

		int thisW = this->width();
		int thisH = this->height();
	}
}

void CMainWindow::On_Close()
{
	if (m_isPlay)
	{
		QMessageBox::StandardButton rb = QMessageBox::warning(this, tr("warn"),
			tr("Do you want to end player?"),
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

void CMainWindow::on_openFile(const QStringList& fileList)
{
	if (fileList.isEmpty())
		return;

	// 创建数据模型
	m_pStringListModel = new QStringListModel(this);

	// 为模型设置StringList，会导入StringList的内容
	m_pStringListModel->setStringList(fileList);

	// 为listView设置数据模型
	m_pPlaylistWidget->setPlayList(m_pStringListModel);

	if (0 != m_pVlc->play(fileList, (void*)(m_pVideoWidget->winId())))
	{
		QMessageBox::information(this, tr("Warn"), tr("can't play this video file"));
		return;
	}

	if (fileList.size() > 0)
	{
		m_pPlaylistWidget->show();
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
	connect(m_pTimer, &QTimer::timeout, this, &CMainWindow::On_timer);
}

// 打开网络媒体流
// rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mp4
void CMainWindow::on_openUrl(const QString& url)
{
	m_pVlc->play(url, (void*)(m_pVideoWidget->winId()));

	m_isPlay = true;

	m_pPlayCtrlBar->showTimeLabel(true);

	m_pTimer = new QTimer(this);
	m_pTimer->start(3000);
	connect(m_pTimer, &QTimer::timeout, this, &CMainWindow::On_timer);
}

void CMainWindow::On_openRightPlaylist()
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

void CMainWindow::On_timer()
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
					this->setCursor(Qt::BlankCursor);   //隐藏鼠标
					m_pTimer->stop();
				}
				else
				{
					this->setCursor(Qt::ArrowCursor);  //显示正常鼠标
				}
			}
		}
		else
		{
			this->setCursor(Qt::ArrowCursor);  //显示正常鼠标
		}
	}
}

void CMainWindow::OnUpdateTimeText(const QString& str)
{
	m_pPlayCtrlBar->setCurPlayTime(str);
}

void CMainWindow::OnUpdateTimeSlider(const int& value)
{
	//m_pVideoWidget->setTimeSliderPos(value);
}

// 倍速播放
void CMainWindow::OnSetPlayRate(double rate)
{
	m_pVlc->setPlayRate(rate);
}

void CMainWindow::OnPlay(const QString& fileName)
{
	QStringList fileList;
	fileList << fileName;

	if (0 != m_pVlc->play(fileList, (void*)(m_pVideoWidget->winId())))
	{
		QMessageBox::information(this, tr("Warn"), tr("can't play this video file"));
		return;
	}
}

void CMainWindow::On_ShowFullScreen()
{
	this->showFullScreen();
	m_pTitleBar->hide();
	m_pPlayCtrlBar->hide();
	m_pPlaylistWidget->hide();
}

void CMainWindow::On_ShowMiniMode()
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

void CMainWindow::mouseDoubleClickEvent(QMouseEvent* event)
{
	QPoint p1 = m_pVideoWidget->mapToGlobal(QPoint(0, 0));
	QRect rect = m_pVideoWidget->rect();

	QRect rect2(p1.x(), p1.y(), rect.width(), rect.height());
	QPoint p2 = QCursor::pos();

	if (rect2.contains(p2))
	{
		m_isMiniMode = false;  // 退出迷你模式

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

void CMainWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		if (this->isFullScreen())
		{
			this->showNormal();
			m_isMiniMode = false;  // 退出迷你模式
			m_pTitleBar->show();
			m_pPlayCtrlBar->show();
		}
	}
}

void CMainWindow::mouseMoveEvent(QMouseEvent* event)
{
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
}


