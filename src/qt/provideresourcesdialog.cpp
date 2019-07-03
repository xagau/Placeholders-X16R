// Copyright (c) 2011-2016 The Bitcoin Core developers
// Copyright (c) 2017 The Placeholder Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "provideresourcesdialog.h"
#include "sendcoinsdialog.h"
#include "ui_provideresourcesdialog.h"

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


#include <iostream>
#include <fstream>
#include <string>
#include <thread>

#include <QString>
#include <QDir>
#include <QProcess>
#include <QFontMetrics>
#include <QMessageBox>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QLabel>
#include <QSettings>
#include <QTextDocument>
#include <QTableWidget>
#include <QTimer>

#include <QUrl>
#include <QUrlQuery>
#include <QEventLoop>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QUuid>

#include <policy/policy.h>
#include <core_io.h>
#include <rpc/mining.h>


ProvideResourcesDialog::ProvideResourcesDialog(const PlatformStyle *_platformStyle, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::ProvideResourcesDialog),
        clientModel(0),
        model(0)        
{
	try { 
	horizontalSliders = new SlidersGroup(Qt::Horizontal, tr("Diskspace (GB SSD)"));
    verticalSliders = new SlidersGroup(Qt::Vertical, tr("Vertical"));

    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(horizontalSliders);
    stackedWidget->addWidget(verticalSliders);

    createControls(tr("Controls"));
	
	connect(horizontalSliders, SIGNAL(valueChanged(int)),
            verticalSliders, SLOT(setValue(int)));
    connect(verticalSliders, SIGNAL(valueChanged(int)),
            diskspaceSpinBox, SLOT(setValue(int)));
    connect(diskspaceSpinBox, SIGNAL(valueChanged(int)),
            horizontalSliders, SLOT(setValue(int)));
	//connect(vvalueSpinBox, SIGNAL(valueChanged(int)),
    //        horizontalSliders, SLOT(setValue(int)));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(controlsGroup);
    layout->addWidget(stackedWidget);
    setLayout(layout);

    coresSpinBox->setValue(0);
    coresCostSpinBox->setValue(0);
    memorySpinBox->setValue(1);
    memoryCostSpinBox->setValue(1);
    diskspaceSpinBox->setValue(8);
    diskspaceCostSpinBox->setValue(8);
    bandwidthSpinBox->setValue(1);
    bandwidthCostSpinBox->setValue(1);

    setWindowTitle(tr("Provide Resources"));
	
	} catch(...) { } 
	

    /** PHL END */
}





void ProvideResourcesDialog::provide()
{
	
	
	PlaceholderUtility* pu = new PlaceholderUtility();
	
	
	if(userId->text().trimmed().isEmpty())
	{
		QMessageBox msgBoxError;
		msgBoxError.setText("Portal User ID Required");
		msgBoxError.exec();			
		userId->setFocus();
		return;
	}
	
	if(password->text().trimmed().isEmpty())
	{
		QMessageBox msgBoxError;
		msgBoxError.setText("Portal Password Required");
		msgBoxError.exec();			
		password->setFocus();
		return;
	}
	
	//if(!pu->isMachineConfiguredForVirtualBox())
	//{
	//	QMessageBox msgBoxError;
	//	msgBoxError.setText("This machine is not configured for Virtual Box");
	//	msgBoxError.exec();			
	//	return;
	//}
	
	
	
	QNetworkAccessManager * manager = new QNetworkAccessManager(this);
	QUrlQuery query;
	QUrl params;
	QByteArray postData;
	
	try { query.addQueryItem("providerId", userId->text().trimmed());} catch(...) { } 
	try { query.addQueryItem("notes", "These are the notes");} catch(...) { } 
	try { query.addQueryItem("selectMaxBandwidth", "" + bandwidthSpinBox->value());} catch(...) { } 
	try { query.addQueryItem("selectMaxCores", "" + coresSpinBox->value());} catch(...) { } 
	try { query.addQueryItem("selectMaxMemory", "" + memorySpinBox->value());} catch(...) { } 
	try { query.addQueryItem("selectMaxDiskspace", "" + diskspaceSpinBox->value());} catch(...) { } 
	try { query.addQueryItem("selectMaxDuration", "1");} catch(...) { } 
	try { query.addQueryItem("selectMaxGPUs", "1");} catch(...) { } 
	try { query.addQueryItem("OS", "default-template");} catch(...) { } 

	try { query.addQueryItem("perBandwidthPrice", "" +  QString::number ( bandwidthCostSpinBox->value()));} catch(...) { } 
	try { query.addQueryItem("perCorePrice", "" +  QString::number ( coresCostSpinBox->value()));} catch(...) { } 
	try { query.addQueryItem("perMemoryPrice", "" +  QString::number (memoryCostSpinBox->value()));} catch(...) { } 
	try { query.addQueryItem("perGPUPrice", "1");} catch(...) { } 
	try { query.addQueryItem("perDiskspacePrice", "" +  QString::number (diskspaceCostSpinBox->value()));} catch(...) { } 
	try { query.addQueryItem("referenceId", QUuid::createUuid().toString());} catch(...) { } 
	try { query.addQueryItem("architecture", "Placeholder Native"); } catch(...) { } 


	params.setQuery(query);
	//postData = params.toEncoded(QUrl::RemoveFragment);

	// Call the webservice
	QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
	connect(networkManager, SIGNAL(finished(QNetworkReply*)),
			SLOT(onPostAnswer(QNetworkReply*)));
			
	QUrl serviceUrl  = QUrl(pu->getProvideServiceEndPointURL());
	
	QNetworkRequest networkRequest(serviceUrl);
	networkRequest.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"data\""));

	networkManager->post(networkRequest,params.query().toUtf8());
	
	if(!pu->isMachineConfiguredForVirtualBox())
	{
		QMessageBox msgBoxError;
		msgBoxError.setText("This machine is not configured for Virtual Box");
		msgBoxError.exec();			
	}
	else {
		QMessageBox msgBoxOK;
		msgBoxOK.setText("This machine has Virtual Box installed");
		msgBoxOK.exec();	
	}
}

void  ProvideResourcesDialog::createControls(const QString &title)
{
	try { 
		controlsGroup = new QGroupBox(title);

		userLabel = new QLabel(tr("User ID:"));
		passwordLabel = new QLabel(tr("Password:"));
		
		userId = new QLineEdit();
		password = new QLineEdit();
		password->setEchoMode(QLineEdit::Password);
		
		coresLabel = new QLabel(tr("Maximum Cores (CPUs):"));
		coresSpinBox = new QSpinBox;
		coresSpinBox->setRange(1, 32);
		coresSpinBox->setSingleStep(1);
		
		coresCostLabel = new QLabel(tr("Maximum Cores (CPUs):"));
		coresCostSpinBox = new QDoubleSpinBox;
		coresCostSpinBox->setPrefix("$");
		coresCostSpinBox->setRange(0.01, 320);
		coresCostSpinBox->setSingleStep(0.01);
		
		memoryLabel = new QLabel(tr("Maximum Memory GB:"));
		memorySpinBox = new QSpinBox;
		memorySpinBox->setRange(1, 1280);
		memorySpinBox->setSingleStep(1);

		memoryCostLabel = new QLabel(tr("Per Per GB Memory:"));
		memoryCostSpinBox = new QDoubleSpinBox;
		memoryCostSpinBox->setPrefix("$");
		memoryCostSpinBox->setRange(0.01, 1280);
		memoryCostSpinBox->setSingleStep(0.01);

		diskspaceLabel   = new QLabel(tr("Maximum Diskspace GB:"));
		diskspaceSpinBox = new QSpinBox;
		diskspaceSpinBox->setRange(1, 1000);
		diskspaceSpinBox->setSingleStep(1);

		diskspaceCostLabel   = new QLabel(tr("Price Per GB:"));
		diskspaceCostSpinBox = new QDoubleSpinBox;
		diskspaceCostSpinBox->setPrefix("$");
		diskspaceCostSpinBox->setRange(0.01, 1000);
		diskspaceCostSpinBox->setSingleStep(0.01);
		
		bandwidthLabel   = new QLabel(tr("Maximum Bandwidth TB:"));
		bandwidthSpinBox = new QSpinBox;
		bandwidthSpinBox->setPrefix("$");
		bandwidthSpinBox->setRange(1, 1000);
		bandwidthSpinBox->setSingleStep(1);

		bandwidthCostLabel   = new QLabel(tr("Price Per TB:"));
		bandwidthCostSpinBox = new QDoubleSpinBox;
		bandwidthCostSpinBox->setPrefix("$");
		bandwidthCostSpinBox->setRange(0.01, 1000);
		bandwidthCostSpinBox->setSingleStep(0.01);
		
		costLabel  = new QLabel(tr("Cost (USD) per Duration\nFor this configuration:"));
		costSpinBox = new QDoubleSpinBox;
		costSpinBox->setRange(0.01, 100000);
		costSpinBox->setPrefix("$");
		costSpinBox->setSingleStep(0.01);
		
		provideResourcesButton = new QPushButton(tr("Provide Resources"));

		connect(provideResourcesButton, SIGNAL(clicked()), this, SLOT(provide()) );			

		QGridLayout *controlsLayout = new QGridLayout;
		
		controlsLayout->addWidget(userLabel, 0, 0);
		controlsLayout->addWidget(userId, 0, 1);

		controlsLayout->addWidget(passwordLabel, 1, 0);
		controlsLayout->addWidget(password, 1, 1);

		controlsLayout->addWidget(coresLabel, 2, 0);
		controlsLayout->addWidget(coresSpinBox, 2, 1);

		controlsLayout->addWidget(coresCostLabel, 3, 0);
		controlsLayout->addWidget(coresCostSpinBox, 3, 1);
		
		controlsLayout->addWidget(memoryLabel, 4, 0);
		controlsLayout->addWidget(memorySpinBox, 4, 1);

		controlsLayout->addWidget(memoryCostLabel, 5, 0);
		controlsLayout->addWidget(memoryCostSpinBox, 5, 1);
		
		controlsLayout->addWidget(diskspaceLabel, 6, 0);
		controlsLayout->addWidget(diskspaceSpinBox, 6, 1);

		controlsLayout->addWidget(diskspaceCostLabel, 7, 0);
		controlsLayout->addWidget(diskspaceCostSpinBox, 7, 1);

		controlsLayout->addWidget(bandwidthLabel, 8, 0);
		controlsLayout->addWidget(bandwidthSpinBox, 8, 1);    

		controlsLayout->addWidget(bandwidthCostLabel, 9, 0);
		controlsLayout->addWidget(bandwidthCostSpinBox, 9, 1);    

		//controlsLayout->addWidget(costLabel, 10, 0);
		//controlsLayout->addWidget(costSpinBox, 10, 1);
		
		//controlsLayout->addWidget(invertedAppearance, 0, 2);
		//controlsLayout->addWidget(invertedKeyBindings, 1, 2);

		//controlsLayout->addWidget(orientationCombo, 7, 0, 1, 3);
		controlsLayout->addWidget(provideResourcesButton, 10, 0, 1, 3);
			
		controlsGroup->setLayout(controlsLayout);
	
	
	} catch(...) { } 
	
}

void ProvideResourcesDialog::setClientModel(ClientModel *_clientModel)
{
    this->clientModel = _clientModel;
}

void ProvideResourcesDialog::setModel(WalletModel *_model)
{
    this->model = _model;
}


ProvideResourcesDialog::~ProvideResourcesDialog()
{
	try { 
		delete ui;
	} catch(...) { } 
}

