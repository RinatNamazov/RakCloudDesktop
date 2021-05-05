/*****************************************************************************
 *
 *  PROJECT:            RakCloudDesktop
 *  LICENSE:            See LICENSE in the top level directory
 *  FILE:               utils.hpp
 *  DESCRIPTION:        Utils
 *  COPYRIGHT:          (c) 2021 RINWARES <rinwares.com>
 *  AUTHOR:             Rinat Namazov <rinat.namazov@rinwares.com>
 *
 *****************************************************************************/

#include "utils.hpp"

Qt::GlobalColor getColorForLog(QString& log) {
    QString logType = log.mid(15, 4);

    if (logType == "trac") {  // trace
        return Qt::GlobalColor::darkGray;
    } else if (logType == "debu") {  // debug
        return Qt::GlobalColor::magenta;
    } else if (logType == "info") {  // info
        return Qt::GlobalColor::blue;
    } else if (logType == "syst") {  // system
        return Qt::GlobalColor::black;
    } else if (logType == "warn") {  // warning
        return Qt::GlobalColor::darkYellow;
    } else if (logType == "erro") {  // error
        return Qt::GlobalColor::red;
    } else if (logType == "fata") {  // fatal
        return Qt::GlobalColor::darkRed;
    } else {  // Lua
        return Qt::GlobalColor::darkGreen;
    }
}

QString formatUptime(qint64 upTime) {
    QString str;
    // int seconds = upTime % 60;
    upTime /= 60;
    // str = QString("%1 sec").arg(seconds);
    if (upTime > 0) {
        int minutes = upTime % 60;
        str         = QString("%1 min").arg(minutes);
        // str         = QString("%1 min, %2").arg(minutes).arg(str);
        upTime /= 60;
        if (upTime > 0) {
            int hours = upTime % 24;
            str       = QString("%1 hours, %2").arg(hours).arg(str);
            upTime /= 24;
            if (upTime > 0) {
                int days = upTime;
                str      = QString("%1 days, %2").arg(days).arg(str);
            }
        }
    }
    return str;
}
