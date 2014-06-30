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
#include "tabmanagerwidget.h"
#include "ui_tabmanagerwidget.h"
#include "mainapplication.h"
#include "browserwindow.h"
#include "webtab.h"
#include "webpage.h"
#include "tabbedwebview.h"
#include "tabwidget.h"
#include "locationbar.h"
#include "bookmarkstools.h"
#include "bookmarkitem.h"
#include "bookmarks.h"

#include <QDesktopWidget>
#include <QDialogButtonBox>
#include <QStackedWidget>
#include <QDialog>
#include <QTimer>

#define WebTabPointerRole Qt::UserRole + 10
#define QupZillaPointerRole Qt::UserRole + 20


TabManagerWidget::TabManagerWidget(BrowserWindow* mainClass, QWidget* parent, bool defaultWidget)
    : QWidget(parent)
    , p_QupZilla(mainClass)
    , ui(new Ui::TabManagerWidget)
    , m_isRefreshing(false)
    , m_refreshBlocked(false)
    , m_webPage(0)
    , m_waitForRefresh(false)
    , m_isDefaultWidget(defaultWidget)

{
    ui->setupUi(this);
    ui->treeWidget->setExpandsOnDoubleClick(false);
    ui->groupTypeComboBox->addItems(QStringList() << tr("Window") << tr("Domain") << tr("Host"));

    ui->sideBySide->setVisible(m_isDefaultWidget);

    ui->closeSelection->setEnabled(false);
    ui->detachSelection->setEnabled(false);
    ui->bookmarkSelection->setEnabled(false);

    connect(ui->closeSelection, SIGNAL(clicked()), this, SLOT(processActions()));
    connect(ui->detachSelection, SIGNAL(clicked()), this, SLOT(processActions()));
    connect(ui->bookmarkSelection, SIGNAL(clicked()), this, SLOT(processActions()));
    connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(itemDoubleClick(QTreeWidgetItem*,int)));
    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(treeWidgetChanged()));
    connect(ui->sideBySide, SIGNAL(clicked()), this, SIGNAL(showSideBySide()));
    connect(ui->groupTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeGroupType(int)));
}

TabManagerWidget::~TabManagerWidget()
{
    delete ui;
}

void TabManagerWidget::setGroupType(GroupType type)
{
    m_groupType = type;
    ui->groupTypeComboBox->setCurrentIndex(type);
}

QString TabManagerWidget::domainFromUrl(const QUrl &url, bool useHostName)
{
    QString appendString;
    QString urlString = url.toString();

    if (url.scheme() == "file") {
        return tr("Local File System:");
    }
    else if (url.scheme() == "qupzilla" || urlString.isEmpty()) {
        return tr("QupZilla:");
    }
    else if (url.scheme() == "ftp") {
        appendString = tr(" [FTP]");
    }

    QString domain = url.host();
    if (domain.isEmpty()) {
        return urlString;
    }

    if (domain.startsWith("www.", Qt::CaseInsensitive)) {
        domain.remove(0, 4);
    }

    if (useHostName || domain.contains(QRegExp("^[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+$"))) {
        return domain.append(appendString).append(":");
    }
    else {
        QStringList parts = domain.split(".");
        if (parts.size() >= 3) {
            parts.removeFirst();
            return parts.join(".").append(appendString).append(":");
        }
        else {
            return domain.append(appendString).append(":");
        }
    }
}

void TabManagerWidget::delayedRefreshTree(WebPage* p)
{
    if (m_refreshBlocked || m_waitForRefresh) {
        return;
    }

    if (m_isRefreshing && !p) {
        return;
    }

    m_webPage = p;
    m_waitForRefresh = true;
    QTimer::singleShot(50, this, SLOT(refreshTree()));
}

void TabManagerWidget::refreshTree()
{
    if (m_refreshBlocked) {
        return;
    }

    if (m_isRefreshing && !m_webPage) {
        return;
    }

    // store selected items
    QList<QWidget*> selectedTabs;
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i) {
        QTreeWidgetItem* winItem = ui->treeWidget->topLevelItem(i);
        if (winItem->checkState(0) == Qt::Unchecked) {
            continue;
        }

        for (int j = 0; j < winItem->childCount(); ++j) {
            QTreeWidgetItem* tabItem = winItem->child(j);
            if (tabItem->checkState(0) == Qt::Unchecked) {
                continue;
            }
            selectedTabs << qvariant_cast<QWidget*>(tabItem->data(0, WebTabPointerRole));
        }
    }

    ui->treeWidget->clear();

    if (m_groupType == GroupByHost) {
        groupByDomainName(true);
    }
    else if (m_groupType == GroupByDomain) {
        groupByDomainName();
    }
    else { // fallback to GroupByWindow
        m_groupType = GroupByWindow;
        groupByWindow();
    }

    // restore selected items
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i) {
        QTreeWidgetItem* winItem = ui->treeWidget->topLevelItem(i);

        for (int j = 0; j < winItem->childCount(); ++j) {
            QTreeWidgetItem* tabItem = winItem->child(j);

            if (selectedTabs.contains(qvariant_cast<QWidget*>(tabItem->data(0, WebTabPointerRole)))) {
                tabItem->setCheckState(0, Qt::Checked);
            }
        }
    }

    ui->treeWidget->expandAll();
    m_isRefreshing = false;
    m_waitForRefresh = false;
}

void TabManagerWidget::itemDoubleClick(QTreeWidgetItem* item, int)
{
    if (!item) {
        return;
    }

    BrowserWindow* mainWindow = qobject_cast<BrowserWindow*>(qvariant_cast<QWidget*>(item->data(0, QupZillaPointerRole)));
    QWidget* tabWidget = qvariant_cast<QWidget*>(item->data(0, WebTabPointerRole));

    if (!mainWindow) {
        return;
    }

    if (mainWindow->isMinimized()) {
        mainWindow->showNormal();
    }
    else {
        mainWindow->show();
    }
    mainWindow->activateWindow();
    mainWindow->raise();
    mainWindow->weView()->setFocus();

    if (tabWidget && tabWidget != mainWindow->tabWidget()->currentWidget()) {
        mainWindow->tabWidget()->setCurrentIndex(mainWindow->tabWidget()->indexOf(tabWidget));
    }
}

void TabManagerWidget::treeWidgetChanged()
{
    bool enabled = false;
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i) {
        QTreeWidgetItem* parentItem = ui->treeWidget->topLevelItem(i);
        if (parentItem->checkState(0) != Qt::Unchecked) {
            enabled = true;
            break;
        }
    }

    ui->closeSelection->setEnabled(enabled);
    ui->detachSelection->setEnabled(enabled);
    ui->bookmarkSelection->setEnabled(enabled);
}

void TabManagerWidget::processActions()
{
    if (!sender()) {
        return;
    }

    m_refreshBlocked = true;

    QHash<BrowserWindow*, WebTab*> selectedTabs;

    const QString &command = sender()->objectName();

    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i) {
        QTreeWidgetItem* winItem = ui->treeWidget->topLevelItem(i);
        if (winItem->checkState(0) == Qt::Unchecked) {
            continue;
        }

        for (int j = 0; j < winItem->childCount(); ++j) {
            QTreeWidgetItem* tabItem = winItem->child(j);
            if (tabItem->checkState(0) == Qt::Unchecked) {
                continue;
            }

            BrowserWindow* mainWindow = qobject_cast<BrowserWindow*>(qvariant_cast<QWidget*>(tabItem->data(0, QupZillaPointerRole)));
            WebTab* webTab = qobject_cast<WebTab*>(qvariant_cast<QWidget*>(tabItem->data(0, WebTabPointerRole)));

            // current supported actions are not applied to pinned tabs
            if (webTab->isPinned()) {
                tabItem->setCheckState(0, Qt::Unchecked);
                continue;
            }

            if (command == "closeSelection") {
                if (webTab->url().toString() == "qupzilla:restore") {
                    continue;
                }
                selectedTabs.insertMulti(mainWindow, webTab);
            }
            else if (command == "detachSelection" || command == "bookmarkSelection") {
                selectedTabs.insertMulti(mainWindow, webTab);
            }
        }
        winItem->setCheckState(0, Qt::Unchecked);
    }

    if (!selectedTabs.isEmpty()) {
        if (command == "closeSelection") {
            closeSelectedTabs(selectedTabs);
        }
        else if (command == "detachSelection") {
            detachSelectedTabs(selectedTabs);
        }
        else if (command == "bookmarkSelection") {
            bookmarkSelectedTabs(selectedTabs);
        }
    }

    m_refreshBlocked = false;
    delayedRefreshTree();
}

void TabManagerWidget::changeGroupType(int type)
{
    if (m_groupType != GroupType(type)) {
        m_groupType = GroupType(type);

        delayedRefreshTree();

        emit groupTypeChanged(GroupType(type));
    }
}

void TabManagerWidget::closeSelectedTabs(const QHash<BrowserWindow*, WebTab*> &tabsHash)
{
    if (tabsHash.isEmpty()) {
        return;
    }

    const QList<BrowserWindow*> &windows = tabsHash.uniqueKeys();
    foreach (BrowserWindow* mainWindow, windows) {
        QList<WebTab*> tabs = tabsHash.values(mainWindow);

        // TabWidget::closeTab() issue: TabWidget::count() is not updated when closing several tabs
        // unfortunately this workaround doesn't respect to "m_dontQuitWithOneTab"
        if (mainWindow->tabWidget()->count() == tabs.size()) {
            if (mApp->windowCount() == 1) {
                mApp->quitApplication();
                return;
            }
            mainWindow->close();
            continue;
        }

        foreach (WebTab* webTab, tabs) {
            mainWindow->tabWidget()->closeTab(webTab->tabIndex());
        }
    }
}

void TabManagerWidget::detachSelectedTabs(const QHash<BrowserWindow*, WebTab*> &tabsHash)
{
    // TODO: use TabWidget::detachTab()
    if (tabsHash.isEmpty() ||
            (tabsHash.uniqueKeys().size() == 1 &&
             tabsHash.size() == tabsHash.keys().at(0)->tabWidget()->count())) {
        return;
    }

    BrowserWindow* newWindow = mApp->createWindow(Qz::BW_OtherRestoredWindow);;
    newWindow->move(mApp->desktop()->availableGeometry(this).topLeft() + QPoint(30, 30));

    const QList<BrowserWindow*> &windows = tabsHash.uniqueKeys();
    foreach (BrowserWindow* mainWindow, windows) {
        const QList<WebTab*> &tabs = tabsHash.values(mainWindow);
        foreach (WebTab* webTab, tabs) {
            mainWindow->tabWidget()->locationBars()->removeWidget(webTab->locationBar());

            disconnect(webTab->webView(), SIGNAL(wantsCloseTab(int)), mainWindow->tabWidget(), SLOT(closeTab(int)));
            disconnect(webTab->webView(), SIGNAL(changed()), mainWindow->tabWidget(), SIGNAL(changed()));
            disconnect(webTab->webView(), SIGNAL(ipChanged(QString)), mainWindow->ipLabel(), SLOT(setText(QString)));

            webTab->detach();
            if (mainWindow && mainWindow->tabWidget()->count() == 0) {
                mainWindow->close();
                mainWindow = 0;
            }

            newWindow->tabWidget()->addView(webTab);
        }
    }
}

bool TabManagerWidget::bookmarkSelectedTabs(const QHash<BrowserWindow*, WebTab*> &tabsHash)
{
    QDialog* dialog = new QDialog(getQupZilla(), Qt::WindowStaysOnTopHint | Qt::MSWindowsFixedSizeDialogHint);
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom, dialog);
    QLabel* label = new QLabel(dialog);
    BookmarksFoldersButton* folderButton = new BookmarksFoldersButton(dialog);

    QDialogButtonBox* box = new QDialogButtonBox(dialog);
    box->addButton(QDialogButtonBox::Ok);
    box->addButton(QDialogButtonBox::Cancel);
    QObject::connect(box, SIGNAL(rejected()), dialog, SLOT(reject()));
    QObject::connect(box, SIGNAL(accepted()), dialog, SLOT(accept()));

    layout->addWidget(label);
    layout->addWidget(folderButton);
    layout->addWidget(box);

    label->setText(tr("Choose folder for bookmarks:"));
    dialog->setWindowTitle(tr("Bookmark Selected Tabs"));

    QSize size = dialog->size();
    size.setWidth(350);
    dialog->resize(size);
    dialog->exec();

    if (dialog->result() == QDialog::Rejected) {
        return false;
    }

    foreach (WebTab* tab, tabsHash) {
        if (!tab->url().isEmpty()) {
            BookmarkItem* bookmark = new BookmarkItem(BookmarkItem::Url);
            bookmark->setTitle(tab->title());
            bookmark->setUrl(tab->url());
            mApp->bookmarks()->addBookmark(folderButton->selectedFolder(), bookmark);
        }
    }

    delete dialog;
    return true;
}

QTreeWidgetItem* TabManagerWidget::createEmptyItem(QTreeWidgetItem* parent, bool addToTree)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(addToTree ? (parent ? parent : ui->treeWidget->invisibleRootItem()) : 0);
    item->setFlags(item->flags() | (parent ? Qt::ItemIsUserCheckable : Qt::ItemIsUserCheckable | Qt::ItemIsTristate));
    item->setCheckState(0, Qt::Unchecked);

    return item;
}

void TabManagerWidget::groupByDomainName(bool useHostName)
{
    QList<BrowserWindow*> windows = mApp->windows();
    int currentWindowIdx = windows.indexOf(getQupZilla());
    if (currentWindowIdx == -1) {
        // getQupZilla() instance is closing
        return;
    }
    windows.move(currentWindowIdx, 0);

    QMap<QString, QTreeWidgetItem*> tabsGroupedByDomain;

    for (int win = 0; win < windows.count(); ++win) {
        BrowserWindow* mainWin = windows.at(win);

        QList<WebTab*> tabs = mainWin->tabWidget()->allTabs();

        for (int tab = 0; tab < tabs.count(); ++tab) {
            WebTab* webTab = tabs.at(tab);
            if (webTab->webView() && m_webPage == webTab->webView()->page()) {
                m_webPage = 0;
                continue;
            }
            QString domain = domainFromUrl(webTab->url(), useHostName);

            if (!tabsGroupedByDomain.contains(domain)) {
                QTreeWidgetItem* groupItem = createEmptyItem(0, false);
                groupItem->setText(0, domain);
                groupItem->setToolTip(0, domain);
                QFont font = groupItem->font(0);
                font.setBold(true);
                groupItem->setFont(0, font);
                tabsGroupedByDomain.insert(domain, groupItem);
            }
            QTreeWidgetItem* groupItem = tabsGroupedByDomain.value(domain);

            QTreeWidgetItem* tabItem = createEmptyItem(groupItem);
            if (webTab == mainWin->weView()->webTab()) {
                QFont font = tabItem->font(0);
                font.setBold(true);
                tabItem->setFont(0, font);
            }
            if (!webTab->isLoading()) {
                if (!webTab->isPinned()) {
                    tabItem->setIcon(0, webTab->icon());
                }
                else {
                    tabItem->setIcon(0, QIcon(":tabmanager/data/tab-pinned.png"));
                }
            }
            else {
                tabItem->setIcon(0, QIcon(":tabmanager/data/tab-loading.png"));
            }
            tabItem->setText(0, webTab->title());
            tabItem->setToolTip(0, webTab->title());

            tabItem->setData(0, WebTabPointerRole, QVariant::fromValue(qobject_cast<QWidget*>(webTab)));
            tabItem->setData(0, QupZillaPointerRole, QVariant::fromValue(qobject_cast<QWidget*>(mainWin)));

            connect(webTab->webView()->page(), SIGNAL(loadFinished(bool)), this, SLOT(delayedRefreshTree()));
        }
    }

    ui->treeWidget->insertTopLevelItems(0, tabsGroupedByDomain.values());
}

void TabManagerWidget::groupByWindow()
{
    QList<BrowserWindow*> windows = mApp->windows();
    int currentWindowIdx = windows.indexOf(getQupZilla());
    if (currentWindowIdx == -1) {
        return;
    }
    m_isRefreshing = true;

    if (!m_isDefaultWidget) {
        windows.move(currentWindowIdx, 0);
        currentWindowIdx = 0;
    }

    for (int win = 0; win < windows.count(); ++win) {
        BrowserWindow* mainWin = windows.at(win);
        QTreeWidgetItem* winItem = createEmptyItem();
        winItem->setText(0, tr("Window %1").arg(QString::number(win + 1)));
        winItem->setToolTip(0, tr("Double click to switch"));
        if (win == currentWindowIdx) {
            QFont font = winItem->font(0);
            font.setBold(true);
            winItem->setFont(0, font);
        }

        winItem->setData(0, QupZillaPointerRole, QVariant::fromValue(qobject_cast<QWidget*>(mainWin)));
        QList<WebTab*> tabs = mainWin->tabWidget()->allTabs();

        for (int tab = 0; tab < tabs.count(); ++tab) {
            WebTab* webTab = tabs.at(tab);
            if (webTab->webView() && m_webPage == webTab->webView()->page()) {
                m_webPage = 0;
                continue;
            }
            QTreeWidgetItem* tabItem = createEmptyItem(winItem);
            if (webTab == mainWin->weView()->webTab()) {
                QFont font = tabItem->font(0);
                font.setBold(true);
                tabItem->setFont(0, font);
            }
            if (!webTab->isLoading()) {
                if (!webTab->isPinned()) {
                    tabItem->setIcon(0, webTab->icon());
                }
                else {
                    tabItem->setIcon(0, QIcon(":tabmanager/data/tab-pinned.png"));
                }
            }
            else {
                tabItem->setIcon(0, QIcon(":tabmanager/data/tab-loading.png"));
            }
            tabItem->setText(0, webTab->title());
            tabItem->setToolTip(0, webTab->title());

            tabItem->setData(0, WebTabPointerRole, QVariant::fromValue(qobject_cast<QWidget*>(webTab)));
            tabItem->setData(0, QupZillaPointerRole, QVariant::fromValue(qobject_cast<QWidget*>(mainWin)));

            connect(webTab->webView()->page(), SIGNAL(loadFinished(bool)), this, SLOT(delayedRefreshTree()));
        }
    }
}

BrowserWindow* TabManagerWidget::getQupZilla()
{
    if (m_isDefaultWidget || !p_QupZilla) {
        return mApp->getWindow();
    }
    else {
        return p_QupZilla.data();
    }
}
