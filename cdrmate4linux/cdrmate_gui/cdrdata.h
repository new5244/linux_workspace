#ifndef CDRDATA_H
#define CDRDATA_H

#include <QDate>

class CDRData
{
public:
    CDRData();

    QString filename;
    QString patient_id;
    QDate date;
    bool exported;
    QString contents;
};

#endif // CDRDATA_H
