#ifndef INFRAME_H
#define INFRAME_H
#include <QWSInputMethod>
#include <QtSql>


class inputWidget;
class InFrame:public QWSInputMethod
{
Q_OBJECT
public:
        InFrame();
        ~InFrame();
        void setMicroFocus(int, int);
        void updateHandler(int);

private:
        inputWidget* inputwidget;
private slots:
        void confirmContent(const QString &);
        void hideInput();
};
#endif // INFRAME_H
