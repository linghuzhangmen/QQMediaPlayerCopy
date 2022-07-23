#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "CPlayButtonGroupWidget.h"

class CPlayCtrlBar : public QWidget
{
	Q_OBJECT

public:
	CPlayCtrlBar(QWidget *parent = Q_NULLPTR);
	~CPlayCtrlBar();

	void showTimeLabel(bool isShow);
	void setCurPlayTime(const QString& curPlaytime);

signals:
	void sig_play();
	void sig_stop();
	void sig_fullScreen();
	void sig_playRate(double rate);

private:
	void resizeEvent(QResizeEvent* event) override;
	bool eventFilter(QObject* watched, QEvent* event) override;

private:
	QLabel* m_pCurPlayTimeLabel = nullptr;
	CPlayButtonGroupWidget* m_pPlayButtonGroupWidget = nullptr;
	QPushButton* m_pSpeedButton = nullptr;
	QPushButton* m_pFullscreenButton = nullptr;
};
