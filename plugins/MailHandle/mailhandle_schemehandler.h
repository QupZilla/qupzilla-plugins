/* ============================================================
* MailHandle plugin for QupZilla
* Copyright (C) 2012-2014  David Rosca <nowrep@gmail.com>
* Copyright (C) 2012-2014  Mladen Pejaković <pejakm@autistici.org>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */

#ifndef MAILHANDLE_SCHEMEHANDLER_H
#define MAILHANDLE_SCHEMEHANDLER_H

#include <QNetworkReply>
#include <QObject>

#include "schemehandlers/schemehandler.h"

class MailHandle_SchemeHandler : public SchemeHandler
{
public:
    MailHandle_SchemeHandler(const QString &settingsPath);

    QString settingsFile();
    void loadSettings();

    QNetworkReply* createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice* outgoingData);

private:
    typedef QPair<QString, QString> QueryItem;
    void addQueryItem(QUrl &url, const QString &key, const QString &value);
    QList<QueryItem> getQueryItems(const QUrl &url);

    int m_wservice;
    QString m_wspath;
    QString m_settingsFile;
};

class MailHandle_Reply : public QNetworkReply
{
    Q_OBJECT
public:
    explicit MailHandle_Reply(const QNetworkRequest &req, QObject* parent = 0);

    void setUrl(const QUrl &url);
    qint64 bytesAvailable() const { return 0; }

protected:
    qint64 readData(char* data, qint64 maxSize);
    void abort() { }

private slots:
    void delayedFinish();

private:
    QUrl m_url;
};

#endif // MAILHANDLE_SCHEMEHANDLER_H
