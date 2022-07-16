#include "CTimeSlider.h"
#include <QEvent>
#include <QMouseEvent>

CTimeSlider::CTimeSlider(QWidget* p)
	:QSlider(p)
{
	setAttribute(Qt::WA_StyledBackground);

	this->setOrientation(Qt::Horizontal);
	this->setFixedHeight(m_SliderHeight);

	// 给事件滑动条安装事件，处理点击效果
	this->installEventFilter(this);

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
			width: 12px; \
			height: 12px; \
			margin: -5px 6px -5px 6px; \
			border-radius:11px;  \
		border: 3px solid #ffffff;}";

	QString qss = "QSlider{ \
		background-color:rgba(22, 22, 22, 0.7); \
		padding-top: 15px;  \
		padding-bottom: 15px; \
		border-radius: 5px;} \
		QSlider::add-page:horizontal{background-color: #FF7826;width:5px;border-radius: 2px;} \
		QSlider::sub-page:horizontal{background-color: #7A7B79;width:5px;border-radius: 2px;} \
		QSlider::groove:horizontal{background:transparent;width:6px;} \
		QSlider::handle:horizontal{height: 14px;width: 14px;margin: 0px -4px 0px -4px;border-radius: 7px;background: white;}";

	//this->setStyleSheet(slider_qss);

	QString sqq = "QSlider::groove:horizontal{  \
	border: 1px solid #bbb; \
	background: white; \
	height: 4x;  border - radius: 2px; \
	left: 5px;right: 5px;}  \
	QSlider::add - page:horizontal{  \
		background: rgb(50,150,250); \
		border: 0px solid #777; \
		border - radius: 2px;}  \
		QSlider#videoSeekSlider::sub - page:horizontal{   \
		background: white;  \
		border - radius: 2px;}   \
		QSlider#videoSeekSlider::handle:horizontal{   \
		border: 0px;  \
		border - image: url(. / Resources / Image / ThemeOne / icon_slider_point.png);  \
		width: 16px;   \
		margin: -7px - 7px - 7px - 7px;}   \
		QSlider#videoVolumnSlider::groove:horizontal{  \
		border: 1px solid #bbb;background: white;  \
		height: 4x;border - radius: 2px;left: 5px;right: 5px;}";

	connect(this, &QSlider::sliderMoved, this, &CTimeSlider::onSliderMoved);
}

CTimeSlider::~CTimeSlider()
{
}

bool CTimeSlider::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() == QEvent::MouseButtonPress && watched == this)
	{
		slider_mouseLButtonPress(watched, event);
	}

	return QWidget::eventFilter(watched, event);
}

void CTimeSlider::slider_mouseLButtonPress(QObject* slider, QEvent* event)
{
	do
	{
		QSlider* sliderCtrl = static_cast<QSlider*>(slider);
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		if (Qt::LeftButton != mouseEvent->button())
		{
			break;
		}

		int cxctl = 0;  // 滑块宽度
		int cxwnd = 0;  // 滑槽长度
		int mxpos = 0;  // 鼠标按下的位置

		if (Qt::Horizontal == sliderCtrl->orientation())
		{
			// 水平样式的 slider
			cxctl = sliderCtrl->minimumSizeHint().width();
			cxwnd = sliderCtrl->width();
			if (sliderCtrl->invertedAppearance())
				mxpos = cxwnd - mouseEvent->x();
			else
				mxpos = mouseEvent->x();
		}
		else
		{
			// 垂直样式的 slider
			cxctl = sliderCtrl->minimumSizeHint().height();
			cxwnd = sliderCtrl->height();
			if (sliderCtrl->invertedAppearance())
				mxpos = mouseEvent->y();
			else
				mxpos = cxwnd - mouseEvent->y();
		}

		if (cxwnd <= cxctl)
		{
			break;
		}

		// 计算结果，并设置新计算得到的 position 值
		int scpos = sliderCtrl->minimum() +
			(int)((sliderCtrl->maximum() - sliderCtrl->minimum()) *
				((mxpos - cxctl / 2.0) / (cxwnd - cxctl)));

		if (sliderCtrl->sliderPosition() == scpos)
		{
			break;
		}

		sliderCtrl->setSliderPosition(scpos);
	} while (0);
}

void CTimeSlider::onSliderMoved(int position)
{
	emit sig_SliderMove(position);
}

void CTimeSlider::setTimeSliderRange(qint64 value)
{
	this->setRange(0, value);
}

void CTimeSlider::setSliderValue(qint64 value)
{
	this->setValue(value);
}