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
#ifndef VIDEONER_SETTINGS_H
#define VIDEONER_SETTINGS_H

#include <QDialog>

namespace Ui
{
class Videoner_Settings;
}

class Videoner_Handler;

class Videoner_Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Videoner_Settings(Videoner_Handler* handler, QWidget* parent = 0);
    ~Videoner_Settings();

private slots:
    void chooseExternalApp();
    void chooseExternalAppYt();
    void chooseExternalAppMed();
    void dialogAccepted();
    
    void enableYouTubeChanged(bool state);
    void enableMediaElChanged(bool state);
private:
    Ui::Videoner_Settings* ui;

    Videoner_Handler* m_handler;
    QString m_settingsFile;
};

#endif // VIDEONER_SETTINGS_H
