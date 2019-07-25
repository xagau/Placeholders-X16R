// Copyright (c) 2011-2016 The Bitcoin Core developers
// Copyright (c) 2017 The Placeholder Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PLACEH_QT_DEPLOYVM_DIALOG_H
#define PLACEH_QT_DEPLOYVM_DIALOG_H

#include "walletmodel.h"
#include "base58.h"

#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include <QTextEdit>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QSpinBox>
#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>


class ClientModel;
class PlatformStyle;
class SendAssetsEntry;
class SendCoinsRecipient;

namespace Ui {
    class DeployVMDialog;
}

QT_BEGIN_NAMESPACE
class QUrl;
QT_END_NAMESPACE

/** Dialog for sending placehs */
class DeployVMDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeployVMDialog(const PlatformStyle *platformStyle, QWidget *parent = 0);
    ~DeployVMDialog();

    void setClientModel(ClientModel *clientModel);
    void setModel(WalletModel *model);

    
	

public Q_SLOTS:
    void clearAll();
    void selectFile();
    void deployToNetwork();
    void viewOnExplorer();
	void signMessage();
	void generateNewAddress();
	
private:
    Ui::DeployVMDialog *ui;
    ClientModel *clientModel;
    WalletModel *model;
    const PlatformStyle *platformStyle;
	
	void console(const char* msg);

    void createHorizontalGroupBox();


    enum { NumGridRows = 3, NumButtons = 4 };


    QGroupBox *horizontalGroupBox;
    QGroupBox *gridGroupBox;
    QGroupBox *formGroupBox;
    QTextEdit *smallEditor;
    QTextEdit *bigEditor;
	QString fileName;
	QString contentMimeType;
	
	QPushButton* deployToNetworkButton;
	QPushButton* clearConsoleButton;
	QPushButton* signButton;
	QPushButton* viewOnExplorerButton;
			
	QLineEdit* artifact = new QLineEdit();
	QLineEdit* checksum = new QLineEdit();
	QLineEdit* signature = new QLineEdit();
	QLineEdit* description = new QLineEdit();
	QLineEdit* tags = new QLineEdit();
	QLineEdit* bounty = new QLineEdit();
	QLineEdit* fileInformation = new QLineEdit();
			
	//QCanvas   *canvas;
	
    QLabel *labels[NumGridRows];
    //QLineEdit *lineEdits[NumGridRows];
    QPushButton *buttons[NumButtons];
    QDialogButtonBox *buttonBox;
   

private Q_SLOTS:
   
    /** PHL END */

    Q_SIGNALS:
            // Fired when a message should be reported to the user
            void message(const QString &title, const QString &message, unsigned int style);
};

#endif // PLACEH_QT_DEPLOYVM_DIALOG_H
