#include "CFrameLessWidgetBase.h"
#include <qt_windows.h>
#include <windowsx.h>
#include <QWindow>


CFrameLessWidgetBase::CFrameLessWidgetBase(QWidget* parent)
	: QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	setAttribute(Qt::WA_Hover);
}

CFrameLessWidgetBase::~CFrameLessWidgetBase()
{
}

bool CFrameLessWidgetBase::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
	MSG* msg = static_cast<MSG*>(message);

	switch (msg->message)
	{
	case WM_NCHITTEST:
	{
		QPoint globalPos = QCursor::pos();
		int x = globalPos.x();
		int y = globalPos.y();

		//int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();   // bug : windows 在高分屏下，坐标值不正确，用Qt API计算
		//int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();

		int nX = x - this->geometry().x();
		int nY = y - this->geometry().y();

		// 如果鼠标位于内部子控件上，则不进行处理
		if (nX > mouse_margin && nX < width() - mouse_margin &&
			nY > mouse_margin && nY < this->height() - mouse_margin)
		{
			if (childAt(nX, nY) != nullptr)
				return QWidget::nativeEvent(eventType, message, result);
		}

		// 鼠标区域位于窗体边框，进行缩放
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
