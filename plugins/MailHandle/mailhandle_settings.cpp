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

#include "mailhandle_settings.h"
#include "ui_mailhandle_settings.h"
#include "mailhandle_schemehandler.h"

#include <QSettings>

MailHandle_Settings::MailHandle_Settings(MailHandle_SchemeHandler* schemehandler, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::MailHandle_Settings)
    , m_schemehandler(schemehandler)
    , m_settingsFile(schemehandler->settingsFile())
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);

    QSettings settings(m_settingsFile, QSettings::IniFormat);
    settings.beginGroup("MailHandle");
    ui->mhservice->setCurrentIndex(settings.value("webservice", 0).toInt());
    ui->mhwspath->setText(settings.value("webservicepath", QString()).toString());
    settings.endGroup();

    mhserviceChanged(ui->mhservice->currentIndex());
    connect(ui->mhservice, SIGNAL(currentIndexChanged(int)), this, SLOT(mhserviceChanged(int)));
    connect(this, SIGNAL(accepted()), this, SLOT(dialogAccepted()));
}

void MailHandle_Settings::dialogAccepted()
{
    QSettings settings(m_settingsFile, QSettings::IniFormat);
    settings.beginGroup("MailHandle");
    settings.setValue("webservice", ui->mhservice->currentIndex());
    settings.setValue("webservicepath", ui->mhwspath->text());
    settings.endGroup();

    m_schemehandler->loadSettings();
}

void MailHandle_Settings::mhserviceChanged(int value)
{
    ui->webservicepath->setEnabled(value == 8);
}


MailHandle_Settings::~MailHandle_Settings()
{
    delete ui;
}
