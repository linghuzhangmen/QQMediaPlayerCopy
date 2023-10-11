#include "VideoWidget.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenu>
#include <QDebug>
#include <qt_windows.h>
#include <QApplication>
#include <QMessageBox>
#include <iostream>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileInfo>

using namespace std;

VideoWidget::VideoWidget(QWidget* p) :
	QWidget(p)
{
	setAttribute(Qt::WA_StyledBackground);  // 禁止父窗口样式影响子控件样式
	this->setAttribute(Qt::WA_OpaquePaintEvent);

	qApp->installNativeEventFilter(this);

	setMouseTracking(true);
	setAcceptDrops(true);

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

	m_pTopWidget = new CVideoWidgetTopWidget(this);

	connect(m_pTopWidget, &CVideoWidgetTopWidget::sig_OpenFile, this, &VideoWidget::sig_OpenFile);
	connect(m_pTopWidget, &CVideoWidgetTopWidget::sig_OpenUrl, this, &VideoWidget::sig_OpenUrl);
	connect(m_pTopWidget, &CVideoWidgetTopWidget::sig_OpenPlaylist, this, &VideoWidget::sig_OpenPlaylist);
}

VideoWidget::~VideoWidget()
{
}

void VideoWidget::showTopWidget(bool show)
{
	if (show)
	{
		m_pTopWidget->show();
		m_pTopWidget->showOpenFileBtn(false);
	}
	else
	{
		m_pTopWidget->hide();
	}
}

void VideoWidget::setPlayStatus(bool play)
{
	m_isPlay = play;
}

void VideoWidget::showTopWidgetOpenfilesButton(bool show)
{
	m_pTopWidget->showOpenFileBtn(show);
}

void VideoWidget::resizeEvent(QResizeEvent* event)
{
	m_dPos = this->pos();

	QPoint pos = this->mapToGlobal(QPoint(0, 0));
	m_pTopWidget->resize(this->width(), this->height());
	m_pTopWidget->move(pos);
	m_pTopWidget->show();
}

void VideoWidget::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasFormat("text/uri-list")) {
		event->acceptProposedAction();
	}
}

// 注意windows下如果使用管理员权限，Qt的拖拽功能将被禁止
void VideoWidget::dropEvent(QDropEvent* event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	if (urls.isEmpty())
		return;

	QStringList video_list;

	// 遍历 URL 列表
	for (const QUrl& url : urls) 
	{
		// 将 URL 转换为本地文件路径
		QString filePath = url.toLocalFile();
		if (isVideoFile(filePath))
		{
			video_list.append(filePath);
		}
	}

	if (video_list.size() < 1)
	{
		return;
	}

	emit sig_OpenFile(video_list);
}

#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
bool VideoWidget::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result)
#else
bool VideoWidget::nativeEventFilter(const QByteArray& eventType, void* message, long* result)
#endif
{
	if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG") 
	{
		MSG* pMsg = reinterpret_cast<MSG*>(message);
		if (pMsg->message == 1025 || pMsg->message == 512)
		{
			QPoint pos = this->window()->mapToGlobal(QPoint(0, 0));
			m_pTopWidget->move(pos + m_dPos);
		}
	}

	return false;
}

bool VideoWidget::isVideoFile(const QString& filePath)
{
	// 创建一个 QFileInfo 对象，它提供了关于文件的信息
	QFileInfo fileInfo(filePath);

	// 获取文件的后缀
	QString fileSuffix = fileInfo.suffix().toLower();

	// 定义一个包含常见视频文件后缀的字符串列表
	QStringList videoSuffixes = { "mp4", "mkv", "avi", "mov", "flv", "wmv" };

	// 检查文件后缀是否在视频后缀列表中
	return videoSuffixes.contains(fileSuffix);
}