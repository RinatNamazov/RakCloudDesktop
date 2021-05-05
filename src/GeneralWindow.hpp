/*****************************************************************************
 *
 *  PROJECT:            RakCloudDesktop
 *  LICENSE:            See LICENSE in the top level directory
 *  FILE:               GeneralWindow.hpp
 *  DESCRIPTION:        General window
 *  COPYRIGHT:          (c) 2021 RINWARES <rinwares.com>
 *  AUTHOR:             Rinat Namazov <rinat.namazov@rinwares.com>
 *
 *****************************************************************************/

#pragma once

#include <QMap>
#include <QWidget>

#include "RakCloudManagerApi.hpp"

QT_BEGIN_NAMESPACE
class QLabel;
class QListWidgetItem;
class QPlainTextEdit;

namespace Ui {
    class GeneralWindow;
}
QT_END_NAMESPACE

class RakCloudManagerApi;

class GeneralWindow : public QWidget {
    Q_OBJECT

public:
    explicit GeneralWindow(RakCloudManagerApi* api, QWidget* parent = nullptr);
    ~GeneralWindow();

    quint32 getCurrentSelectedBot() const;

protected:
    void changeEvent(QEvent* event);

private slots:
    void apiReceiveMessage(const rcresponse::ResponseMessageWrapper& msg);

    void on_botList_currentItemChanged(QListWidgetItem* item, QListWidgetItem*);
    void on_botCmdInput_returnPressed();
    void on_botCmdEnterButton_clicked();
    void on_botReloadScriptsButton_clicked();
    void on_managerCmdInput_returnPressed();
    void on_managerCmdEnterButton_clicked();
    void on_managerReloadScriptsButton_clicked();

private:
    Ui::GeneralWindow* ui;
    RakCloudManagerApi*    m_managerApi;
    QPlainTextEdit*        m_dummyBotLogOutput;

    QPlainTextEdit* getLogOutputWidgetForBotId(quint32 botId);

    void onCreateBot(quint32 botId, QString botName, bool botStarted, bool changeCount = true);
    void onStartBot(quint32 botId);
    void onStopBot(quint32 botId);
    void onRemoveBot(quint32 botId);
};
