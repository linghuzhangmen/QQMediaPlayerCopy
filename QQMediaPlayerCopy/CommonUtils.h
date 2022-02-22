#pragma once
#include<QString>

class CommonUtils
{
public:
	CommonUtils();
	~CommonUtils();

	static void writeIni(QString inipath, QString keystr, QString value);
	static QString readIni(QString inipath, QString keystr);
};

