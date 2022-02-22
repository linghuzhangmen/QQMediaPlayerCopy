#include "CommonUtils.h"
#include <QSettings>


CommonUtils::CommonUtils()
{
}

CommonUtils::~CommonUtils()
{
}

void CommonUtils::writeIni(QString inipath, QString keystr, QString value)
{
	QSettings *configIniWrite = new QSettings(inipath, QSettings::IniFormat);
	configIniWrite->setValue(keystr, value);

	//写入完成后删除指针
	delete configIniWrite;
}

QString CommonUtils::readIni(QString inipath, QString keystr)
{
	QSettings *configIniRead = new QSettings(inipath, QSettings::IniFormat);
	QString ipResult = configIniRead->value(keystr).toString();

	//读入入完成后删除指针
	delete configIniRead;

	return ipResult;
}