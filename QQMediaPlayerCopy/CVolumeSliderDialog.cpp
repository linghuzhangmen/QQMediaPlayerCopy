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
	setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);   //ToolTip : ��������ʾ���뿪ʱ��ʧ
	setStyleSheet("QDialog{background-color: rgba(54, 54, 54, 0.5);}");  //0.5��ʾ͸���ȣ�0��ʾȫ͸����1��ʾ��͸����Ҳ����ʹ�ðٷְٱ�ʾ�磺 frm->setStyleSheet(��QFrame{background-color: rgba(255, 0, 0, 50%);}��);

	connect(m_pSlider, &QSlider::valueChanged, [=](int value) {
		emit sig_SliderValueChanged(value);
		});
}

CVolumeSliderDialog::~CVolumeSliderDialog()
{
}

//�ο�qt�ĵ�:bool QWidget::event(QEvent *event)
//����popup��dialog�д�����Ӱ����Ҫȥ������дevent����
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
			::SetClassLong(hwnd, GCL_STYLE, class_style); // windowsϵͳ����
		}
	}

	return QWidget::event(event);
}
