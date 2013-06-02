/* ============================================================
* MailHandle plugin for QupZilla
* Copyright (C) 2012-2013  David Rosca <nowrep@gmail.com>
* Copyright (C) 2012-2013  Mladen Pejaković <pejakm@gmail.com>
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

MailHandle_SchemeHandler::MailHandle_SchemeHandler()
{
}


QNetworkReply* MailHandle_SchemeHandler::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice* outgoingData)
{
    Q_UNUSED(op)
    Q_UNUSED(outgoingData)

    QUrl mailto = request.url();
//   if (m_wservice = 0) {
    QUrl mlink("https://mail.google.com/mail/?view=cm&fs=1&tf=1&source=mailto");

    mlink.addQueryItem("to", mailto.toEncoded(QUrl::RemoveQuery | QUrl::RemoveScheme));

    typedef QPair<QString, QString> QueryItem;
    foreach(QueryItem item, mailto.queryItems()) {
        if (item.first == "subject") {
            mlink.addQueryItem("su", item.second);
        }
        else {
            mlink.addQueryItem(item.first, item.second);
        }
    }
//   }

//   if (m_wservice = 1) {
//     QUrl mlink("http://win.mail.ru/cgi-bin/sentmsg?mailto=");
//
//     mlink.addQueryItem(mailto.toEncoded(QUrl::RemoveQuery | QUrl::RemoveScheme));
//   }
//
//   if (m_wservice = 2) {
//     QUrl mlink("http://mail.yandex.ru/compose?mailto=");
//
//     mlink.addQueryItem(mailto.toEncoded(QUrl::RemoveQuery | QUrl::RemoveScheme));
//   }

    // Redirect
    MailHandle_Reply* reply = new MailHandle_Reply(request);
    reply->setUrl(mlink);
    return reply;
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
