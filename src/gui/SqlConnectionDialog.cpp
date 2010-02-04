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

#include <connect/DbParameters.h>
#include <connect/ProxyParameters.h>
#include <gui/SqlConnectionDialog.h>
#include <QComboBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QNetworkProxy>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlError>

#include <QtDebug>

/*
 * Constructor
 */
SqlConnectionDialog::SqlConnectionDialog(DbParameters *ipDbParameters, 
                     ProxyParameters *ipProxyParameters, 
                     bool ipLoadSession,
                     QWidget *ipParent)
    : QDialog(ipParent), mDbParameters(ipDbParameters), mProxyParameters(ipProxyParameters)
{
    ui.setupUi(this);

    createDialog(ipLoadSession);
    if (!ipDbParameters) {
    mDbParameters = new DbParameters();
    }
    if (!ipProxyParameters) {
    mProxyParameters = new ProxyParameters();
    }
    initConnectionFields();
}

/*
 * Create dialog
 */
void
SqlConnectionDialog::createDialog(bool ipLoadSession)
{

    QStringList drivers = QSqlDatabase::drivers();
    ui.mDbDriverCombo->addItems(drivers);

    /* temporary (2 mysql and odbc searches are needed) */
    QStringList redundantDrivers;
    redundantDrivers << "QMYSQL" << "QMYSQL" << "QSQLITE" << "QOCI" << "QODBC" << "QODBC" << "QTDS" << "QDB2" << "QIBASE";

    foreach(QString drv, redundantDrivers) {
        int index = ui.mDbDriverCombo->findText(drv, Qt::MatchContains);
        if (-1 != index) {
            ui.mDbDriverCombo->removeItem(index);
        }
    }

    // port
    ui.mDbPortEdit->setValidator(new QIntValidator(1, 65535, this));

    // user password
    ui.mDbPasswordEdit->setEchoMode(QLineEdit::Password);

    // proxy type
    ui.mProxyTypeBox->addItem(QString("Socks5"), QNetworkProxy::Socks5Proxy);
    ui.mProxyTypeBox->addItem(QString("Http"), QVariant(QNetworkProxy::HttpProxy));
    ui.mProxyTypeBox->addItem(QString("HttpCaching"), QVariant(QNetworkProxy::HttpCachingProxy));
//    mProxyTypeBox->addItem(QString("FtpCaching"), QVariant(QNetworkProxy::FtpCachingProxy));

    // proxy port
    ui.mProxyPortEdit->setValidator(new QIntValidator(1, 65535, this));


    // proxy password
    ui.mProxyPasswordEdit->setEchoMode(QLineEdit::Password);

    ui.mDbDriverCombo->setEnabled(!ipLoadSession);
    ui.mDbHostEdit->setEnabled(!ipLoadSession);
    ui.mDbPortEdit->setEnabled(!ipLoadSession);
    ui.mDbNameEdit->setEnabled(!ipLoadSession);
    ui.mDbUserEdit->setEnabled(!ipLoadSession);
    ui.mProxyTypeBox->setEnabled(!ipLoadSession);
    ui.mProxyHostNameEdit->setEnabled(!ipLoadSession);
    ui.mProxyPortEdit->setEnabled(!ipLoadSession);
    ui.mProxyUserEdit->setEnabled(!ipLoadSession);

}

/*
 * Set fields in the dialog from the given connection parameters
 */
void
SqlConnectionDialog::initConnectionFields()
{
    if (!mDbParameters || !mProxyParameters) {
    return;
    }

    ui.mDbDriverCombo->setCurrentIndex(ui.mDbDriverCombo->findText(mDbParameters->dbDriver()));
    ui.mDbHostEdit->setText(mDbParameters->dbHost());
    ui.mDbPortEdit->setText(QString::number(mDbParameters->dbPort()));
    ui.mDbNameEdit->setText(mDbParameters->dbName());
    ui.mDbUserEdit->setText(mDbParameters->dbUser());
    
    ui.mUseProxyBox->setChecked(mProxyParameters->useProxy());
    ui.mProxyTypeBox->setCurrentIndex(ui.mProxyTypeBox->findData(mProxyParameters->proxyType()));
    ui.mProxyHostNameEdit->setText(mProxyParameters->proxyHost());
    ui.mProxyPortEdit->setText(QString::number(mProxyParameters->proxyPort()));
    ui.mProxyUserEdit->setText(mProxyParameters->proxyUser());
}

/*
 * Establish connection
 */
void
SqlConnectionDialog::addConnection() 
{
    // proxy section
    if (ui.mUseProxyBox->isChecked()) {
        // remember connection paramters
    mProxyParameters->setUseProxy(true);
    mProxyParameters->setProxyType((QNetworkProxy::ProxyType)ui.mProxyTypeBox->itemData(ui.mProxyTypeBox->currentIndex()).toInt());
    mProxyParameters->setProxyHost(ui.mProxyHostNameEdit->text());
    mProxyParameters->setProxyPort(ui.mProxyPortEdit->text().toInt());
    mProxyParameters->setProxyUser(ui.mProxyUserEdit->text());

    setProxy((*mProxyParameters));
    } else {
    mProxyParameters->setUseProxy(false);
    }
    // proxy section end

    // remember database settings
    mDbParameters->setDbDriver(ui.mDbDriverCombo->currentText());
    mDbParameters->setDbHost(ui.mDbHostEdit->text());
    mDbParameters->setDbPort(ui.mDbPortEdit->text().toInt());
    mDbParameters->setDbName(ui.mDbNameEdit->text());
    mDbParameters->setDbUser(ui.mDbUserEdit->text());
    mDbParameters->setDbPassword(ui.mDbPasswordEdit->text());

    // create connection to database
    if (createConnection((*mDbParameters))) {
    accept();
    } else {
    QMessageBox::warning(
        this,
        tr("Error"),
        tr("Connection refused: ") + QSqlDatabase::database("mainConnect").lastError().text());
    reject();
    }
}

