/*****************************************************************************
 *
 *  PROJECT:            RakCloudDesktop
 *  LICENSE:            See LICENSE in the top level directory
 *  FILE:               RakCloudManagerApi.hpp
 *  DESCRIPTION:        API for RakCloudManager
 *  COPYRIGHT:          (c) 2021 RINWARES <rinwares.com>
 *  AUTHOR:             Rinat Namazov <rinat.namazov@rinwares.com>
 *
 *****************************************************************************/

#pragma once

#include <QByteArray>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QtWebSockets/QWebSocket>

#include "proto/rcrequest.pb.h"
#include "proto/rcresponse.pb.h"

const quint32 INVALID_BOT_ID = static_cast<quint32>(-1);

class RakCloudManagerApi : public QObject {
    Q_OBJECT

signals:
    void connected();
    void disconnected();
    void recieveMessage(const rcresponse::ResponseMessageWrapper& msg);

public:
    explicit RakCloudManagerApi(const QUrl& url, const QString& token, QObject* parent = nullptr);
    ~RakCloudManagerApi();

    bool isConnected() const noexcept;

    void openConnect();

    void subscribeManagerLogs();
    void subscribeBotLogs();
    void subscribeBotEvents();
    void subscribeForAllEvents();

    void sendGetBotsListRequest();
    void sendGetSystemInfoRequest();

    void sendManagerCommand(const QString& command);
    void sendBotCommand(quint32 botId, const QString& command);
    void sendGetBotInfoRequest(quint32 botId);
    void sendGetScriptsList(bool manager = true, bool bot = true);
    void sendReloadManagerScripts();
    void sendReloadBotScripts(quint32 botId);

private slots:
    void onConnected();
    void onDisconnected();
    void onBinaryMessageReceived(QByteArray message);
    void onSslErrors(const QList<QSslError>& errors);

private:
    QWebSocket m_webSocket;
    QUrl       m_url;
    QString    m_token;
    bool       m_isConnected;

    bool send(rcrequest::RequestMessageWrapper* msg);
};
