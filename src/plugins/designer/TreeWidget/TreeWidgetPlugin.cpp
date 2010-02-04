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

#include <gui/TreeWidget.h>
#include "TreeWidgetPlugin.h"

TreeWidgetPlugin::TreeWidgetPlugin(QObject *ipParent)
    : QObject(ipParent)
{
    initialized = false;
}

void
TreeWidgetPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized) {
    return;
    }

    initialized = true;
}


bool
TreeWidgetPlugin::isInitialized() const
{
    return initialized;
}

QWidget*
TreeWidgetPlugin::createWidget(QWidget *ipParent)
{
    return new TreeWidget(ipParent);
}

QString
TreeWidgetPlugin::name() const
{
    return "TreeWidget";
}

QString
TreeWidgetPlugin::group() const
{
    return "VDB Widgets [Ascent]";
}

QIcon
TreeWidgetPlugin::icon() const
{
    return QIcon();
}

QString
TreeWidgetPlugin::toolTip() const
{
    return "";
}


QString
TreeWidgetPlugin::whatsThis() const
{
    return "";
}

bool
TreeWidgetPlugin::isContainer() const
{
    return false;
}

QString
TreeWidgetPlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
    " <widget class=\"TreeWidget\" name=\"mTreeWidget\">\n"
    "  <property name=\"geometry\">\n"
    "   <rect>\n"
    "    <x>0</x>\n"
    "    <y>0</y>\n"
    "    <width>100</width>\n"
    "    <height>100</height>\n"
    "   </rect>\n"
    "  </property>\n"
    "  <property name=\"toolTip\" >\n"
    "   <string>Tree widget</string>\n"
    "  </property>\n"
    "  <property name=\"whatsThis\" >\n"
    "   <string>The tree widget is used for displaying db objects.</string>\n"
    "  </property>\n"
    " </widget>\n"
    "</ui>\n";
}

QString
TreeWidgetPlugin::includeFile() const
{
    return "<gui/TreeWidget.h>";
}


Q_EXPORT_PLUGIN2(treewidgetplugin, TreeWidgetPlugin)
