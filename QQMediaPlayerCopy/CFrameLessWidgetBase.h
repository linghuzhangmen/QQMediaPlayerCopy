/*

无边框窗口基类

可拉伸

其它窗口派生于该类即可

*/

#pragma once
#include <QWidget>

class CFrameLessWidgetBase : public QWidget
{
public:
	CFrameLessWidgetBase(QWidget* parent = nullptr);
	~CFrameLessWidgetBase();

protected:
	bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;

private:
	int mouse_margin = 5;
};

