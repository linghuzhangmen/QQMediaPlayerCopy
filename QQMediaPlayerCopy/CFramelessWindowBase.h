/*

QMainWindow无边框窗口基类

可拉伸

其它QMainWindow窗口派生于该类即可

*/

#pragma once
#include <QMainWindow>

class CFramelessWindowBase : public QMainWindow
{
public:
	CFramelessWindowBase(QWidget* parent = nullptr);
	~CFramelessWindowBase();

protected:
	bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;

private:
	int mouse_margin = 5;
};

