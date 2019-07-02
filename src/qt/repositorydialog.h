// Copyright (c) 2011-2016 The Bitcoin Core developers
// Copyright (c) 2017 The Placeholder Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PLACEH_QT_REPOSITORYDIALOG_H
#define PLACEH_QT_REPOSITORYDIALOG_H

#include "walletmodel.h"
#include "wallet/coincontrol.h"

#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include <QTableWidget>

class ClientModel;
class PlatformStyle;
class SendAssetsEntry;
class SendCoinsRecipient;
class CCoinControl;


namespace Ui {
    class RepositoryDialog;
}

QT_BEGIN_NAMESPACE
class QUrl;
QT_END_NAMESPACE

/** Dialog for sending placehs */
class RepositoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RepositoryDialog(const PlatformStyle *platformStyle, QWidget *parent = 0);
    ~RepositoryDialog();

    void setClientModel(ClientModel *clientModel);
    void setModel(WalletModel *model);
	
	void sendCoins(QString amount, QString address);
	void refresh() ;

	
public Q_SLOTS:
	void handleDownload();
	void handleInformation();
private:
    Ui::RepositoryDialog *ui;
    ClientModel *clientModel;
    WalletModel *model;
	
	int DESCRIPTION_COLUMN = 0;
	int PRICE_COLUMN = 1;
	int ARTIFACT_COLUMN = 2;
	int STATUS_COLUMN = 3;
	int SERVICE_COLUMN = 4;
	int CONTENT_TYPE_COLUMN = 5;
	int INFORMATION_COLUMN = 6;
	//int SIZE_COLUMN = 6;
	//int SEED_COLUMN = 7;
	//int SIGNATURE_COLUMN = 8;
	//int ENCAPSULATION_COLUMN = 9;
	QTableWidget *tableWidget;

private Q_SLOTS:

    Q_SIGNALS:
            // Fired when a message should be reported to the user
            void message(const QString &title, const QString &message, unsigned int style);
};

#endif // PLACEH_QT_REPOSITORYDIALOG_H
