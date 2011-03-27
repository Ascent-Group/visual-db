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

#ifndef CONTROL_SESSION_H
#define CONTROL_SESSION_H

#include <connect/ConnectionInfo.h>
#include <gui/GraphicsScene.h>
#include <QFile>

namespace Control {

class Session {

    public:
        explicit Session(const QString &iSessionFile);
        virtual ~Session();

        bool setSessionFile(const QString &iSessionFile);
        QString sessionFile() const;

        bool startWriting();
        bool stopWriting();
        bool saveConnectionInfo(const Connect::ConnectionInfo &iConnectionInfo);
        bool saveScene(const Gui::GraphicsScene &iGraphicsScene);

        bool startReading();
        bool stopReading();
        bool loadConnectionInfo(Connect::ConnectionInfo &oConnectionInfo);
        bool loadScene(Gui::GraphicsScene &oGraphicsScene);

        static bool save(const QString &iSessionFile, const QList<Connect::ConnectionInfo> &iConnectionInfoList, 
                    const QList<Gui::GraphicsScene> &iGraphicsSceneList);

        static bool load(const QString &iSessionFile, QList<Connect::ConnectionInfo> &oConnectionInfoList,
                    QList<Gui::GraphicsScene> &oGraphicsSceneList);

    private:
        QString mSessionFile;
        QFile mFile;
        QDomDocument mXmlDoc;
        QDomElement mRootXmlElement;
        bool mWasWritingStarted;
        bool mWasReadingStarted;
};

}

#endif // CONTROL_SESSION_H
