/*****************************************************************************
 *
 *  PROJECT:            RakCloudDesktop
 *  LICENSE:            See LICENSE in the top level directory
 *  FILE:               MainWindow.cpp
 *  DESCRIPTION:        Main window
 *  COPYRIGHT:          (c) 2021 RINWARES <rinwares.com>
 *  AUTHOR:             Rinat Namazov <rinat.namazov@rinwares.com>
 *
 *****************************************************************************/

#include "MainWindow.hpp"

#include <QDesktopServices>
#include <QDir>
#include <QLabel>
#include <QMessageBox>
#include <QScreen>
#include <QSettings>
#include <QStackedWidget>
#include <QUrl>

#include "AuthWindow.hpp"
#include "GeneralWindow.hpp"
#include "RakCloudManagerApi.hpp"
#include "RinwaresRootCa.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_generalWindow(nullptr), m_managerApi(nullptr) {
    QCoreApplication::setOrganizationName("RINWARES");
    QCoreApplication::setOrganizationDomain("rinwares.com");
    QCoreApplication::setApplicationName("RakCloud");
    QCoreApplication::setApplicationVersion("v.0.1.4");

    addRinwaresRootCaToDefaultConfiguration();

    ui->setupUi(this);

    createLanguageMenu();

    setWindowTitle("RakCloud");

    // The status on the right side of the window shows the status of the connection to the bot
    // server.
    m_rightStatus = new QLabel();
    m_rightStatus->setContentsMargins(0, 0, 5, 3);
    ui->statusBar->addPermanentWidget(m_rightStatus);

    m_settings = new QSettings();

    m_authWindow = new AuthWindow(this);
    m_authWindow->setKeyFile(m_settings->value("key_file", "").toString());

    connect(m_authWindow, &AuthWindow::showMessage, this, &MainWindow::showRightStatusMessage);
    connect(m_authWindow, &AuthWindow::changeKeyFile, this, &MainWindow::changeKeyFile);
    connect(m_authWindow, &AuthWindow::onValidKey, this, &MainWindow::onValidKey);

    ui->stackedWidget->addWidget(m_authWindow);
    ui->stackedWidget->setCurrentWidget(m_authWindow);

    statusBar()->showMessage(tr("Authorization..."));
}

MainWindow::~MainWindow() {
    delete ui;
    if (m_authWindow != nullptr) {
        delete m_authWindow;
    }
    if (m_generalWindow != nullptr) {
        delete m_generalWindow;
    }
    if (m_managerApi != nullptr) {
        delete m_managerApi;
    }
    delete m_settings;
}

void MainWindow::changeKeyFile(const QString& filePath) {
    m_settings->setValue("key_file", filePath);
}

void MainWindow::showLeftStatusMessage(const QString& message) {
    statusBar()->showMessage(message);
}

void MainWindow::showRightStatusMessage(const QString& message) { m_rightStatus->setText(message); }

void MainWindow::onValidKey(const QUrl& url, const QString& token) {
    showLeftStatusMessage(tr("Connecting..."));

    m_managerApi = new RakCloudManagerApi(url, token, this);

    connect(m_managerApi, &RakCloudManagerApi::connected, this, &MainWindow::apiConnected);
    connect(m_managerApi, &RakCloudManagerApi::disconnected, this, &MainWindow::apiDisconnected);

    m_managerApi->openConnect();
}

void MainWindow::changeEvent(QEvent* event) {
    if (event != nullptr) {
        switch (event->type()) {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
            case QEvent::LocaleChange:
                loadLanguage(QLocale::system().name());
                break;
            default:
                break;
        }
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::closeEvent(QCloseEvent*) {
    // Save the current position and size of the window.
    m_settings->setValue("size", size());
    m_settings->setValue("pos", pos());
}

void MainWindow::slotLanguageChanged(QAction* action) {
    if (action != nullptr) {
        loadLanguage(action->data().toString());
    }
}

void MainWindow::apiConnected() {
    showRightStatusMessage("<font color=green>" + tr("Connected") + "</font>");

    m_generalWindow = new GeneralWindow(m_managerApi, this);
    ui->stackedWidget->addWidget(m_generalWindow);
    ui->stackedWidget->setCurrentWidget(m_generalWindow);

    // Setting the size and position of the window.
    resize(m_settings->value("size", QSize(700, 500)).toSize());

    QList<QScreen*> screens   = QGuiApplication::screens();
    QRect           screenGeo = screens.first()->availableGeometry();

    QPoint pos = m_settings->value("pos").toPoint();
    if (pos.isNull()) {
        pos = screenGeo.center() - rect().center();
    } else {
        // Is the window off the screen?
        if (pos.x() > screenGeo.x() || pos.y() > screenGeo.y()) {
            resize(700, 500);
            pos = screenGeo.center() - rect().center();
        }
    }
    move(pos);

    // After connecting to the manager of bot server,
    // you need to request information and display them.
    m_managerApi->sendGetSystemInfoRequest();
    m_managerApi->sendGetBotsListRequest();
    // m_managerApi->sendGetScriptsList();
    m_managerApi->subscribeForAllEvents();

    showLeftStatusMessage(tr("Ready"));
}

void MainWindow::apiDisconnected() {
    showRightStatusMessage("<font color=gray>" + tr("Disconnected") + "</font>");
    showLeftStatusMessage("");
    // Todo: There are still old elements in the client window, maybe you need to clear everything
    // or change the window?
}

void MainWindow::on_actionSettings_triggered() {
    // Todo: Display a new window with settings.
    // 1. Possibility of deauthentication.
    // 2. Damn, what else?
    // Maybe rename the item in the menu to "Sign out", if you don't think of more functions?
}

void MainWindow::on_actionCheckForUpdates_triggered() {
    // Todo: Send a request to the server to check for a client update.
    QDesktopServices::openUrl(QUrl("https://github.com/RinatNamazov/RakCloudDesktop/releases"));
}

void MainWindow::on_actionSite_triggered() {
    QDesktopServices::openUrl(QUrl("https://rakcloud.ru"));
}

void MainWindow::on_actionAbout_triggered() {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("RakCloud About"));
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(QString(R"(<p style='text-align:center'>
<img src=':/icons/rakcloud_logo.png' height='100' align='center'>
<br/><br/>
%1: %2</p><br/>
RakCloudDesktop — %3<br/><br/>
RakCloud — %4<br/><br/>
%5: <a href='https://rakcloud.ru'>rakcloud.ru</a>, <a href='https://rinwares.com'>rinwares.com</a><br/>
%6: <a href='https://wiki.rinwares.com/rakcloud'>wiki.rinwares.com</a><br/>
GitHub: <a href='https://github.com/RinatNamazov/RakCloudDesktop'>github.com/RinatNamazov/RakCloudDesktop</a><br/>
BlastHack: <a href='https://blast.hk/threads/66666'>blast.hk/threads/66666</a><br/><br/>
%7: <a href='https://t.me/Rinat_Namazov'>Rinat Namazov</a><br/><br/>
Copyright © 2021 RINWARES. All rights reserved.)")
                       .arg(tr("Version"), QCoreApplication::applicationVersion(),
                            tr("Official client of RakCloud."),
                            tr("Cloud-based platform for building fully automated bot systems of "
                               "any size in SA-MP and CR-MP."),
                            tr("Site"), tr("Wiki"), tr("Developer")));
    msgBox.exec();
}

void MainWindow::on_actionAboutQt_triggered() { QMessageBox::aboutQt(this); }

void MainWindow::createLanguageMenu() {
    // We create the menu entries dynamically, dependent on the existing translations.
    QActionGroup* langGroup = new QActionGroup(ui->menuLanguage);
    langGroup->setExclusive(true);

    connect(langGroup, &QActionGroup::triggered, this, &MainWindow::slotLanguageChanged);

    m_langPath = QApplication::applicationDirPath();
    m_langPath.append("/languages");
    QStringList fileNames = QDir(m_langPath).entryList(QStringList("*.qm"));

    QString systemLocale = QLocale::system().name();

    int langCount = fileNames.size();
    if (langCount == 0) {
        ui->menuLanguage->deleteLater();
    } else {
        bool     systemLocaleExsits   = false;
        QAction* defaultEnglishAction = nullptr;
        for (int i = 0; i < fileNames.size(); ++i) {
            // Get locale extracted by filename.
            QString locale = fileNames[i];
            locale.truncate(locale.lastIndexOf('.'));  // Remove file extension.

            QString lang = QLocale::languageToString(QLocale(locale).language());

            QIcon ico(QString("%1/icons/%2.svg").arg(m_langPath, locale));

            QAction* action = new QAction(ico, lang, this);
            action->setCheckable(true);
            action->setData(locale);

            ui->menuLanguage->addAction(action);
            langGroup->addAction(action);

            // Set default translators and language checked.
            if (locale == systemLocale) {
                systemLocaleExsits = true;
                action->setChecked(true);
            } else if (locale == "en_US") {
                defaultEnglishAction = action;
            }
        }

        if (systemLocaleExsits) {
            // Load the translation the same as for the system.
            loadLanguage(systemLocale);
        } else if (defaultEnglishAction != nullptr) {
            // Set English by default.
            defaultEnglishAction->setChecked(true);
        }
    }
}

void MainWindow::switchTranslator(QTranslator& translator, const QString& filePath) {
    // Remove the old translator.
    QCoreApplication::removeTranslator(&translator);

    // Load the new translator.
    if (translator.load(filePath)) {
        QCoreApplication::installTranslator(&translator);
    }
}

void MainWindow::loadLanguage(const QString& newLanguage) {
    if (m_currLang != newLanguage) {
        m_currLang     = newLanguage;
        QLocale locale = QLocale(newLanguage);
        QLocale::setDefault(locale);
        QString languageName = QLocale::languageToString(locale.language());

        // Path to application translation files.
        QString langFilePath = QString("%1/%2.qm").arg(m_langPath, newLanguage);
        switchTranslator(m_translator, langFilePath);

        // Path to Qt translation files.
        QString qtLangFilePath
            = QString("%1/../translations/qt_%2.qm")
                  .arg(m_langPath, newLanguage.mid(0, newLanguage.lastIndexOf('_')));
        switchTranslator(m_translatorQt, qtLangFilePath);

        statusBar()->showMessage(tr("Current language changed to %1").arg(languageName));
    }
}
