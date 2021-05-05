/*****************************************************************************
 *
 *  PROJECT:            RakCloudDesktop
 *  LICENSE:            See LICENSE in the top level directory
 *  FILE:               AuthWindow.hpp
 *  DESCRIPTION:        Authorization window
 *  COPYRIGHT:          (c) 2021 RINWARES <rinwares.com>
 *  AUTHOR:             Rinat Namazov <rinat.namazov@rinwares.com>
 *
 *****************************************************************************/

#pragma once

#include <QString>
#include <QUrl>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class AuthWindow;
}
QT_END_NAMESPACE

class AuthWindow : public QWidget {
    Q_OBJECT

public:
    explicit AuthWindow(QWidget* parent = nullptr);
    ~AuthWindow();

    void setKeyFile(const QString& filePath);

signals:
    void showMessage(const QString& message);
    void changeKeyFile(const QString& filePath);
    void onValidKey(const QUrl& url, const QString& token);

protected:
    void changeEvent(QEvent* event);

private slots:
    void on_browseButton_clicked();
    void on_loginButton_clicked();

private:
    Ui::AuthWindow* ui;
};
