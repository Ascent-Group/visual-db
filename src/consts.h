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

/*!
 * \namespace Consts
 * \brief
 * \todo comment this
 */
namespace Consts
{
    // begin Appearance group
    extern const QString APPEARANCE_GRP;

    extern const QString LANG_SETTING;
    // end Appearance group

    // begin Preferences group
    extern const QString PREFS_GRP;

    extern const QString SESSION_DIR_SETTING;
    extern const QString NEW_TAB_AUTO_SWITCH_SETTING;
    extern const QString LOAD_LAST_SESSION_SETTING;
    extern const QString COUNT_SAVED_SESSIONS_SETTING;
    extern const QString SHOW_INDICES_SETTING;
    // end Preferences group

    // begin Color group
    extern const QString COLOR_GRP;

    extern const QString BACKGROUND_SETTING;
    extern const QString TABLE_SETTING;
    extern const QString FONT_SETTING;
    extern const QString ARROW_SETTING;
    extern const QString BORDER_SETTING;
    // end Preferences group

    // begin group View
    extern const QString VIEW_GRP;

    extern const QString ALIGN_TO_GRID_SETTING;
    extern const QString SHOW_GRID_SETTING;
    extern const QString DIVIDE_INTO_PAGES_SETTING;
    // end group View

    // begin group LastSession
    extern const QString LAST_SESSION_GRP;

    extern const QString SAVED_SESSION_SETTING;
    extern const QString DB_DRV_SETTING;
    extern const QString DB_NAME_SETTING;
    extern const QString DB_USER_SETTING;
    extern const QString DB_PASS_SETTING;
    extern const QString DB_HOST_SETTING;
    extern const QString DB_PORT_SETTING;
    extern const QString USE_PROXY_SETTING;
    extern const QString PROXY_TYPE_SETTING;
    extern const QString PROXY_HOST_SETTING;
    extern const QString PROXY_PORT_SETTING;
    extern const QString PROXY_USER_SETTING;
    // end group LastSession

} // namespace Consts

#endif // CONSTS_H

