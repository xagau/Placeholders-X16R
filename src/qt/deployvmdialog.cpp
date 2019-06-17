// Copyright (c) 2011-2016 The Bitcoin Core developers
// Copyright (c) 2017 The Placeholder Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "deployvmdialog.h"
#include "sendcoinsdialog.h"
#include "ui_deployvmdialog.h"

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
#include "crc32.h"
#include "placeholderutility.h"

#include <iostream>
#include <fstream>
#include <string>
#include <thread>

#include <QtWidgets>

#include <QString>
#include <QDir>
#include <QProcess>
#include <QFontMetrics>
#include <QMessageBox>
#include <QScrollBar>
#include <QSettings>
#include <QLabel>
	
#include <QHBoxLayout>
#include <QTextDocument>
#include <QTableWidget>
#include <QTimer>
#include <policy/policy.h>
#include <core_io.h>
#include <rpc/mining.h>

#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QHttpMultiPart>
#include <QByteArray>
#include <QDesktopServices>
//#include <QCryptographicHash>


#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include <QTextEdit>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QFileDialog>

#include <QLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QSpinBox>
#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>


#include <zlib.h>


DeployVMDialog::DeployVMDialog(const PlatformStyle *_platformStyle, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DeployVMDialog),
        clientModel(0),
        model(0)     
{

	bigEditor = new QTextEdit;
    bigEditor->setPlainText(tr("Ready..."));

	 	
    createHorizontalGroupBox();

	
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	 
	mainLayout->addWidget(horizontalGroupBox);

    mainLayout->addWidget(bigEditor);
    mainLayout->addWidget(buttonBox);
	
	setLayout(mainLayout);

    setWindowTitle(tr("Deploy To Network"));
	
}

void DeployVMDialog::createHorizontalGroupBox()
{
	int maxHeight = 55;
	int margin    = 15;
	
    horizontalGroupBox = new QGroupBox(tr("Network Deployment Details"));
	horizontalGroupBox->setFixedHeight(maxHeight);
	
    QHBoxLayout *layout = new QHBoxLayout;

	

        QPushButton* browseButton = new QPushButton(tr("Browse for File"));
		
        layout->addWidget(browseButton);
		QLabel* checksumLabel = new QLabel(tr("Checksum"));
        layout->addWidget(checksumLabel);
		
		checksum->setMaximumWidth(100);
		checksum->setFixedWidth(80);
		
		layout->addWidget(checksum);
		
		QLabel* artifactLabel = new QLabel(tr("Artifact"));		
		layout->addWidget(artifactLabel);

		layout->addWidget(artifact);
		
		QLabel* signatureLabel = new QLabel(tr("Signature"));		
		layout->addWidget(signatureLabel);


		QCheckBox* signatureCheckBox = new QCheckBox(tr("Generate Signature"));
		layout->addWidget(signature);
		layout->addWidget(signatureCheckBox);
		
		QLabel* bountyLabel = new QLabel(tr("Bounty"));
		layout->addWidget(bountyLabel);

		QLineEdit* bounty = new QLineEdit();
		bounty->setMaximumWidth(80);
		bounty->setFixedWidth(100);
		
        layout->addWidget(bounty);
		
		deployToNetworkButton = new QPushButton(tr("Deploy to Network"));

		deployToNetworkButton->setEnabled(false);
		
		viewOnExplorerButton = new QPushButton(tr("View on Explorer"));

        layout->addWidget(deployToNetworkButton);
        layout->addWidget(viewOnExplorerButton);
		
		clearConsoleButton = new QPushButton(tr("Clear"));
		layout->addWidget(clearConsoleButton);
		
		//layout->addStretch(1);

		connect(viewOnExplorerButton, SIGNAL(clicked()), this, SLOT(viewOnExplorer()));
		connect(clearConsoleButton, SIGNAL(clicked()), bigEditor, SLOT(clear()));
		connect(browseButton, SIGNAL(clicked()), this, SLOT(selectFile()));
		connect(deployToNetworkButton, SIGNAL(clicked()), this, SLOT(deployToNetwork()));
		
		//
		horizontalGroupBox->setLayout(layout);
}


void DeployVMDialog::selectFile()
{
	try { 
		fileName = QFileDialog::getOpenFileName(this, ("Open File"),
													  "/home",
													  ("Artifact (*.png *.gif *.jpg *.torrent *.txt *.html *.xml *.htm *.mov *.mpeg *.mp3 *.js *.pdf *.vdi *.json)"));

		deployToNetworkButton->setEnabled(true);
		Crc32 *crc32 = new Crc32();
		
		quint32 crc = crc32->calculateFromFile(fileName);
		
		QString cs = QString::number(crc);
		checksum->setText(cs);
		
		if( fileName.endsWith(".zip", Qt::CaseInsensitive) ) { 
		
			QMessageBox msgBoxC;
			msgBoxC.setText("This zip will be converted to torrent for seeding");
			msgBoxC.exec();										

		}
		else if( fileName.endsWith(".vdi", Qt::CaseInsensitive) ) { 
			QMessageBox msgBoxC;
			msgBoxC.setText("This VDI will be converted to torrent for seeding");
			msgBoxC.exec();										
		
		}
		else if( fileName.endsWith(".mpeg", Qt::CaseInsensitive) ) { 
			QMessageBox msgBoxC;
			msgBoxC.setText("This MPEG will be converted to torrent for seeding");
			msgBoxC.exec();										
		
		}
		else if( fileName.endsWith(".jpg", Qt::CaseInsensitive) ) { 
			contentMimeType = "image/jpeg";								
		}		
		else if( fileName.endsWith(".gif", Qt::CaseInsensitive) ) { 
			contentMimeType = "image/gif";								
		}		
		else if( fileName.endsWith(".png", Qt::CaseInsensitive) ) { 
			contentMimeType = "image/png";								
		}		
		else if( fileName.endsWith(".js", Qt::CaseInsensitive) ) { 
			contentMimeType = "text/javascript";								
		}	
		else if( fileName.endsWith(".json", Qt::CaseInsensitive) ) { 
			contentMimeType = "application/json";								
		}
		else if( fileName.endsWith(".mp3", Qt::CaseInsensitive) ) { 
			contentMimeType = "audio/mpeg";								
		}
		else if( fileName.endsWith(".xml", Qt::CaseInsensitive) ) { 
			contentMimeType = "text/xml";								
		}
		else if( fileName.endsWith(".txt", Qt::CaseInsensitive) ) { 
			contentMimeType = "text/plain";								
		}
		else if( fileName.endsWith(".pdf", Qt::CaseInsensitive) ) { 
			contentMimeType = "application/pdf";								
		}
		else if( fileName.endsWith(".html", Qt::CaseInsensitive) || fileName.endsWith(".htm", Qt::CaseInsensitive) ) { 
			contentMimeType = "text/html";								
		}
		else if( fileName.endsWith(".torrent", Qt::CaseInsensitive) ) { 
			contentMimeType = "application/x-bittorrent";								
		}
		
	}
	catch(...) { 
		QMessageBox msgBoxC;
		msgBoxC.setText("An error occurred mapping the MIME type");
		msgBoxC.exec();				
	}
}


void DeployVMDialog::viewOnExplorer()
{
	QString address = artifact->text();
	if (address.trimmed().isEmpty()){
		QMessageBox msgBoxC;
		msgBoxC.setText("No Artifact!!!");
		msgBoxC.exec();	
		
		console("No Artifact!!!");
		console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
		
		return;
	}
	console("Open browser");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	
	//QString address = artifact->text();
	QString link = "http://explore.placeh.io:8080/raw/" + address;
	QDesktopServices::openUrl(QUrl(link));
	console("OK");
}


void DeployVMDialog::console(const char* message)
{
	bigEditor->setText(bigEditor->toPlainText() + "" + message + "\n");		
		
	QApplication::processEvents();
		
	QTextCursor cursor1= bigEditor->textCursor();
	cursor1.atEnd();
	bigEditor->setTextCursor(cursor1);
	bigEditor->ensureCursorVisible();
	bigEditor->verticalScrollBar()->setValue(bigEditor->verticalScrollBar()->maximum());
	bigEditor->repaint();
}

void DeployVMDialog::deployToNetwork()
{
	
	console("Create Network Access Manager");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
	console("Connecting signals and slots to network reply...");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	connect(networkManager, SIGNAL(finished(QNetworkReply*)),
			SLOT(onPostAnswer(QNetworkReply*)));

	console("Locating Network Repository");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	
	PlaceholderUtility* pu = new PlaceholderUtility();
	QUrl testUrl(pu->getDeployEntryPointURL());
	QUrlQuery query;
	//QUrl params;
	QByteArray postData;

	console("Setting MIME type");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	
	query.addQueryItem("contentType", contentMimeType);//"application/x-bittorrent");
	query.addQueryItem("verifySignature", "-");
	console("WARNING: BAD SIGNATURE!!!");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	query.addQueryItem("signature", signature->text());
	console("WARNING: NO CHECKSUM!!!");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	query.addQueryItem("crc32Long", "-");
	query.addQueryItem("crc32Hex", "-");
	query.addQueryItem("verifyCommand", "-");
	//query.addQueryItem("file", "-=-=-=-=-=-=-");
	console("Loading to artifact");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	query.addQueryItem("artifact", artifact->text());
	query.addQueryItem("address", artifact->text());
	query.addQueryItem("data", "-=-=-=" + artifact->text() + "=-=-=-");
	console("Setting network parameters...");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

	postData = testUrl.toEncoded(QUrl::RemoveFragment);
	
	testUrl.setQuery(query);
	console("Encoding fragment...");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	
	QNetworkRequest request;
	request.setUrl(testUrl);
	console("Setting Raw Header");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	request.setRawHeader("User-Agent", "Placeholders-X16R 2.0.30.1");

	console("Creating Multipart HTTP Form Data");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

	QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

	QString preview_path  = fileName; 
	QHttpPart previewPathPart;
	previewPathPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"path\""));
	previewPathPart.setBody(preview_path.toLatin1());
	console("Body Loaded");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

	QHttpPart previewFilePart;
	previewFilePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant( "image/jpeg"));
	previewFilePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\""));
	QFile *file = new QFile(preview_path);
	
	console("File check");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	if (!file->exists()) {
		console("Upload Error. File does not exist. Aborting! ");
		console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
		//emit error(tr("Upload Error. File does not exist: ") + preview_path);
		return;
	}	
	console("Open I/O");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	file->open(QIODevice::ReadOnly);
	previewFilePart.setBodyDevice(file);
	file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart
	console("Appending data");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	
	multiPart->append(previewPathPart);
	multiPart->append(previewFilePart);
	console("Post Data to Network");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	
	QNetworkReply* reply = networkManager->post(request, multiPart);
	//multiPart->setParent(reply); // delete the multiPart with the reply
	
	console("Connecting SIGNAL");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

	connect(networkManager, SIGNAL(finished(QNetworkReply*)),
			SLOT(onPostAnswer(QNetworkReply*)));
	
	console("Complete");


}


/*
void DeployVMDialog::deployToNetwork()
{
	//https://stackoverflow.com/questions/23344370/qhttpmultipart-upload-multiple-files
	bigEditor->setText("");	
	console("Creating network access manager...");
	QNetworkAccessManager * manager = new QNetworkAccessManager(this);
	console("OK...");
	
	console("Building network request...");
	QUrlQuery query;
	QUrl params;
	QByteArray postData;
	
	query.addQueryItem("contentType", "application/x-bittorrent");
	query.addQueryItem("verifySignature", "-");
	query.addQueryItem("signature", "-");
	query.addQueryItem("crc32Long", "-");
	query.addQueryItem("crc32Hex", "-");
	query.addQueryItem("verifyCommand", "-");
	query.addQueryItem("file", "-=-=-=-=-=-=-");
	query.addQueryItem("artifact", "F9ddp3zXbNd5zwxBngvRBe4zQgmoViNuma");
	query.addQueryItem("address", "F9ddp3zXbNd5zwxBngvRBe4zQgmoViNuma");
	query.addQueryItem("data", "-=-=-=FFF-=-=-=-");
	console("Setting network parameters...");

	params.setQuery(postData);
	console("Encoding fragment...");
	
	
	postData = params.toEncoded(QUrl::RemoveFragment);
	console("Use network access manager...");

	// Call the webservice
	QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
	console("Connecting signals and slots to network reply...");
	connect(networkManager, SIGNAL(finished(QNetworkReply*)),
			SLOT(onPostAnswer(QNetworkReply*)));
			
	console("Construct service URL...");
	QUrl serviceUrl  = QUrl("http://explore.placeh.io:8080/deploy");
	console("Create network request...");
	
	QNetworkRequest networkRequest(serviceUrl);
	
	console("Setting header...");
	networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
	
	console("Broadcasting data...");
	
	networkManager->post(networkRequest,postData);

	console("Done...");

	
	
	//char* somedata = "Hello World";
	
	//unsigned long  crc = crc32(0L, Z_NULL, 0);
	//crc = crc32(crc, (const unsigned char*)somedata, 11);
	
	//console("" + crc );
	
	
	//for(int i = 0; i < 10000; i++) { 
	//	std::string s = std::to_string(i);
	//	const char *pchar = s.c_str();
	//	
	//	console(pchar);
	//}	
}
*/



void DeployVMDialog::setClientModel(ClientModel *_clientModel)
{
    this->clientModel = _clientModel;
}



void DeployVMDialog::setModel(WalletModel *_model)
{
    this->model = _model;
}

DeployVMDialog::~DeployVMDialog()
{
   	try { 
		delete ui;
	} catch(...) { } 
}

/** PHL END */
