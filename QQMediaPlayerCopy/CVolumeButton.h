/*

�������ڰ�ť

���ܣ�
	1. �������������ʱ��ʾslider dialog
	2. ���ʱmute

*/

#pragma once

#include <QPushButton>
#include "CVolumeSliderDialog.h"

class CVolumeButton : public QPushButton
{
	Q_OBJECT

public:
	CVolumeButton(QWidget* parent);
	~CVolumeButton();

	bool getMute() const
	{
		return m_isMute;
	}

	void setMute(bool mute) { m_isMute = mute; }

private:
	void paintEvent(QPaintEvent* event) override;
	void enterEvent(QEvent* event) override;  // Qt6.5.3�Ѳ����ָĻ���Qt5��QEvent
	void mousePressEvent(QMouseEvent* event) override;
	void timerEvent(QTimerEvent* event) override;

signals:
	void sig_VolumeValue(int value);

private:
	bool m_isMute = false;  //�Ƿ���
	CVolumeSliderDialog* m_pVolumeSliderDlg = nullptr;
};
