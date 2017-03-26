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
#include "ssm_plugin.h"
#include "datapaths.h"
#include "pluginproxy.h"
#include "restoremanager.h"
#include "mainapplication.h"

#include <QDir>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QTranslator>
#include <QInputDialog>
#include <QFileSystemWatcher>

SSM_Plugin::SSM_Plugin()
    : QObject()
    , m_sessionsMenu(0)
    , m_isDirty(true)
{
}

PluginSpec SSM_Plugin::pluginSpec()
{
    PluginSpec spec;
    spec.name = "Simple Session Manager";
    spec.info = "A simple plugin to manage QupZilla sessions";
    spec.description = "A plugin to save/restore sessions";
    spec.version = "0.1.0";
    spec.author = "Razi Alavizadeh <s.r.alavizadeh@gmail.com>";
    spec.icon = QPixmap(":simplesessionmanager/data/simplesessionmanager.png");
    spec.hasSettings = false;

    return spec;
}

void SSM_Plugin::init(InitState state, const QString &settingsPath)
{
    Q_UNUSED(state)

    m_settingsPath = settingsPath + QL1S("/SimpleSessionManager");

    QDir sessionsDir(m_settingsPath + QL1S("/sessions"));
    if (!sessionsDir.exists()) {
        sessionsDir.mkpath(sessionsDir.absolutePath());
    }
}

bool SSM_Plugin::testPlugin()
{
    return (Qz::VERSION == QL1S(QUPZILLA_VERSION));
}

void SSM_Plugin::unload()
{
    if (m_sessionsMenu) {
        delete m_sessionsMenu;
        m_sessionsMenu = 0;
    }
}

QTranslator* SSM_Plugin::getTranslator(const QString &locale)
{
    QTranslator* translator = new QTranslator(this);
    translator->load(locale, ":/SSM_Plugin/locale/");
    return translator;
}

void SSM_Plugin::populateExtensionsMenu(QMenu* menu)
{
    menu->addMenu(simpleSessionsMenu());
}

void SSM_Plugin::openSession()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (!action)
        return;

    const QString sessionPath = action->data().toString();

    QVector<RestoreManager::WindowData> sessionData;
    RestoreManager::createFromFile(sessionPath, sessionData);

    if (!sessionData.isEmpty()) {
        mApp->openSession(0, sessionData);
    }
}

void SSM_Plugin::renameSession(QString &sessionFilePath, bool copy)
{
    if (sessionFilePath.isEmpty()) {
        QAction* action = qobject_cast<QAction*>(sender());
        if (!action)
            return;

        sessionFilePath = action->data().toString();
    }

    bool ok;
    QString newName = QInputDialog::getText(mApp->activeWindow(), tr("Rename Session"),
                                            tr("Please enter a new name:"), QLineEdit::Normal,
                                            QFileInfo(sessionFilePath).baseName(), &ok);

    if (ok) {
        const QString newSessionPath = QString("%1/sessions/%2.dat").arg(m_settingsPath).arg(newName);
        if (QFile::exists(newSessionPath)) {
            QMessageBox::information(mApp->activeWindow(), tr("Error!"), tr("The session file \"%1\" exists. Please enter another name.").arg(newName));
            renameSession(sessionFilePath, copy);
            return;
        }

        if (copy) {
            if (!QFile::copy(sessionFilePath, newSessionPath)) {
                QMessageBox::information(mApp->activeWindow(), tr("Error!"), tr("An error occurred when copying session file."));
                return;
            }
        }
        else {
            if (!QFile::rename(sessionFilePath, newSessionPath)) {
                QMessageBox::information(mApp->activeWindow(), tr("Error!"), tr("An error occurred when renaming session file."));
                return;
            }
        }
    }
}

void SSM_Plugin::backupSession()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (!action)
        return;

    renameSession(action->data().toString(), true);
}

void SSM_Plugin::deleteSession()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (!action)
        return;

    const QString filePath = action->data().toString();

    QMessageBox::StandardButton result = QMessageBox::information(mApp->activeWindow(), tr("Warning!"), tr("Are you sure to delete following session?\n%1")
                                                                  .arg(QDir::toNativeSeparators(filePath)), QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        QFile::remove(filePath);
    }
}

void SSM_Plugin::saveSession()
{
    bool ok;
    QString sessionName = QInputDialog::getText(mApp->activeWindow(), tr("Save Session"),
                                         tr("Please enter a name to save session:"), QLineEdit::Normal,
                                         tr("Saved Session (%1)").arg(QDateTime::currentDateTime().toString("dd MMM yyyy HH-mm-ss")), &ok);

    if (ok) {
        const QString filePath = QString("%1/sessions/%2.dat").arg(m_settingsPath).arg(sessionName);
        if (QFile::exists(filePath)) {
            QMessageBox::information(mApp->activeWindow(), tr("Error!"), tr("The session file \"%1\" exists. Please enter another name.").arg(sessionName));
            saveSession();
            return;
        }

        mApp->writeCurrentSession(filePath);
    }
}

void SSM_Plugin::sessionsDirectoryChanged()
{
    m_isDirty = true;
}

QMenu *SSM_Plugin::simpleSessionsMenu()
{
    if (!m_isDirty && m_sessionsMenu && !m_sessionsMenu->actions().isEmpty())
        return m_sessionsMenu;

    QDir dir(m_settingsPath + QL1S("/sessions"));

    if (!m_sessionsMenu) {
        m_sessionsMenu = new QMenu(tr("Simple Session Manager"));

        QFileSystemWatcher* sessionFilesWatcher = new QFileSystemWatcher({dir.canonicalPath()}, this);
        connect(sessionFilesWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(sessionsDirectoryChanged()));
    }
    else {
        m_sessionsMenu->clear();
    }

    const QFileInfoList sessionFiles = dir.entryInfoList({QSL("*.*")}, QDir::Files)
            << QFileInfo(DataPaths::currentProfilePath() + QL1S("/session.dat"))
            << QFileInfo(DataPaths::currentProfilePath() + QL1S("/session.dat.old"))
            << QFileInfo(DataPaths::currentProfilePath() + QL1S("/session.dat.old1"));

    QStringList fileNames;

    for (int i = 0; i < sessionFiles.size(); ++i) {
        const QFileInfo &file = sessionFiles.at(i);
        QVector<RestoreManager::WindowData> data;
        RestoreManager::createFromFile(file.absoluteFilePath(), data);

        if (!data.isEmpty()) {
            QString title = file.baseName();

            if (i == sessionFiles.size() - 3) {
                m_sessionsMenu->addSeparator();
                title = tr("Last Session");
            }
            else if (i == sessionFiles.size() - 2)
                title = tr("First Backup");
            else if (i == sessionFiles.size() - 1)
                title = tr("Second Backup");
            else if (fileNames.contains(title))
                title = file.fileName();

            fileNames << title;

            QMenu* sessionSubmenu = new QMenu(title, m_sessionsMenu);
            sessionSubmenu->addAction(tr("Open"), this, SLOT(openSession()))->setData(file.absoluteFilePath());

            if (i < sessionFiles.size() - 3) {
                sessionSubmenu->addAction(tr("Rename"), this, SLOT(renameSession()))->setData(file.absoluteFilePath());
                sessionSubmenu->addAction(tr("Remove"), this, SLOT(deleteSession()))->setData(file.absoluteFilePath());
            }
            else {
                sessionSubmenu->addAction(tr("Backup"), this, SLOT(backupSession()))->setData(file.absoluteFilePath());
            }

            m_sessionsMenu->addMenu(sessionSubmenu);
        }
    }

    m_sessionsMenu->addSeparator();
    m_sessionsMenu->addAction(tr("Save Current Session..."), this, SLOT(saveSession()));

    m_isDirty = false;

    return m_sessionsMenu;
}
