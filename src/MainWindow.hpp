/*****************************************************************************
 *
 *  PROJECT:            RakCloudDesktop
 *  LICENSE:            See LICENSE in the top level directory
 *  FILE:               MainWindow.hpp
 *  DESCRIPTION:        Main window
 *  COPYRIGHT:          (c) 2021 RINWARES <rinwares.com>
 *  AUTHOR:             Rinat Namazov <rinat.namazov@rinwares.com>
 *
 *****************************************************************************/

#pragma once

#include <QMainWindow>
#include <QString>
#include <QTranslator>
#include <QUrl>

QT_BEGIN_NAMESPACE
class QSettings;
class QCloseEvent;
class QLabel;
class QStackedWidget;

namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class AuthWindow;
class GeneralWindow;
class RakCloudManagerApi;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void changeKeyFile(const QString& filePath);
    void showLeftStatusMessage(const QString& message);
    void showRightStatusMessage(const QString& message);
    void onValidKey(const QUrl& url, const QString& token);

protected:
    void changeEvent(QEvent* event);

private slots:
    void closeEvent(QCloseEvent* event);

    void slotLanguageChanged(QAction* action);

    void apiConnected();
    void apiDisconnected();

    void on_actionSettings_triggered();
    void on_actionCheckForUpdates_triggered();
    void on_actionSite_triggered();
    void on_actionAbout_triggered();
    void on_actionAboutQt_triggered();

private:
    Ui::MainWindow* ui;
    AuthWindow*     m_authWindow;
    GeneralWindow*  m_generalWindow;
    QSettings*      m_settings;
    QLabel*         m_rightStatus;
    RakCloudManagerApi* m_managerApi;
    QTranslator         m_translator;
    QTranslator         m_translatorQt;
    QString             m_currLang;
    QString             m_langPath;

    void createLanguageMenu();
    void switchTranslator(QTranslator& translator, const QString& filePath);
    void loadLanguage(const QString& newLanguage);
};
