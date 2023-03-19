/*

视频显示窗口类

*/


#pragma once

#include <QWidget>
#include <QPushButton>
#include <QAbstractNativeEventFilter>
#include "CVideoWidgetTopWidget.h"


class VideoWidget : public QWidget, public QAbstractNativeEventFilter
{
	Q_OBJECT

public:
	VideoWidget(QWidget* p = NULL);
	~VideoWidget();

	void showTopWidget(bool show);
	void setPlayStatus(bool play);
	void showTopWidgetOpenfilesButton(bool show);

protected:
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
	bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override;
#else
	bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override;
#endif
	void resizeEvent(QResizeEvent* event) override;

signals:
	void sig_OpenFile(const QStringList& fileList);
	void sig_OpenFloder(QString path);
	void sig_OpenPlaylist();
	void sig_Totalms(const qint64& duration);   //发给主界面
	void sig_VideoPositionChanged(const qint64& pos);   //发给主界面
	void sig_SliderMoved(int value);
	void sig_OpenUrl(const QString& url);

private:
	QPoint m_dPos;   //VideoWidget相对于父窗口的位置
	CVideoWidgetTopWidget* m_pTopWidget = nullptr;
	bool m_isPlay = false;
};
