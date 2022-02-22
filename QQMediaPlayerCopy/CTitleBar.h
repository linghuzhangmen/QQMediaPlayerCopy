#pragma once

#include <QWidget>
#include "ui_CTitleBar.h"

class CTitleBar : public QWidget
{
	Q_OBJECT

public:
	CTitleBar(QWidget *parent = Q_NULLPTR);
	~CTitleBar();

	int getHeight();

private:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event);

signals:
	void sig_close();

private slots:
	void on_Clicked();

private:
	Ui::CTitleBar ui;
};
