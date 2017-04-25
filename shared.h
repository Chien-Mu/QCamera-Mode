#ifndef SHARED_H
#define SHARED_H

#include <QDebug>
#include <QDateTime>
#include <QRect>
#include <QMetaType>
#define SCANTOTAL 2

struct INFO_SN{
    QRect rect;
    QByteArray text;
};

struct INFO{
    INFO_SN SN[SCANTOTAL];
    int total;
};
Q_DECLARE_METATYPE(INFO)

class Shared
{
public:
    Shared();
    QString getDate();
    QString getDateTime();
    QString getDateTime_FILE();
    QString getDateTime_print();
    QString getDateTime_yesterday_FILE();
};

#endif // SHARED_H
