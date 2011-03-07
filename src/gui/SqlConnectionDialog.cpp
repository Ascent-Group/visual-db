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

#include <connect/ConnectionInfo.h>
#include <gui/SqlConnectionDialog.h>
#include <QComboBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
//#include <QNetworkProxy>
#include <QPushButton>
#include <QSqlDatabase>
//#include <QSqlError>

#include <QtDebug>

namespace Gui {

/*!
 * Constructor
 */
SqlConnectionDialog::SqlConnectionDialog(bool iLoadSession, QWidget *iParent)
    : QDialog(iParent)
{
    ui.setupUi(this);

    createDialog(iLoadSession);
    initConnectionFields(-1);
}

/*!
 * Destructor
 */
SqlConnectionDialog::~SqlConnectionDialog()
{
    // \note no need to delete db parameters and proxy parameters here, because here we
    // use the ones that are owned by main window and main window will handle their
    // destruction.
}

/*!
 * The 'connection failed' failed flag has been added because of the following reasons:
 * the dialog has only 2 result codes which are not enough to handle 3 possible cases for
 * our connection dialog:
 * <ol>
 *     <li>Pressed cancel. <b>Note:</b>Nothing to be done in this case</li>
 *     <li>Pressed Ok. The provided connection parameres are VALID for successfulll
 *     connection.</li>
 *     <li>Pressed Ok. The provided connection parameters are INVALID and it is impossible
 *     to establish a new connection.</li>
 * </ol>
 *
 * \return true If the accept button was clicked but the given parameters caused error
 * during connection.
 * \return false If the accept button was clicked and the connection has been successfully
 * established.
 *
 * \todo remove this func
 */
//bool
//SqlConnectionDialog::connectionFailed() const
//{
//    return mConnectionFailed;
//}

/*!
 * \return The currently selected connection info
 */
Connect::ConnectionInfo
SqlConnectionDialog::connectionInfo() const
{
    using namespace Connect;
    ConnectionInfo connInfo;
    DbHostInfo dbHostInfo;

    dbHostInfo.setAddress(ui.mDbHostEdit->text());
    dbHostInfo.setPort(ui.mDbPortEdit->text().toUInt()),
    dbHostInfo.setUser(ui.mDbUserEdit->text()),
    dbHostInfo.setPassword(ui.mDbPasswordEdit->text()),
    dbHostInfo.setDbName(ui.mDbNameEdit->text());
    dbHostInfo.setDbDriver(ui.mDbDriverCombo->currentText());

    connInfo.setDbHostInfo(dbHostInfo);

    connInfo.setUseProxy(ui.mUseProxyBox->isChecked());
    if (connInfo.useProxy()) {
        ProxyHostInfo proxyHostInfo;
        proxyHostInfo.setAddress(ui.mProxyHostNameEdit->text());
        proxyHostInfo.setPort(ui.mProxyPortEdit->text().toUInt());
        proxyHostInfo.setUser(ui.mProxyUserEdit->text());
        proxyHostInfo.setPassword(ui.mProxyPasswordEdit->text());
        proxyHostInfo.setType((QNetworkProxy::ProxyType)ui.mProxyTypeBox->itemData(ui.mProxyTypeBox->currentIndex()).toUInt());

        connInfo.setProxyHostInfo(proxyHostInfo);
    }

    return connInfo;
}

/*!
 *
 */
void
SqlConnectionDialog::setConnectionInfos(const QVector<Connect::ConnectionInfo> &iInfos)
{
    mConnectionInfos = iInfos;

    qint32 index = 0;
    QString text;
    foreach (const Connect::ConnectionInfo &info, mConnectionInfos) {
        text = QString("%1@%2 (%3)")
            .arg(info.dbHostInfo().dbName())
            .arg(info.dbHostInfo().address())
            .arg(info.dbHostInfo().user());

        ui.mConnectionsBox->insertItem(index++, text);
    }

    // add dummy entry for new connections
    ui.mConnectionsBox->insertItem(index, tr("<New connection>"));
}

/*!
 * \brief Create dialog
 */
void
SqlConnectionDialog::createDialog(bool iLoadSession)
{
    QStringList drivers = QSqlDatabase::drivers();
    ui.mDbDriverCombo->addItems(drivers);

    /* temporary (2 mysql and odbc searches are needed) */
    QStringList redundantDrivers;
    redundantDrivers << "QMYSQL" << "QMYSQL" << "QSQLITE" << "QOCI" << "QODBC" << "QODBC" << "QTDS" << "QDB2" << "QIBASE";

    foreach(const QString &drv, redundantDrivers) {
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

    ui.mDbDriverCombo->setEnabled(!iLoadSession);
    ui.mDbHostEdit->setEnabled(!iLoadSession);
    ui.mDbPortEdit->setEnabled(!iLoadSession);
    ui.mDbNameEdit->setEnabled(!iLoadSession);
    ui.mDbUserEdit->setEnabled(!iLoadSession);
    // password edit should be always enabled
    ui.mDbPasswordEdit->setEnabled(true);

    ui.mUseProxyBox->setEnabled(true);
    ui.mProxyTypeBox->setEnabled(ui.mUseProxyBox->isChecked());
    ui.mProxyHostNameEdit->setEnabled(ui.mUseProxyBox->isChecked());
    ui.mProxyPortEdit->setEnabled(ui.mUseProxyBox->isChecked());
    ui.mProxyUserEdit->setEnabled(ui.mUseProxyBox->isChecked());
    ui.mProxyPasswordEdit->setEnabled(ui.mUseProxyBox->isChecked());

    // initially we set connection failed flag to true
//    mConnectionFailed = true;
}

/*!
 * \brief Set fields in the dialog from the given connection parameters
 */
void
SqlConnectionDialog::initConnectionFields(int iIndex)
{
    Connect::ConnectionInfo info;
    if (0 >= iIndex && iIndex <= mConnectionInfos.size()) {
        info = mConnectionInfos.value(iIndex);
    }

    ui.mDbDriverCombo->setCurrentIndex(ui.mDbDriverCombo->findText(info.dbHostInfo().dbDriver()));
    ui.mDbHostEdit->setText(info.dbHostInfo().address());
    ui.mDbPortEdit->setText(QString::number(info.dbHostInfo().port()));
    ui.mDbNameEdit->setText(info.dbHostInfo().dbName());
    ui.mDbUserEdit->setText(info.dbHostInfo().user());

    ui.mUseProxyBox->setChecked(info.useProxy());
    ui.mProxyTypeBox->setCurrentIndex(ui.mProxyTypeBox->findData(info.proxyHostInfo().type()));
    ui.mProxyHostNameEdit->setText(info.proxyHostInfo().address());
    ui.mProxyPortEdit->setText(QString::number(info.proxyHostInfo().port()));
    ui.mProxyUserEdit->setText(info.proxyHostInfo().user());
}

/*!
 * \brief Establish connection
 */
//void
//SqlConnectionDialog::addConnection()
//{
//    using namespace Connect;
//
//    // proxy section
////    if (ui.mUseProxyBox->isChecked()) {
//        // remember connection paramters
//        ProxyHostInfo proxyHostInfo(ui.mProxyHostNameEdit->text(),
//                ui.mProxyPortEdit->text().toUInt(), ui.mProxyUserEdit->text(), "",
//                (QNetworkProxy::ProxyType)ui.mProxyTypeBox->itemData(ui.mProxyTypeBox->currentIndex()).toUInt());
//        mConnectionInfo.setProxyHostInfo(proxyHostInfo);
//        mConnectionInfo.setUseProxy(ui.mUseProxyBox->isChecked());
////        setProxy(ProxyHostInfo);
////    } else {
////        mConnectionInfo.setUseProxy(false);
////    }
//
//    // remember database settings
//    DbHostInfo dbHostInfo(ui.mDbHostEdit->text(), ui.mDbPortEdit->text().toUInt(),
//            ui.mDbUserEdit->text(), ui.mDbPasswordEdit->text(), ui.mDbNameEdit->text(), ui.mDbDriverCombo->currentText());
//    mConnectionInfo.setDbHostInfo(dbHostInfo);
//
//    // create connection to database
//    mConnectionFailed = !createConnection(dbHostInfo);
//
//    /*!
//     * If we are in addConnection dialog, then we definitely pressed Ok and NOT Cancel
//     * button.
//     */
//    if (mConnectionFailed) {
//        QMessageBox::warning(
//                this,
//                tr("Error"),
//                tr("Connection refused: ") + QSqlDatabase::database("mainConnect").lastError().text());
//    }
//    accept();
//}

/*!
 * \brief Switch on/off proxy connection parameters
 *
 * \param[in] iToggle - True if proxy is enabled, false if proxy is disabled
 */
void
SqlConnectionDialog::switchProxy(bool iToggle)
{
    ui.mProxyTypeBox->setEnabled(iToggle);
    ui.mProxyHostNameEdit->setEnabled(iToggle);
    ui.mProxyPortEdit->setEnabled(iToggle);
    ui.mProxyUserEdit->setEnabled(iToggle);
    ui.mProxyPasswordEdit->setEnabled(iToggle);
}

}

