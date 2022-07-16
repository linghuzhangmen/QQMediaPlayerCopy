#pragma once

#include <QSlider>

class CTimeSlider : public QSlider
{
	Q_OBJECT

public:
	CTimeSlider(QWidget* p = nullptr);
	~CTimeSlider();

private:
	bool eventFilter(QObject* watched, QEvent* event) override;

private:
	void slider_mouseLButtonPress(QObject* slider, QEvent* event);
	void setTimeSliderRange(qint64 value);
	void setSliderValue(qint64 value);

private slots:
	void onSliderMoved(int position);

signals:
	void sig_SliderMove(int value);

private:
	int m_bSliderPressed = false;
	int m_SliderHeight = 15;
};

