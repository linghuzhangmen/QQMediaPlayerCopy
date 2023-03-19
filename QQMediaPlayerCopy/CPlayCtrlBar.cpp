/*

底部控制栏

*/

#include "CPlayCtrlBar.h"
#include <QMenu>
#include <QEvent>

#define TIME_SLIDER_HEIGHT   10

CPlayCtrlBar::CPlayCtrlBar(QWidget *parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);  // 禁止父窗口样式影响子控件样式

	setFixedHeight(67);
	setStyleSheet("QWidget{background-color:rgb(17,17,17);}");

	InitData();
}

CPlayCtrlBar::~CPlayCtrlBar()
{
}

void CPlayCtrlBar::showTimeLabel(bool isShow)
{
	if (isShow)
	{
		m_pCurPlayTimeLabel->show();
	}
	else
	{
		m_pCurPlayTimeLabel->hide();
	}
}

//设置当前播放时间
void CPlayCtrlBar::setCurPlayTime(const QString& curPlaytime)
{
	m_pCurPlayTimeLabel->setText(curPlaytime);
}

void CPlayCtrlBar::resizeEvent(QResizeEvent* event)
{
	RestoreUI();
}

bool CPlayCtrlBar::eventFilter(QObject* watched, QEvent* event)
{
	if (m_pSpeedButton)
	{
		if (event->type() == QEvent::Show && watched == m_pSpeedButton->menu())
		{
			int menuWidth = m_pSpeedButton->menu()->width();
			int menuHeight = m_pSpeedButton->menu()->height();
			int buttonWidth = m_pSpeedButton->width();
			int buttonHeight = m_pSpeedButton->height();
			int menuXPos = m_pSpeedButton->menu()->pos().x();  
			int menuYPos = m_pSpeedButton->mapToGlobal(QPoint(0, 0)).y() - menuHeight - 3;
			m_pSpeedButton->menu()->move(menuXPos, menuYPos);
			return true;
		}
	}

	return false;
}

void CPlayCtrlBar::setTimeSliderRange(qint64 value)
{
	m_pTimeSlider->setRange(0, value);
}

void CPlayCtrlBar::setSliderValue(qint64 value)
{
	m_pTimeSlider->setValue(value);
}

void CPlayCtrlBar::showSlider(bool flag)
{
	m_pTimeSlider->show();
}

void CPlayCtrlBar::InitData()
{
	m_pTimeSlider = new CTimeSlider(this);
	m_pTimeSlider->setFixedHeight(TIME_SLIDER_HEIGHT);
	m_pTimeSlider->hide();

	QString slider_qss = "QSlider{background:transparent; \
		border-style: outset; \
		border-style: outset;  \
			border-radius: 10px;} \
		QSlider::groove:horizontal{ \
			height: 12px; \
			background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4); \
			margin: 2px 0} \
		QSlider::handle:horizontal { \
			background: QRadialGradient(cx:0, cy:0, radius: 1, fx:0.5, fy:0.5, stop:0 white, stop:1 green); \
			width: 16px; \
			height: 16px; \
			margin: -5px 6px -5px 6px; \
			border-radius:11px;  \
		border: 3px solid #ffffff;}";

	m_pTimeSlider->setStyleSheet(slider_qss);

	connect(m_pTimeSlider, &QSlider::sliderMoved, this, &CPlayCtrlBar::onSliderMoved);

	QString label_qss = "QLabel{font-family:Microsoft YaHei; font-size:18px; color:rgb(255,255,255);}";

	m_pCurPlayTimeLabel = new QLabel(this);
	m_pCurPlayTimeLabel->setText("");
	m_pCurPlayTimeLabel->setFixedSize(180, 32);
	m_pCurPlayTimeLabel->setStyleSheet(label_qss);

	m_pPlayButtonGroupWidget = new CPlayButtonGroupWidget(this);
	int tempH = m_pPlayButtonGroupWidget->height();

	m_pSpeedButton = new QPushButton(this);
	m_pSpeedButton->setText(tr("1.0speed"));
	m_pSpeedButton->setFixedSize(40, 32);

	m_pSpeedButton->setStyleSheet("QPushButton{ \
		background-color:rgb(64, 70, 80);color:white;border:none; \
		text-align:right;font-size: 12px; \
		border-radius:6px;} \
		QPushButton:hover{background-color:rgb(99, 99, 99);border:none;} \
		QPushButton:pressed{border:none;} \
		QPushButton::menu-indicator{image:none}");

	QMenu* pMenu = new QMenu(this);
	//pMenu->setFixedWidth(80);
	QAction* pAc1 = new QAction(tr("0.5 speed"), this);
	QAction* pAc2 = new QAction(tr("0.75 speed"), this);
	QAction* pAc3 = new QAction(tr("1.0 speed"), this);
	QAction* pAc4 = new QAction(tr("1.25 speed"), this);
	QAction* pAc5 = new QAction(tr("1.5 speed"), this);
	QAction* pAc6 = new QAction(tr("1.75 speed"), this);
	QAction* pAc7 = new QAction(tr("2.0 speed"), this);
	pMenu->addAction(pAc1);
	pMenu->addAction(pAc2);
	pMenu->addAction(pAc3);
	pMenu->addAction(pAc4);
	pMenu->addAction(pAc5);
	pMenu->addAction(pAc6);
	pMenu->addAction(pAc7);
	m_pSpeedButton->setMenu(pMenu);
	pMenu->installEventFilter(this);  //设置事件过滤，调整弹出菜单的位置

	connect(pAc1, &QAction::triggered, [=] {
		m_pSpeedButton->setText("0.5x");
		emit sig_playRate(0.5);
		});

	connect(pAc2, &QAction::triggered, [=] {
		m_pSpeedButton->setText("0.75x");
		emit sig_playRate(0.75);
		});

	connect(pAc3, &QAction::triggered, [=] {
		m_pSpeedButton->setText("1.0x");
		emit sig_playRate(1.0);
		});

	connect(pAc4, &QAction::triggered, [=] {
		m_pSpeedButton->setText("1.25x");
		emit sig_playRate(1.25);
		});

	connect(pAc5, &QAction::triggered, [=] {
		m_pSpeedButton->setText("1.5x");
		emit sig_playRate(1.5);
		});

	connect(pAc6, &QAction::triggered, [=] {
		m_pSpeedButton->setText("1.75x");
		emit sig_playRate(1.75);
		});

	connect(pAc7, &QAction::triggered, [=] {
		m_pSpeedButton->setText("2.0x");
		emit sig_playRate(2.0);
		});

	std::string menuItemQss = R"(
		QMenu
		{
			background-color:rgb(73, 73, 73);
		}
 
		QMenu::item
		{
			 font:16px;
			 color:white;
			 background-color:rgb(73, 73, 73);
			 margin:8px 8px;
		}
 
		/*选择项设置*/
		QMenu::item:selected
		{
			 background-color:rgb(54, 54, 54);
		}
	)";

	pMenu->setStyleSheet(QString::fromStdString(menuItemQss));
	pMenu->setWindowOpacity(0.7);

	m_pFullscreenButton = new QPushButton(this);
	m_pFullscreenButton->setText("");
	m_pFullscreenButton->setFixedSize(32, 32);

	m_pFullscreenButton->setStyleSheet("QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/fullscreen.svg);border:none;}"
		"QPushButton:hover{background-color:rgb(99, 99, 99);background-image:url(:/playCtrlBar/resources/playctrlBar/fullscreen_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/fullscreen.svg);border:none;}");

	showTimeLabel(false);

	connect(m_pPlayButtonGroupWidget, &CPlayButtonGroupWidget::sig_play, this, &CPlayCtrlBar::sig_play);
	connect(m_pPlayButtonGroupWidget, &CPlayButtonGroupWidget::sig_stop, this, &CPlayCtrlBar::sig_stop);
	connect(m_pFullscreenButton, &QPushButton::clicked, this, &CPlayCtrlBar::sig_fullScreen);
}

void CPlayCtrlBar::RestoreUI()
{
	int w = this->width();
	int h = this->height();

	m_pTimeSlider->move(1, 1);
	m_pTimeSlider->setFixedWidth(w - 2);

	int x0 = 10;
	int y0 = 14 + 9;
	m_pCurPlayTimeLabel->move(x0, y0);

	int x1 = (w - m_pPlayButtonGroupWidget->width()) / 2;
	int y1 = 14;
	m_pPlayButtonGroupWidget->move(x1, y1);

	m_pFullscreenButton->move(w - 5 - 32, y0);
	m_pSpeedButton->move(w - 5 - 32 - 50 - 40, y0);
}

void CPlayCtrlBar::onSliderMoved(int position)
{
	emit sig_SliderMove(position);
}