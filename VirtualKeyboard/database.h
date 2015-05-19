#ifndef DATABASE_H
#define DATABASE_H
#include <QString>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QFile>
#include <QTextStream>

bool connectDatabase(void){
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data.db");
    if(!db.open()) {
//        QMessageBox::information(0,"","SQLITE");
    }
    QSqlQuery query;    
    query.exec("create table pinyin(py varchar(10),chn varchar(10))");
    QFile file("WINPY");
    QString py;
    QString chn;
    QString content;
    QRegExp regExp("([^A-Za-z]*)([*]{0,1}[A-Za-z]*)");
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream text_stream(&file);
        while(!file.atEnd()){
            content = text_stream.readLine();
            if(regExp.indexIn(content)>-1) {
                py = regExp.cap(2);
                chn = regExp.cap(1);
                if(chn.endsWith('*'))
                {
                    chn.chop(1);
                }
                query.exec("insert into pinyin values('"+py+"','"+chn+"')");
            }
        }
    }
    file.close();
    //wubi
    query.exec("create table wubi(wb varchar(10),chn varchar(10))");
    QFile file2("WINWB");
    QString wb;
    QRegExp regExp2("([^A-Za-z]*)([A-Za-z]*)");
    if(file2.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream text_stream2(&file2);
        while(!file2.atEnd()){
            content = text_stream2.readLine();
            if(regExp2.indexIn(content)>-1) {
                wb = regExp2.cap(2);
                chn = regExp2.cap(1);
                query.exec("insert into wubi values('"+wb+"','"+chn+"')");
            }
        }
    }
    file2.close();
    db.close();
    return true;
}

#endif
