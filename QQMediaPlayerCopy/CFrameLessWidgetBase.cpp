#include "CFrameLessWidgetBase.h"
#include <QPainter>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <Windows.h>
#include <windowsx.h>
#endif

#pragma comment(lib, "user32.lib")
#pragma comment(lib,"dwmapi.lib")

CFrameLessWidgetBase::CFrameLessWidgetBase(QWidget* parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_Hover);
}

CFrameLessWidgetBase::~CFrameLessWidgetBase()
{
}

#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
bool CFrameLessWidgetBase::nativeEvent(const QByteArray& eventType, 
	void* message, qintptr* result)
#else
bool CFrameLessWidgetBase::nativeEvent(const QByteArray& eventType, 
	void* message, long* result)
#endif
{
	MSG* param = static_cast<MSG*>(message);

	switch (param->message)
	{
	case WM_NCHITTEST:
	{
		int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
		int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();

		// ������λ���ڲ��ӿؼ��ϣ��򲻽��д���
		if (nX > mouse_margin && nX < width() - mouse_margin &&
			nY > mouse_margin && nY < this->height() - mouse_margin)
		{
			if (childAt(nX, nY) != nullptr)
				return QWidget::nativeEvent(eventType, message, result);
		}

		// �������λ�ڴ���߿򣬽�������
		if ((nX > 0) && (nX < mouse_margin))
			*result = HTLEFT;

		if ((nX > this->width() - mouse_margin) && (nX < this->width()))
			*result = HTRIGHT;

		if ((nY > 0) && (nY < mouse_margin))
			*result = HTTOP;

		if ((nY > this->height() - mouse_margin) && (nY < this->height()))
			*result = HTBOTTOM;

		if ((nX > 0) && (nX < mouse_margin) && (nY > 0)
			&& (nY < mouse_margin))
			*result = HTTOPLEFT;

		if ((nX > this->width() - mouse_margin) && (nX < this->width())
			&& (nY > 0) && (nY < mouse_margin))
			*result = HTTOPRIGHT;

		if ((nX > 0) && (nX < mouse_margin)
			&& (nY > this->height() - mouse_margin) && (nY < this->height()))
			*result = HTBOTTOMLEFT;

		if ((nX > this->width() - mouse_margin) && (nX < this->width())
			&& (nY > this->height() - mouse_margin) && (nY < this->height()))
			*result = HTBOTTOMRIGHT;

		return true;
	}
	}

	return QWidget::nativeEvent(eventType, message, result);
}

