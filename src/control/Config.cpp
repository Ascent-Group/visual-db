/*-
 * Copyright (c) 2009, Ascent Group.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
 *     * Neither the name of the Ascent Group nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <consts.h>
#include <control/Config.h>

namespace Control
{

/*!
 * Constructor
 */
Config::Config()
    : mSettings()
{
}

/*!
 * Destructor
 */
Config::~Config()
{
}

/*!
 * Copy constructor
 * \note FORBIDDEN
 */
Config::Config(const Config &)
{
}

/*!
 * Assignment operator
 * \note FORBIDDEN
 */
Config&
Config::operator=(const Config &)
{
    return *this;
}

/*!
 * \return Preset user interface language
 */
QLocale::Language
Config::language() const
{
    quint32 lang = mSettings.value(Consts::APPEARANCE_GRP + "/" + Consts::LANG_SETTING, QLocale::English).toUInt();
    return static_cast<QLocale::Language>(lang);
}

/*!
 * \param[in] iLang - User interface language
 */
void
Config::setLanguage(QLocale::Language iLang)
{
    mSettings.setValue(Consts::APPEARANCE_GRP + "/" + Consts::LANG_SETTING, iLang);
}

/*!
 * \return Preset path for storing session description files.
 */
QString
Config::sessionDir() const
{
    return mSettings.value(Consts::PREFS_GRP + "/" + Consts::SESSION_DIR_SETTING, "./").toString();
}

/*!
 * \param[in] iSessionDir - Directory where we want to store session files.
 */
void
Config::setSessionDir(const QString &iSessionDir)
{
    mSettings.setValue(Consts::PREFS_GRP + "/" + Consts::SESSION_DIR_SETTING, iSessionDir);
}

/*!
 * \note Since this setting is a boolean setting, then we don't keep a member for it. This
 * is because we can't say when it is empty and/or modified. This is true for all boolean
 * settings.
 *
 * \return true - If the new tab should be immediately switched to
 * \return false - Otherwise
 */
bool
Config::newTabAutoSwitch() const
{
    return mSettings.value(Consts::PREFS_GRP + "/" + Consts::NEW_TAB_AUTO_SWITCH_SETTING, true).toBool();
}

/*!
 * \param[in] iFlag - indicates whether we want to automatically switch to a just created
 * tab or not
 *
 * \see Config::newTabAutoSwitch
 */
void
Config::setNewTabAutoSwitch(bool iFlag)
{
    mSettings.setValue(Consts::PREFS_GRP + "/" + Consts::NEW_TAB_AUTO_SWITCH_SETTING, iFlag);
}

/*!
 * \return true - If the last sessiob should be restored at application start.
 */
bool
Config::loadLastSession() const
{
    return mSettings.value(Consts::PREFS_GRP + "/" + Consts::LOAD_LAST_SESSION_SETTING, false).toBool();
}

/*!
 * \param[in] iFlag - Indicates whether the last session should be restored at application
 * start or not.
 */
void
Config::setLoadLastSession(bool iFlag)
{
    mSettings.setValue(Consts::PREFS_GRP + "/" + Consts::LOAD_LAST_SESSION_SETTING, iFlag);
}

/*!
 * \return Number of sessions to save on exit
 */
quint32
Config::savedSessionsNumber() const
{
    return mSettings.value(Consts::PREFS_GRP + "/" + Consts::COUNT_SAVED_SESSIONS_SETTING, 0).toUInt();
}

/*!
 * \param[in] iNum - How many sessions we want to save. If the new number of sessions is
 * less than the one that is currently set, then the redundant entries in the settings
 * file will be removed.
 */
void
Config::setSavedSessionsNumber(quint32 iNum)
{
    // if we reduce the number of sessions then it is a good idea to do a little cleanup
    if (savedSessionsNumber() > iNum) {
        for (quint32 i = iNum; i < savedSessionsNumber(); ++i) {
            mSettings.remove(Consts::LAST_SESSION_GRP + "/" + Consts::SAVED_SESSION_SETTING + "_" + QString().setNum(i));
        }
    }

    mSettings.setValue(Consts::PREFS_GRP + "/" + Consts::COUNT_SAVED_SESSIONS_SETTING, iNum);
}

/*!
 * \return true - If the indices should be displayed
 * \return flase - Otherwise
 */
bool
Config::showIndices() const
{
    return mSettings.value(Consts::PREFS_GRP + "/" + Consts::SHOW_INDICES_SETTING, false).toBool();
}

/*!
 * \param[in] iFlag - Flag to indicate whether indices will be shown or not
 */
void
Config::setShowIndices(bool iFlag)
{
    mSettings.setValue(Consts::PREFS_GRP + "/" + Consts::SHOW_INDICES_SETTING, iFlag);
}

/*!
 * \return Color of table's background
 */
QColor
Config::backgroundColor() const
{
    return mSettings.value(Consts::COLOR_GRP + "/" + Consts::BACKGROUND_SETTING, Qt::white).value<QColor>();
}

/*!
 * \param[in] iColor - background color
 */
void
Config::setBackgroundColor(const QColor &iColor)
{
    mSettings.setValue(Consts::COLOR_GRP + "/" + Consts::BACKGROUND_SETTING, iColor);
}

/*!
 * \return Color of table
 */
QColor
Config::tableColor() const
{
    return mSettings.value(Consts::COLOR_GRP + "/" + Consts::TABLE_SETTING, Qt::white).value<QColor>();
}

/*!
 * \param[in] iColor - Table color
 */
void
Config::setTableColor(const QColor &iColor)
{
    mSettings.setValue(Consts::COLOR_GRP + "/" + Consts::TABLE_SETTING, iColor);
}

/*!
 * \return Font color
 */
QColor
Config::fontColor() const
{
    return mSettings.value(Consts::COLOR_GRP + "/" + Consts::FONT_SETTING, Qt::black).value<QColor>();
}

/*!
 * \param[in] iColor - Font color
 */
void
Config::setFontColor(const QColor &iColor)
{
    mSettings.setValue(Consts::COLOR_GRP + "/" + Consts::FONT_SETTING, iColor);
}

/*!
 * \return Arrow's color
 */
QColor
Config::arrowColor() const
{
    return mSettings.value(Consts::COLOR_GRP + "/" + Consts::ARROW_SETTING, Qt::black).value<QColor>();
}

/*!
 * \param[in] iColor - Arrow's color
 */
void
Config::setArrowColor(const QColor &iColor)
{
    mSettings.setValue(Consts::COLOR_GRP + "/" + Consts::ARROW_SETTING, iColor);
}

/*!
 * \return Table's border color
 */
QColor
Config::borderColor() const
{
    return mSettings.value(Consts::COLOR_GRP + "/" + Consts::BORDER_SETTING, Qt::black).value<QColor>();
}

/*!
 * \param[in] iColor - Table's border color
 */
void
Config::setBorderColor(const QColor &iColor)
{
    mSettings.setValue(Consts::COLOR_GRP + "/" + Consts::BORDER_SETTING, iColor);
}

/*!
 * \return true - If graphics items should be aligned to grid
 * \return false - Otherwise
 */
bool
Config::alignToGrid() const
{
    return mSettings.value(Consts::VIEW_GRP + "/" + Consts::ALIGN_TO_GRID_SETTING, false).toBool();
}

/*!
 * \param[in] iFlag - Flag that indicates whether the item is aligned to grid or not
 */
void
Config::setAlignToGrid(bool iFlag)
{
    mSettings.setValue(Consts::VIEW_GRP + "/" + Consts::ALIGN_TO_GRID_SETTING, iFlag);
}

/*!
 * \return true - If the grid should be displayed on the scene
 * \return false - Otherwise
 */
bool
Config::showGrid() const
{
    return mSettings.value(Consts::VIEW_GRP + "/" + Consts::SHOW_GRID_SETTING, true).toBool();
}

/*!
 * \param[in] iFlag - Flag that indicates whether the grid should be displayed or not
 */
void
Config::setShowGrid(bool iFlag)
{
    mSettings.setValue(Consts::VIEW_GRP + "/" + Consts::SHOW_GRID_SETTING, iFlag);
}

/*!
 * \return true - If the scene should be divided into pages
 * \return false - Otherwise
 */
bool
Config::divideIntoPages() const
{
    return mSettings.value(Consts::VIEW_GRP + "/" + Consts::DIVIDE_INTO_PAGES_SETTING, true).toBool();
}

/*!
 * \param[in] iFlag - Flag that indicates whether the scene should be divided into pages
 * or not
 */
void
Config::setDivideIntoPages(bool iFlag)
{
    mSettings.setValue(Consts::VIEW_GRP + "/" + Consts::DIVIDE_INTO_PAGES_SETTING, iFlag);
}

/*!
 * \param[in] iIndex - Index of the saved session whose vdb file we are trying to get
 *
 * \return Path to the saved session description file, or an empty string if the specified
 * index is incorrect. The index is said to be incorrect when it is >=
 * savedSessionNumber().
 */
QString
Config::savedSession(qint32 iIndex) const
{
    if (savedSessionsNumber() <= iIndex) {
        return QString("");
    }

    return mSettings.value(Consts::LAST_SESSION_GRP + "/" + Consts::SAVED_SESSION_SETTING + "_" + QString().setNum(iIndex)).toString();
}

/*!
 * \param[in] iPath - Path to the session description file
 * \param[in] iIndex - Index of the position where we want to store the path. If iIndex is
 * -1 then the path is stored in the first empty position.
 *
 * \return The number of the position, where the path has been stored which is
 * [0..savedSessionsNumber()]
 */
qint32
Config::setSavedSession(const QString &iPath, qint32 iIndex)
{
    /*!
     * -inf | ... | -1              | 0 | 1 | ... | savedSessionsNumber() | ... | +inf |
     *  -1  | ... | first empty pos | 0 | 1 | ... |          -1           | ... |  -1  |
     *              or -1 if all are|
     *              taken already   |
     *
     */
    quint32 sessionsCount = savedSessionsNumber();
    if (iIndex >= static_cast<qint32>(sessionsCount) || -1 > iIndex) {
        iIndex = -1;
    } else if (-1 == iIndex) {
        QString value;
        do {
            value = mSettings.value(Consts::LAST_SESSION_GRP + "/" + Consts::SAVED_SESSION_SETTING + "_" + QString().setNum(iIndex+1)).toString();
        } while (++iIndex < sessionsCount && !value.isEmpty());

        // no empty position found
        if (iIndex == sessionsCount) {
            iIndex = -1;
        }
    }

    if (0 <= iIndex && iIndex < sessionsCount) {
        mSettings.setValue(Consts::LAST_SESSION_GRP + "/" + Consts::SAVED_SESSION_SETTING + "_" + QString().setNum(iIndex), iPath);
    }

    return iIndex;
}

} // namespace Control

