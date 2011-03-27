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

Session::Session(const QString &iSessionFile)
    : mSessionFile(iSessionFile)
    , mFile(iSessionFile)
    , mXmlDoc("VisualDB")
    , mWasWritingStarted(false)
    , mWasReadingStarted(false)
{
    mRootXmlElement = mXmlDoc.createElement("visual-db");
    mXmlDoc.appendChild(mRootXmlElement);
}

Session::~Session()
{
    mFile.close();
}

bool
Session::setSessionFile(const QString &iSessionFile)
{
    mSessionFile = iSessionFile;
    mFile.setFileName(iSessionFile);
    return QFile::exists(iSessionFile);
}

QString
Session::sessionFile() const
{
    return mSessionFile;
}

bool
Session::startWriting()
{
    if (mWasReadingStarted) {
        qCritical() << "Error: you should call stopReading() method first!";
        return false;
    }

    if (QFile::exists(mSessionFile)) {
        qCritical() << "Error: file " << mSessionFile << " does not exists!";
        return false;
    }

    if (mFile.isOpen()) {
        mFile.close();
    }

    if (!mFile.open(QIODevice::WriteOnly)) {
        qCritical() << "Error while opening file " << mSessionFile << " for writing!";
        return false;
    }

    mWasWritingStarted = true;

    return true;
}

bool
Session::stopWriting()
{
    QTextStream stream(&mFile);
    stream << mXmlDoc.toString();
    mFile.close();

    mWasWritingStarted = false;

    return true;
}

bool
Session::startReading()
{
    if (mWasWritingStarted) {
        qCritical() << "Error: you should call stopWriting() method first!";
        return false;
    }

    if (QFile::exists(mSessionFile)) {
        qCritical() << "Error: file " << mSessionFile << " does not exists!";
        return false;
    }

    if (mFile.isOpen()) {
        mFile.close();
    }

    if (!mFile.open(QIODevice::ReadOnly)) {
        qCritical() << "Error while opening file " << mSessionFile << " for reading!";
        return false;
    }

    mWasReadingStarted = true;

    return true;
}

bool
Session::stopReading()
{
    mFile.close();

    mWasReadingStarted = false;

    return true;
}

bool
Session::saveConnectionInfo(const Connect::ConnectionInfo &iConnectionInfo)
{
    if (!mWasWritingStarted) {
        qCritical() << "Error: you should call startWriting() method first!";
        return false;
    }
    
    iConnectionInfo.toXml(mXmlDoc, mRootXmlElement);
    return true;
}

bool
Session::saveScene(const Gui::GraphicsScene &iGraphicsScene)
{
    if (!mWasWritingStarted) {
        qCritical() << "Error: you should call startWriting() method!";
        return false;   
    }

    iGraphicsScene.toXml(mXmlDoc, mRootXmlElement);
    return true;
}

bool
Session::loadConnectionInfo(Connect::ConnectionInfo &oConnectionInfo)
{
    if (!mWasReadingStarted) {
        qCritical() << "Error: you should call startReading() method!";
        return false;   
    }

    QDomElement docElem = mXmlDoc.documentElement();
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

bool
Session::loadScene(Gui::GraphicsScene &oGraphicsScene)
{
    if (!mWasReadingStarted) {
        qCritical() << "Error: you should call startReading() method!";
        return false;   
    }

    QDomElement docElem = mXmlDoc.documentElement();
    QDomNode child = docElem.firstChild();
    while (!child.isNull()) {
        QDomElement element = child.toElement(); // try to convert the node to an element.
        if (!element.isNull()) {
            if (element.tagName() == "scene") {
                oGraphicsScene.fromXml(element);
                return true;
            }
        }
        child = child.nextSibling();
    }

    return false;
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
            if (element.tagName() == "connection") {
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

}
