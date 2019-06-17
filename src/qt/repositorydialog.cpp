// Copyright (c) 2011-2016 The Bitcoin Core developers
// Copyright (c) 2017 The Placeholder Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "repositorydialog.h"
#include "sendcoinsdialog.h"
#include "ui_assetsdialog.h"
#include "ui_repositorydialog.h"

#include "addresstablemodel.h"
#include "placehunits.h"
#include "clientmodel.h"
#include "assetcontroldialog.h"
#include "guiutil.h"
#include "optionsmodel.h"
#include "platformstyle.h"
#include "sendassetsentry.h"
#include "walletmodel.h"

#include "base58.h"
#include "chainparams.h"
#include "wallet/coincontrol.h"
#include "validation.h" // mempool and minRelayTxFee
#include "ui_interface.h"
#include "txmempool.h"
#include "policy/fees.h"
#include "wallet/fees.h"
#include "createassetdialog.h"
#include "reissueassetdialog.h"

#include "placeholderutility.h"

#include <iostream>
#include <fstream>
#include <string>
#include <thread>

#include <QString>
#include <QDir>
#include <QProcess>
#include <QFontMetrics>
#include <QMessageBox>
#include <QGridLayout>
#include <QScrollBar>
#include <QSettings>
#include <QTextDocument>
#include <QTableWidget>
#include <QTimer>
#include <policy/policy.h>
#include <core_io.h>
#include <rpc/mining.h>


void processTorrents()
{
	PlaceholderUtility* pu = new PlaceholderUtility();
	pu->seedRepository();
}


RepositoryDialog::RepositoryDialog(const PlatformStyle *_platformStyle, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::RepositoryDialog),
        clientModel(0),
        model(0)
{
	
	int cols = 11;
	int rows = 0;

	PlaceholderUtility* pu = new PlaceholderUtility();
	pu->updateList();
	
	//std::thread (getTorrents).detach();
	std::thread (processTorrents).detach();
	
	rows = pu->getNumberArtifacts();
	
	QTableWidget *tableWidget = new QTableWidget(rows, cols, this);

	tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Encapsulation"));
	tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Artifact"));
	tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Requirements"));
	tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Signed"));
	tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Seed"));
	tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("File Type"));
	tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("Size"));
	tableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem("Status"));
	tableWidget->setHorizontalHeaderItem(8, new QTableWidgetItem("Bounty"));
	tableWidget->setHorizontalHeaderItem(9, new QTableWidgetItem("Description"));
	tableWidget->setHorizontalHeaderItem(10, new QTableWidgetItem("Service"));
	
	

// RAII cleanup


	
	tableWidget->resize(2000,2000);
	
	std::string line;
	std::ifstream myfile (pu->getRepositoryListFile().toUtf8().constData());
	if (myfile.is_open())
	{
		int i = 0;
		while ( std::getline (myfile,line) )
		{
			QString q = QString::fromLocal8Bit(line.c_str());		
			QTableWidgetItem *encapsulationItem = new QTableWidgetItem("Raw");
			tableWidget->setItem(i, 0, encapsulationItem);
			QTableWidgetItem *addressItem = new QTableWidgetItem(q);
			tableWidget->setItem(i, 1, addressItem);
			QTableWidgetItem *hostItem = new QTableWidgetItem("N/A");
			tableWidget->setItem(i, 2, hostItem);
			QTableWidgetItem *signedItem = new QTableWidgetItem("NO");
			tableWidget->setItem(i, 3, signedItem);
			QTableWidgetItem *seedItem = new QTableWidgetItem("YES");
			tableWidget->setItem(i, 4, seedItem);
			QTableWidgetItem *fileTypeItem = new QTableWidgetItem("VDI");
			tableWidget->setItem(i, 5, fileTypeItem);
			QTableWidgetItem *sizeItem = new QTableWidgetItem("-/- MB");
			tableWidget->setItem(i, 6, sizeItem);
			QTableWidgetItem *statusItem = new QTableWidgetItem("AVAILABLE");
			tableWidget->setItem(i, 7, statusItem);
			QTableWidgetItem *bountyItem = new QTableWidgetItem("0.00 PHL");
			tableWidget->setItem(i, 8, bountyItem);
			QTableWidgetItem *descriptionItem = new QTableWidgetItem("-/-");
			tableWidget->setItem(i, 9, descriptionItem);
			
			QWidget* pWidget = new QWidget();
			QPushButton* btnDownload = new QPushButton();
			btnDownload->setText("Download");
			QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
			pLayout->addWidget(btnDownload);
			pLayout->setAlignment(Qt::AlignCenter);
			pLayout->setContentsMargins(0, 0, 0, 0);
			pWidget->setLayout(pLayout);

			tableWidget->setCellWidget(i, 10, pWidget);

			i++;
						
		}
		myfile.close();
	}
	else {
		QMessageBox msgBoxC;
		msgBoxC.setText("Unable to open repository file list.json in {repository}");
		msgBoxC.exec();
	}

	QGridLayout *layout = new QGridLayout(parent);

	
    layout->addWidget(tableWidget, 1, Qt::AlignLeft);

    setLayout(layout);
    /** PHL END */
}

void RepositoryDialog::setClientModel(ClientModel *_clientModel)
{
    this->clientModel = _clientModel;

}



void RepositoryDialog::setModel(WalletModel *_model)
{
    this->model = _model;

}

RepositoryDialog::~RepositoryDialog()
{
	try { 
		delete ui;
	} catch(...){}
}

/** PHL END */
