/*****************************************************************************
 *
 *  PROJECT:            RakCloudDesktop
 *  LICENSE:            See LICENSE in the top level directory
 *  FILE:               GeneralWindow.cpp
 *  DESCRIPTION:        General window
 *  COPYRIGHT:          (c) 2021 RINWARES <rinwares.com>
 *  AUTHOR:             Rinat Namazov <rinat.namazov@rinwares.com>
 *
 *****************************************************************************/

#include "GeneralWindow.hpp"

#include <QDebug>
#include <QKeyEvent>
#include <QListWidget>
#include <QMessageBox>
#include <QStatusBar>
#include <QTimeZone>

#include "ui_GeneralWindow.h"
#include "utils.hpp"

GeneralWindow::GeneralWindow(RakCloudManagerApi* api, QWidget* parent)
    : QWidget(parent), ui(new Ui::GeneralWindow), m_managerApi(api) {
    ui->setupUi(this);

    connect(m_managerApi, &RakCloudManagerApi::recieveMessage, this,
            &GeneralWindow::apiReceiveMessage);

    m_dummyBotLogOutput = ui->botLogOutput;
    ui->botCmdInput->setEnabled(false);
    ui->botCmdEnterButton->setEnabled(false);
}

GeneralWindow::~GeneralWindow() { delete ui; }

quint32 GeneralWindow::getCurrentSelectedBot() const {
    QListWidgetItem* item = ui->botList->currentItem();
    if (item != nullptr) {
        return item->data(Qt::UserRole).toUInt();
    }
    return -1;
}

void GeneralWindow::changeEvent(QEvent* event) {
    if (event != nullptr) {
        switch (event->type()) {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);

                if (m_managerApi != nullptr && m_managerApi->isConnected()) {
                    // After changing the language, you need to re-render the information.
                    m_managerApi->sendGetSystemInfoRequest();

                    quint32 botId = getCurrentSelectedBot();
                    if (botId != INVALID_BOT_ID) {
                        m_managerApi->sendGetBotInfoRequest(botId);
                    }
                }
                break;
            default:
                break;
        }
    }
    QWidget::changeEvent(event);
}

void GeneralWindow::apiReceiveMessage(const rcresponse::ResponseMessageWrapper& msg) {
    switch (msg.msg_case()) {
        case rcresponse::ResponseMessageWrapper::MsgCase::kSystemInfo: {
            auto systemInfo = msg.system_info();
            ui->managerVersion->setText(systemInfo.version().c_str());
            ui->totalBots->setText(QString::number(systemInfo.total_bots()));
            ui->runningBots->setText(QString::number(systemInfo.running_bots()));
            ui->managerUpTime->setText(formatUptime(systemInfo.uptime()));
            break;
        }
        case rcresponse::ResponseMessageWrapper::MsgCase::kBotList: {
            auto bots = msg.bot_list().bots();
            for (const auto& bot : bots) {
                onCreateBot(bot.id(), QString::fromStdString(bot.name()), bot.started(), false);
            }
            break;
        }
        case rcresponse::ResponseMessageWrapper::MsgCase::kLog: {
            auto    log  = msg.log();
            QString text = QString::fromStdString(log.text());
            quint32 botId = log.bot_id();
            if (botId == INVALID_BOT_ID) {
                QTextCharFormat tf = ui->managerLogOutput->currentCharFormat();
                tf.setForeground(QBrush(getColorForLog(text)));
                ui->managerLogOutput->setCurrentCharFormat(tf);
                ui->managerLogOutput->appendPlainText(text);
            } else {
                QPlainTextEdit* botLogOutput = getLogOutputWidgetForBotId(botId);
                QTextCharFormat tf           = botLogOutput->currentCharFormat();
                tf.setForeground(QBrush(getColorForLog(text)));
                botLogOutput->setCurrentCharFormat(tf);
                botLogOutput->appendPlainText(text);
            }
            break;
        }
        case rcresponse::ResponseMessageWrapper::MsgCase::kBotInfo: {
            auto botInfo = msg.bot_info();

            if (!botInfo.included_samp()) {
                ui->botId->setText(QString::number(botInfo.bot_id()));
                ui->botUpTime->setText(formatUptime(botInfo.uptime()));
            } else {
                auto player = botInfo.local_player();

                ui->sampId->setText(QString::number(player.id()));
                ui->nickName->setText(QString::fromStdString(player.nickname()));
                ui->money->setText(QString::number(player.money()));
                ui->level->setText(QString::number(player.score()));
                ui->health->setText(QString::number(player.health()));
                ui->armour->setText(QString::number(player.armour()));

                // auto pos = player.position();
                // ui->position->setText(
                //    QString("X: %1, Y: %2, Z: %3").arg(pos.x(), pos.y(), pos.z()));

                auto server = botInfo.samp_server();

                ui->players->setText(
                    QString("%1 / %2").arg(server.player_count(), server.max_players()));
                ui->server->setText(QString::fromStdString(server.address()));
            }
            break;
        }
        case rcresponse::ResponseMessageWrapper::MsgCase::kBotEvent: {
            auto    botEvent = msg.bot_event();
            quint32 botId    = botEvent.bot_id();
            switch (botEvent.event_type()) {
                case rcresponse::BotEventProto_EventType::
                    BotEventProto_EventType_EVENT_TYPE_CREATED: {
                    onCreateBot(botId, QString::fromStdString(botEvent.bot_name()), false);
                    break;
                }
                case rcresponse::BotEventProto_EventType::
                    BotEventProto_EventType_EVENT_TYPE_STARTED: {
                    onStartBot(botId);
                    break;
                }
                case rcresponse::BotEventProto_EventType::
                    BotEventProto_EventType_EVENT_TYPE_STOPED: {
                    onStopBot(botId);
                    break;
                }
                case rcresponse::BotEventProto_EventType::
                    BotEventProto_EventType_EVENT_TYPE_REMOVED: {
                    onRemoveBot(botId);
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            qDebug() << "[API] Received unknown message.";
            break;
    }
}

void GeneralWindow::on_botList_currentItemChanged(QListWidgetItem* item, QListWidgetItem*) {
    if (item == nullptr) {
        if (ui->botLogOutput != nullptr) {
            ui->botLogOutput->hide();
        }
        ui->botLogOutput = m_dummyBotLogOutput;
        ui->botLogOutput->show();
    } else {
        quint32 botId = item->data(Qt::UserRole).toUInt();

        ui->botLogOutput->hide();
        ui->botLogOutput = getLogOutputWidgetForBotId(botId);
        ui->botLogOutput->show();

        bool botStarted = ui->botLogOutput->property("bot_started").toBool();
        ui->botCmdInput->setEnabled(botStarted);
        ui->botCmdEnterButton->setEnabled(botStarted);

        m_managerApi->sendGetBotInfoRequest(botId);
    }
}

void GeneralWindow::on_botCmdInput_returnPressed() { on_botCmdEnterButton_clicked(); }

void GeneralWindow::on_botCmdEnterButton_clicked() {
    QString command = ui->botCmdInput->text().trimmed();
    if (!command.isEmpty()) {
        m_managerApi->sendBotCommand(getCurrentSelectedBot(), command);
        ui->botCmdInput->addToHistoryAndResetPos();
    }
}

void GeneralWindow::on_botReloadScriptsButton_clicked() {
    quint32 botId = getCurrentSelectedBot();
    if (botId != INVALID_BOT_ID) {
        m_managerApi->sendReloadBotScripts(botId);
    }
}

void GeneralWindow::on_managerCmdInput_returnPressed() { on_managerCmdEnterButton_clicked(); }

void GeneralWindow::on_managerCmdEnterButton_clicked() {
    QString command = ui->managerCmdInput->text().trimmed();
    if (!command.isEmpty()) {
        m_managerApi->sendManagerCommand(command);
        ui->managerCmdInput->addToHistoryAndResetPos();
    }
}

void GeneralWindow::on_managerReloadScriptsButton_clicked() {
    m_managerApi->sendReloadManagerScripts();
}

QPlainTextEdit* GeneralWindow::getLogOutputWidgetForBotId(quint32 botId) {
    for (int row = 0; row < ui->botLogAndInputVL->count(); ++row) {
        QWidget* item      = ui->botLogAndInputVL->itemAt(row)->widget();
        quint32  itemBotId = item->property("bot_id").toUInt();
        if (itemBotId == botId) {
            return static_cast<QPlainTextEdit*>(item);
            break;
        }
    }
    return nullptr;
}

void GeneralWindow::onCreateBot(quint32 botId, QString botName, bool botStarted, bool changeCount) {
    QString botLabel;

    if (botName.isEmpty()) {
        botLabel = QString("Bot %1").arg(botId);
    } else {
        botLabel = QString("Bot %1 - %2").arg(botId).arg(botName);
    }

    QListWidgetItem* newItem = new QListWidgetItem;
    newItem->setData(Qt::UserRole, botId);
    newItem->setText(botLabel);
    ui->botList->insertItem(botId, newItem);

    QPlainTextEdit* botLogWidget = new QPlainTextEdit(ui->bots);
    botLogWidget->setObjectName(QString::fromUtf8("botLogOutput_%1").arg(botId));
    botLogWidget->setReadOnly(true);
    botLogWidget->setProperty("bot_id", botId);
    botLogWidget->setProperty("bot_started", botStarted);
    botLogWidget->hide();
    ui->botLogAndInputVL->insertWidget(0, botLogWidget);

    if (changeCount) {
        quint32 totalBotsCount = ui->totalBots->text().toUInt() + 1;
        ui->totalBots->setText(QString::number(totalBotsCount));
    }
}

void GeneralWindow::onStartBot(quint32 botId) {
    quint32 runningBotsCount = ui->runningBots->text().toUInt() + 1;
    ui->runningBots->setText(QString::number(runningBotsCount));

    for (int row = 0; row < ui->botLogAndInputVL->count(); ++row) {
        QLayoutItem* item      = ui->botLogAndInputVL->itemAt(row);
        QWidget*     widget    = item->widget();
        quint32      itemBotId = widget->property("bot_id").toUInt();
        if (itemBotId == botId) {
            widget->setProperty("bot_started", true);
            if (widget == ui->botLogOutput) {
                ui->botCmdInput->setEnabled(true);
                ui->botCmdEnterButton->setEnabled(true);
            }
            break;
        }
    }
}

void GeneralWindow::onStopBot(quint32 botId) {
    quint32 runningBotsCount = ui->runningBots->text().toUInt() - 1;
    ui->runningBots->setText(QString::number(runningBotsCount));

    for (int row = 0; row < ui->botLogAndInputVL->count(); ++row) {
        QLayoutItem* item      = ui->botLogAndInputVL->itemAt(row);
        QWidget*     widget    = item->widget();
        quint32      itemBotId = widget->property("bot_id").toUInt();
        if (itemBotId == botId) {
            widget->setProperty("bot_started", false);
            if (widget == ui->botLogOutput) {
                ui->botCmdInput->setEnabled(false);
                ui->botCmdEnterButton->setEnabled(false);
            }
            break;
        }
    }
}

void GeneralWindow::onRemoveBot(quint32 botId) {
    for (int row = 0; row < ui->botList->count(); ++row) {
        QListWidgetItem* item      = ui->botList->item(row);
        quint32          itemBotId = item->data(Qt::UserRole).toUInt();
        if (itemBotId == botId) {
            ui->botList->removeItemWidget(item);
            delete item;
            break;
        }
    }

    quint32 totalBotsCount = ui->totalBots->text().toUInt() - 1;
    ui->totalBots->setText(QString::number(totalBotsCount));

    for (int row = 0; row < ui->botLogAndInputVL->count(); ++row) {
        QLayoutItem*    item      = ui->botLogAndInputVL->itemAt(row);
        QPlainTextEdit* widget    = dynamic_cast<QPlainTextEdit*>(item->widget());
        quint32      itemBotId = widget->property("bot_id").toUInt();
        if (itemBotId == botId) {
            if (widget == ui->botLogOutput || totalBotsCount == 0) {
                // If the bot currently selected is removing or all bots destroyed.
                on_botList_currentItemChanged(nullptr, nullptr);
            }
            ui->botLogAndInputVL->removeItem(item);
            delete widget;
            delete item;
            break;
        }
    }
}
