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

#ifndef CONTROL_CONFIG_H
#define CONTROL_CONFIG_H

#include <QColor>
#include <QLocale>
#include <QSettings>

namespace Control
{

/*!
 * \class Config
 * \headerfile control/Config.h
 * \brief Class for settings management.
 */
class Config
{
    public:
        Config();
        ~Config();

        QLocale::Language language() const;
        void setLanguage(QLocale::Language);

        // \todo add getters for default setting values

        QString sessionDir() const;
        void setSessionDir(const QString &);

        bool newTabAutoSwitch() const;
        void setNewTabAutoSwitch(bool);

        bool loadLastSession() const;
        void setLoadLastSession(bool);

        quint32 savedSessionsNumber() const;
        void setSavedSessionsNumber(quint32);

        bool showIndices() const;
        void setShowIndices(bool);

        QColor backgroundColor() const;
        void setBackgroundColor(const QColor &);

        QColor tableColor() const;
        void setTableColor(const QColor &);

        QColor fontColor() const;
        void setFontColor(const QColor &);

        QColor arrowColor() const;
        void setArrowColor(const QColor &);

        QColor borderColor() const;
        void setBorderColor(const QColor &);

        bool alignToGrid() const;
        void setAlignToGrid(bool);

        bool showGrid() const;
        void setShowGrid(bool);

        bool divideIntoPages() const;
        void setDivideIntoPages(bool);

        QString savedSession(qint32) const;
        qint32 setSavedSession(const QString &, qint32 iIndex = -1);

    private:
        QSettings mSettings;

        Config(const Config &);
        Config& operator=(const Config &);
};

} // namespace Control

#endif // CONTROL_CONFIG_H

