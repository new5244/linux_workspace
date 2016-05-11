#ifndef EXDATA_H
#define EXDATA_H

#include <QString>
#include <QDate>

class ExData
{
public:
    ExData();

    QString filename;
    QString data_no;
    QDateTime date;
    int type;
    QString contents;
    bool transferred;

};

#endif // EXDATA_H
