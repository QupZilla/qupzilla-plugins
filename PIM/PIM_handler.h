/* ============================================================
* Personal Information Manager plugin for QupZilla
* Copyright (C) 2012  David Rosca <nowrep@gmail.com>
* Copyright (C) 2012  Mladen Pejaković <pejakm@gmail.com>
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
#ifndef PIM_HANDLER_H
#define PIM_HANDLER_H

#include <QObject>
#include <QWebElement>
#include <QMessageBox>
#include <QWebHitTestResult>
#include <QMenu>
#include <QWeakPointer>
#include <QHash>

class QMouseEvent;
class QWebElement;
class QLabel;
class WebView;
class PIM_Handler : public QObject
{
    Q_OBJECT
public:
    explicit PIM_Handler(const QString &sPath, QObject* parent = 0);

    QString settingsFile() const;
    void loadSettings();

    void populateWebViewMenu(QMenu* menu, WebView* view, const QWebHitTestResult &hitTest);

signals:

public slots:

private slots:
    void pimInsert();

private:
    enum PI_Type {
        PI_FirstName = 0,
        PI_LastName = 1,
        PI_Email = 2,
        PI_Phone = 3,
        PI_Mobile = 4,
        PI_Address = 5,
        PI_City = 6,
        PI_Zip = 7,
        PI_State = 8,
        PI_Country = 9,
        PI_HomePage = 10,
        PI_Special1 = 11,
        PI_Special2 = 12,
        PI_Special3 = 13,
        PI_Max = 14
    };

    QHash<PI_Type, QString> m_allInfo;
    QHash<PI_Type, QString> m_translations;

    QWeakPointer<WebView> m_view;
    QWebElement m_element;

    QString m_settingsFile;
};

#endif // PIM_HANDLER_H
