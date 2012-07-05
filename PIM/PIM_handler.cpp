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
#include "PIM_handler.h"
#include "PIM_settings.h"
#include "webview.h"

#include <QApplication>
#include <QSettings>
#include <QWebPage>
#include <QWebFrame>
#include <QLabel>
#include <QToolTip>

PIM_Handler::PIM_Handler(const QString &sPath, QObject* parent)
    : QObject(parent)
    , m_settingsFile(sPath + "extensions.ini")
{
    loadSettings();
}

void PIM_Handler::loadSettings()
{
    QSettings settings(m_settingsFile, QSettings::IniFormat);

    settings.beginGroup("PIM");
    m_allInfo[PI_FirstName] = settings.value("FirstName", "").toString();
    m_allInfo[PI_LastName] = settings.value("LastName", "").toString();
    m_allInfo[PI_Email] = settings.value("Email", "").toString();
    m_allInfo[PI_Phone] = settings.value("Phone", "").toString();
    m_allInfo[PI_Mobile] = settings.value("Mobile", "").toString();
    m_allInfo[PI_Address] = settings.value("Address", "").toString();
    m_allInfo[PI_City] = settings.value("City", "").toString();
    m_allInfo[PI_Zip] = settings.value("Zip", "").toString();
    m_allInfo[PI_State] = settings.value("State", "").toString();
    m_allInfo[PI_Country] = settings.value("Country", "").toString();
    m_allInfo[PI_HomePage] = settings.value("HomePage", "").toString();
    m_allInfo[PI_Special1] = settings.value("Special1", "").toString();
    m_allInfo[PI_Special2] = settings.value("Special2", "").toString();
    m_allInfo[PI_Special3] = settings.value("Special3", "").toString();
    settings.endGroup();
}

void PIM_Handler::showSettings(QWidget* parent)
{
    PIM_Settings* settings = new PIM_Settings(m_settingsFile, parent);
    settings->show();

    connect(settings, SIGNAL(accepted()), this, SLOT(loadSettings()));
}

void PIM_Handler::populateWebViewMenu(QMenu* menu, WebView* view, const QWebHitTestResult &hitTest)
{
    m_view = view;
    m_element = hitTest.element();

    if (!hitTest.isContentEditable()) {
        return;
    }

    m_translations[PI_FirstName] = tr("First Name");
    m_translations[PI_LastName] = tr("Last Name");
    m_translations[PI_Email] = tr("E-mail");
    m_translations[PI_Phone] = tr("Phone");
    m_translations[PI_Mobile] = tr("Mobile");
    m_translations[PI_Address] = tr("Address");
    m_translations[PI_City] = tr("City");
    m_translations[PI_Zip] = tr("Zip");
    m_translations[PI_State] = tr("State/Region");
    m_translations[PI_Country] = tr("Country");
    m_translations[PI_HomePage] = tr("Home Page");
    m_translations[PI_Special1] = tr("Special 1");
    m_translations[PI_Special2] = tr("Special 2");
    m_translations[PI_Special3] = tr("Special 3");

    QMenu* pimMenu = new QMenu(tr("Insert Personal"));
    pimMenu->setIcon(QIcon(":/PIM/data/PIM.png"));

    for (int i = 0; i < PI_Max; ++i) {
        const QString &info = m_allInfo[PI_Type(i)];
        if (info.isEmpty()) {
            continue;
        }

        QAction* action = pimMenu->addAction(m_translations[PI_Type(i)], this, SLOT(pimInsert()));
        action->setData(info);
    }

    pimMenu->addSeparator();
    pimMenu->addAction(tr("Edit"), this, SLOT(showSettings()));

    menu->addMenu(pimMenu);
    menu->addSeparator();
}

void PIM_Handler::pimInsert()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (m_element.isNull() || !action) {
        return;
    }

    QString info = action->data().toString();
    info.replace('"', "\\\"");
    m_element.evaluateJavaScript(QString("this.value += \"%1\"").arg(info));
}
