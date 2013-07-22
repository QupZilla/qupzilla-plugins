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
#ifndef TABMANAGERWIDGET_H
#define TABMANAGERWIDGET_H

#include <QWidget>
#include <QPointer>
#include <QHash>

namespace Ui
{
class TabManagerWidget;
}
class QUrl;
class QTreeWidgetItem;
class QupZilla;
class WebPage;
class WebTab;

class TabManagerWidget : public QWidget
{
    Q_OBJECT

public:
    enum GroupType {
        GroupByWindow = 0,
        GroupByDomain = 1,
        GroupByHost = 2
    };

    explicit TabManagerWidget(QupZilla* mainClass, QWidget* parent = 0, bool defaultWidget = false);
    ~TabManagerWidget();

    void closeSelectedTabs(const QHash<QupZilla *, WebTab *> &tabsHash);
    void detachSelectedTabs(const QHash<QupZilla *, WebTab *> &tabsHash);
    void bookmarkSelectedTabs(const QHash<QupZilla *, WebTab *> &tabsHash);

    void setGroupType(GroupType type);

    static QString domainFromUrl(const QUrl &url, bool useHostName = false);

public slots:
    void delayedRefreshTree(WebPage* p = 0);
    void changeGroupType(int type);

private:
    QTreeWidgetItem* createEmptyItem(QTreeWidgetItem* parent = 0, bool addToTree = true);
    void groupByDomainName(bool useHostName = false);
    void groupByWindow();
    QupZilla* getQupZilla();

    Ui::TabManagerWidget* ui;
    QPointer<QupZilla> p_QupZilla;
    WebPage* m_webPage;

    bool m_isRefreshing;
    bool m_refreshBlocked;
    bool m_waitForRefresh;
    bool m_isDefaultWidget;
    GroupType m_groupType;

private slots:
    void refreshTree();
    void processActions();
    void itemDoubleClick(QTreeWidgetItem* item, int);
    void treeWidgetChanged();

signals:
    void showSideBySide();
    void groupTypeChanged(TabManagerWidget::GroupType);
};

#endif // TABMANAGERWIDGET_H
