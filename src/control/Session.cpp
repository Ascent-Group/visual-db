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
    : mXmlDoc("VisualDB")
{
    QDomElement rootElement = mXmlDoc.createElement("visual-db");
    mXmlDoc.appendChild(rootElement);
    
    mSessionElement = mXmlDoc.createElement("session");
    rootElement.appendChild(mSessionElement);
}

Session::~Session()
{
}

quint32
Session::countConnections() const
{
    return mXmlDoc.elementsByTagName("connection").size();
}

bool
Session::save(const QString &iFileName)
{
    // open file
    QFile file(iFileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qCritical() << "[Error] while opening file " << iFileName << " for writing!";
        return false;
    }

    // calculate total number of sessions
    mSessionElement.setAttribute("count", mXmlDoc.elementsByTagName("connection").size());

    // flush xml to file
    QTextStream stream(&file);
    stream << mXmlDoc.toString();
    file.close();

    return true;
}

bool
Session::saveConnectionInfo(const Connect::ConnectionInfo &iConnectionInfo, quint32 iId)
{
    QDomElement connection = xmlConnection(iId);
    iConnectionInfo.toXml(mXmlDoc, connection);
    return true;
}

bool
Session::saveScene(const Gui::GraphicsScene &iGraphicsScene, quint32 iId)
{
    QDomElement connection = xmlConnection(iId);
    iGraphicsScene.toXml(mXmlDoc, connection);
    return true;
}

bool
Session::load(const QString &iFileName)
{
    QFile file(iFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "[Error] while opening file " << iFileName << " for writing!";
        return false;
    }

    if (!mXmlDoc.setContent(&file)) {
        qCritical() << "[Error] while loading xml structure from file " << iFileName;
        file.close();
        return false;
    }
    file.close();

    return true;
}

bool
Session::loadConnectionInfo(Connect::ConnectionInfo &oConnectionInfo, quint32 iId) const
{
    for (qint32 i = 0; i < mXmlDoc.elementsByTagName("connection").size(); ++i) {
        QDomElement connection = mXmlDoc.elementsByTagName("connection").at(i).toElement();

        if (!connection.isNull() && connection.attribute("id").toUInt() == iId) {
            QDomElement node = connection.firstChild().toElement();
            while (!node.isNull()) {
                if (node.tagName() == "connectionInfo") {
                    oConnectionInfo.fromXml(node);
                    return true;
                }
            }
            node = node.nextSibling().toElement();
        }
    }

    qCritical() << "[Error] Could not found connection info for connection with id (" << iId << ")";
    return false;
}

bool
Session::loadScene(Gui::GraphicsScene &oGraphicsScene, quint32 iId) const
{
    for (qint32 i = 0; i < mXmlDoc.elementsByTagName("scene").size(); ++i) {
        QDomElement connection = mXmlDoc.elementsByTagName("scene").at(i).toElement();

        if (!connection.isNull() && connection.attribute("id").toUInt() == iId) {
            QDomElement node = connection.firstChild().toElement();
            while (!node.isNull()) {
                if (node.tagName() == "scene") {
                    oGraphicsScene.fromXml(node);
                    return true;
                }
            }
            node = node.nextSibling().toElement();
        }
    }

    qCritical() << "[Error] Could not found scene for connection with id (" << iId << ")";
    return false;
}

QDomElement
Session::xmlConnection(quint32 iId)
{
    for (qint32 i = 0; i < mXmlDoc.elementsByTagName("connection").size(); ++i) {
        QDomElement connection = mXmlDoc.elementsByTagName("connection").at(i).toElement();

        if (!connection.isNull() && connection.attribute("id").toUInt() == iId) {
            return connection;
        }
    }

    QDomElement connection = mXmlDoc.createElement("connection");
    connection.setAttribute("id", iId);
    mSessionElement.appendChild(connection);

    return connection;
}

bool
Session::save(const QString &iSessionFile, const QList<Connect::ConnectionInfo> &iConnectionInfoList, 
        const QList<Gui::GraphicsScene> &iGraphicsSceneList)
{
    if (iConnectionInfoList.size() != iGraphicsSceneList.size()) {
        return false;
    }

    QFile file;
    file.setFileName(iSessionFile);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QDomDocument doc("VisualDB");
    QDomElement root = doc.createElement("visual-db");
    doc.appendChild(root);

    for (int i = 0; i < iConnectionInfoList.size(); ++i) {
        iConnectionInfoList.at(i).toXml(doc, root);
        iGraphicsSceneList.at(i).toXml(doc, root);
    }

    QTextStream stream(&file);
    stream << doc.toString();
    file.close();

    return true;
}

bool
Session::load(const QString &iSessionFile, QList<Connect::ConnectionInfo> &oConnectionInfoList,
        QList<Gui::GraphicsScene> &oGraphicsSceneList)
{
    using namespace Connect;
    using namespace Gui;

    oConnectionInfoList.clear();
    oGraphicsSceneList.clear();

    QFile file;
    file.setFileName(iSessionFile);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QDomDocument doc("VisualDB");
    if (!doc.setContent(&file)) {
        file.close();
        return false;
    }
    file.close();

    QDomElement docElem = doc.documentElement();
    QDomNode child = docElem.firstChild();
    while (!child.isNull()) {
        QDomElement element = child.toElement(); // try to convert the node to an element.
        if (!element.isNull()) {
            if (element.tagName() == "connectionInfo") {
                ConnectionInfo connectionInfo;
                connectionInfo.fromXml(element);
                oConnectionInfoList.append(connectionInfo);
            }
            if (element.tagName() == "scene") {
                GraphicsScene graphicsScene;
                graphicsScene.fromXml(element);
                oGraphicsSceneList.append(graphicsScene);
            }
        }
        child = child.nextSibling();
    }

    return true;
}

//Session &
//operator<<(Session &iSession, const Connect::ConnectionInfo &iConnectionInfo)
//{
//    iSession.saveConnectionInfo(iConnectionInfo);
//    return iSession;
//}
//
//Session &
//operator>>(Session &iSession, Connect::ConnectionInfo &oConnectionInfo)
//{
//    iSession.loadConnectionInfo(oConnectionInfo);
//    return iSession;
//}
//
//Session &
//operator<<(Session &iSession, const Gui::GraphicsScene &iGraphicsScene)
//{
//    iSession.saveScene(iGraphicsScene);
//    return iSession;
//}
//
//Session &
//operator>>(Session &iSession, Gui::GraphicsScene &oGraphicsScene)
//{
//    iSession.loadScene(oGraphicsScene);
//    return iSession;
//}

}
