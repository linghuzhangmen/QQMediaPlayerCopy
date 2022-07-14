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
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
	bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
#else
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif

private:
	int mouse_margin = 5;
};

