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

#ifndef CONSTS_H
#define CONSTS_H

#include <QString>

namespace Consts
{
    // begin Appearance group
    /*! Name of a group of settings concerning UI appearance */
    extern const QString APPEARANCE_GRP;

    /*! Name of a language setting */
    extern const QString LANG_SETTING;
    // end Appearance group

    // begin Preferences group
    /*! Name of a group of settings concerning preferences and app behaviour */
    extern const QString PREFS_GRP;

    /*! Name of a setting that holds the path where all sessions are saved */
    extern const QString SESSION_DIR_SETTING;
    /*! Name of a setting that indicaties whether a new tab should be switched to or not */
    extern const QString NEW_TAB_AUTO_SWITCH_SETTING;
    /*!
     * Name of a setting that indicates whether last saved session should be restored on
     * startup or not
     */
    extern const QString LOAD_LAST_SESSION_SETTING;
    /*! Name of a setting that defines the maximum number of sessions to save */
    extern const QString SAVED_SESSIONS_NUMBER_SETTING;
    /*! Name of a setting that indicates whether indices should be shown or not */
    extern const QString SHOW_INDICES_SETTING;
    // end Preferences group

    // begin Color group
    /*! Name of a group of settings concerning colors */
    extern const QString COLOR_GRP;

    /*! Name of a setting that defines the background color */
    extern const QString BACKGROUND_SETTING;
    /*! Name of a setting that defines the table color */
    extern const QString TABLE_SETTING;
    /*! Name of a setting that defines the font color */
    extern const QString FONT_SETTING;
    /*! Name of a setting that defines the arrow color */
    extern const QString ARROW_SETTING;
    /*! Name of a setting that defines the border color */
    extern const QString BORDER_SETTING;
    // end Preferences group

    // begin group View
    /*! Name of a group of settings concerning graphics scene view */
    extern const QString VIEW_GRP;

    /*!
     * Name of a setting that indicates whether db objects should be aligned to grid on
     * the graphics scene ot not
     */
    extern const QString ALIGN_TO_GRID_SETTING;
    /*! Name of a setting that indicates whether the grid should be shown on scene or not */
    extern const QString SHOW_GRID_SETTING;
    /*! Name of a setting that indicates whether the scene should be divided into pages or not */
    extern const QString DIVIDE_INTO_PAGES_SETTING;
    // end group View

    // begin group LastSession
    /*! Name of a group of settings concerning last session */
    extern const QString LAST_SESSION_GRP;

    /*! Name of a setting that holds the path to a saved session description file*/
    extern const QString SAVED_SESSION_SETTING;
    // \todo All settings below will be removed. We will save this parameters in xml file
    /*! Name of a setting that holds a name of db driver that has been used during last session */
    extern const QString DB_DRV_SETTING;
    /*! Name of a setting that holds a name of db explored during last session */
    extern const QString DB_NAME_SETTING;
    /*! Name of a setting that holds a username used to connect to db during last session */
    extern const QString DB_USER_SETTING;
    /*! Name of a setting that holds user's password used to connect to db during last session */
    extern const QString DB_PASS_SETTING;
    /*! Name of a setting that holds host address used to connect to db during last session*/
    extern const QString DB_HOST_SETTING;
    /*! Name of a setting that holds port number used to connect to db during last session */
    extern const QString DB_PORT_SETTING;
    /*! Name of a setting that indicates whether a proxy has been used or not */
    extern const QString USE_PROXY_SETTING;
    /*! Name of a setting that holds proxy's type */
    extern const QString PROXY_TYPE_SETTING;
    /*! Name of a setting that holds proxy's host address*/
    extern const QString PROXY_HOST_SETTING;
    /*! Name of a setting that holds proxy's port number */
    extern const QString PROXY_PORT_SETTING;
    /*! Name of a setting that holds proxy's username */
    extern const QString PROXY_USER_SETTING;
    // end group LastSession

    /*! Extension of a file with a saed session */
    extern const QString SESSION_FILE_EXT;
} // namespace Consts

#endif // CONSTS_H

