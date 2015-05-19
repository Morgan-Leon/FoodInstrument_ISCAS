#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include <QWidget>
#include <QSignalMapper>
#include <QKeyEvent>
#include <QToolButton>
#include <QTextEdit>
#include <QPushButton>
#include <QTextCodec>
#include <QtSql>
#include "ui_inputwidget.h"

class QWSInputMethod;
enum DisplaychnNUM{DISPLAYNUM=5};

namespace Ui {
    class inputWidget;
}

class inputWidget : public QWidget,public Ui::inputWidget
{
    Q_OBJECT

public:
    inputWidget(QWSInputMethod *im);
    ~inputWidget();

private:
    bool isCapsLock;
    bool isSign;
    bool isSign2;
    bool isEnglish;
    bool isPinyin;

    int displayNumBegin;
    int displayNumEnd;
    QString pinyinStr;
    QString wubinStr;
    QString chineseStr;

    QSignalMapper* mapper;
    QList<QPushButton *> allButtons;
    bool checkNotTextKey(int);
    bool checkNUMKey(int);
    void changeTextCaps(bool);
    void changeSign(bool);
    QSqlDatabase db;
    QSqlTableModel *model;

    void changeDisplay(bool);

signals:
    void sendString(const QString);
    void sendHide();
private slots:
    void on_lineEditInput_textChanged(QString );
    void on_pwBtn_toggled(bool checked);
    void on_ecBtn_toggled(bool checked);
    void on_sign2Btn_toggled(bool checked);
    void on_signBtn_toggled(bool checked);
    void on_capslockBtn_toggled(bool checked);
    void btn_clicked(int);

private:
    QPoint windowPos;
    QPoint dPos;
    QPoint mousePos;
private slots:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent (QMouseEvent *);

};

#endif // INPUTWIDGET_H
