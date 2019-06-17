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
#include <policy/policy.h>
#include <core_io.h>
#include <rpc/mining.h>


ProvideResourcesDialog::ProvideResourcesDialog(const PlatformStyle *_platformStyle, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::ProvideResourcesDialog),
        clientModel(0),
        model(0)        
{
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
    memorySpinBox->setValue(1);
    diskspaceSpinBox->setValue(8);
    bandwidthSpinBox->setValue(1);

    setWindowTitle(tr("Sliders"));

    /** PHL END */
}

void  ProvideResourcesDialog::createControls(const QString &title)
{
    controlsGroup = new QGroupBox(title);

    coresLabel = new QLabel(tr("Maximum Cores (CPUs):"));
    memoryLabel = new QLabel(tr("Maximum Memory GB:"));
    diskspaceLabel   = new QLabel(tr("Maximum Diskspace GB:"));
    bandwidthLabel   = new QLabel(tr("Maximum Bandwidth TB:"));
    costLabel  = new QLabel(tr("Cost (USD) per Duration\nFor this configuration:"));

    invertedAppearance = new QCheckBox(tr("Use up to maximum"));
    invertedKeyBindings = new QCheckBox(tr("Use up to maximum"));
	
	coresSpinBox = new QSpinBox;
    coresSpinBox->setRange(1, 32);
    coresSpinBox->setSingleStep(1);

    memorySpinBox = new QSpinBox;
    memorySpinBox->setRange(1, 128);
    memorySpinBox->setSingleStep(1);

    diskspaceSpinBox = new QSpinBox;
    diskspaceSpinBox->setRange(1, 1000);
    diskspaceSpinBox->setSingleStep(1);

    bandwidthSpinBox = new QSpinBox;
    bandwidthSpinBox->setRange(1, 1000);
    bandwidthSpinBox->setSingleStep(1);
	
	
	costSpinBox = new QSpinBox;
    costSpinBox->setRange(0.01, 100000);
    costSpinBox->setSingleStep(0.01);

    orientationCombo = new QComboBox;
    orientationCombo->addItem(tr("Automatically host best offer"));
    orientationCombo->addItem(tr("Strictly enforce rate"));
	
	connect(orientationCombo, SIGNAL(activated(int)),
            stackedWidget, SLOT(setCurrentIndex(int)));


    QGridLayout *controlsLayout = new QGridLayout;
    controlsLayout->addWidget(coresLabel, 0, 0);
    controlsLayout->addWidget(memoryLabel, 1, 0);
    controlsLayout->addWidget(diskspaceLabel, 2, 0);
    controlsLayout->addWidget(bandwidthLabel, 3, 0);
    controlsLayout->addWidget(costLabel, 4, 0);
    controlsLayout->addWidget(coresSpinBox, 0, 1);
    controlsLayout->addWidget(memorySpinBox, 1, 1);
    controlsLayout->addWidget(diskspaceSpinBox, 2, 1);
    controlsLayout->addWidget(bandwidthSpinBox, 3, 1);    
	controlsLayout->addWidget(costSpinBox, 4, 1);
	
    controlsLayout->addWidget(invertedAppearance, 0, 2);
    controlsLayout->addWidget(invertedKeyBindings, 1, 2);
    controlsLayout->addWidget(orientationCombo, 5, 0, 1, 3);
    controlsGroup->setLayout(controlsLayout);
	
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

