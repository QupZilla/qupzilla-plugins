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
#include "tabmanagerwidgetcontroller.h"
#include "tabmanagerwidget.h"
#include "clickablelabel.h"
#include "qupzilla.h"
#include "tabwidget.h"
#include "mainapplication.h"

#include <QDesktopWidget>
#include <QStatusBar>
#include <QAction>
#include <QStyle>

#include <QDebug>

TabManagerWidgetController::TabManagerWidgetController(QObject* parent)
    : SideBarInterface(parent)
    , m_defaultTabManager(0)
    , m_viewType(ShowAsWindow)
    , m_groupType(TabManagerWidget::GroupByWindow)
{
}

TabManagerWidgetController::~TabManagerWidgetController()
{
}

QString TabManagerWidgetController::title() const
{
    return tr("Tab Manager");
}

QAction* TabManagerWidgetController::createMenuAction()
{
    QAction* act = new QAction(tr("Tab Manager"), 0);
    act->setCheckable(true);
    act->setIcon(QIcon(":tabmanager/data/tabmanager.png"));
    act->setShortcut(QKeySequence("Ctrl+Shift+M"));
    act->setData("TabManager");

    return act;
}

QWidget* TabManagerWidgetController::createSideBarWidget(QupZilla* mainWindow)
{
    return createTabManagerWidget(mainWindow, mainWindow);
}

QWidget* TabManagerWidgetController::createStatusBarIcon(QupZilla* mainWindow)
{
    if (!defaultTabManager()) {
        return 0;
    }

    if (m_statusBarIcons.contains(mainWindow)) {
        return m_statusBarIcons.value(mainWindow);
    }

    ClickableLabel* icon = new ClickableLabel(mainWindow);
    icon->setCursor(Qt::PointingHandCursor);
    QPixmap p(":tabmanager/data/tabmanager.png");
    icon->setPixmap(p.scaledToHeight(16));
    icon->setToolTip(tr("Show/Hide Tab Manager"));

    QAction* toggleViewAction = createMenuAction();
    toggleViewAction->setCheckable(false);
    toggleViewAction->setParent(icon);
    mainWindow->addAction(toggleViewAction);
    connect(toggleViewAction, SIGNAL(triggered()), this, SLOT(toggleDefaultTabManagerView()));

    connect(icon, SIGNAL(clicked(QPoint)), this, SLOT(toggleDefaultTabManagerView()));

    m_statusBarIcons.insert(mainWindow, icon);
    m_actions.insert(mainWindow, toggleViewAction);

    return icon;
}

TabManagerWidgetController::ViewType TabManagerWidgetController::viewType()
{
    return m_viewType;
}

void TabManagerWidgetController::setViewType(ViewType type)
{
    m_viewType = type;
}

TabManagerWidget::GroupType TabManagerWidgetController::groupType()
{
    return m_groupType;
}

void TabManagerWidgetController::setGroupType(TabManagerWidget::GroupType type)
{
    m_groupType = type;
}

TabManagerWidget* TabManagerWidgetController::createTabManagerWidget(QupZilla* mainClass, QWidget* parent, bool defaultWidget)
{
    TabManagerWidget* tabManagerWidget = new TabManagerWidget(mainClass, parent, defaultWidget);
    tabManagerWidget->setGroupType(m_groupType);

    if (defaultWidget) {
        m_defaultTabManager = tabManagerWidget;
        QAction* toggleViewAction = createMenuAction();
        toggleViewAction->setCheckable(false);
        toggleViewAction->setParent(m_defaultTabManager);
        m_defaultTabManager->addAction(toggleViewAction);
        connect(toggleViewAction, SIGNAL(triggered()), this, SLOT(toggleDefaultTabManagerView()));
        connect(tabManagerWidget, SIGNAL(showSideBySide()), this, SLOT(showSideBySide()));
    }
    else {
        m_defaultTabManager = 0;
    }

    connect(tabManagerWidget, SIGNAL(groupTypeChanged(TabManagerWidget::GroupType)), this, SLOT(setGroupType(TabManagerWidget::GroupType)));
    connect(this, SIGNAL(requestRefreshTree(WebPage*)), tabManagerWidget, SLOT(delayedRefreshTree(WebPage*)));

    emit requestRefreshTree();

    return tabManagerWidget;
}

TabManagerWidget *TabManagerWidgetController::defaultTabManager()
{
    return m_defaultTabManager;
}

void TabManagerWidgetController::addStatusBarIcon(QupZilla* window)
{
    if (window) {
        window->statusBar()->addPermanentWidget(createStatusBarIcon(window));
    }
}

void TabManagerWidgetController::removeStatusBarIcon(QupZilla *window)
{
    if (window) {
        window->statusBar()->removeWidget(m_statusBarIcons.value(window));
        window->removeAction(m_actions.value(window));
        delete m_actions.value(window);
        delete m_statusBarIcons.value(window);
        m_statusBarIcons.remove(window);
        m_actions.remove(window);
    }
}

void TabManagerWidgetController::mainWindowCreated(QupZilla* window, bool refresh)
{
    if (window) {
        addStatusBarIcon(window);
        connect(window->tabWidget(), SIGNAL(currentChanged(int)), this, SIGNAL(requestRefreshTree()));
    }

    if (refresh) {
        emit requestRefreshTree();
    }
}

void TabManagerWidgetController::mainWindowDeleted(QupZilla *window)
{
    removeStatusBarIcon(window);

    emit requestRefreshTree();
}

void TabManagerWidgetController::toggleDefaultTabManagerView()
{
    if (defaultTabManager()) {
        defaultTabManager()->activateWindow();
        defaultTabManager()->setVisible(!defaultTabManager()->isVisible());
    }
}

void TabManagerWidgetController::showSideBySide()
{
    if (!defaultTabManager()) {
        return;
    }

    const QRect &availableGeometry = mApp->desktop()->availableGeometry();
    static int frameWidth = (defaultTabManager()->frameGeometry().width() - defaultTabManager()->geometry().width()) / 2;
    static int titleBarHeight = defaultTabManager()->style()->pixelMetric(QStyle::PM_TitleBarHeight);

    QRect managerRect(frameWidth, titleBarHeight, qMin(400, availableGeometry.width() / 4), availableGeometry.height() - titleBarHeight - frameWidth);
    QRect qupzillaRect(managerRect.topRight().x() + 2 * frameWidth, titleBarHeight, availableGeometry.width() - managerRect.width() - 4 * frameWidth, managerRect.height());

    defaultTabManager()->setGeometry(managerRect);
    mApp->getWindow()->setGeometry(qupzillaRect);
    mApp->getWindow()->show();
    mApp->getWindow()->raise();

    defaultTabManager()->show();
    defaultTabManager()->activateWindow();
    defaultTabManager()->raise();
}
