#include "CPlayButtonGroupWidget.h"
#include <QHBoxLayout>

CPlayButtonGroupWidget::CPlayButtonGroupWidget(QWidget *parent)
	: QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_StyledBackground);  // 禁止父窗口样式影响子控件样式

	m_pStopButton = new QPushButton(this);
	m_pStopButton->setText("");
	m_pStopButton->setFixedSize(32, 32);
	m_pStopButton->setStyleSheet("QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/stop.svg);border:none;}"
		"QPushButton:hover{background-image:url(:/playCtrlBar/resources/playctrlBar/stop_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/stop.svg);border:none;}");

	m_pPreButton = new QPushButton(this);
	m_pPreButton->setText("");
	m_pPreButton->setFixedSize(32, 32);
	m_pPreButton->setStyleSheet("QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/pre.svg);border:none;}"
		"QPushButton:hover{background-image:url(:/playCtrlBar/resources/playctrlBar/pre_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/pre.svg);border:none;}");

	m_pPlayButton = new QPushButton(this);
	m_pPlayButton->setText("");
	m_pPlayButton->setFixedSize(48, 48);
	m_pPlayButton->setStyleSheet("QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/play.svg);border:none;}"
		"QPushButton:hover{background-image:url(:/playCtrlBar/resources/playctrlBar/play_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/play.svg);border:none;}");

	m_pNextButton = new QPushButton(this);
	m_pNextButton->setText("");
	m_pNextButton->setFixedSize(32, 32);
	m_pNextButton->setStyleSheet("QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/next.svg);border:none;}"
		"QPushButton:hover{background-image:url(:/playCtrlBar/resources/playctrlBar/next_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/next.svg);border:none;}");

	m_pAudioButton = new QPushButton(this);
	m_pAudioButton->setText("");
	m_pAudioButton->setFixedSize(32, 32);
	m_pAudioButton->setStyleSheet("QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/audio_open.svg);border:none;}"
		"QPushButton:hover{background-image:url(:/playCtrlBar/resources/playctrlBar/audio_open_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/audio_open.svg);border:none;}");

	QHBoxLayout* pHLay = new QHBoxLayout(this);
	pHLay->addWidget(m_pStopButton);
	pHLay->setSpacing(8);
	pHLay->addWidget(m_pPreButton);
	pHLay->setSpacing(8);
	pHLay->addWidget(m_pPlayButton);
	pHLay->setSpacing(8);
	pHLay->addWidget(m_pNextButton);
	pHLay->setSpacing(8);
	pHLay->addWidget(m_pAudioButton);

	pHLay->setContentsMargins(10, 10, 10, 10);

	setLayout(pHLay);

	setFixedSize(32 * 4 + 48 + 4 * 8 + 20 , 68);

	connect(m_pPlayButton, &QPushButton::clicked, this, &CPlayButtonGroupWidget::sig_play);
	connect(m_pStopButton, &QPushButton::clicked, this, &CPlayButtonGroupWidget::sig_stop);
	connect(m_pPreButton, &QPushButton::clicked, this, &CPlayButtonGroupWidget::sig_pre);
	connect(m_pNextButton, &QPushButton::clicked, this, &CPlayButtonGroupWidget::sig_next);
}

CPlayButtonGroupWidget::~CPlayButtonGroupWidget()
{
}
