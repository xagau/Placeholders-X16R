﻿// Copyright (c) 2011-2016 The Bitcoin Core developers
// Copyright (c) 2017 The Placeholder Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PLACEH_QT_PROVIDE_RESOURCES_DIALOG_H
#define PLACEH_QT_PROVIDE_RESOURCES_H

#include "walletmodel.h"
#include "placeholderutility.h"

#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include <QPushButton>

#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QUuid>
#include <QComboBox>
#include "slidersgroup.h"
#include <QStackedWidget>
#include <QtWidgets>

class ClientModel;
class PlatformStyle;
class SendAssetsEntry;
class SendCoinsRecipient;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QSpinBox;
class QStackedWidget;

class SlidersGroup;

namespace Ui {
    class ProvideResourcesDialog;
}

QT_BEGIN_NAMESPACE
class QUrl;
QT_END_NAMESPACE

/** Dialog for sending placehs */
class ProvideResourcesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProvideResourcesDialog(const PlatformStyle *platformStyle, QWidget *parent = 0);
    ~ProvideResourcesDialog();

    void setClientModel(ClientModel *clientModel);
    void setModel(WalletModel *model);

	void createControls(const QString &title);

	

    SlidersGroup *horizontalSliders;
    SlidersGroup *verticalSliders;
    QStackedWidget *stackedWidget;

    QGroupBox *controlsGroup;
    QLabel *userLabel;
    QLabel *passwordLabel;
	
    QLabel *coresLabel;
    QLabel *coresCostLabel;
    QLabel *memoryLabel;
    QLabel *memoryCostLabel;
    QLabel *diskspaceLabel;
    QLabel *diskspaceCostLabel;
    QLabel *bandwidthLabel;
    QLabel *bandwidthCostLabel;
    QLabel *costLabel;
    QLabel *doWorkLabel;
    
	QCheckBox *doWork;

	QCheckBox *invertedAppearance;
    QCheckBox *invertedKeyBindings;
    QLineEdit *userId;
    QLineEdit *password;
    QSpinBox *coresSpinBox;
    QDoubleSpinBox *coresCostSpinBox;
    QSpinBox *memorySpinBox;
    QDoubleSpinBox *memoryCostSpinBox;
    QSpinBox *diskspaceSpinBox;
	QDoubleSpinBox *diskspaceCostSpinBox;

    QSpinBox *bandwidthSpinBox;
	QDoubleSpinBox *bandwidthCostSpinBox;

    QDoubleSpinBox *costSpinBox;
    QComboBox *orientationCombo;
    QPushButton *provideResourcesButton;   

public Q_SLOTS:
	void provide();
    

private:
    Ui::ProvideResourcesDialog *ui;
    ClientModel *clientModel;
    WalletModel *model;
    bool fNewRecipientAllowed;
    bool fFeeMinimized;
    const PlatformStyle *platformStyle;

    
private Q_SLOTS:
    

    Q_SIGNALS:
            // Fired when a message should be reported to the user
            void message(const QString &title, const QString &message, unsigned int style);
};

#endif // PLACEH_QT_PROVIDE_RESOURCES_DIALOG_H
