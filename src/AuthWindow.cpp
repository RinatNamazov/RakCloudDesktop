/*****************************************************************************
 *
 *  PROJECT:            RakCloudDesktop
 *  LICENSE:            See LICENSE in the top level directory
 *  FILE:               AuthWindow.cpp
 *  DESCRIPTION:        Authorization window
 *  COPYRIGHT:          (c) 2021 RINWARES <rinwares.com>
 *  AUTHOR:             Rinat Namazov <rinat.namazov@rinwares.com>
 *
 *****************************************************************************/

#include "AuthWindow.hpp"

#include <jwt-cpp/jwt.h>

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <string>

#include "ui_AuthWindow.h"

AuthWindow::AuthWindow(QWidget* parent) : QWidget(parent), ui(new Ui::AuthWindow) {
    ui->setupUi(this);
}

AuthWindow::~AuthWindow() { delete ui; }

void AuthWindow::setKeyFile(const QString& filePath) { ui->keyFile->setText(filePath); }

void AuthWindow::changeEvent(QEvent* event) {
    if (event != nullptr) {
        switch (event->type()) {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
            default:
                break;
        }
    }
    QWidget::changeEvent(event);
}

void AuthWindow::on_browseButton_clicked() {
    const QString filePath = QFileDialog::getOpenFileName(
        this, tr("Select key file"), QDir::currentPath(), tr("Key file") + " (*.key)");

    ui->keyFile->setText(filePath);
    emit changeKeyFile(filePath);
}

void AuthWindow::on_loginButton_clicked() {
    static const std::string JWT_PUBLIC_KEY = R"~KEY~(-----BEGIN PUBLIC KEY-----
MIGbMBAGByqGSM49AgEGBSuBBAAjA4GGAAQBC2japn7WxxfAVcMuu9CcqOrUzEJ/
ztQ0FS9K2jwVEPzP22USitgLkjJWRSDkwxIT55q5SfyhiCl0/s5inIPFxKsA/n/w
MDehZlFSrd5YLEpk1h/GOukZERMDEaYujZP34F4OQozowFeCu5O30f0co8EPzFy7
sBwqbnzsYtlw4EN83TI=
-----END PUBLIC KEY-----)~KEY~";

    static auto verifier = jwt::verify().allow_algorithm(jwt::algorithm::es512{JWT_PUBLIC_KEY});

    QString filePath = ui->keyFile->text();

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        emit showMessage("<font color=red>" + file.errorString() + "</font>");
        return;
    }

    emit changeKeyFile(filePath);

    QByteArray  content = file.readAll();
    std::string token(content.constData(), content.length());

    try {
        auto jwt = jwt::decode(token);
        if (jwt.has_expires_at()) {
            verifier.verify(jwt);  // It will throw an exception.

            if (!jwt.has_payload_claim("url") || !jwt.has_payload_claim("level")) {
                emit showMessage("<font color=red>" + tr("Invalid token") + "</font>");
                return;
            }

            QUrl url = QUrl(QString::fromStdString(jwt.get_payload_claim("url").as_string()));

            emit showMessage("<font color=green>" + tr("Valid token") + "</font>");

            emit onValidKey(url, QString::fromStdString(token));
        } else {
            emit showMessage("<font color=red>" + tr("Invalid token") + "</font>");
        }
    } catch (const std::exception& e) {
        emit showMessage("<font color=red>" + QString(e.what()) + "</font>");
    }
}
