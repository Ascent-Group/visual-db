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

#include <QDomDocument>
#include <QDomElement>
#include <control/Session.h>

namespace Control {

Session::Session()
{
}

Session::~Session()
{
}

bool Session::setFile(const QString &iFileName)
{
    mFile.close();

    mFile.setFileName(iFileName);
    if (!mFile.open(QIODevice::WriteOnly)) {
        return false;
    }

    return true;
}

bool Session::setConnectionInfo(const Connect::ConnectionInfo &iConnectionInfo)
{
    if (!mFile.isOpen()) {
        return false;
    }

    QDomDocument doc("VisualDB");
    QDomElement root = doc.createElement("visual-db");
    doc.appendChild(root);
    iConnectionInfo.toXml(doc, root);
//    root.appendChild(ui.mSceneWidget->toXml(doc, ui.mShowGridAction->isChecked(), ui.mDivideIntoPagesAction->isChecked(),
//                ui.mShowLegendAction->isChecked(), ui.mShowControlWidgetAction->isChecked()));

    return true;
}

bool Session::getConnectionInfo(Connect::ConnectionInfo &oConnectionInfo) const
{
    if (!mFile.isOpen()) {
        return false;
    }
 
    QDomDocument doc("VisualDB");
    // FIXME: remove const_cast
    if (!doc.setContent(const_cast<QFile *>(&mFile))) {
        return false;
    }

    QDomElement docElem = doc.documentElement();
    QDomNode child = docElem.firstChild();
    while (!child.isNull()) {
        QDomElement element = child.toElement(); // try to convert the node to an element.
        if (!element.isNull()) {
            if (element.tagName() == "connection") {
                oConnectionInfo.fromXml(element);
                return true;
            }
        }
        child = child.nextSibling();
    }
   
    return false;
}

}
