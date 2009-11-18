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

#ifndef SQLCONNECTIONDIALOG_H
#define SQLCONNECTIONDIALOG_H

#include <QDialog>

class DbParameters;
class ProxyParameters;
class QComboBox;
class QGroupBox;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

/*
 * Dialog for connection to the database
 */
class SqlConnectionDialog : public QDialog
{
    Q_OBJECT

    public:
	SqlConnectionDialog(DbParameters *, ProxyParameters *, bool, QWidget *ipParent = 0);

    private:
	QLabel *mDbDriverLabel;
	QLabel *mDbHostLabel;
	QLabel *mDbPortLabel;
	QLabel *mDbNameLabel;
	QLabel *mDbUserLabel;
	QLabel *mDbPasswordLabel;

	QComboBox *mDbDriverCombo;

	QLineEdit *mDbHostEdit;
	QLineEdit *mDbPortEdit;
	QLineEdit *mDbNameEdit;
	QLineEdit *mDbUserEdit;
	QLineEdit *mDbPasswordEdit;

        QGroupBox *mUseProxyBox;

        QLabel *mProxyTypeLabel;
        QLabel *mProxyHostNameLabel;
        QLabel *mProxyPortLabel;
        QLabel *mProxyUserLabel;
        QLabel *mProxyPasswordLabel;

        QComboBox *mProxyTypeBox;
        QLineEdit *mProxyHostNameEdit;
        QLineEdit *mProxyPortEdit;
        QLineEdit *mProxyUserEdit;
        QLineEdit *mProxyPasswordEdit;

	QPushButton *mConnectButton;
	QPushButton *mCancelButton;

	QGridLayout *mMainLayout;

	DbParameters *mDbParameters;
	ProxyParameters *mProxyParameters;

    private:
        void createDialog(bool);
        void initConnectionFields();

    private slots:
        void addConnection();
};

#endif // SQLCONNECTIONDIALOG_H
