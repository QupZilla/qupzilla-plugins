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

#include "mailhandle_schemehandler.h"
#include "mainapplication.h"
#include "networkmanager.h"

#include <QNetworkRequest>
#include <QTimer>
#include <QSettings>
#if QT_VERSION >= 0x050000
#include <QUrlQuery>
#endif

MailHandle_SchemeHandler::MailHandle_SchemeHandler(const QString &settingsPath)
    : m_settingsFile(settingsPath + QL1S("/extensions.ini"))
{
    loadSettings();
}

QString MailHandle_SchemeHandler::settingsFile()
{
    return m_settingsFile;
}

void MailHandle_SchemeHandler::loadSettings()
{
    QSettings settings(m_settingsFile, QSettings::IniFormat);

    settings.beginGroup("MailHandle");
    m_wservice = settings.value("webservice", 0).toInt();
    m_wspath = settings.value("webservicepath", QString()).toString();
    settings.endGroup();
}

void MailHandle_SchemeHandler::addQueryItem(QUrl &url, const QString &key, const QString &value)
{
#if QT_VERSION >= 0x050000
    QUrlQuery query(url);
    query.addQueryItem(key, value);
    url.setQuery(query);
#else
    url.addQueryItem(key, value);
#endif
}

QList<MailHandle_SchemeHandler::QueryItem> MailHandle_SchemeHandler::getQueryItems(const QUrl &url)
{
#if QT_VERSION >= 0x050000
    QUrlQuery query(url);
    return query.queryItems();
#else
    return url.queryItems();
#endif
}

QNetworkReply* MailHandle_SchemeHandler::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice* outgoingData)
{
    Q_UNUSED(op)
    Q_UNUSED(outgoingData)

    QUrl mailto = request.url();
//     Someone please fix this ugliness :D
    switch (m_wservice) {
    case 0: {
        QUrl mlink("https://mail.google.com/mail/?view=cm&fs=1&tf=1&source=mailto");

        addQueryItem(mlink, "to", mailto.toEncoded(QUrl::RemoveQuery | QUrl::RemoveScheme));

        foreach (QueryItem item, getQueryItems(mailto)) {
            if (item.first == "subject") {
                addQueryItem(mlink, "su", item.second);
            }
            else {
                addQueryItem(mlink, item.first, item.second);
            }
        }
        MailHandle_Reply* reply = new MailHandle_Reply(request);
        reply->setUrl(mlink);
        return reply;
        break;
    }
    case 1: {
        QUrl mlink("http://win.mail.ru/cgi-bin/sentmsg?mailto=");

        addQueryItem(mlink, "To", mailto.toEncoded(QUrl::RemoveQuery | QUrl::RemoveScheme));

        foreach (QueryItem item, getQueryItems(mailto)) {
            if (item.first == "subject") {
                addQueryItem(mlink, "Subject", item.second);
            }
            if (item.first == "cc") {
                addQueryItem(mlink, "CC", item.second);
            }
            if (item.first == "bcc") {
                addQueryItem(mlink, "BCC", item.second);
            }
            if (item.first == "body") {
                addQueryItem(mlink, "Body", item.second);
            }
            else {
                addQueryItem(mlink, item.first, item.second);
            }
        }

        MailHandle_Reply* reply = new MailHandle_Reply(request);
        reply->setUrl(mlink);
        return reply;
        break;
    }
    case 2: {
        QString stringy = "http://mail.yandex.ru/compose?mailto=";
        stringy.append(mailto.toEncoded(QUrl::RemoveScheme));
        QUrl mlink(stringy);
        MailHandle_Reply* reply = new MailHandle_Reply(request);
        reply->setUrl(mlink);
        return reply;
        break;
    }
    case 3: {
        QUrl mlink("https://mail.live.com/default.aspx?rru=compose");

        addQueryItem(mlink, "to", mailto.toEncoded(QUrl::RemoveQuery | QUrl::RemoveScheme));

        foreach (QueryItem item, getQueryItems(mailto)) {
            addQueryItem(mlink, item.first, item.second);
        }

        MailHandle_Reply* reply = new MailHandle_Reply(request);
        reply->setUrl(mlink);
        return reply;
        break;
    }
    case 4: {
        QUrl mlink("http://compose.mail.yahoo.com/?");

        addQueryItem(mlink, "To", mailto.toEncoded(QUrl::RemoveQuery | QUrl::RemoveScheme));

        foreach (QueryItem item, getQueryItems(mailto)) {
            if (item.first == "subject") {
                addQueryItem(mlink, "Subj", item.second);
            }
            if (item.first == "cc") {
                addQueryItem(mlink, "Cc", item.second);
            }
            if (item.first == "bcc") {
                addQueryItem(mlink, "Bcc", item.second);
            }
            if (item.first == "body") {
                addQueryItem(mlink, "Body", item.second);
            }
            else {
                addQueryItem(mlink, item.first, item.second);
            }
        }

        MailHandle_Reply* reply = new MailHandle_Reply(request);
        reply->setUrl(mlink);
        return reply;
        break;
    }
    case 5: {
        QUrl mlink("https://mail.opera.com/action/compose/?");

        addQueryItem(mlink, "to", mailto.toEncoded(QUrl::RemoveQuery | QUrl::RemoveScheme));

        foreach (QueryItem item, getQueryItems(mailto)) {
            addQueryItem(mlink, item.first, item.second);
        }

        MailHandle_Reply* reply = new MailHandle_Reply(request);
        reply->setUrl(mlink);
        return reply;
        break;
    }
    case 6: {
        QUrl mlink("https://fastmail.fm/action/compose/?");

        addQueryItem(mlink, "to", mailto.toEncoded(QUrl::RemoveQuery | QUrl::RemoveScheme));

        foreach (QueryItem item, getQueryItems(mailto)) {
            addQueryItem(mlink, item.first, item.second);
        }

        MailHandle_Reply* reply = new MailHandle_Reply(request);
        reply->setUrl(mlink);
        return reply;
        break;
    }
    case 7: {
        QUrl mlink("https://email.t-online.de/?service=writemail");

        addQueryItem(mlink, "to", mailto.toEncoded(QUrl::RemoveQuery | QUrl::RemoveScheme));

        foreach (QueryItem item, getQueryItems(mailto)) {
            addQueryItem(mlink, item.first, item.second);
        }

        MailHandle_Reply* reply = new MailHandle_Reply(request);
        reply->setUrl(mlink);
        return reply;
        break;
    }
    case 8: {
        QUrl mlink(m_wspath + "/?_task=mail&_action=compose");

        addQueryItem(mlink, "to", mailto.toEncoded(QUrl::RemoveQuery | QUrl::RemoveScheme));

        foreach (QueryItem item, getQueryItems(mailto)) {
            addQueryItem(mlink, item.first, item.second);
        }

        MailHandle_Reply* reply = new MailHandle_Reply(request);
        reply->setUrl(mlink);
        return reply;
        break;
    }

    default:
        break;
    }

    return 0;
}

MailHandle_Reply::MailHandle_Reply(const QNetworkRequest &req, QObject* parent)
    : QNetworkReply(parent)
{
    setOperation(QNetworkAccessManager::GetOperation);
    setError(QNetworkReply::NoError, QLatin1String("No Error"));
    setRequest(req);
    setUrl(req.url());

    QTimer::singleShot(0, this, SLOT(delayedFinish()));
}

void MailHandle_Reply::setUrl(const QUrl &url)
{
    m_url = url;
}

qint64 MailHandle_Reply::readData(char* data, qint64 maxSize)
{
    Q_UNUSED(data)
    Q_UNUSED(maxSize)

    return 0;
}

void MailHandle_Reply::delayedFinish()
{
    // HTTP 301 - Moved Permanently
    setAttribute(QNetworkRequest::HttpStatusCodeAttribute, 301);
    setAttribute(QNetworkRequest::RedirectionTargetAttribute, m_url);

    emit metaDataChanged();
    emit finished();
}
