#include "CVolumeSliderDialog.h"
#include <QVBoxLayout>
#include <QEvent>
#include <windows.h>

CVolumeSliderDialog::CVolumeSliderDialog(QWidget* parent)
	: QDialog(parent)
{
	this->setFixedSize(40, 200);
	QVBoxLayout* pVLay = new QVBoxLayout(this);
	m_pSlider = new QSlider(this);
	m_pSlider->setOrientation(Qt::Vertical);
	pVLay->addWidget(m_pSlider);

	setFixedSize(40, 120);
	setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);   //ToolTip : 悬浮是显示，离开时消失
	setStyleSheet("QDialog{background-color: rgba(54, 54, 54, 0.5);}");  //0.5表示透明度，0表示全透明、1表示不透明；也可以使用百分百表示如： frm->setStyleSheet(“QFrame{background-color: rgba(255, 0, 0, 50%);}”);

	connect(m_pSlider, &QSlider::valueChanged, [=](int value) {
		emit sig_SliderValueChanged(value);
		});
}

CVolumeSliderDialog::~CVolumeSliderDialog()
{
}

//参考qt文档:bool QWidget::event(QEvent *event)
//设置popup后，dialog有窗口阴影，需要去除就重写event函数
bool CVolumeSliderDialog::event(QEvent* event)
{
	static bool class_amended = false;

	if (event->type() == QEvent::WinIdChange)
	{
		HWND hwnd = (HWND)winId();

		if (class_amended == false)
		{
			class_amended = true;
			DWORD class_style = ::GetClassLong(hwnd, GCL_STYLE);
			class_style &= ~CS_DROPSHADOW;
			::SetClassLong(hwnd, GCL_STYLE, class_style); // windows系统函数
		}
	}

	return QWidget::event(event);
}
