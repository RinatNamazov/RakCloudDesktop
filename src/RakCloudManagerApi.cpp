/*****************************************************************************
 *
 *  PROJECT:            RakCloudDesktop
 *  LICENSE:            See LICENSE in the top level directory
 *  FILE:               RakCloudManagerApi.cpp
 *  DESCRIPTION:        API for RakCloudManager
 *  COPYRIGHT:          (c) 2021 RINWARES <rinwares.com>
 *  AUTHOR:             Rinat Namazov <rinat.namazov@rinwares.com>
 *
 *****************************************************************************/

#include "RakCloudManagerApi.hpp"

#include <QByteArray>
#include <QNetworkRequest>

RakCloudManagerApi::RakCloudManagerApi(const QUrl& url, const QString& token, QObject* parent)
    : QObject(parent), m_url(url), m_token(token) {
    connect(&m_webSocket, &QWebSocket::connected, this, &RakCloudManagerApi::connected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &RakCloudManagerApi::disconnected);

    connect(&m_webSocket, &QWebSocket::connected, this, &RakCloudManagerApi::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &RakCloudManagerApi::onDisconnected);
    connect(&m_webSocket, &QWebSocket::sslErrors, this, &RakCloudManagerApi::onSslErrors);
    connect(&m_webSocket, &QWebSocket::binaryMessageReceived, this,
            &RakCloudManagerApi::onBinaryMessageReceived);
    connect(&m_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            [this](QAbstractSocket::SocketError error) {
                qDebug() << error << "\n" << m_webSocket.errorString();
            });
}

RakCloudManagerApi::~RakCloudManagerApi() {
    m_webSocket.close();
    google::protobuf::ShutdownProtobufLibrary();
}

bool RakCloudManagerApi::isConnected() const noexcept { return m_isConnected; }

void RakCloudManagerApi::openConnect() {
    QNetworkRequest nr;
    nr.setUrl(m_url);
    nr.setRawHeader("Authorization", QString("Bearer " + m_token).toUtf8());
    m_webSocket.open(nr);
}

bool RakCloudManagerApi::send(rcrequest::RequestMessageWrapper* msg) {
    std::string str;
    if (!msg->SerializeToString(&str)) {
        return false;
    }
    QByteArray data(str.data(), str.length());
    m_webSocket.sendBinaryMessage(data);
    return true;
}

void RakCloudManagerApi::onConnected() { m_isConnected = true; }

void RakCloudManagerApi::onDisconnected() { m_isConnected = false; }

void RakCloudManagerApi::onBinaryMessageReceived(QByteArray message) {
    rcresponse::ResponseMessageWrapper msg;
    if (msg.ParseFromArray(message, message.size())) {
        emit recieveMessage(msg);
    }
}

void RakCloudManagerApi::onSslErrors(const QList<QSslError>& errors) { qDebug() << errors; }

void RakCloudManagerApi::subscribeManagerLogs() {
    rcrequest::RequestMessageWrapper msg;
    rcrequest::SubscribeProto        sp;
    sp.set_type(rcrequest::SubscribeProto_SubscribeType::SubscribeProto_SubscribeType_SUBSCRIBE);
    sp.add_topics(rcrequest::SubscribeProto_SubscribeTopic_MANAGER_LOGS);
    *msg.mutable_subscribe() = sp;
    send(&msg);
}

void RakCloudManagerApi::subscribeBotLogs() {
    rcrequest::RequestMessageWrapper msg;
    rcrequest::SubscribeProto        sp;
    sp.set_type(rcrequest::SubscribeProto_SubscribeType::SubscribeProto_SubscribeType_SUBSCRIBE);
    sp.add_topics(rcrequest::SubscribeProto_SubscribeTopic_BOT_LOGS);
    *msg.mutable_subscribe() = sp;
    send(&msg);
}

void RakCloudManagerApi::subscribeBotEvents() {
    rcrequest::RequestMessageWrapper msg;
    rcrequest::SubscribeProto        sp;
    sp.set_type(rcrequest::SubscribeProto_SubscribeType::SubscribeProto_SubscribeType_SUBSCRIBE);
    sp.add_topics(rcrequest::SubscribeProto_SubscribeTopic_BOT_EVENTS);
    *msg.mutable_subscribe() = sp;
    send(&msg);
}

void RakCloudManagerApi::subscribeForAllEvents() {
    rcrequest::RequestMessageWrapper msg;
    rcrequest::SubscribeProto        sp;
    sp.set_type(rcrequest::SubscribeProto_SubscribeType::SubscribeProto_SubscribeType_SUBSCRIBE);
    sp.add_topics(rcrequest::SubscribeProto_SubscribeTopic_MANAGER_LOGS);
    sp.add_topics(rcrequest::SubscribeProto_SubscribeTopic_BOT_EVENTS);
    sp.add_topics(rcrequest::SubscribeProto_SubscribeTopic_BOT_LOGS);
    sp.add_topics(rcrequest::SubscribeProto_SubscribeTopic_BOT_INFO);
    *msg.mutable_subscribe() = sp;
    send(&msg);
}

void RakCloudManagerApi::sendGetBotsListRequest() {
    rcrequest::RequestMessageWrapper msg;
    rcrequest::RequestBotListProto   rblp;
    rblp.set_full(false);
    *msg.mutable_request_bot_list() = rblp;
    send(&msg);
}

void RakCloudManagerApi::sendGetSystemInfoRequest() {
    rcrequest::RequestMessageWrapper msg;
    rcrequest::RequestSystemInfoProto rsip;
    *msg.mutable_request_system_info() = rsip;
    send(&msg);
}

void RakCloudManagerApi::sendManagerCommand(const QString& command) {
    rcrequest::RequestMessageWrapper msg;
    rcrequest::ManagerCommandProto   mcp;
    mcp.set_command(command.toStdString());
    *msg.mutable_manager_command() = mcp;
    send(&msg);
}

void RakCloudManagerApi::sendBotCommand(quint32 botId, const QString& command) {
    rcrequest::RequestMessageWrapper msg;
    rcrequest::BotCommandProto       bcp;
    bcp.set_bot_id(botId);
    bcp.set_command(command.toStdString());
    *msg.mutable_bot_command() = bcp;
    send(&msg);
}

void RakCloudManagerApi::sendGetBotInfoRequest(quint32 botId) {
    rcrequest::RequestMessageWrapper msg;
    rcrequest::RequestBotInfoProto   rbip;
    rbip.add_ids(botId);
    rbip.set_samp_info(true);
    *msg.mutable_request_bot_info() = rbip;
    send(&msg);
}

void RakCloudManagerApi::sendGetScriptsList(bool manager, bool bot) {
    rcrequest::RequestMessageWrapper   msg;
    rcrequest::RequestScriptsListProto rslp;
    rslp.set_manager(manager);
    rslp.set_bot(bot);
    *msg.mutable_request_scripts_list() = rslp;
    send(&msg);
}

void RakCloudManagerApi::sendReloadManagerScripts() {
    rcrequest::RequestMessageWrapper msg;
    rcrequest::ReloadScriptsProto    rsp;
    rsp.set_bot_id(-1);
    *msg.mutable_reload_scripts() = rsp;
    send(&msg);
}

void RakCloudManagerApi::sendReloadBotScripts(quint32 botId) {
    rcrequest::RequestMessageWrapper msg;
    rcrequest::ReloadScriptsProto    rsp;
    rsp.set_bot_id(botId);
    *msg.mutable_reload_scripts() = rsp;
    send(&msg);
}
