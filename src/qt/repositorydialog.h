// Copyright (c) 2011-2016 The Bitcoin Core developers
// Copyright (c) 2017 The Placeholder Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PLACEH_QT_REPOSITORYDIALOG_H
#define PLACEH_QT_REPOSITORYDIALOG_H

#include "walletmodel.h"

#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QTimer>

class ClientModel;
class PlatformStyle;
class SendAssetsEntry;
class SendCoinsRecipient;

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
  
public Q_SLOTS:

private:
    Ui::RepositoryDialog *ui;
    ClientModel *clientModel;
    WalletModel *model;

private Q_SLOTS:

    Q_SIGNALS:
            // Fired when a message should be reported to the user
            void message(const QString &title, const QString &message, unsigned int style);
};

#endif // PLACEH_QT_REPOSITORYDIALOG_H
