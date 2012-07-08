/* ============================================================
* GreaseMonkey plugin for QupZilla
* Copyright (C) 2012  David Rosca <nowrep@gmail.com>
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
#include "gm_urlmatcher.h"

#include <QDebug>

GM_UrlMatcher::GM_UrlMatcher(const QString &pattern)
    : m_pattern(pattern)
    , m_useRegExp(false)
    , m_useContains(false)
    , m_useStartsWith(false)
    , m_useEndsWith(false)
{
    parsePattern(m_pattern);
}

QString GM_UrlMatcher::pattern() const
{
    return m_pattern;
}

bool GM_UrlMatcher::match(const QString &urlString) const
{
    if (m_useContains) {
        return urlString.contains(m_matchString);
    }
    else if (m_useStartsWith) {
        return urlString.startsWith(m_matchString);
    }
    else if (m_useEndsWith) {
        return urlString.endsWith(m_matchString);
    }
    else {
        return m_regExp.indexIn(urlString) != -1;
    }
}

void GM_UrlMatcher::parsePattern(QString pattern)
{
    if (pattern.startsWith('/') && pattern.endsWith('/')) {
        pattern = pattern.mid(1);
        pattern = pattern.left(pattern.size() - 1);

        m_regExp = QRegExp(pattern, Qt::CaseInsensitive);
        m_useRegExp = true;
        return;
    }

    bool containTld = pattern.contains(".tld");
    bool wildcardEnd = pattern.endsWith('*');
    bool wildcardStart = pattern.startsWith('*');
    int wildcardCount = pattern.count('*');

    if (!containTld && wildcardEnd && wildcardCount == 1) {
        m_useStartsWith = true;
        m_matchString = pattern.left(pattern.size() - 1);
        return;
    }

    if (!containTld && wildcardStart && wildcardCount == 1) {
        m_useEndsWith = true;
        m_matchString = pattern.mid(1);
        return;
    }

    if (!containTld && wildcardStart && wildcardEnd && wildcardCount == 2) {
        pattern = pattern.mid(1);
        pattern = pattern.left(pattern.size() - 1);

        m_useContains = true;
        m_matchString = pattern;
        return;
    }

    pattern.replace(QRegExp("(\\W)"), "\\\\1")
    .replace(QRegExp("\\*+"), "*")
    .replace(QRegExp("^\\\\\\|"), "^")
    .replace(QRegExp("\\\\\\|$"), "$")
    .replace(QRegExp("\\\\\\*"), ".*")
    .replace("\\.tld", "\\.[a-z.]{2,6}");

    m_useRegExp = true;
    m_regExp = QRegExp(pattern, Qt::CaseInsensitive);
}
