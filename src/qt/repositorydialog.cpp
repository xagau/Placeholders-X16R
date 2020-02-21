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

#include "crc32.h"
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
#include <QList>

#include <QDir>
#include <QIODevice>
#include <QProcess>
#include <QFontMetrics>
#include <QFont>
#include <QFontDatabase>
#include <QMessageBox>

#include <QGridLayout>
#include <QScrollBar>
#include <QSettings>
#include <QTextDocument>
#include <QDesktopServices>
#include <QTableWidget>
#include <QHeaderView>

#include <QTimer>
#include <policy/policy.h>
#include <core_io.h>
#include <rpc/mining.h>

#include <QJsonObject>

#include <QUrl>
#include <QEventLoop>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>


#include <QApplication>
#include <QDebug>

void processTorrents()
{
	PlaceholderUtility* pu = new PlaceholderUtility();
	pu->seedRepository();
}

void RepositoryDialog::refresh()
{
	//
	QString criteria = "";
	criteria = filter->text();
	
	PlaceholderUtility* pu = new PlaceholderUtility();
	if( criteria.trimmed().isEmpty() ){ 
		//tableWidget->setRowCount(0);
		// if nothing specified, return and do nothing.
		pu->updateRecentList(); // for now - just return anything recent stuff.
		
	} else {
		pu->updateList(criteria);	
	}
	
	int rows = pu->getNumberArtifacts();
	tableWidget->setRowCount(rows);
	//tableWidget->resize(2000,2000);
	
	std::string line;
	std::ifstream myfile (pu->getRepositoryListFile().toUtf8().constData());
	if (myfile.is_open())
	{
		int i = 0;
		QPalette *palette = new QPalette();
		palette->setColor(QPalette::Base,Qt::gray);
		palette->setColor(QPalette::Text,Qt::darkGray);

		while ( std::getline (myfile,line) )
		{
			QString q = QString::fromLocal8Bit(line.c_str());	

			QEventLoop loop;
			QNetworkAccessManager nam;
			QNetworkRequest req(QUrl(pu->getArtifactDetailURL() + q));
			QNetworkReply *reply = nam.get(req);
			connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
			loop.exec();
			QByteArray buffer = reply->readAll();

			QString dataLine = buffer.constData();
			
			QJsonObject o = pu->objectFromString(dataLine.toUtf8().constData());

			QString encapsulation = o.value("encapsulation").toString();
			QString asigned = o.value("signed").toString();
			QString signature = o.value("signature").toString();
			QString seed = o.value("seed").toString();
			QString contentType = o.value("contentType").toString();
			QString size = o.value("size").toString();
			QString status = o.value("status").toString();
			QString bounty = o.value("bounty").toString();
			QString description = o.value("description").toString();
			QString checksum = o.value("checksum").toString();
			QString service = o.value("service").toString();
		

			
			QTableWidgetItem *encapsulationItem = new QTableWidgetItem(encapsulation);
			encapsulationItem->setFlags(encapsulationItem->flags() ^ Qt::ItemIsEditable); 
			//encapsulationItem->setPalette(palette);			
			//tableWidget->setItem(i, ENCAPSULATION_COLUMN, encapsulationItem);
			
			QTableWidgetItem *addressItem = new QTableWidgetItem(q);
			//addressItem->setFlags(addressItem->flags() ^ Qt::ItemIsEditable); 
			//addressItem->setPalette(palette);
			tableWidget->setItem(i, ARTIFACT_COLUMN, addressItem);
			
			
			//QTableWidgetItem *informationItem = new QTableWidgetItem(information);
			//informationItem->setFlags(signatureItem->flags() ^ Qt::ItemIsEditable); 
			//informationItem->setPalette(palette);
			//tableWidget->setItem(i, INFORMATION_COLUMN, signatureItem);

			
			QTableWidgetItem *checksumItem = new QTableWidgetItem(checksum);
			checksumItem->setFlags(checksumItem->flags() ^ Qt::ItemIsEditable); 
			//checksumItem->setTextAlignment(Qt::AlignRight);
			checksumItem->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
			tableWidget->setItem(i, CHECKSUM_COLUMN, checksumItem);

			QTableWidgetItem *fileTypeItem = new QTableWidgetItem(contentType);
			fileTypeItem->setFlags(fileTypeItem->flags() ^ Qt::ItemIsEditable); 
			tableWidget->setItem(i, CONTENT_TYPE_COLUMN, fileTypeItem);

			QTableWidgetItem *sizeItem = new QTableWidgetItem(size);
			sizeItem->setFlags(sizeItem->flags() ^ Qt::ItemIsEditable); 
			//tableWidget->setItem(i, SIZE_COLUMN, sizeItem);
			
			QTableWidgetItem *statusItem = new QTableWidgetItem(status);
			statusItem->setFlags(statusItem->flags() ^ Qt::ItemIsEditable); 
			tableWidget->setItem(i, STATUS_COLUMN, statusItem);

			QTableWidgetItem *bountyItem = new QTableWidgetItem(bounty);
			bountyItem->setFlags(bountyItem->flags() ^ Qt::ItemIsEditable); 
			bountyItem->setTextAlignment(Qt::AlignVCenter|Qt::AlignRight);
			//tableWidget->setItem(i, CHECKSUM_COLUMN, checksumItem);
			tableWidget->setItem(i, PRICE_COLUMN, bountyItem);

			QTableWidgetItem *descriptionItem = new QTableWidgetItem(description);
			descriptionItem->setFlags(descriptionItem->flags() ^ Qt::ItemIsEditable); 		
			tableWidget->setItem(i, DESCRIPTION_COLUMN, descriptionItem);
			
 
			QWidget*     pWidgetDownload   = new QWidget();
			QWidget*     pWidgetInfo       = new QWidget();
			QPushButton* btnDownload       = new QPushButton();
			QPushButton* btnInformation    = new QPushButton();
			
			connect(btnDownload, SIGNAL (clicked()), this, SLOT (handleDownload()));
			connect(btnInformation, SIGNAL (clicked()), this, SLOT (handleInformation()));

			btnInformation->setText("[?]");
			btnInformation->setFixedWidth(25);
			QHBoxLayout* pLayoutInfo = new QHBoxLayout(pWidgetInfo);
			pLayoutInfo->addWidget(btnInformation);
			pLayoutInfo->setAlignment(Qt::AlignCenter);
			pLayoutInfo->setContentsMargins(0, 0, 0, 0);
			pWidgetInfo->setLayout(pLayoutInfo);
			pWidgetInfo->setFixedWidth(25);
			tableWidget->setCellWidget(i, INFORMATION_COLUMN, pWidgetInfo);
			
			btnDownload->setText("Download");
			QHBoxLayout* pLayoutDownload = new QHBoxLayout(pWidgetDownload);
			pLayoutDownload->addWidget(btnDownload);
			pLayoutDownload->setAlignment(Qt::AlignCenter);
			pLayoutDownload->setContentsMargins(0, 0, 0, 0);
			pWidgetDownload->setLayout(pLayoutDownload);

			tableWidget->setCellWidget(i, SERVICE_COLUMN, pWidgetDownload);

			i++;
						
		}
		myfile.close();
	}
	else {
		QMessageBox msgBoxError;
		msgBoxError.setText("Unable to open repository file list.json in {repository}");
		msgBoxError.exec();
	}
	
	tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	
}

RepositoryDialog::RepositoryDialog(const PlatformStyle *_platformStyle, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::RepositoryDialog),
        clientModel(0),
        model(0)
{
		

	
// RAII cleanup

	int cols = 8;
	int rows = 0;
	int maxHeight = 50;
	
	QVBoxLayout *mainLayout = new QVBoxLayout();
	QHBoxLayout *base = new QHBoxLayout;
	
	horizontalGroupBox = new QGroupBox(tr("Keywords / Tags"));
	horizontalGroupBox->setFixedHeight(maxHeight);
	filter->setPlaceholderText("filter results");
	filter->setToolTip("Use this field to filter results.");
	
	search = new QPushButton(tr("Search"));

	search->setFixedWidth(50);
	search->setToolTip("Click this button to refresh results with filter criteria.");
	
	connect(search, SIGNAL(clicked()), this, SLOT(refresh()));
	
	base->addWidget(filter);
	base->addWidget(search);
	//base->setStretch(1);
	horizontalGroupBox->setLayout(base);
	
	
	//QGridLayout *layout = new QGridLayout();
	PlaceholderUtility* pu = new PlaceholderUtility();
		
	rows = pu->getNumberArtifacts();
	
	tableWidget = new QTableWidget(rows, cols, this);	
	tableWidget->setAlternatingRowColors(true);
	tableWidget->setStyleSheet("alternate-background-color: white;background-color: #f0f0f0;");

	tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	QHeaderView* header = tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
	header->setSectionResizeMode( DESCRIPTION_COLUMN, QHeaderView::Interactive );
	header->resizeSection(DESCRIPTION_COLUMN, 300);
	header->setSectionResizeMode( ARTIFACT_COLUMN, QHeaderView::ResizeToContents );
	header->setSectionResizeMode( PRICE_COLUMN, QHeaderView::ResizeToContents );
	header->setSectionResizeMode( CONTENT_TYPE_COLUMN, QHeaderView::ResizeToContents );
	header->setSectionResizeMode( STATUS_COLUMN, QHeaderView::ResizeToContents );
	header->setSectionResizeMode( INFORMATION_COLUMN, QHeaderView::ResizeToContents );
	header->setSectionResizeMode( SERVICE_COLUMN, QHeaderView::ResizeToContents );
	header->setSectionResizeMode( CHECKSUM_COLUMN, QHeaderView::ResizeToContents );

	//tableWidget->setHorizontalHeaderItem(ENCAPSULATION_COLUMN, new QTableWidgetItem("Encapsulation"));
	tableWidget->setHorizontalHeaderItem(ARTIFACT_COLUMN, new QTableWidgetItem("Payment Address"));
	tableWidget->setHorizontalHeaderItem(INFORMATION_COLUMN, new QTableWidgetItem("Information"));
	//tableWidget->setHorizontalHeaderItem(SEED_COLUMN, new QTableWidgetItem("Seed"));
	tableWidget->setHorizontalHeaderItem(CONTENT_TYPE_COLUMN, new QTableWidgetItem("Content Type"));
	//tableWidget->setHorizontalHeaderItem(SIZE_COLUMN, new QTableWidgetItem("Size"));
	tableWidget->setHorizontalHeaderItem(STATUS_COLUMN, new QTableWidgetItem("Status"));
	tableWidget->setHorizontalHeaderItem(PRICE_COLUMN, new QTableWidgetItem("Price (PHL)"));
	tableWidget->setHorizontalHeaderItem(DESCRIPTION_COLUMN, new QTableWidgetItem("Description"));
	tableWidget->setHorizontalHeaderItem(SERVICE_COLUMN, new QTableWidgetItem("Service"));
	tableWidget->setHorizontalHeaderItem(CHECKSUM_COLUMN, new QTableWidgetItem("Checksum"));

	//PlaceholderUtility* pu = new PlaceholderUtility();

	//tableWidget->setRowCount(0);

	
	
	//layout->addWidget(tableWidget, 1, Qt::AlignLeft);

	//mainLayout->addStretch();
	//mainLayout->addWidget(horizontalGroupBox, 1, Qt::AlignLeft);
	//mainLayout->addWidget(tableWidget, 2, Qt::AlignLeft);
	mainLayout->addWidget(horizontalGroupBox);//, 1, Qt::AlignLeft);
	mainLayout->addWidget(tableWidget);//, 2, Qt::AlignLeft);
	//mainLayout->setStretch(0,1);
	//mainLayout->setStretch(1,1);

	setLayout(mainLayout);
    //setLayout(layout);
	
}

void RepositoryDialog::setClientModel(ClientModel *_clientModel)
{
    this->clientModel = _clientModel;

}



void RepositoryDialog::setModel(WalletModel *_model)
{
    this->model = _model;

}

void RepositoryDialog::sendCoins(QString amount, QString _address)
{
	try { 
	
		SendCoinsRecipient recipient; // = new SendCoinRecipient();
		recipient.address = _address; //, "Marketplace Purchase", 1000000, "Marketplace Purchase" );
		recipient.amount = (CAmount)(amount.toDouble() * (double)COIN);
		recipient.label = "Marketplace Purchase";
		recipient.message = "Marketplace Purchase";
		recipient.fSubtractFeeFromAmount = true;
		
		QList<SendCoinsRecipient> list;
		list.append(recipient);
		
		CCoinControl ctrl;
		
		WalletModelTransaction currentTransaction(list);
		
		WalletModel::UnlockContext ctx(this->model->requestUnlock());
		if(!ctx.isValid())
		{
			// Unlock wallet was cancelled
			//fNewRecipientAllowed = true;
				
			QMessageBox msgBoxError;
			msgBoxError.setText("Could not unlock wallet");
			msgBoxError.exec();
				
			return;
		}
		

		
		this->model->prepareTransaction(currentTransaction, ctrl);

		
		this->model->sendCoins(currentTransaction);
		

	} catch(...) { 
		QMessageBox msgBoxError;
		msgBoxError.setText("An error occured trying to send coins");
		msgBoxError.exec();
	
	}
			

	
}

void RepositoryDialog::handleInformation()
{
	int row = tableWidget->currentRow();

	QWidget *w = qobject_cast<QWidget *>(sender()->parent());
	if(w){
		row = tableWidget->indexAt(w->pos()).row();
	}	

	QString artifactSelected = tableWidget->item(row, ARTIFACT_COLUMN)->text(); 

	
	PlaceholderUtility* pu = new PlaceholderUtility();
	
	QEventLoop loop;
	QNetworkAccessManager nam;
	QNetworkRequest req(QUrl(pu->getArtifactDetailURL() + artifactSelected));
	QNetworkReply *reply = nam.get(req);
	connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	loop.exec();
	QByteArray buffer = reply->readAll();
	QString dataLine = buffer.constData();
		
	QJsonObject o = pu->objectFromString(dataLine.toUtf8().constData());

	QString encapsulation = o.value("encapsulation").toString();
	QString asigned = o.value("signed").toString();
	QString signature = o.value("signature").toString();
	QString seed = o.value("seed").toString();
	QString contentType = o.value("contentType").toString();
	QString size = o.value("size").toString();
	QString checksum = o.value("checksum").toString();
	QString status = o.value("status").toString();
	QString bounty = o.value("bounty").toString();
	QString description = o.value("description").toString();
	QString service = o.value("service").toString();
	QString tags = o.value("tags").toString();
			
	
		
	QString msg =  "Encapsulation:" + encapsulation + "\n" + 
				   "Seed:         " + seed + "\n" + 
				   "Signature:    " + signature + "\n" +
				   "Checksum:     " + checksum + "\n" + 
				   "Size:         " + size + "\n" + 
				   "Tags:         " + tags + "\n";
			   
	QMessageBox msgBoxInformation;
	//msgBoxInformation.setStyleSheet("QMessageBox { font-family: monospace; }");
	const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
	
	msgBoxInformation.setFont(fixedFont);
	msgBoxInformation.setText(msg);
	msgBoxInformation.exec();
}

bool started = false;

void doDownload(QString artifact)
{
	started = true;
	PlaceholderUtility* pu = new PlaceholderUtility();
	pu->download(artifact);			
	started = false;
}

void RepositoryDialog::handleDownload()
{
	try { 
	

		int row = tableWidget->currentRow();

		QWidget *w = qobject_cast<QWidget *>(sender()->parent());
		if(w){
			row = tableWidget->indexAt(w->pos()).row();
		}	


		QString artifactSelected = tableWidget->item(row, ARTIFACT_COLUMN)->text(); 
		QString bountySelected = tableWidget->item(row, PRICE_COLUMN)->text();
		QString description = tableWidget->item(row, DESCRIPTION_COLUMN)->text();
		QString contentType = tableWidget->item(row, CONTENT_TYPE_COLUMN)->text();
		QString checksum = tableWidget->item(row, CHECKSUM_COLUMN)->text();

		QString theTitle = "Confirm Payment?";
		QString theQuestion = "Pay Address " + artifactSelected + "\n " + bountySelected + " PHL\n to download:\n " + description + "\n";
		QMessageBox::StandardButton reply;

		
		reply = QMessageBox::question(this, theTitle,  theQuestion, QMessageBox::Yes|QMessageBox::No);

		if (reply == QMessageBox::Yes) {
			try { 
			if( bountySelected != "0.00000000" ) { 
				sendCoins(bountySelected, artifactSelected);
			}
			} catch(...) { } 
					
			
			PlaceholderUtility* pu = new PlaceholderUtility();
			QString newExtension = pu->getExtensionByContentType(contentType);
			QMessageBox msgBoxError;
			msgBoxError.setText("Downloading to " + pu->getVDIPath() + "/" + artifactSelected + ".artifact.\n Content Type:" + contentType + "\nRenaming to: " + newExtension);
			msgBoxError.exec();		
					
			//PlaceholderUtility* pu = new PlaceholderUtility();
			//doDownload(artifactSelected);
			std::thread t(doDownload, artifactSelected);
			t.detach();
			
			//pu->download(artifactSelected);	
		
			try { 
				bool flag = true;
				Crc32* crc32 = new Crc32();
				QString fName = pu->getVDIPath() + "/" + artifactSelected + ".artifact";
				while( flag ) { 
						
					quint32 crc = crc32->calculateFromFile(fName);
					quint32 check = (quint32)checksum.toUInt();
					if( check != crc ) {
						//qDebug() << "File not open yet " << file.error();
						 QCoreApplication::processEvents();
						 QApplication::processEvents() ;
					} else{
					
						flag = false;
						file.close();
						qDebug() << "File is open";
					}

					if( started == false ) { 
						flag = false;
						file.close();
						
					}
							
				}
				
			} catch(...) { 
				QMessageBox msgBoxErrorA;
				msgBoxErrorA.setText("Checksum did not validate");
				msgBoxErrorA.exec();		
						
			} 
			
					
			try { 
				QMessageBox msgBoxDone;
				msgBoxDone.setText("Download Complete.");
				msgBoxDone.exec();	

				QThread::sleep(unsigned long second); // allow the OS to finish closing the file/.
						
				QFile::rename(pu->getVDIPath() + "/" + artifactSelected + ".artifact", pu->getVDIPath() + "/" + artifactSelected + newExtension);
						
				QMessageBox::StandardButton newReply;
				QString theNewTitle = "Open File?";
				QString theNewQuestion = "Do you want to open:" + artifactSelected + newExtension + "?\n";
				newReply = QMessageBox::question(this, theNewTitle,  theNewQuestion, QMessageBox::Yes|QMessageBox::No);
				
				if (newReply == QMessageBox::Yes) {
					QDesktopServices::openUrl(QUrl(pu->getVDIPath() + "/" + artifactSelected + newExtension));
				}
			
			} catch(...) { 
				QMessageBox msgBoxErrorA;
				msgBoxErrorA.setText("Renaming artifact / opening failed. Will open parent containing folder now.");
				msgBoxErrorA.exec();
				QDesktopServices::openUrl(QUrl(pu->getVDIPath()));
			}
			
			
		} else {
			qDebug() << "Yes was *not* clicked";
		}
	
	} catch(...) { 
		QMessageBox msgBoxDone;
		msgBoxDone.setText("Null PTR.");
		msgBoxDone.exec();						
	
	}
}


RepositoryDialog::~RepositoryDialog()
{
	try { 
		delete ui;
	} catch(...){}
}

/** PHL END */
