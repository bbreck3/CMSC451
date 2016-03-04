#ifndef CONTRACTMODEL
#define CONTRACTMODEL
#include <QAbstractListModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "types.h"
#include "etheripc.h"
#include "etherlog.h"
#include <QPushButton>
#include <QLineEdit>



#include <QAbstractListModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "types.h"
#include "etheripc.h"
#include "etherlog.h"


#include <QMainWindow>


namespace Etherwall{
//class ContractModel;


    class ContractModel : public QAbstractListModel
    {
        Q_OBJECT
        Q_PROPERTY(QString selectedAccountRow READ getSelectedAccountRow WRITE setSelectedAccountRow NOTIFY accountSelectionChanged)
        Q_PROPERTY(QString selectedAccountRow1 READ getSelectedAccountRow1 WRITE setSelectedAccountRow1 NOTIFY accountSelectionChanged)
        Q_PROPERTY(QString selectedAccountRow2 READ getSelectedAccountRow2 WRITE setSelectedAccountRow2 NOTIFY accountSelectionChanged)
        Q_PROPERTY(QString selectedAccountRow3 READ getSelectedAccountRow3 WRITE setSelectedAccountRow3 NOTIFY accountSelectionChanged)
       // Q_PROPERTY(QString selectedAccount READ getSelectedAccount NOTIFY accountSelectionChanged)
        Q_PROPERTY(QString total READ getTotal NOTIFY totalChanged)
    public:
        //ContractModel(EtherIPC& ipc);
        //ContractModel(EtherIPC& ipc);

         ContractModel(EtherIPC& ipc);
        QString test;
        QLineEdit item5, input6;


        QString getError() const;
        QHash<int, QByteArray> roleNames() const;
       int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
       bool containsAccount(const QString& from, const QString& to, int& i1, int& i2) const;
      const QJsonArray getAccountsJsonArray() const;
       const QString getTotal() const;



      Q_INVOKABLE void newAccount(const QString& pw);
      Q_INVOKABLE void deleteAccount(const QString& pw, int index);
      Q_INVOKABLE void unlockAccount(const QString& pw, int duration, int index);
      Q_INVOKABLE bool isLocked(int index) const;
      Q_INVOKABLE const QString getAccountHash(int index) const;
     // ~ContractModel();

    public slots:
        void connectToServerDone();
        void getAccountsDone(const AccountList& list);
        void newAccountDone(const QString& hash, int index);
        void deleteAccountDone(bool result, int index);
        void unlockAccountDone(bool result, int index);
        void accountChanged(const AccountInfo& info);
        void newBlock(const QJsonObject& block);
        void checkAccountLocks();

       void on_btnSubmit_clicked();
       void on_LEItem1_editingFinished();
       void on_LEItem2_editingFinished();
       void on_LEItem3_editingFinished();
       void on_LEItem4_editingFinished();



    signals:
      void accountSelectionChanged(int);
      void accountLockedChanged(int);
       void totalChanged();

       void onButtonClick();
      void getText1();
      void getText2();



     private:
       EtherIPC& fIpc;
       AccountList fAccountList;
       QString fSelectedAccountRow;
       QString fSelectedAccountRow1;
       QString fSelectedAccountRow2;
       QString fSelectedAccountRow3;
       QString fSelectedAccount;

       QString getSelectedAccountRow() const;
       QString getSelectedAccountRow1() const;
       QString getSelectedAccountRow2() const;
       QString getSelectedAccountRow3() const;
       void setSelectedAccountRow(QString row);
       void setSelectedAccountRow1(QString row);
       void setSelectedAccountRow2(QString row);
       void setSelectedAccountRow3(QString row);
       const QString getSelectedAccount() const;
       const QString getSelectedAccount1() const;
       const QString getSelectedAccount2() const;
       const QString getSelectedAccount3() const;

       Etherwall::ContractModel *contractModel;
    };
}

#endif // CONTRACTMODEL

