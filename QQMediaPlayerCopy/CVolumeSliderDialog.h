#pragma once

#include <QDialog>
#include <QSlider>

class CVolumeSliderDialog : public QDialog
{
	Q_OBJECT

public:
	CVolumeSliderDialog(QWidget *parent = Q_NULLPTR);
	~CVolumeSliderDialog();

	void setSliderValue(int value)
	{
		m_pSlider->setValue(value);
	}

protected:
	bool event(QEvent* event) override;

signals:
	void sig_SliderValueChanged(int value);

private:
	QSlider* m_pSlider = nullptr;
};
