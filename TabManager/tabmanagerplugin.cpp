/* ============================================================
* TabManager plugin for QupZilla
* Copyright (C) 2013  S. Razi Alavizadeh <s.r.alavizadeh@gmail.com>
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
#include "tabmanagerplugin.h"
#include "tabmanagerwidgetcontroller.h"
#include "tabmanagerwidget.h"
#include "qupzilla.h"
#include "pluginproxy.h"
#include "mainapplication.h"
#include "sidebar.h"

#include <QInputDialog>
#include <QTranslator>
#include <QSettings>
#include <QAction>

TabManagerPlugin::TabManagerPlugin()
    : QObject()
    , m_controller(0)
    , m_tabManagerWidget(0)
{
}

PluginSpec TabManagerPlugin::pluginSpec()
{
    PluginSpec spec;
    spec.name = "Tab Manager";
    spec.info = "Tab Manager plugin for QupZilla";
    spec.description = "Adds ability to managing tabs and windows";
    spec.version = "0.1.1";
    spec.author = "Razi Alavizadeh <s.r.alavizadeh@gmail.com>";
    spec.icon = QPixmap(":tabmanager/data/tabmanager.png");
    spec.hasSettings = true;

    return spec;
}

void TabManagerPlugin::init(InitState state, const QString &settingsPath)
{
    m_controller = new TabManagerWidgetController(this);
    connect(mApp->plugins(), SIGNAL(mainWindowCreated(QupZilla*)), m_controller, SLOT(mainWindowCreated(QupZilla*)));
    connect(mApp->plugins(), SIGNAL(mainWindowDeleted(QupZilla*)), m_controller, SLOT(mainWindowDeleted(QupZilla*)));
    connect(mApp->plugins(), SIGNAL(webPageCreated(WebPage*)), m_controller, SIGNAL(requestRefreshTree()));
    connect(mApp->plugins(), SIGNAL(webPageDeleted(WebPage*)), m_controller, SIGNAL(requestRefreshTree(WebPage*)));

    m_settingsPath = settingsPath;
    // load settings
    QSettings settings(m_settingsPath + "extensions.ini", QSettings::IniFormat);
    settings.beginGroup("TabManager");
    m_controller->setGroupType(TabManagerWidget::GroupType(settings.value("GroupType", TabManagerWidget::GroupByWindow).toInt()));
    insertManagerWidget(TabManagerWidgetController::ViewType(settings.value("ViewType", TabManagerWidgetController::ShowAsWindow).toInt()));
    settings.endGroup();

    if (state == LateInitState) {
        foreach (QupZilla* window, mApp->mainWindows()) {
            m_controller->mainWindowCreated(window, false);
        }
    }
}

void TabManagerPlugin::unload()
{
    // save settings
    QSettings settings(m_settingsPath + "extensions.ini", QSettings::IniFormat);
    settings.beginGroup("TabManager");
    settings.setValue("GroupType", m_controller->groupType());
    settings.setValue("ViewType", m_controller->viewType());
    settings.endGroup();

    removeManagerWidget();

    delete m_controller;
}

bool TabManagerPlugin::testPlugin()
{
    return (QupZilla::VERSION == QLatin1String("1.5.0"));
}

QTranslator* TabManagerPlugin::getTranslator(const QString &locale)
{
    QTranslator* translator = new QTranslator(this);
    translator->load(locale, ":/tabmanager/locale/");
    return translator;
}

void TabManagerPlugin::showSettings(QWidget* parent)
{
    bool ok;
    QString viewType = QInputDialog::getItem(parent, tr("Tab Manager View Type"),
                       tr("<p>Please select view type:<br />"
                          "<b>Note:</b> The \"<i>Window</i>\" type is recommended for managing lots of windows/tabs")
                       , QStringList() << tr("SideBar") << tr("Window")
                       , m_controller->viewType(), false, &ok, Qt::WindowStaysOnTopHint);
    TabManagerWidgetController::ViewType type;
    if (viewType == tr("SideBar")) {
        type = TabManagerWidgetController::ShowAsSideBar;
    }
    else {
        type = TabManagerWidgetController::ShowAsWindow;
    }

    if (ok && type != m_controller->viewType()) {
        removeManagerWidget();
        insertManagerWidget(type);

        if (type == TabManagerWidgetController::ShowAsSideBar) {
            mApp->getWindow()->sideBarManager()->showSideBar("TabManager");
        }
        else if (type == TabManagerWidgetController::ShowAsWindow) {
            // add statusbar icon
            foreach (QupZilla* window, mApp->mainWindows()) {
                m_controller->addStatusBarIcon(window);
            }
        }
    }
}

void TabManagerPlugin::insertManagerWidget(TabManagerWidgetController::ViewType type)
{
    m_controller->setViewType(type);
    if (type == TabManagerWidgetController::ShowAsSideBar) {
        SideBarManager::addSidebar("TabManager", m_controller);
    }
    else if (type == TabManagerWidgetController::ShowAsWindow) {
        if (!m_tabManagerWidget) {
            m_tabManagerWidget = m_controller->createTabManagerWidget(mApp->getWindow(), 0, true);
            m_tabManagerWidget->setWindowFlags(Qt::Window);
        }
        m_tabManagerWidget->show();
    }
}

void TabManagerPlugin::removeManagerWidget()
{
    if (m_controller->viewType() == TabManagerWidgetController::ShowAsSideBar) {
        SideBarManager::removeSidebar("TabManager");
    }
    else if (m_controller->viewType() == TabManagerWidgetController::ShowAsWindow) {
        // remove statusbar icon
        foreach (QupZilla* window, mApp->mainWindows()) {
            m_controller->removeStatusBarIcon(window);
        }

        m_tabManagerWidget->close();
        delete m_tabManagerWidget;
        m_tabManagerWidget = 0;
    }
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(TabManager, TabManagerPlugin)
#endif
