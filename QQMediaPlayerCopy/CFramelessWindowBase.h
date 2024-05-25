/*

QMainWindow�ޱ߿򴰿ڻ���

������

����QMainWindow���������ڸ��༴��

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

