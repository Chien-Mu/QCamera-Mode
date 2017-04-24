#include "shared.h"

Shared::Shared(){}

QString Shared::getDate(){
    return QDate::currentDate().toString("yyyyMMdd");
}

QString Shared::getDateTime(){
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}

QString Shared::getDateTime_FILE(){
    return QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss_");
}

QString Shared::getDateTime_print(){
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss\t");
}

QString Shared::getDateTime_yesterday_FILE(){
    QDateTime time;
    QString BeforeDaystr = time.currentDateTime().addDays(-1).toString("yyyyMMdd");//昨天日期
    QString datetime = time.currentDateTime().toString("_yyyyMMdd-hhmmss_");
    return QString(BeforeDaystr + datetime);
}
