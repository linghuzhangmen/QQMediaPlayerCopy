/*

�ޱ߿򴰿ڻ���

������

�������������ڸ��༴��

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

