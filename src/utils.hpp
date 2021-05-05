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

#pragma once

#include <QColor>
#include <QString>

// In the future, the logs will be in binary format.
Qt::GlobalColor getColorForLog(QString& log);

QString formatUptime(qint64 upTime);
