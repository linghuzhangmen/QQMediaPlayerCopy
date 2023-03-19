#pragma once
#include <QWidget>
#include <QPushButton>
#include "COpenFileButton.h"
#include <QSlider>


class CVideoWidgetTopWidget : public QWidget
{
	Q_OBJECT

public:
	CVideoWidgetTopWidget(QWidget* p = nullptr);
	~CVideoWidgetTopWidget();

	void showRightBtn(bool flag);
	void showOpenFileBtn(bool flag);
	
	void setPlayStatus(bool play) { m_isPlay = play; }

protected:
	void resizeEvent(QResizeEvent* event) override;

signals:
	void sig_OpenFile(const QStringList& fileList);
	void sig_OpenFloder(QString path);
	void sig_OpenUrl(const QString& url);
	void sig_OpenPlaylist();

private:
	QPushButton* m_pOpenrRightlistButton = nullptr;
	COpenFileButton* m_pOpenFileButton = nullptr;
	//CTimeSlider* m_pTimeSlider = nullptr;
	int m_bSliderPressed = false;
	bool m_isPlay = false;
}; 

