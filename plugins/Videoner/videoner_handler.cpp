/* ============================================================
* Videoner plugin for QupZilla
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
#include "videoner_handler.h"
#include "webview.h"
#include "webhittestresult.h"

#include <QApplication>
#include <QSettings>
#include <QLabel>
#include <QToolTip>

Videoner_Handler::Videoner_Handler(const QString &sPath, QObject* parent)
    : QObject(parent)
    , m_settingsFile(sPath + QL1S("/extensions.ini"))
{
    loadSettings();
}

QString Videoner_Handler::settingsFile()
{
    return m_settingsFile;
}

void Videoner_Handler::loadSettings()
{
    QSettings settings(m_settingsFile, QSettings::IniFormat);

    settings.beginGroup("Videoner");
    m_extExe = settings.value("videonerExtApp", QString()).toString();
    m_extArgs = settings.value("videonerExtArgs", QString()).toString();
    m_extExeYt = settings.value("videonerExtAppYt", QString()).toString();
    m_extArgsYt = settings.value("videonerExtArgsYt", QString()).toString();
    m_extExeMed = settings.value("videonerExtAppMediaEl", QString()).toString();
    m_extArgsMed = settings.value("videonerExtArgsMediaEl", QString()).toString();
    m_pageyt = settings.value("enableYouTube", true).toBool();
    m_sepyth = settings.value("enableSeparateYt", false).toBool();
    m_pagevm = settings.value("enableVimeo", false).toBool();
    m_pagell = settings.value("enableLiveLeak", false).toBool();
    m_pagemc = settings.value("enableMetaCafe", false).toBool();
    m_pagedm = settings.value("enableDailyMotion", false).toBool();
    m_pagefa = settings.value("enableFacebook", false).toBool();
    m_pagebr = settings.value("enableBreak", false).toBool();
    m_pagehu = settings.value("enableHulu", false).toBool();
    m_medel = settings.value("enableMediaEl", true).toBool();
    m_sepmedel = settings.value("enableSepMediaEl", false).toBool();
    settings.endGroup();
}

void Videoner_Handler::populateWebViewMenu(QMenu* menu, WebView* view, const WebHitTestResult &r)
{
    m_view = view;
    if (m_pageyt) {
        QRegExp rx1("youtube.com/watch\\?.*v=([^&]+)|youtu.be/([^&]+)|y2u.be/([^&]+)|youtube.com/embed/([^&]+)");
        QString videoId1;

        rx1.indexIn(r.linkUrl().toString());
        for (int i = 1; i < 4; ++i) {
            if (!rx1.cap(i).isEmpty()) {
                videoId1 = rx1.cap(i);
                break;
            }
        }
        if (videoId1.isEmpty()) {
            rx1.indexIn(view->url().toString());
            for (int i = 1; i < 4; ++i) {
                if (!rx1.cap(i).isEmpty()) {
                    videoId1 = rx1.cap(i);
                    break;
                }
            }
        }
        if (!videoId1.isEmpty()) {
            QString videoPage1;
            videoPage1 = "https://www.youtube.com/watch?v=" + videoId1;
            menu->addAction(QIcon(":/videoner/data/videoner.png"), tr("Videonize!"), this, (m_sepyth ? SLOT(startExternalHandlerYt()) : SLOT(startExternalHandler())))->setData(videoPage1);
        }
    }
    if (m_pagevm) {
        QRegExp rx2("vimeo.com/([^d]+)");
        QString videoId2;

        rx2.indexIn(r.linkUrl().toString());
        for (int i = 1; i < 4; ++i) {
            if (!rx2.cap(i).isEmpty()) {
                videoId2 = rx2.cap(i);
                break;
            }
        }
        if (videoId2.isEmpty()) {
            rx2.indexIn(view->url().toString());
            for (int i = 1; i < 4; ++i) {
                if (!rx2.cap(i).isEmpty()) {
                    videoId2 = rx2.cap(i);
                    break;
                }
            }
        }
        if (!videoId2.isEmpty()) {
            QString videoPage2;
            videoPage2 = "http://vimeo.com/" + videoId2;
            menu->addAction(QIcon(":/videoner/data/videoner.png"), tr("Videonize!"), this, SLOT(startExternalHandler()))->setData(videoPage2);
        }
    }
    if (m_pagell) {
        QRegExp rx3("www.liveleak.com/view\\?i=([a-zA-Z0-9_]{4}[^d]+)");
        QString videoId3;

        rx3.indexIn(r.linkUrl().toString());
        for (int i = 1; i < 4; ++i) {
            if (!rx3.cap(i).isEmpty()) {
                videoId3 = rx3.cap(i);
                break;
            }
        }
        if (videoId3.isEmpty()) {
            rx3.indexIn(view->url().toString());
            for (int i = 1; i < 4; ++i) {
                if (!rx3.cap(i).isEmpty()) {
                    videoId3 = rx3.cap(i);
                    break;
                }
            }
        }
        if (!videoId3.isEmpty()) {
            QString videoPage3;
            videoPage3 = "http://www.liveleak.com/view?i=" + videoId3;
            menu->addAction(QIcon(":/videoner/data/videoner.png"), tr("Videonize!"), this, SLOT(startExternalHandler()))->setData(videoPage3);
        }
    }
    if (m_pagemc) {
        QRegExp rx4("www.metacafe.com/watch/([^d]+)/([^&]+)");
        QString videoId4;

        rx4.indexIn(r.linkUrl().toString());
        for (int i = 1; i < 4; ++i) {
            if (!rx4.cap(i).isEmpty()) {
                videoId4 = rx4.cap(i);
                break;
            }
        }
        if (videoId4.isEmpty()) {
            rx4.indexIn(view->url().toString());
            for (int i = 1; i < 4; ++i) {
                if (!rx4.cap(i).isEmpty()) {
                    videoId4 = rx4.cap(i);
                    break;
                }
            }
        }
        if (!videoId4.isEmpty()) {
            QString videoPage4;
            videoPage4 = "www.metacafe.com/watch/" + videoId4;
            menu->addAction(QIcon(":/videoner/data/videoner.png"), tr("Videonize!"), this, SLOT(startExternalHandler()))->setData(videoPage4);
        }
    }
    if (m_pagedm) {
        QRegExp rx5("dailymotion.com/video/([a-z0-9]+_[^&]+)");
        QString videoId5;

        rx5.indexIn(r.linkUrl().toString());
        for (int i = 1; i < 4; ++i) {
            if (!rx5.cap(i).isEmpty()) {
                videoId5 = rx5.cap(i);
                break;
            }
        }
        if (videoId5.isEmpty()) {
            rx5.indexIn(view->url().toString());
            for (int i = 1; i < 4; ++i) {
                if (!rx5.cap(i).isEmpty()) {
                    videoId5 = rx5.cap(i);
                    break;
                }
            }
        }
        if (!videoId5.isEmpty()) {
            QString videoPage5;
            videoPage5 = "www.dailymotion.com/video/" + videoId5;
            menu->addAction(QIcon(":/videoner/data/videoner.png"), tr("Videonize!"), this, SLOT(startExternalHandler()))->setData(videoPage5);
        }
    }
    if (m_pagefa) {
        QRegExp rx6("www.facebook.com/video.php\\?v=([^&]+)");
        QString videoId6;

        rx6.indexIn(r.linkUrl().toString());
        for (int i = 1; i < 4; ++i) {
            if (!rx6.cap(i).isEmpty()) {
                videoId6 = rx6.cap(i);
                break;
            }
        }
        if (videoId6.isEmpty()) {
            rx6.indexIn(view->url().toString());
            for (int i = 1; i < 4; ++i) {
                if (!rx6.cap(i).isEmpty()) {
                    videoId6 = rx6.cap(i);
                    break;
                }
            }
        }
        if (!videoId6.isEmpty()) {
            QString videoPage6;
            videoPage6 = "www.facebook.com/video.php?v=" + videoId6;
            menu->addAction(QIcon(":/videoner/data/videoner.png"), tr("Videonize!"), this, SLOT(startExternalHandler()))->setData(videoPage6);
        }
    }
    if (m_pagebr) {
        QRegExp rx7("www.break.com/video/([^&]+)");
        QString videoId7;

        rx7.indexIn(r.linkUrl().toString());
        for (int i = 1; i < 4; ++i) {
            if (!rx7.cap(i).isEmpty()) {
                videoId7 = rx7.cap(i);
                break;
            }
        }
        if (videoId7.isEmpty()) {
            rx7.indexIn(view->url().toString());
            for (int i = 1; i < 4; ++i) {
                if (!rx7.cap(i).isEmpty()) {
                    videoId7 = rx7.cap(i);
                    break;
                }
            }
        }
        if (!videoId7.isEmpty()) {
            QString videoPage7;
            videoPage7 = "www.break.com/video/" + videoId7;
            menu->addAction(QIcon(":/videoner/data/videoner.png"), tr("Videonize!"), this, SLOT(startExternalHandler()))->setData(videoPage7);
        }
    }
    if (m_pagehu) {
        QRegExp rx8("www.hulu.com/watch/([^d]+)");
        QString videoId8;

        rx8.indexIn(r.linkUrl().toString());
        for (int i = 1; i < 4; ++i) {
            if (!rx8.cap(i).isEmpty()) {
                videoId8 = rx8.cap(i);
                break;
            }
        }
        if (videoId8.isEmpty()) {
            rx8.indexIn(view->url().toString());
            for (int i = 1; i < 4; ++i) {
                if (!rx8.cap(i).isEmpty()) {
                    videoId8 = rx8.cap(i);
                    break;
                }
            }
        }
        if (!videoId8.isEmpty()) {
            QString videoPage8;
            videoPage8 = "www.hulu.com/watch/" + videoId8;
            menu->addAction(QIcon(":/videoner/data/videoner.png"), tr("Videonize!"), this, SLOT(startExternalHandler()))->setData(videoPage8);
        }
    }
    if (m_medel) {
        if (r.tagName() == QLatin1String("video") || r.tagName() == QLatin1String("audio")) {
            QUrl mediaLink = r.mediaUrl();
            menu->addAction(QIcon(":/videoner/data/videoner.png"), tr("Videonize!"), this, (m_sepmedel ? SLOT(startExternalHandlerMed()) : SLOT(startExternalHandler())))->setData(mediaLink);
        }
    }
}

void Videoner_Handler::startExternalHandler()
{
    const QUrl url = qobject_cast<QAction*>(sender())->data().toUrl();
    QStringList arguments = m_extArgs.split(QLatin1Char(' '), QString::SkipEmptyParts);
    arguments << url.toString();

    bool success = QProcess::startDetached(m_extExe, arguments);

    if (!success) {
        QString info = "<ul><li><b>" + tr("Executable: ") + "</b>" + m_extExe + "</li><li><b>" + tr("Arguments: ") + "</b>" + arguments.join(" ") + "</li></ul>";
        QMessageBox::critical(0, tr("Cannot start external viewer"), tr("Cannot start external viewer! %1").arg(info));
    }
}

void Videoner_Handler::startExternalHandlerYt()
{
    const QUrl url = qobject_cast<QAction*>(sender())->data().toUrl();
    QStringList arguments = m_extArgsYt.split(QLatin1Char(' '), QString::SkipEmptyParts);
    arguments << url.toString();

    bool success = QProcess::startDetached(m_extExeYt, arguments);

    if (!success) {
        QString info = "<ul><li><b>" + tr("Executable: ") + "</b>" + m_extExeYt + "</li><li><b>" + tr("Arguments: ") + "</b>" + arguments.join(" ") + "</li></ul>";
        QMessageBox::critical(0, tr("Cannot start external viewer"), tr("Cannot start external viewer! %1").arg(info));
    }
}

void Videoner_Handler::startExternalHandlerMed()
{
    const QUrl url = qobject_cast<QAction*>(sender())->data().toUrl();
    QStringList arguments = m_extArgsMed.split(QLatin1Char(' '), QString::SkipEmptyParts);
    arguments << url.toString();

    bool success = QProcess::startDetached(m_extExeMed, arguments);

    if (!success) {
        QString info = "<ul><li><b>" + tr("Executable: ") + "</b>" + m_extExeMed + "</li><li><b>" + tr("Arguments: ") + "</b>" + arguments.join(" ") + "</li></ul>";
        QMessageBox::critical(0, tr("Cannot start external viewer"), tr("Cannot start external viewer! %1").arg(info));
    }
}
