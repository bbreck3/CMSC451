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
/** @file transactionmodel.cpp
 * @author Ales Katona <almindor@gmail.com>
 * @date 2015
 *
 * Transaction model implementation
 */

#include "transactionmodel.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSettings>

namespace Etherwall {

    TransactionModel::TransactionModel(EtherIPC& ipc, const AccountModel& accountModel) :
        QAbstractListModel(0), fIpc(ipc), fAccountModel(accountModel), fBlockNumber(0), fGasPrice("unknown"), fGasEstimate("unknown"), fNetManager(this)
    {
        connect(&ipc, &EtherIPC::connectToServerDone, this, &TransactionModel::connectToServerDone);
        connect(&ipc, &EtherIPC::getAccountsDone, this, &TransactionModel::getAccountsDone);
        connect(&ipc, &EtherIPC::getBlockNumberDone, this, &TransactionModel::getBlockNumberDone);
        connect(&ipc, &EtherIPC::getGasPriceDone, this, &TransactionModel::getGasPriceDone);
        connect(&ipc, &EtherIPC::estimateGasDone, this, &TransactionModel::estimateGasDone);
        connect(&ipc, &EtherIPC::sendTransactionDone, this, &TransactionModel::sendTransactionDone);
        connect(&ipc, &EtherIPC::newTransaction, this, &TransactionModel::newTransaction);
        connect(&ipc, &EtherIPC::newBlock, this, &TransactionModel::newBlock);

        connect(&fNetManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(loadHistoryDone(QNetworkReply*)));
    }


    /**
     *
     *
     *  TextField input Stuff.....from Distributor
     *
     */


    QString TransactionModel::getName() const {
        return fName;
    }

    QString TransactionModel::getItem() const {
        return fItem;
    }
    QString TransactionModel::getSeriel() const {
        return fSeriel;
    }

    QString TransactionModel::getCompany() const {
        return fCompany;
    }
    QString TransactionModel::getDescription() const {
        return fDescription;
    }


    void TransactionModel::setName(QString name) {
        fName = name;
        qDebug()<< "Debug: Name: -->" << fName;
    }

    void TransactionModel::setItem(QString item) {
        fItem = item;
        qDebug()<< "Debug: Item: -->" << fItem;
    }

    void TransactionModel::setSeriel(QString seriel) {
        fSeriel = seriel;
        qDebug()<< "Debug: seriel: -->" << fSeriel;
    }

    void TransactionModel::setCompany(QString company) {
        fCompany = company;
        qDebug()<< "Debug: company: -->" << fCompany;
    }
    void TransactionModel::setDescription(QString desc) {
        fDescription =desc;
        qDebug()<< "Debug: desc: -->" << fDescription;
    }
    const QString TransactionModel::getNameStr() const {
        return fName;
    }
    const QString TransactionModel::getItemStr() const {
        return fItem;
    }

    const QString TransactionModel::getSerielStr() const {
        return fSeriel;
    }
    const QString TransactionModel::getCompanyStr() const {
        return fCompany;
    }
    const QString TransactionModel::getDescriptionStr() const {
        return fDescription;
    }



    /**
     * End textfield input for Distributor
     */



    /**
     *     Begin Customer info
     */


    QString TransactionModel::getToString() const {
        return fto;
    }
    QString TransactionModel::getFromString() const {
        return ffrom;
    }
    QString TransactionModel::getAmountString() const {
        return famount;
    }
    QString TransactionModel::getGasString() const{
        return fgas;
    }


    void TransactionModel::setToString(QString to) {
        fto=to;
        qDebug()<< "Debug: to: -->" << fto;
    }
    void TransactionModel::setFromString(QString from) {
        ffrom=from;
        qDebug()<< "Debug: from: -->" << ffrom;
    }
    void TransactionModel::setAmountString(QString amount) {
        famount=amount;
        qDebug()<< "Debug: amount: -->" << famount;
    }
    void TransactionModel::setGasString(QString gas) {
        fgas=gas;
        qDebug()<< "Debug: amount: -->" << fgas;
    }


    const QString TransactionModel::getTo() const {
        return fto;
    }
    const QString TransactionModel::getFrom() const {
        return ffrom;
    }
    const QString TransactionModel::getAmount() const {
        return famount;
    }
    const QString TransactionModel::getGasPriceStr() const {
        return fgas;
    }
    




    quint64 TransactionModel::getBlockNumber() const {
        return fBlockNumber;
    }

    const QString& TransactionModel::getGasPrice() const {
        return fGasPrice;
    }

    const QString& TransactionModel::getGasEstimate() const {
        return fGasEstimate;
    }

    //HashMap that maps the id of the Column with the key
    QHash<int, QByteArray> TransactionModel::roleNames() const {
        QHash<int, QByteArray> roles;
        roles[THashRole] = "hash";
        roles[NonceRole] = "nonce";
        roles[SenderRole] = "sender";
        roles[ReceiverRole] = "receiver";
        roles[ValueRole] = "value";
        roles[BlockNumberRole] = "blocknumber";
        roles[BlockHashRole] = "blockhash";
        roles[TransactionIndexRole] = "tindex";
        roles[GasRole] = "gas";
        roles[GasPriceRole] = "gasprice";
        roles[InputRole] = "input";
        roles[DepthRole] = "depth";
        roles[NameRole] = "name";
        roles[ItemRole] = "item";
        roles[DescRole] = "desc";
        roles[SerielRole] = "seriel";
        roles[CompanyRole] ="company";

        return roles;
    }

    int TransactionModel::rowCount(const QModelIndex & parent __attribute__ ((unused))) const {
        return fTransactionList.size();
    }

    QVariant TransactionModel::data(const QModelIndex & index, int role) const {
        const int row = index.row();

        // calculate distance from current block
        if ( role == DepthRole ) {
            quint64 transBlockNum = fTransactionList.at(row).value(BlockNumberRole).toULongLong();
            if ( transBlockNum == 0 ) { // still pending
                return -1;
            }

            quint64 diff = fBlockNumber - transBlockNum;
            return diff;
        }

        return fTransactionList.at(row).value(role);
    }

    int TransactionModel::containsTransaction(const QString& hash) {
        int i = 0;
        foreach ( const TransactionInfo& t, fTransactionList ) {
            if ( t.value(THashRole).toString() == hash ) {
                return i;
            }
            i++;
        }

        return -1;
    }

    void TransactionModel::connectToServerDone() {
        fIpc.getBlockNumber();
        fIpc.getGasPrice();
    }

    void TransactionModel::getAccountsDone(const AccountList& list __attribute__((unused))) {
        //refresh(); --> do not uncommment this, this will reditect the refresh values of the table to a server that does not exists
        //loadHistory();.... still not certain if this has anythingto do with the reason the the transaction are not stored stored....
    }

    void TransactionModel::getBlockNumberDone(quint64 num) {
       if ( num <= fBlockNumber ) {
            return;
        }

        fBlockNumber = num;
        emit blockNumberChanged(num);

        if ( !fTransactionList.isEmpty() ) { // depth changed for all
            const QModelIndex& leftIndex = QAbstractListModel::createIndex(0, 5);
            const QModelIndex& rightIndex = QAbstractListModel::createIndex(fTransactionList.length() - 1, 5);
            emit dataChanged(leftIndex, rightIndex, QVector<int>(1, DepthRole));
        }
    }

    void TransactionModel::getGasPriceDone(const QString& num) {
        fGasPrice = num;
        emit gasPriceChanged(num);
    }

    void TransactionModel::estimateGasDone(const QString& num) {
        fGasEstimate = num;
        emit gasEstimateChanged(num);
    }



   /***
    *   This is the method responcible for sending the taransaction information to the blockchain:
    *       NOTE: I have getters in side this method. This is because You should not send information off to the blockchain before you have all the information
    *       needed. Hence you take in the customer information first: the to , from, amount and gas price as store them temporarily until the distriutor has put their informatio in
    *       Thus when the sendTranstion method is called, you need  call recall the stored inforamtion inorder to deploy all information about the transaction on the blockchain.
    *
    */
    void TransactionModel::sendTransaction(const QString& name, const QString& prod_id, const QString& sNum, const QString& company, const QString& desc){//, const QString& item, const QString& description) {
       //variable to hold the stored information
        QString from = getFrom();
        QString to = getTo();
        QString gas = getGasPriceStr();
        QString value  = getAmount();

        qDebug() <<"Debug-->" << from << to << value <<  gas << name << prod_id << company << desc;// <<  name << item << description;

        //this is what sends the transaction through on the block chain
        fIpc.sendTransaction(from, to, value, gas, name, prod_id, sNum, company,desc);//, name, item, description );

        //this is what puts the info to display on the list
        fQueuedTransaction.init(from, to, value, gas, name, prod_id, sNum, company, desc);
    }

    /**
     * @brief TransactionModel::prepSendTransaction
     * @param from
     * @param to
     * @param value
     * @param gas
     *
     *      prepSendTransaction: onclick from the CustomerTab, sets the current value for teporary storage until needed later on
     *
     *
     */


    void TransactionModel::prepSendTransaction(const QString& from, const QString& to, const QString& value, const QString& gas){//, const QString& item, const QString& description) {


            //setter fro all relevant information
            setToString(to);
            setFromString(from);
            setAmountString(value);
            setGasString(gas);



        qDebug() <<"Debug-->" << from << to << value <<  gas;// <<  name << item << description;


    }



    void TransactionModel::sendTransactionDone(const QString& hash) {
        fQueuedTransaction.setHash(hash);
        addTransaction(fQueuedTransaction);
        EtherLog::logMsg("Transaction sent hash: " + hash);
    }

    void TransactionModel::newTransaction(const TransactionInfo &info) {
        int ai1, ai2;
        const QString& sender = info.value(SenderRole).toString();
        const QString& receiver = info.value(ReceiverRole).toString();
        const QString& name = info.value(NameRole).toString();
        qDebug() << "Debug name-->" << name;
        qDebug() << "Debug sener -->" << sender;
        qDebug() << "Debug receiever -->" << receiver;

        if ( fAccountModel.containsAccount(sender, receiver, ai1, ai2) ) { // either our sent or someone sent to us
            const int n = containsTransaction(info.value(THashRole).toString());
            qDebug() << "Value of n -->" << n ;

            if ( n >= 0 ) { // ours
                fTransactionList[n] = info;
                const QModelIndex& leftIndex = QAbstractListModel::createIndex(n, 0);
                const QModelIndex& rightIndex = QAbstractListModel::createIndex(n, 12);
                emit dataChanged(leftIndex, rightIndex);
                storeTransaction(fTransactionList.at(n));
            } else { // external from someone to us
                /**
                        The below line is where the list information gets added to the table...



                  */
                addTransaction(info);
            }
        }
    }

    void TransactionModel::newBlock(const QJsonObject& block) {
        const QJsonArray transactions = block.value("transactions").toArray();
        const quint64 blockNum = Helpers::toQUInt64(block.value("number"));

        if ( blockNum == 0 ) {
            return; // not interested in pending blocks
        }

        foreach ( QJsonValue t, transactions ) {
            const QJsonObject to = t.toObject();
            const QString thash = to.value("hash").toString();
            const QString sender = to.value("from").toString();
            const QString receiver = to.value("to").toString();
            const QString name = to.value("name").toString();
            int i1, i2;

            const int n = containsTransaction(thash);
            if ( n >= 0 ) {
                fTransactionList[n].setBlockNumber(blockNum);
                const QModelIndex& leftIndex = QAbstractListModel::createIndex(n, 0);
                const QModelIndex& rightIndex = QAbstractListModel::createIndex(n, 12);
                QVector<int> roles(2);
                roles[0] = BlockNumberRole;
                roles[1] = DepthRole;
                emit dataChanged(leftIndex, rightIndex, roles);
                storeTransaction(fTransactionList.at(n));
            } else if ( fAccountModel.containsAccount(sender, receiver, i1, i2) ) {
                addTransaction(TransactionInfo(to));
            }
        }
    }

    int TransactionModel::getInsertIndex(const TransactionInfo& info) const {
        const quint64 block = info.value(BlockNumberRole).toULongLong();

        if ( block == 0 ) {
            return 0; // new/pending
        }

        for ( int i = 0; i < fTransactionList.length(); i++ ) {
            const quint64 oldBlock = fTransactionList.at(i).value(BlockNumberRole).toULongLong();
            if ( oldBlock <= block ) {
                return i;
            }
        }

        return fTransactionList.length();
    }

    void TransactionModel::addTransaction(const TransactionInfo& info) {

        const int index = getInsertIndex(info);
        beginInsertRows(QModelIndex(), index, index);
        /**
         *
         *  this adds stuff to the list--> the transactionlist...
         *
         * */
        fTransactionList.insert(index, info);
        endInsertRows();

        storeTransaction(info);
    }


    //this is the method that could be potentially be where the storing the transaction is breaking,,,,
    void TransactionModel::storeTransaction(const TransactionInfo& info) {
        // save to persistent memory for re-run
        const quint64 blockNum = info.value(BlockNumberRole).toULongLong();
        QSettings settings;
        settings.beginGroup("transactions");
        settings.setValue(Helpers::toDecStr(blockNum) + "_" + info.value(TransactionIndexRole).toString(), info.toJsonString());
        settings.endGroup();
    }

    void TransactionModel::refresh() {
        QSettings settings;
        settings.beginGroup("transactions");
        QStringList list = settings.allKeys();

        foreach ( const QString bns, list ) {
            const QString val = settings.value(bns, "bogus").toString();
            if ( val.contains("{") ) { // new format, get data and reload only recent transactions
                QJsonParseError parseError;
                const QJsonDocument jsonDoc = QJsonDocument::fromJson(val.toUtf8(), &parseError);

                if ( parseError.error != QJsonParseError::NoError ) {
                    EtherLog::logMsg("Error parsing stored transaction: " + parseError.errorString(), LS_Error);
                } else {
                    const TransactionInfo info(jsonDoc.object());
                    newTransaction(info);
                    // if transaction is newer than 1 day restore it from geth anyhow to ensure correctness in case of reorg
                    if ( fBlockNumber - info.getBlockNumber() < 5400 ) {
                        fIpc.getTransactionByHash(info.getHash());
                    }
                }
            } else if ( val != "bogus" ) { // old format, re-get and store full data
                fIpc.getTransactionByHash(val);
                settings.remove(bns);
            }
        }
        settings.endGroup();
    }

    const QString TransactionModel::estimateTotal(const QString& value, const QString& gas) const {
        BigInt::Rossi valRossi = Helpers::etherStrToRossi(value);
        BigInt::Rossi valGas = Helpers::decStrToRossi(gas);
        BigInt::Rossi valGasPrice = Helpers::etherStrToRossi(fGasPrice);

        if ( valRossi == BigInt::Rossi(0) ) {
            return "0";
        }

        const QString wei = QString((valRossi + valGas * valGasPrice).toStrDec().data());

        return Helpers::weiStrToEtherStr(wei);
    }

    const QString TransactionModel::getSender(int index) const {
        if ( index >= 0 && index < fTransactionList.length() ) {
            return fTransactionList.at(index).value(SenderRole).toString();
        }

        return QString();
    }

    const QString TransactionModel::getReceiver(int index) const {
        if ( index >= 0 && index < fTransactionList.length() ) {
            return fTransactionList.at(index).value(ReceiverRole).toString();
        }

        return QString();
    }

    const QJsonObject TransactionModel::getJson(int index, bool decimal) const {
       if ( index < 0 || index >= fTransactionList.length() ) {
            return QJsonObject();
        }

        return fTransactionList.at(index).toJson(decimal);
    }

    const QString TransactionModel::getMaxValue(int row, const QString& gas) const {
        const QModelIndex index = QAbstractListModel::createIndex(row, 2);

        BigInt::Rossi balanceWeiRossi = Helpers::etherStrToRossi( fAccountModel.data(index, BalanceRole).toString() );
        const BigInt::Rossi gasRossi = Helpers::decStrToRossi(gas);
        const BigInt::Rossi gasPriceRossi = Helpers::etherStrToRossi(fGasPrice);
        const BigInt::Rossi gasTotalRossi = gasRossi * gasPriceRossi;

        if ( balanceWeiRossi < gasTotalRossi ) {
            return "0";
        }
        const BigInt::Rossi resultWeiRossi = (balanceWeiRossi - gasTotalRossi);

        const QString resultWei = QString(resultWeiRossi.toStrDec().data());

        return Helpers::weiStrToEtherStr(resultWei);
    }

    //DO NOT UNDER ANYCIRCUMSTANCES UNCOMMNET THIS--> IT WILL BREAK EVERYTHING
    void TransactionModel::loadHistory() {
        // get historical transactions from etherdata
        //QNetworkRequest request(QUrl("http://data.etherwall.com/api/transactions"));
        /*request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QJsonObject objectJson;
        objectJson["accounts"] = fAccountModel.getAccountsJsonArray();
        const QByteArray data = QJsonDocument(objectJson).toJson();

        EtherLog::logMsg("HTTP Post request: " + data, LS_Debug);

        fNetManager.post(request, data);*/
    }

    void TransactionModel::loadHistoryDone(QNetworkReply *reply) {
        if ( reply == NULL ) {
            return EtherLog::logMsg("Undefined history reply", LS_Error);
        }

        const QByteArray data = reply->readAll();
        EtherLog::logMsg("HTTP Post reply: " + data, LS_Debug);

        QJsonParseError parseError;
        const QJsonDocument resDoc = QJsonDocument::fromJson(data, &parseError);

        if ( parseError.error != QJsonParseError::NoError ) {
            return EtherLog::logMsg("Response parse error: " + parseError.errorString(), LS_Error);
        }

        const QJsonObject resObj = resDoc.object();
        const bool success = resObj.value("success").toBool();

        if ( !success ) {
            const QString error = resObj.value("error").toString("unknown error");
            return EtherLog::logMsg("Response error: " + error, LS_Error);
        }
        const QJsonArray result = resObj.value("result").toArray();

        int stored = 0;
        foreach ( const QJsonValue jv, result ) {
            const QJsonObject jo = jv.toObject();
            const QString hash = jo.value("hash").toString("bogus");

            if ( hash == "bogus" ) {
                return EtherLog::logMsg("Response hash missing", LS_Error);
            }

            if ( containsTransaction(hash) < 0 ) {
                fIpc.getTransactionByHash(hash);
                stored++;
            }
        }

        if ( stored > 0 ) {
            EtherLog::logMsg("Restored " + QString::number(stored) + " transactions from etherdata server", LS_Info);
        }

        reply->close();
    }

}
