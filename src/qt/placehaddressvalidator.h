// Copyright (c) 2011-2014 The Bitcoin Core developers
// Copyright (c) 2017 The Placeholder Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PLACEH_QT_PLACEHADDRESSVALIDATOR_H
#define PLACEH_QT_PLACEHADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class PlacehAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit PlacehAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Placeh address widget validator, checks for a valid placeh address.
 */
class PlacehAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit PlacehAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // PLACEH_QT_PLACEHADDRESSVALIDATOR_H
