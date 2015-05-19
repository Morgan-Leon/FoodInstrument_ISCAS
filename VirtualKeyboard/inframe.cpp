#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include "inframe.h"
#include "inputwidget.h"

InFrame::InFrame()
{
    inputwidget = new inputWidget(this);
//    inputwidget->setWindowModality(Qt::ApplicationModal);
}

InFrame::~InFrame()
{
    delete(inputwidget);
}
void InFrame::confirmContent(const QString &content)
{
    sendCommitString(content);
}

void InFrame::updateHandler(int type)
{
    switch(type)
    {
       case QWSInputMethod::FocusOut:
                inputwidget->hide();
                break;
        case QWSInputMethod::FocusIn:                
                inputwidget->show();
                break;

        default:
                break;
     }
}
void InFrame::hideInput()
{
    inputwidget->hide();
}
