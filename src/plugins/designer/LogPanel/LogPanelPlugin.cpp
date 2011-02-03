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

#include <gui/LogPanel.h>
#include "LogPanelPlugin.h"

LogPanelPlugin::LogPanelPlugin(QObject *ipParent)
    : QObject(ipParent)
{
    initialized = false;
}

void
LogPanelPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized) {
        return;
    }

    initialized = true;
}


bool
LogPanelPlugin::isInitialized() const
{
    return initialized;
}

QWidget*
LogPanelPlugin::createWidget(QWidget *ipParent)
{
    using namespace Gui;
    return new LogPanel(ipParent);
}

QString
LogPanelPlugin::name() const
{
    return "LogPanel";
}

QString
LogPanelPlugin::group() const
{
    return "VDB Widgets [Ascent]";
}

QIcon
LogPanelPlugin::icon() const
{
    return QIcon();
}

QString
LogPanelPlugin::toolTip() const
{
    return "";
}


QString
LogPanelPlugin::whatsThis() const
{
    return "";
}

bool
LogPanelPlugin::isContainer() const
{
    return false;
}

QString
LogPanelPlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
    " <widget class=\"LogPanel\" name=\"mLogPanel\">\n"
    "  <property name=\"geometry\">\n"
    "   <rect>\n"
    "    <x>0</x>\n"
    "    <y>0</y>\n"
    "    <width>100</width>\n"
    "    <height>100</height>\n"
    "   </rect>\n"
    "  </property>\n"
    "  <property name=\"toolTip\" >\n"
    "   <string>Log panel</string>\n"
    "  </property>\n"
    "  <property name=\"whatsThis\" >\n"
    "   <string>The log panel widget is used for displaying apps output.</string>\n"
    "  </property>\n"
    " </widget>\n"
    "</ui>\n";
}

QString
LogPanelPlugin::includeFile() const
{
    return "<gui/LogPanel.h>";
}


Q_EXPORT_PLUGIN2(logpanelplugin, LogPanelPlugin)

