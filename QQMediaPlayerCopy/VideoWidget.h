/*

��Ƶ��ʾ������

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

	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;

private:
	bool isVideoFile(const QString& filePath);

signals:
	void sig_OpenFile(const QStringList& fileList);
	void sig_OpenFloder(QString path);
	void sig_OpenPlaylist();
	void sig_Totalms(const qint64& duration);   //����������
	void sig_VideoPositionChanged(const qint64& pos);   //����������
	void sig_SliderMoved(int value);
	void sig_OpenUrl(const QString& url);

private:
	QPoint m_dPos;   //VideoWidget����ڸ����ڵ�λ��
	CVideoWidgetTopWidget* m_pTopWidget = nullptr;
	bool m_isPlay = false;
};
