/*

��Ƶ��ʾ��

*/

#pragma once
#include <QWidget>
#include <QPushButton>
#include "COpenFileButton.h"
#include <QSlider>

class VideoWidget : public QWidget
{
	Q_OBJECT

public:
	VideoWidget(QWidget *p = NULL);
	~VideoWidget();

	void ShowOpenFileButton(bool show);
	void setPlay(bool play);
	QWidget* playWidget() const;
	void setTimeSliderRange(int maxValue);
	void setTimeSliderPos(int pos);
	void hideCtrl(bool hide);

protected:
	void resizeEvent(QResizeEvent* event) override;
	void enterEvent(QEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

signals:
	void sig_OpenFile(const QStringList& fileList);
	void sig_OpenFloder(QString path);
	void sig_OpenPlaylist();

private:
	QWidget* m_pVideoPlayWidget = nullptr;
	QPushButton* m_pOpenrRightlistButton = nullptr;
	COpenFileButton* m_pOpenFileButton = nullptr;
	QSlider* m_pTimeSlider = nullptr;
	bool   m_isPlay = false;
};

