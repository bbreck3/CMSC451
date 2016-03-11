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
/** @file transactionmodel.h
 * @author Ales Katona <almindor@gmail.com>
 * @date 2015
 *
 * Transaction model header
 */


#ifndef TRANSACTIONMODEL_H
#define TRANSACTIONMODEL_H


#include <QAbstractListModel>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "types.h"
#include "etheripc.h"
#include "accountmodel.h"
#include "etherlog.h"

namespace Etherwall {

    class TransactionModel : public QAbstractListModel
    {
        Q_OBJECT
        Q_PROPERTY(quint64 blockNumber READ getBlockNumber NOTIFY blockNumberChanged FINAL)
        Q_PROPERTY(QString gasPrice READ getGasPrice NOTIFY gasPriceChanged FINAL)
        Q_PROPERTY(QString gasEstimate READ getGasEstimate NOTIFY gasEstimateChanged FINAL)

        //Defines that actions for the Distributor textfields --> signal
        Q_PROPERTY(QString fName READ getNameStr WRITE setName NOTIFY accountSelectionChanged)
        Q_PROPERTY(QString fItem READ getItemStr WRITE setItem NOTIFY accountSelectionChanged)
        Q_PROPERTY(QString fSeriel READ getSerielStr WRITE setSeriel NOTIFY accountSelectionChanged)
        Q_PROPERTY(QString fCompany READ getCompanyStr WRITE setCompany NOTIFY accountSelectionChanged)
        Q_PROPERTY(QString fDescription READ getDescriptionStr WRITE setDescription NOTIFY accountSelectionChanged)

        //Defines that actions for the sending of: to , from and amount --> signal
        Q_PROPERTY(QString to READ getToString WRITE setToString NOTIFY accountSelectionChanged)
        Q_PROPERTY(QString from READ getFromString WRITE setFromString NOTIFY accountSelectionChanged)
        Q_PROPERTY(QString amount READ getAmountString WRITE setAmountString NOTIFY accountSelectionChanged)
        Q_PROPERTY(QString gas READ getGasString WRITE setGasString NOTIFY accountSelectionChanged)




    public:
        TransactionModel(EtherIPC& ipc, const AccountModel& accountModel);
        quint64 getBlockNumber() const;
        const QString& getGasPrice() const;
        const QString& getGasEstimate() const;
        QHash<int, QByteArray> roleNames() const;
        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        int containsTransaction(const QString& hash);
        Q_INVOKABLE const QString estimateTotal(const QString& value, const QString& gas) const;
        Q_INVOKABLE void loadHistory();
        Q_INVOKABLE const QString getSender(int index) const;
        Q_INVOKABLE const QString getReceiver(int index) const;
        Q_INVOKABLE const QJsonObject getJson(int index, bool decimal) const;
        Q_INVOKABLE const QString getMaxValue(int row, const QString& gas) const;
        double getHistoryProgress() const;
    public slots:
        void connectToServerDone();
        void getAccountsDone(const AccountList& list);
        void getBlockNumberDone(quint64 num);
        void getGasPriceDone(const QString& num);
        void estimateGasDone(const QString& num);
        void sendTransactionDone(const QString& hash);
        //temporarily holds infor until distrubtor adds his information
        void prepSendTransaction(const QString& from, const QString& to, const QString& value, const QString& gas);// const QString& name, const QString& item, const QString& desc = QString());
        // deploys all information to blockchain
        //void sendTransaction(const QString& from, const QString& to, const QString& value, const QString& gas, const QString& name, const QString& prod_id, const QString& sNum, const QString& item, const QString desc=QString());
         void sendTransaction(const QString& name, const QString& prod_id, const QString& sNum, const QString& item, const QString& desc);
        void newTransaction(const TransactionInfo& info);
        void newBlock(const QJsonObject& block);
        void refresh();
        void loadHistoryDone(QNetworkReply* reply);
    signals:
        void accountSelectionChanged(int);
        void blockNumberChanged(quint64 num);
        void gasPriceChanged(const QString& price);
        void gasEstimateChanged(const QString& price);
        void historyChanged();
    private:
        EtherIPC& fIpc;
        const AccountModel& fAccountModel;

        /**

                TextFields for distributor


         */



        QString fName;
        QString fItem;
        QString fSeriel;
        QString fCompany;
        QString fDescription;
        //QString fSelectedAccount;



        QString getName() const;
        QString getItem() const;
        QString getSeriel() const;
        QString getCompany() const;
        QString getDescription() const;
        void setName(QString str1);
        void setItem(QString str2);
        void setSeriel(QString str3);
        void setCompany(QString str4);
         void setDescription(QString str5);
        const QString getNameStr() const;
        const QString getItemStr() const;
        const QString getSerielStr() const;
        const QString getCompanyStr() const;
        const QString getDescriptionStr() const;



        /***
         *              Temp storage for Customerstuf: to from amount....etc
         *
         *
         */

            QString fto;
            QString ffrom;
            QString famount;
            QString fgas;

            QString getToString() const;
            QString getFromString() const;
            QString getAmountString() const;
            QString getGasString() const;
          //  QString getSelectedAccountRow3() const;
            void setToString(QString row);
            void setFromString(QString row);
            void setAmountString(QString row);
            void setGasString(QString gas);
          //  void setSelectedAccountRow3(QString row);
            const QString getTo() const;
            const QString getFrom() const;
            const QString getAmount() const;
            const QString getGasPriceStr() const;
           // const QString getFrom() const;



        TransactionList fTransactionList;
        quint64 fBlockNumber;
        QString fGasPrice;
        QString fGasEstimate;
        TransactionInfo fQueuedTransaction;
        QNetworkAccessManager fNetManager;

        int getInsertIndex(const TransactionInfo& info) const;
        void addTransaction(const TransactionInfo& info);
        void storeTransaction(const TransactionInfo& info);
    };

}


#endif // TRANSACTIONMODEL_H
