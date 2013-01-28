/* ============================================================
* Videoner plugin for QupZilla
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

#include "videoner_settings.h"
#include "ui_videoner_settings.h"
#include "videoner_handler.h"

#include <QSettings>
#include <QFileDialog>

Videoner_Settings::Videoner_Settings(Videoner_Handler* handler, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::Videoner_Settings)
    , m_handler(handler)
    , m_settingsFile(handler->settingsFile())
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);

    QSettings settings(m_settingsFile, QSettings::IniFormat);
    settings.beginGroup("Videoner");
    ui->extExe->setText(settings.value("videonerExtApp", QString()).toString());
    ui->extExeArgs->setText(settings.value("videonerExtArgs", QString()).toString());
    ui->enableYt->setChecked(settings.value("enableYouTube", true).toBool());
    ui->enableVm->setChecked(settings.value("enableVimeo", true).toBool());
    ui->enableDm->setChecked(settings.value("enableDailyMotion", false).toBool());
    ui->enableHu->setChecked(settings.value("enableHulu", false).toBool());
    ui->enableMc->setChecked(settings.value("enableMetaCafe", false).toBool());
    settings.endGroup();

    connect(ui->chooseExtView, SIGNAL(clicked()), this, SLOT(chooseExternalApp()));
    connect(this, SIGNAL(accepted()), this, SLOT(dialogAccepted()));
}

void Videoner_Settings::dialogAccepted()
{
    QSettings settings(m_settingsFile, QSettings::IniFormat);
    settings.beginGroup("Videoner");
    settings.setValue("videonerExtApp", ui->extExe->text());
    settings.setValue("videonerExtArgs", ui->extExeArgs->text());
    settings.setValue("enableYouTube", ui->enableYt->isChecked());
    settings.setValue("enableVimeo", ui->enableVm->isChecked());
    settings.setValue("enableDailyMotion", ui->enableDm->isChecked());
    settings.setValue("enableHulu", ui->enableHu->isChecked());
    settings.setValue("enableMetaCafe", ui->enableMc->isChecked());
    settings.endGroup();

    m_handler->loadSettings();
}

void Videoner_Settings::chooseExternalApp()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Choose executable location..."), QDir::homePath());
    if (path.isEmpty()) {
        return;
    }

    ui->extExe->setText(path);
}

Videoner_Settings::~Videoner_Settings()
{
    delete ui;
}
