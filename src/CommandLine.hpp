/*****************************************************************************
 *
 *  PROJECT:            RakCloudDesktop
 *  LICENSE:            See LICENSE in the top level directory
 *  FILE:               CommandLine.hpp
 *  DESCRIPTION:        Command line widget
 *  COPYRIGHT:          (c) 2021 RINWARES <rinwares.com>
 *  AUTHOR:             Rinat Namazov <rinat.namazov@rinwares.com>
 *
 *****************************************************************************/

#pragma once

#include <QLineEdit>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QKeyEvent;
QT_END_NAMESPACE

class CommandLine : public QLineEdit {
    Q_OBJECT
public:
    CommandLine(QWidget* parent = nullptr, int historyMaxSize = 10);

    void setHistoryMaxSize(int historyMaxSize);
    void resetCurrentHistoryPos();
    void setCurrentHistoryPos(int historyPos);
    void addToHistory();
    void addToHistoryAndResetPos();
    void clearHistory();
    void pushHistory(QString historyText);
    void popHistory();

protected:
    void keyPressEvent(QKeyEvent* event);

private:
    int         m_historyMaxSize;
    QStringList m_history;
    QString     m_lastText;
    int         m_historyCurPos;  // Draft: -1, Last: 0, Old: max_size-1...
};
