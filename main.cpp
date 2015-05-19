#include <QtGui/QApplication>
#include <QTextCodec>
#include "./main_dlg/main_dlg.h"

#include "VirtualKeyboard/inframe.h"
#include <QWSServer>
#include <QPalette>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gb18030"));  //gb18030
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
    QWSInputMethod* im = new InFrame;
    QWSServer::setCurrentInputMethod(im);
    QWSServer::setCursorVisible(false);
    QBrush brush(QColor(0,0,0));
    QWSServer::setBackground(brush);

    Main_Dlg w;
    w.show();

    return a.exec();
}

