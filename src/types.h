/*
    This file is part of etherwall.
    etherwall is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    etherwall is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with etherwall. If not, see <http://www.gnu.org/licenses/>.
*/
/** @file types.h
 * @author Ales Katona <almindor@gmail.com>
 * @date 2015
 *
 * Types header
 */

#ifndef TYPES_H
#define TYPES_H

#include <QString>
#include <QList>
#include <QVariant>
#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDateTime>
#include "bigint.h"

namespace Etherwall {

#ifdef Q_OS_WIN32
    static const QString DefaultIPCPath = "\\\\.\\pipe\\geth.ipc";
#else
    #ifdef Q_OS_MACX
    static const QString DefaultIPCPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/Library/Ethereum/geth.ipc";
    #else
    static const QString DefaultIPCPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.ethereum/geth.ipc";
    #endif
#endif

    enum LogRoles {
        MsgRole = Qt::UserRole + 1,
        DateRole,
        SeverityRole
    };

    enum LogSeverity {
        LS_Debug,
        LS_Info,
        LS_Warning,
        LS_Error
    };

    class LogInfo
    {
    public:
        LogInfo(const QString& info, LogSeverity sev);
        const QVariant value(int role) const;
    private:
        QString fMsg;
        QDateTime fDate;
        LogSeverity fSeverity;

        const QString getSeverityString() const;
    };

    typedef QList<LogInfo> LogList;

    enum RequestTypes {
        NoRequest,
        NewAccount,
        DeleteAccount,
        GetBlockNumber,
        GetAccountRefs,
        GetBalance,
        GetTransactionCount,
        GetPeerCount,
        SendTransaction,
        UnlockAccount,
        GetGasPrice,
        EstimateGas,
        NewFilter,
        GetFilterChanges,
        UninstallFilter,
        GetTransactionByHash,
        GetBlock,
        GetClientVersion
    };

    enum AccountRoles {
        LockedRole = Qt::UserRole + 1,
        HashRole,
        BalanceRole,
        TransCountRole,
        SummaryRole
    };

    class AccountInfo
    {
    public:
        AccountInfo(const QString& hash, const QString& balance, quint64 transCount);

        const QVariant value(const int role) const;
        void setBalance(const QString& balance);
        void setTransactionCount(quint64 count);
        void lock();
        void unlock(qint64 toTime);
        bool isLocked(bool internal = false) const;
    private:
        QString fHash;
        QString fBalance; // in ether
        quint64 fTransCount;
        bool fLocked;
    };

    typedef QList<AccountInfo> AccountList;

    enum TransactionRoles {
        THashRole = Qt::UserRole + 1,
        NonceRole,
        SenderRole,
        ReceiverRole,
        ValueRole,
        BlockNumberRole,
        BlockHashRole,
        TransactionIndexRole,
        GasRole,
        GasPriceRole,
        //Additinos to the table for the HashMap
        InputRole,
        DepthRole,
        NameRole,
        ItemRole,
        DescRole,
        SerielRole,
        CompanyRole,

    };

    class TransactionInfo
    {
    public:
        //Overloaded methods for the TransactionInfo Class
        TransactionInfo();
        TransactionInfo(const QJsonObject& source);
        TransactionInfo(const QString& hash, quint64 blockNum); // for storing from server reply
        TransactionInfo(const QString& item1,QString& item2, QString& item3, QString& item4, QString item5);
        TransactionInfo(const QString& from, const QString& to, const QString& value, const QString& gas, const QString& name, const QString& item, const QString& desc);
        TransactionInfo(const QString& from, const QString& to, const QString& value, const QString& gas, const QString& name);
        TransactionInfo(const QString& from, const QString& to, const QString& value, const QString& gas);



        const QVariant value(const int role) const;
        quint64 getBlockNumber() const;
        void setBlockNumber(quint64 num);
        const QString getHash() const;
        void setHash(const QString& hash);
       // void init(const QString& from, const QString& to, const QString& value, const QString& gas);//, const QString& name, const QString& item, const QString& desc = QString());
        //Initialize the transaction for table
        void init(const QString& from, const QString& to, const QString& value, const QString& gas, const QString& name, const QString& item, const QString& seriel,const QString& compnay, const QString& desc);
        const QJsonObject toJson(bool decimal = false) const;
        const QString toJsonString(bool decimal = false) const;
    private:

        //String Definiation for teh table
        QString item1;
        QString item2;
        QString item3;
        QString item4;
        QString item5;
        QString fHash;
        quint64 fNonce;
        QString fSender;
        QString fReceiver;
        QString fValue; // in ether
        quint64 fBlockNumber;
        QString fBlockHash;
        quint64 fTransactionIndex;
        QString fGas;
        QString fGasPrice;
        QString fInput;
        QString fName;
        QString fItem;
        QString fDesc;
        QString fSeriel;
        QString fCompany;
    };

    typedef QList<TransactionInfo> TransactionList;

    class Helpers {
    public:
        static const QString toDecStr(const QJsonValue &jv);
        static const QString toDecStrEther(const QJsonValue &jv);
        static const QString toDecStr(quint64 val);
        static const QString toHexStr(quint64 val);
        static const QString toHexWeiStr(const QString& val);
        static const QString toHexWeiStr(quint64 val);
        static const QString decStrToHexStr(const QString& dec);
        static const QString weiStrToEtherStr(const QString& wei);
        static BigInt::Rossi decStrToRossi(const QString& dec);
        static BigInt::Rossi etherStrToRossi(const QString& dec);
        static const QString formatEtherStr(const QString& ether);
        static const QJsonArray toQJsonArray(const AccountList& list);
        static quint64 toQUInt64(const QJsonValue& jv);
    };

}

#endif // TYPES_H
