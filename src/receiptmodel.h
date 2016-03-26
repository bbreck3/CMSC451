#ifndef RECEIPTMODEL_H
#define RECEIPTMODEL_H


#include <QAbstractListModel>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "types.h"
#include "etheripc.h"
#include "accountmodel.h"
#include "etherlog.h"

namespace Etherwall {

    class ReceiptModel : public QAbstractListModel
    {
        Q_OBJECT
        Q_PROPERTY(quint64 blockNumber READ getBlockNumber NOTIFY blockNumberChanged FINAL)
        Q_PROPERTY(QString gasPrice READ getGasPrice NOTIFY gasPriceChanged FINAL)
        Q_PROPERTY(QString gasEstimate READ getGasEstimate NOTIFY gasEstimateChanged FINAL)
    public:
        ReceiptModel(EtherIPC& ipc, const AccountModel& accountModel);
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
       // void sendTransaction(const QString& from, const QString& to, const QString& value, const QString& gas = QString());
        void newTransaction(const TransactionInfo& info);
        void newBlock(const QJsonObject& block);
        void refresh();
        void loadHistoryDone(QNetworkReply* reply);
    signals:
        void blockNumberChanged(quint64 num);
        void gasPriceChanged(const QString& price);
        void gasEstimateChanged(const QString& price);
        void historyChanged();
    private:
        EtherIPC& fIpc;
        const AccountModel& fAccountModel;
        TransactionList fTransactionList;


        quint64 fBlockNumber;
        QString fGasPrice;
        QString fGasEstimate;
        QString test;
        TransactionInfo fQueuedTransaction;
        QNetworkAccessManager fNetManager;
        void readfile(QString fName);
        int getInsertIndex(const TransactionInfo& info) const;
        void addTransaction(const TransactionInfo& info);
        void storeTransaction(const TransactionInfo& info);
    };

}



#endif // RECEIPTMODEL_H
