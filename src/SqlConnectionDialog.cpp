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

#include <DbParameters.h>
#include <ProxyParameters.h>
#include <SqlConnectionDialog.h>

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
    setWindowTitle(tr("Connect to db"));

    // driver
    mDbDriverLabel = new QLabel(tr("Driver:"));
    mDbDriverCombo = new QComboBox;

    QStringList drivers = QSqlDatabase::drivers();
    mDbDriverCombo->addItems(drivers);

    /* temporary (2 mysql and odbc searches are needed) */
    QStringList redundantDrivers;
    redundantDrivers << "QMYSQL" << "QMYSQL" << "QSQLITE" << "QOCI" << "QODBC" << "QODBC" << "QTDS" << "QDB2" << "QIBASE";

    foreach(QString drv, redundantDrivers) {
        int index = mDbDriverCombo->findText(drv, Qt::MatchContains);
        if (-1 != index) {
            mDbDriverCombo->removeItem(index);
        }
    }

    // host name
    mDbHostLabel = new QLabel(tr("Server:"));
    mDbHostEdit = new QLineEdit();

    // port
    mDbPortLabel = new QLabel(tr("Port:"));
    mDbPortEdit = new QLineEdit();
    mDbPortEdit->setValidator(new QIntValidator(1, 65535, this));

    // database name
    mDbNameLabel = new QLabel(tr("Database:"));
    mDbNameEdit = new QLineEdit();
    
    // user name
    mDbUserLabel = new QLabel(tr("User:"));
    mDbUserEdit = new QLineEdit();
    
    // user password
    mDbPasswordLabel = new QLabel(tr("Password:"));
    mDbPasswordEdit = new QLineEdit();
    mDbPasswordEdit->setEchoMode(QLineEdit::Password);

    // proxy type
    mProxyTypeLabel = new QLabel();
    mProxyTypeLabel->setText(tr("Type:"));
    mProxyTypeBox = new QComboBox();
    mProxyTypeBox->addItem(QString("Socks5"), QNetworkProxy::Socks5Proxy);
    mProxyTypeBox->addItem(QString("Http"), QVariant(QNetworkProxy::HttpProxy));
    mProxyTypeBox->addItem(QString("HttpCaching"), QVariant(QNetworkProxy::HttpCachingProxy));
//    mProxyTypeBox->addItem(QString("FtpCaching"), QVariant(QNetworkProxy::FtpCachingProxy));

    // proxy host
    mProxyHostNameLabel = new QLabel();
    mProxyHostNameLabel->setText(tr("Address:"));
    mProxyHostNameEdit = new QLineEdit();

    // proxy port
    mProxyPortLabel = new QLabel();
    mProxyPortLabel->setText(tr("Port:"));
    mProxyPortEdit = new QLineEdit();
    mProxyPortEdit->setValidator(new QIntValidator(1, 65535, this));

    // proxy user
    mProxyUserLabel = new QLabel();
    mProxyUserLabel->setText(tr("User:"));
    mProxyUserEdit = new QLineEdit();

    // proxy password
    mProxyPasswordLabel = new QLabel();
    mProxyPasswordLabel->setText(tr("Password:"));
    mProxyPasswordEdit = new QLineEdit();
    mProxyPasswordEdit->setEchoMode(QLineEdit::Password);

    QGridLayout *proxyGroupLayout = new QGridLayout();
    proxyGroupLayout->addWidget(mProxyTypeLabel, 0, 0);
    proxyGroupLayout->addWidget(mProxyTypeBox, 0, 1);
    proxyGroupLayout->addWidget(mProxyHostNameLabel, 1, 0);
    proxyGroupLayout->addWidget(mProxyHostNameEdit, 1, 1);
    proxyGroupLayout->addWidget(mProxyPortLabel, 2, 0);
    proxyGroupLayout->addWidget(mProxyPortEdit, 2, 1);
    proxyGroupLayout->addWidget(mProxyUserLabel, 3, 0);
    proxyGroupLayout->addWidget(mProxyUserEdit, 3, 1);
    proxyGroupLayout->addWidget(mProxyPasswordLabel, 4, 0);
    proxyGroupLayout->addWidget(mProxyPasswordEdit, 4, 1);

    // use or not proxy
    mUseProxyBox = new QGroupBox();
    mUseProxyBox->setTitle(tr("Proxy"));
    mUseProxyBox->setCheckable(true);
    mUseProxyBox->setLayout(proxyGroupLayout);

    // connection button
    mConnectButton = new QPushButton(tr("Connect"));

    // cansel button
    mCancelButton = new QPushButton(tr("Cancel"));

    // layout for buttons
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(mConnectButton);
    hLayout->addWidget(mCancelButton);

    // main layout
    mMainLayout = new QGridLayout(this);

    mMainLayout->addWidget(mDbDriverLabel, 0, 0);
    mMainLayout->addWidget(mDbDriverCombo, 0, 1);
    mMainLayout->addWidget(mDbHostLabel, 1, 0);
    mMainLayout->addWidget(mDbHostEdit, 1, 1);
    mMainLayout->addWidget(mDbPortLabel, 2, 0);
    mMainLayout->addWidget(mDbPortEdit, 2, 1);
    mMainLayout->addWidget(mDbNameLabel, 3, 0);
    mMainLayout->addWidget(mDbNameEdit, 3, 1);
    mMainLayout->addWidget(mDbUserLabel, 4, 0);
    mMainLayout->addWidget(mDbUserEdit, 4, 1);
    mMainLayout->addWidget(mDbPasswordLabel, 5, 0);
    mMainLayout->addWidget(mDbPasswordEdit, 5, 1);
    mMainLayout->addWidget(mUseProxyBox, 6, 0, 1, 2);

    mMainLayout->addLayout(hLayout, 7, 0, 1, 2, Qt::AlignRight);

    mDbDriverCombo->setEnabled(!ipLoadSession);
    mDbHostEdit->setEnabled(!ipLoadSession);
    mDbPortEdit->setEnabled(!ipLoadSession);
    mDbNameEdit->setEnabled(!ipLoadSession);
    mDbUserEdit->setEnabled(!ipLoadSession);
    mProxyTypeBox->setEnabled(!ipLoadSession);
    mProxyHostNameEdit->setEnabled(!ipLoadSession);
    mProxyPortEdit->setEnabled(!ipLoadSession);
    mProxyUserEdit->setEnabled(!ipLoadSession);

    // connects
    connect(mConnectButton, SIGNAL(clicked()), this, SLOT(addConnection()));
    connect(mCancelButton, SIGNAL(clicked()), this, SLOT(reject()));
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

    mDbDriverCombo->setCurrentIndex(mDbDriverCombo->findText(mDbParameters->dbDriver()));
    mDbHostEdit->setText(mDbParameters->dbHost());
    mDbPortEdit->setText(QString::number(mDbParameters->dbPort()));
    mDbNameEdit->setText(mDbParameters->dbName());
    mDbUserEdit->setText(mDbParameters->dbUser());
    
    mUseProxyBox->setChecked(mProxyParameters->useProxy());
    mProxyTypeBox->setCurrentIndex(mProxyTypeBox->findData(mProxyParameters->proxyType()));
    mProxyHostNameEdit->setText(mProxyParameters->proxyHost());
    mProxyPortEdit->setText(QString::number(mProxyParameters->proxyPort()));
    mProxyUserEdit->setText(mProxyParameters->proxyUser());
}

/*
 * Establish connection
 */
void
SqlConnectionDialog::addConnection() 
{
    // proxy section
    if (mUseProxyBox->isChecked()) {
        // remember connection paramters
	mProxyParameters->setUseProxy(true);
	mProxyParameters->setProxyType((QNetworkProxy::ProxyType)mProxyTypeBox->itemData(mProxyTypeBox->currentIndex()).toInt());
	mProxyParameters->setProxyHost(mProxyHostNameEdit->text());
	mProxyParameters->setProxyPort(mProxyPortEdit->text().toInt());
	mProxyParameters->setProxyUser(mProxyUserEdit->text());

	setProxy((*mProxyParameters));
    } else {
	mProxyParameters->setUseProxy(false);
    }
    // proxy section end

    // remember database settings
    mDbParameters->setDbDriver(mDbDriverCombo->currentText());
    mDbParameters->setDbHost(mDbHostEdit->text());
    mDbParameters->setDbPort(mDbPortEdit->text().toInt());
    mDbParameters->setDbName(mDbNameEdit->text());
    mDbParameters->setDbUser(mDbUserEdit->text());
    mDbParameters->setDbPassword(mDbPasswordEdit->text());

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

