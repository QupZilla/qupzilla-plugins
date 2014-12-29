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
    ui->extExeYt->setText(settings.value("videonerExtAppYt", QString()).toString());
    ui->extExeArgsYt->setText(settings.value("videonerExtArgsYt", QString()).toString());
    ui->extExeMed->setText(settings.value("videonerExtAppMediaEl", QString()).toString());
    ui->extExeArgsMed->setText(settings.value("videonerExtArgsMediaEl", QString()).toString());
    ui->enableYt->setChecked(settings.value("enableYouTube", true).toBool());
    ui->enableSeparateYt->setChecked(settings.value("enableSeparateYt", false).toBool());
    ui->enableVm->setChecked(settings.value("enableVimeo", false).toBool());
    ui->enableLl->setChecked(settings.value("enableLiveLeak", false).toBool());
    ui->enableMc->setChecked(settings.value("enableMetaCafe", false).toBool());
    ui->enableDm->setChecked(settings.value("enableDailyMotion", false).toBool());
    ui->enableFa->setChecked(settings.value("enableFacebook", false).toBool());
    ui->enableBr->setChecked(settings.value("enableBreak", false).toBool());
    ui->enableHu->setChecked(settings.value("enableHulu", false).toBool());
    ui->enableMed->setChecked(settings.value("enableMediaEl", true).toBool());
    ui->enableSepMed->setChecked(settings.value("enableSepMediaEl", false).toBool());
    settings.endGroup();

    connect(ui->enableYt, SIGNAL(toggled(bool)), this, SLOT(enableYouTubeChanged(bool)));
    enableYouTubeChanged(ui->enableYt->isChecked());

    connect(ui->enableMed, SIGNAL(toggled(bool)), this, SLOT(enableMediaElChanged(bool)));
    enableMediaElChanged(ui->enableMed->isChecked());

    connect(ui->chooseExtView, SIGNAL(clicked()), this, SLOT(chooseExternalApp()));
    connect(ui->chooseExtViewYt, SIGNAL(clicked()), this, SLOT(chooseExternalAppYt()));
    connect(ui->chooseExtViewMed, SIGNAL(clicked()), this, SLOT(chooseExternalAppMed()));
    connect(this, SIGNAL(accepted()), this, SLOT(dialogAccepted()));
}

void Videoner_Settings::dialogAccepted()
{
    QSettings settings(m_settingsFile, QSettings::IniFormat);
    settings.beginGroup("Videoner");
    settings.setValue("videonerExtApp", ui->extExe->text());
    settings.setValue("videonerExtArgs", ui->extExeArgs->text());
    settings.setValue("videonerExtAppYt", ui->extExeYt->text());
    settings.setValue("videonerExtArgsYt", ui->extExeArgsYt->text());
    settings.setValue("videonerExtAppMediaEl", ui->extExeMed->text());
    settings.setValue("videonerExtArgsMediaEl", ui->extExeArgsMed->text());
    settings.setValue("enableYouTube", ui->enableYt->isChecked());
    settings.setValue("enableSeparateYt", ui->enableSeparateYt->isChecked());
    settings.setValue("enableVimeo", ui->enableVm->isChecked());
    settings.setValue("enableLiveLeak", ui->enableLl->isChecked());
    settings.setValue("enableMetaCafe", ui->enableMc->isChecked());
    settings.setValue("enableDailyMotion", ui->enableDm->isChecked());
    settings.setValue("enableFacebook", ui->enableFa->isChecked());
    settings.setValue("enableBreak", ui->enableBr->isChecked());
    settings.setValue("enableHulu", ui->enableHu->isChecked());
    settings.setValue("enableMediaEl", ui->enableMed->isChecked());
    settings.setValue("enableSepMediaEl", ui->enableSepMed->isChecked());
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

void Videoner_Settings::chooseExternalAppYt()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Choose executable location..."), QDir::homePath());
    if (path.isEmpty()) {
        return;
    }

    ui->extExeYt->setText(path);
}

void Videoner_Settings::chooseExternalAppMed()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Choose executable location..."), QDir::homePath());
    if (path.isEmpty()) {
        return;
    }

    ui->extExeMed->setText(path);
}

void Videoner_Settings::enableYouTubeChanged(bool state)
{
    ui->enableSeparateYt->setEnabled(state);
    ui->extExeYt->setEnabled(state);
    ui->extExeArgsYt->setEnabled(state);
    ui->chooseExtViewYt->setEnabled(state);
    ui->labelExeYt->setEnabled(state);
    ui->labelArgsYt->setEnabled(state);
}

void Videoner_Settings::enableMediaElChanged(bool state)
{
    ui->enableSepMed->setEnabled(state);
    ui->extExeMed->setEnabled(state);
    ui->extExeArgsMed->setEnabled(state);
    ui->chooseExtViewMed->setEnabled(state);
    ui->labelExeMed->setEnabled(state);
    ui->labelArgsMed->setEnabled(state);
}

Videoner_Settings::~Videoner_Settings()
{
    delete ui;
}
