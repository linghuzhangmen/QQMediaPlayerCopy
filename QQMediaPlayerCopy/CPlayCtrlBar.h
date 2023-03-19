#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "CPlayButtonGroupWidget.h"
#include "CTimeSlider.h"

class CPlayCtrlBar : public QWidget
{
	Q_OBJECT

public:
	CPlayCtrlBar(QWidget *parent = Q_NULLPTR);
	~CPlayCtrlBar();

	void showTimeLabel(bool isShow);
	void setCurPlayTime(const QString& curPlaytime);

	void setTimeSliderRange(qint64 value);
	void setSliderValue(qint64 value);
	void showSlider(bool flag);

private:
	void InitData();
	void RestoreUI();

private slots:
	void onSliderMoved(int position);

signals:
	void sig_play();
	void sig_stop();
	void sig_fullScreen();
	void sig_playRate(double rate);
	void sig_SliderMove(int value);

private:
	void resizeEvent(QResizeEvent* event) override;
	bool eventFilter(QObject* watched, QEvent* event) override;

private:
	CTimeSlider* m_pTimeSlider = nullptr;
	QLabel* m_pCurPlayTimeLabel = nullptr;
	CPlayButtonGroupWidget* m_pPlayButtonGroupWidget = nullptr;
	QPushButton* m_pSpeedButton = nullptr;
	QPushButton* m_pFullscreenButton = nullptr;
};
