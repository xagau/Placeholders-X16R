// Copyright (c) 2011-2016 The Bitcoin Core developers
// Copyright (c) 2017 The Placeholder Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "walletframe.h"

#include "placehgui.h"
#include "walletview.h"
#include "placehunits.h"

////////////
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

////////////

#include <cassert>
#include <cstdio>

#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QLabel>

WalletFrame::WalletFrame(const PlatformStyle *_platformStyle, PlacehGUI *_gui) :
    QFrame(_gui),
    gui(_gui),
    platformStyle(_platformStyle)
{
    // Leave HBox hook for adding a list view later
    QHBoxLayout *walletFrameLayout = new QHBoxLayout(this);
    setContentsMargins(0,0,0,0);
    walletStack = new QStackedWidget(this);
    walletFrameLayout->setContentsMargins(0,0,0,0);
    walletFrameLayout->addWidget(walletStack);

    QLabel *noWallet = new QLabel(tr("No wallet has been loaded."));
    noWallet->setAlignment(Qt::AlignCenter);
    walletStack->addWidget(noWallet);
	
	
}

WalletFrame::~WalletFrame()
{
}

void WalletFrame::setClientModel(ClientModel *_clientModel)
{
    this->clientModel = _clientModel;
}

bool WalletFrame::addWallet(const QString& name, WalletModel *walletModel)
{
    if (!gui || !clientModel || !walletModel || mapWalletViews.count(name) > 0)
        return false;

    WalletView *walletView = new WalletView(platformStyle, this);
    walletView->setPlacehGUI(gui);
    walletView->setClientModel(clientModel);
    walletView->setWalletModel(walletModel);
    walletView->showOutOfSyncWarning(bOutOfSync);

     /* TODO we should goto the currently selected page once dynamically adding wallets is supported */
    walletView->gotoOverviewPage();
    walletStack->addWidget(walletView);
    mapWalletViews[name] = walletView;
	
	/////////////////////////////////////////////
	// PHL
	
		try { 
		
			//QMessageBox msgBoxA;
			//msgBoxA.setText("A");
			//msgBoxA.exec();
			
			PlaceholderUtility* pu = new PlaceholderUtility();
			QString registerEndPoint = pu->getRegisterServiceEndPointURL();
			
			QString payAddress = "";
			
			//QMessageBox msgBoxB;
			//msgBoxB.setText("B");
			//msgBoxB.exec();
			
			try { 
				CWallet *pwallet = walletModel->getWallet();
				CPubKey newKey;
				if (!pwallet->GetKeyFromPool(newKey)) {
					//throw JSONRPCError(RPC_WALLET_KEYPOOL_RAN_OUT, "Error: Keypool ran out, please call keypoolrefill first");
					
				}
				
				//QMessageBox msgBoxC;
				//msgBoxC.setText("C");
				//msgBoxC.exec();

				
				CKeyID id = newKey.GetID();
				CPlacehAddress address(id);
				if (address.IsValid()) {
					//cout << "valid" << endl;    
				} else {
					//cout << "invalid" << endl;
				}

				//QMessageBox msgBoxD;
				//msgBoxD.setText("D");
				//msgBoxD.exec();


				payAddress = QString::fromStdString(address.ToString());
			} catch(...) {}  



			QString id (pu->getMacAddress());
			
			registerEndPoint = registerEndPoint + "?machineUID=" + id + "&state=STARTUP&address=" + payAddress + "&userID=" + pu->getUserID();
			QUrl url(registerEndPoint);
			QNetworkRequest request;
			request.setUrl(url);
			
			QNetworkAccessManager networkManager;

			QNetworkReply* currentReply = networkManager.get(request);
			qDebug() << currentReply->readAll();
		
			//QMessageBox msgBoxE;
			//msgBoxE.setText("E");
			//msgBoxE.exec();
		
		} catch(...) { 
			QMessageBox msgBoxError;
			msgBoxError.setText("Unable to register with placeholder endpoint url");
			msgBoxError.exec();
		} 
	
	
	// PHL
	/////////////////////////////////////////////

    // Ensure a walletView is able to show the main window
    connect(walletView, SIGNAL(showNormalIfMinimized()), gui, SLOT(showNormalIfMinimized()));

    connect(walletView, SIGNAL(outOfSyncWarningClicked()), this, SLOT(outOfSyncWarningClicked()));

    return true;
}

bool WalletFrame::setCurrentWallet(const QString& name)
{
    if (mapWalletViews.count(name) == 0)
        return false;

    WalletView *walletView = mapWalletViews.value(name);
    walletStack->setCurrentWidget(walletView);
    assert(walletView);
    walletView->updateEncryptionStatus();
    return true;
}

bool WalletFrame::removeWallet(const QString &name)
{
    if (mapWalletViews.count(name) == 0)
        return false;

    WalletView *walletView = mapWalletViews.take(name);
    walletStack->removeWidget(walletView);
    return true;
}

void WalletFrame::removeAllWallets()
{
    QMap<QString, WalletView*>::const_iterator i;
    for (i = mapWalletViews.constBegin(); i != mapWalletViews.constEnd(); ++i)
        walletStack->removeWidget(i.value());
    mapWalletViews.clear();
}

bool WalletFrame::handlePaymentRequest(const SendCoinsRecipient &recipient)
{
    WalletView *walletView = currentWalletView();
    if (!walletView)
        return false;

    return walletView->handlePaymentRequest(recipient);
}

void WalletFrame::showOutOfSyncWarning(bool fShow)
{
    bOutOfSync = fShow;
    QMap<QString, WalletView*>::const_iterator i;
    for (i = mapWalletViews.constBegin(); i != mapWalletViews.constEnd(); ++i)
        i.value()->showOutOfSyncWarning(fShow);
}

void WalletFrame::gotoOverviewPage()
{
    QMap<QString, WalletView*>::const_iterator i;
    for (i = mapWalletViews.constBegin(); i != mapWalletViews.constEnd(); ++i)
        i.value()->gotoOverviewPage();
}

void WalletFrame::gotoHistoryPage()
{
    QMap<QString, WalletView*>::const_iterator i;
    for (i = mapWalletViews.constBegin(); i != mapWalletViews.constEnd(); ++i)
        i.value()->gotoHistoryPage();
}

void WalletFrame::gotoReceiveCoinsPage()
{
    QMap<QString, WalletView*>::const_iterator i;
    for (i = mapWalletViews.constBegin(); i != mapWalletViews.constEnd(); ++i)
        i.value()->gotoReceiveCoinsPage();
}

void WalletFrame::gotoSendCoinsPage(QString addr)
{
    QMap<QString, WalletView*>::const_iterator i;
    for (i = mapWalletViews.constBegin(); i != mapWalletViews.constEnd(); ++i)
        i.value()->gotoSendCoinsPage(addr);
}

void WalletFrame::gotoSignMessageTab(QString addr)
{
    WalletView *walletView = currentWalletView();
    if (walletView)
        walletView->gotoSignMessageTab(addr);
}

void WalletFrame::gotoVerifyMessageTab(QString addr)
{
    WalletView *walletView = currentWalletView();
    if (walletView)
        walletView->gotoVerifyMessageTab(addr);
}

void WalletFrame::encryptWallet(bool status)
{
    WalletView *walletView = currentWalletView();
    if (walletView)
        walletView->encryptWallet(status);
}

void WalletFrame::backupWallet()
{
    WalletView *walletView = currentWalletView();
    if (walletView)
        walletView->backupWallet();
}

void WalletFrame::changePassphrase()
{
    WalletView *walletView = currentWalletView();
    if (walletView)
        walletView->changePassphrase();
}

void WalletFrame::unlockWallet()
{
    WalletView *walletView = currentWalletView();
    if (walletView)
        walletView->unlockWallet();
}

void WalletFrame::usedSendingAddresses()
{
    WalletView *walletView = currentWalletView();
    if (walletView)
        walletView->usedSendingAddresses();
}

void WalletFrame::usedReceivingAddresses()
{
    WalletView *walletView = currentWalletView();
    if (walletView)
        walletView->usedReceivingAddresses();
}

WalletView *WalletFrame::currentWalletView()
{
    return qobject_cast<WalletView*>(walletStack->currentWidget());
}

void WalletFrame::outOfSyncWarningClicked()
{
    Q_EMIT requestedSyncWarningInfo();
}

/** PHL START */
void WalletFrame::gotoAssetsPage()
{
    QMap<QString, WalletView*>::const_iterator i;
    for (i = mapWalletViews.constBegin(); i != mapWalletViews.constEnd(); ++i)
        i.value()->gotoAssetsPage();
}

/** PHL START */
void WalletFrame::gotoRepositoryPage()
{
    QMap<QString, WalletView*>::const_iterator i;
    for (i = mapWalletViews.constBegin(); i != mapWalletViews.constEnd(); ++i)
        i.value()->gotoRepositoryPage();
}

/** PHL START */
void WalletFrame::gotoProvideResourcesPage()
{
    QMap<QString, WalletView*>::const_iterator i;
    for (i = mapWalletViews.constBegin(); i != mapWalletViews.constEnd(); ++i)
        i.value()->gotoProvideResourcesPage();
}

void WalletFrame::gotoDeployVMPage()
{
    QMap<QString, WalletView*>::const_iterator i;
    for (i = mapWalletViews.constBegin(); i != mapWalletViews.constEnd(); ++i)
        i.value()->gotoDeployVMPage();
}

void WalletFrame::displayAssetInfo()
{
    QMap<QString, WalletView*>::const_iterator i;
    for (i = mapWalletViews.constBegin(); i != mapWalletViews.constEnd(); ++i)
        i.value()->displayAssetInfo();
}
