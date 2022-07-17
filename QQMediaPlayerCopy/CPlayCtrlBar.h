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

private:
	void resizeEvent(QResizeEvent* event) override;

private:
	QLabel* m_pCurPlayTimeLabel = nullptr;
	CPlayButtonGroupWidget* m_pPlayButtonGroupWidget = nullptr;
	QPushButton* m_pToolBoxButton = nullptr;
	QPushButton* m_pEffectButton = nullptr;
	QPushButton* m_pFullscreenButton = nullptr;
};
