/* ============================================================
* Simple Session Manager plugin for QupZilla
* Copyright (C) 2017  Razi Alavizadeh <s.r.alavizadeh@gmail.com>
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
#ifndef SSM_Plugin_H
#define SSM_Plugin_H

#include "plugininterface.h"

class SSM_Plugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
    Q_PLUGIN_METADATA(IID "QupZilla.Browser.plugin.SimpleSessionManager")

public:
    explicit SSM_Plugin();
    PluginSpec pluginSpec();

    void init(InitState state, const QString &settingsPath);
    bool testPlugin();
    void unload();

    QTranslator* getTranslator(const QString &locale);
    void populateExtensionsMenu(QMenu* menu);

private slots:
    void openSession();
    void renameSession(QString &sessionFilePath = QString(), bool copy = false);
    void backupSession();
    void deleteSession();
    void saveSession();
    void sessionsDirectoryChanged();

private:
    QMenu* simpleSessionsMenu();
    QMenu* m_sessionsMenu;
    bool m_isDirty;

    QString m_settingsPath;
};

#endif // SSM_Plugin_H
