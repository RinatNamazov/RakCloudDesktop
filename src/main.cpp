/*****************************************************************************
 *
 *  PROJECT:            RakCloudDesktop
 *  LICENSE:            See LICENSE in the top level directory
 *  FILE:               main.cpp
 *  DESCRIPTION:        Main
 *  COPYRIGHT:          (c) 2021 RINWARES <rinwares.com>
 *  AUTHOR:             Rinat Namazov <rinat.namazov@rinwares.com>
 *
 *****************************************************************************/

#include <QApplication>
#include <QDebug>

#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mw;
    mw.show();

    return app.exec();
}
