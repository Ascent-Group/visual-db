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

/*!
 * \mainpage VisualDB v1.2
 * \image html MainWindow.png "Application's main window"
 */

#include <QApplication>
#include <QTranslator>
#include <QIcon>

#include <control/Config.h>
#include <gui/MainWindow.h>

int main(int argc, char **argv)
{
    /*
    QTextCodec *codec = QTextCodec::codecForName("cp1251");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    */

    QApplication app(argc, argv);

    // settings intialization
    app.setOrganizationName("Ascent Group");
    app.setOrganizationDomain("sourceforge.net");
    app.setApplicationName("VisualDB");
    app.setWindowIcon(QIcon(":/img/logo.png"));

    // set additional plugins path
    app.addLibraryPath("./lib/");

    const Control::Config cfg;
    QTranslator translator;

    // load qm translation
    switch (cfg.language()) {
        case QLocale::Russian:
            translator.load(":/visual_db_ru");
            break;
        case QLocale::English:
        default:
            translator.load(":/visual_db_en");
    }

    //
    app.installTranslator(&translator);

    using namespace Gui;

    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();

    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

    return app.exec();
}

