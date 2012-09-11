/* ============================================================
* YouTuber plugin for QupZilla
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

#include "youtuber_settings.h"
#include "ui_youtuber_settings.h"
#include "youtuber_handler.h"

#include <QSettings>
#include <QFileDialog>

YouTuber_Settings::YouTuber_Settings(YouTuber_Handler* handler, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::YouTuber_Settings)
    , m_handler(handler)
    , m_settingsFile(handler->settingsFile())
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);

    QSettings settings(m_settingsFile, QSettings::IniFormat);
    settings.beginGroup("YouTuber");
    ui->extExe->setText(settings.value("Application", QString()).toString());
    ui->extArgs->setText(settings.value("Arguments", QString()).toString());
    settings.endGroup();

    connect(ui->chooseExtView, SIGNAL(clicked()), this, SLOT(chooseExternalApp()));
    connect(this, SIGNAL(accepted()), this, SLOT(dialogAccepted()));
}

void YouTuber_Settings::dialogAccepted()
{
    QSettings settings(m_settingsFile, QSettings::IniFormat);
    settings.beginGroup("YouTuber");
    settings.setValue("Application", ui->extExe->text());
    settings.setValue("Arguments", ui->extArgs->text());
    settings.endGroup();

    m_handler->loadSettings();
}

void YouTuber_Settings::chooseExternalApp()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Choose executable location..."), QDir::homePath());
    if (path.isEmpty()) {
        return;
    }

    ui->extExe->setText(path);
}

YouTuber_Settings::~YouTuber_Settings()
{
    delete ui;
}
