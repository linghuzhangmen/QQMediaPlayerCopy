/*

Qt libvlc 视频播放器器项目

开发环境  ： win10/win11 vs2022 Qt6.5 x64
VS Build :  x64


作者：令狐掌门
博客链接：https://blog.csdn.net/yao_hou/category_9275800.html

*/


#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <string>
#include <QTranslator>
#include <windows.h>
#include <QMessageBox>
#include "CDefer.h"

using namespace std;

int main(int argc, char *argv[])
{
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        QMessageBox::critical(NULL, "Error", "CoInitialize Failed");
        return -1;
    }

    DEFER([]() { 
        CoUninitialize();
    });

#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)) && defined(_WIN32)
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif

    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/resources/logo.png"));

    QTranslator* trans = new QTranslator();

    //注意路径的反斜线，翻译文件加载错误将不会翻译
    QString qm_path = a.applicationDirPath() + "/translations/Translation_zh_Hans.qm";
    trans->load(qm_path);
    a.installTranslator(trans);

    CMainWindow w;
    w.show();
    return a.exec();
}
