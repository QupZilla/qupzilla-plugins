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

#include "mailhandle_settings.h"
#include "ui_mailhandle_settings.h"
#include "mailhandle_handler.h"

#include <QSettings>

MailHandle_Settings::MailHandle_Settings(MailHandle_Handler* handler, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::MailHandle_Settings)
    , m_handler(handler)
    , m_settingsFile(handler->settingsFile())
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);

    QSettings settings(m_settingsFile, QSettings::IniFormat);
    settings.beginGroup("MailHandle");
    ui->mhservice->setCurrentIndex(settings.value("webservice", 0).toInt());
    settings.endGroup();

    connect(this, SIGNAL(accepted()), this, SLOT(dialogAccepted()));
}

void MailHandle_Settings::dialogAccepted()
{
    QSettings settings(m_settingsFile, QSettings::IniFormat);
    settings.beginGroup("MailHandle");
    settings.setValue("webservice", ui->mhservice->currentIndex());
    settings.endGroup();

    m_handler->loadSettings();
}

MailHandle_Settings::~MailHandle_Settings()
{
    delete ui;
}
