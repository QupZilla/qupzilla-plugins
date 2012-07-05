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

QString PIM_Handler::settingsFile()
{
    return m_settingsFile;
}

void PIM_Handler::loadSettings()
{
    QSettings settings(m_settingsFile, QSettings::IniFormat);

    settings.beginGroup("PIM");
    m_pim_firstname = settings.value("FirstName", "").toString();
    m_pim_lastname = settings.value("LastName", "").toString();
    m_pim_email = settings.value("Email", "").toString();
    m_pim_phone = settings.value("Phone", "").toString();
    m_pim_mobile = settings.value("Mobile", "").toString();
    m_pim_address = settings.value("Address", "").toString();
    m_pim_city = settings.value("City", "").toString();
    m_pim_zip = settings.value("Zip", "").toString();
    m_pim_state = settings.value("State", "").toString();
    m_pim_country = settings.value("Country", "").toString();
    m_pim_homepage = settings.value("HomePage", "").toString();
    m_pim_special1 = settings.value("Special1", "").toString();
    m_pim_special2 = settings.value("Special2", "").toString();
    m_pim_special3 = settings.value("Special3", "").toString();
    settings.endGroup();
}

bool PIM_Handler::handleMousePress(QObject* obj, QMouseEvent* event)
{
    WebView* view = qobject_cast<WebView*>(obj);
    if (!view) {
        return false;
    }

    m_view = view;

    return false;
}

void PIM_Handler::populateWebViewMenu(QMenu* menu, WebView* view, const QWebHitTestResult &hitTest)
{
    m_view = view;
    
    if (hitTest.isContentEditable()) {

//     TODO Add icon: QIcon(":/PIM/data/PIM.png")
    QMenu* pimMenu = new QMenu(tr("Insert"));

    if (!m_pim_firstname.isEmpty()) {
        pimMenu->addAction(tr("First Name"), this, SLOT(pimInsert()))->setData(m_pim_firstname);
    }
    if (!m_pim_lastname.isEmpty()) {
        pimMenu->addAction(tr("Last Name"), this, SLOT(pimInsert()))->setData(m_pim_lastname);
    }
    if (!m_pim_email.isEmpty()) {
        pimMenu->addAction(tr("E-mail"), this, SLOT(pimInsert()))->setData(m_pim_email);
    }
    if (!m_pim_phone.isEmpty()) {
        pimMenu->addAction(tr("Phone"), this, SLOT(pimInsert()))->setData(m_pim_phone);
    }
    if (!m_pim_mobile.isEmpty()) {
        pimMenu->addAction(tr("Mobile"), this, SLOT(pimInsert()))->setData(m_pim_mobile);
    }
    if (!m_pim_address.isEmpty()) {
        pimMenu->addAction(tr("Address"), this, SLOT(pimInsert()))->setData(m_pim_address);
    }
    if (!m_pim_city.isEmpty()) {
        pimMenu->addAction(tr("City"), this, SLOT(pimInsert()))->setData(m_pim_city);
    }
    if (!m_pim_zip.isEmpty()) {
        pimMenu->addAction(tr("Zip"), this, SLOT(pimInsert()))->setData(m_pim_zip);
    }
    if (!m_pim_state.isEmpty()) {
        pimMenu->addAction(tr("State/Region"), this, SLOT(pimInsert()))->setData(m_pim_state);
    }
    if (!m_pim_country.isEmpty()) {
        pimMenu->addAction(tr("Country"), this, SLOT(pimInsert()))->setData(m_pim_country);
    }
    if (!m_pim_homepage.isEmpty()) {
        pimMenu->addAction(tr("Home Page"), this, SLOT(pimInsert()))->setData(m_pim_homepage);
    }
    if (!m_pim_special1.isEmpty()) {
        pimMenu->addAction(tr("Special1"), this, SLOT(pimInsert()))->setData(m_pim_special1);
    }
    if (!m_pim_special2.isEmpty()) {
        pimMenu->addAction(tr("Special2"), this, SLOT(pimInsert()))->setData(m_pim_special2);
    }
    if (!m_pim_special3.isEmpty()) {
        pimMenu->addAction(tr("Special3"), this, SLOT(pimInsert()))->setData(m_pim_special3);
    }

    menu->addMenu(pimMenu);
    menu->addSeparator();
    }
}

void PIM_Handler::pimInsert()
{

}
