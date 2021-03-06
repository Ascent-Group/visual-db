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

#include <gui/AppearancePage.h>
#include "AppearancePagePlugin.h"

AppearancePagePlugin::AppearancePagePlugin(QObject *ipParent)
    : QObject(ipParent)
{
    initialized = false;
}

void
AppearancePagePlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized) {
    return;
    }

    initialized = true;
}


bool
AppearancePagePlugin::isInitialized() const
{
    return initialized;
}

QWidget*
AppearancePagePlugin::createWidget(QWidget *ipParent)
{
    return new AppearancePage(ipParent);
}

QString
AppearancePagePlugin::name() const
{
    return "AppearancePage";
}

QString
AppearancePagePlugin::group() const
{
    return "VDB Widgets [Ascent]";
}

QIcon
AppearancePagePlugin::icon() const
{
    return QIcon();
}

QString
AppearancePagePlugin::toolTip() const
{
    return "";
}


QString
AppearancePagePlugin::whatsThis() const
{
    return "";
}

bool
AppearancePagePlugin::isContainer() const
{
    return false;
}

QString
AppearancePagePlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
    " <widget class=\"AppearancePage\" name=\"mAppearancePage\">\n"
    "  <property name=\"geometry\">\n"
    "   <rect>\n"
    "    <x>0</x>\n"
    "    <y>0</y>\n"
    "    <width>100</width>\n"
    "    <height>100</height>\n"
    "   </rect>\n"
    "  </property>\n"
    "  <property name=\"toolTip\" >\n"
    "   <string>Appearance page</string>\n"
    "  </property>\n"
    "  <property name=\"whatsThis\" >\n"
    "   <string>The widget is used in the options dialog for apperance features.</string>\n"
    "  </property>\n"
    " </widget>\n"
    "</ui>\n";
}

QString
AppearancePagePlugin::includeFile() const
{
    return "<gui/AppearancePage.h>";
}


Q_EXPORT_PLUGIN2(appearancepageplugin, AppearancePagePlugin)

