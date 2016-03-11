#include "contractmodel.h"
#include "types.h"
#include <QDebug>
#include <QSettings>
#include <QDateTime>



#include <QAbstractListModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "types.h"
#include "etheripc.h"
#include "etherlog.h"
#include <QMessageBox>


namespace Etherwall {

QString test;
    ContractModel::ContractModel( EtherIPC& ipc) :
        QAbstractListModel(0), fIpc(ipc), fAccountList(), fSelectedAccountRow(-1)
    {
        connect(&ipc, &EtherIPC::connectToServerDone, this, &ContractModel::connectToServerDone);
        connect(&ipc, &EtherIPC::getAccountsDone, this, &ContractModel::getAccountsDone);
        connect(&ipc, &EtherIPC::newAccountDone, this, &ContractModel::newAccountDone);
        connect(&ipc, &EtherIPC::deleteAccountDone, this, &ContractModel::deleteAccountDone);
        connect(&ipc, &EtherIPC::unlockAccountDone, this, &ContractModel::unlockAccountDone);
        connect(&ipc, &EtherIPC::accountChanged, this, &ContractModel::accountChanged);
        connect(&ipc, &EtherIPC::newBlock, this, &ContractModel::newBlock);
    }

    void ContractModel::on_btnSubmit_clicked(){
        test="Debug";

        //contractModel->submit->text()
        qDebug() << "aslkdfal";

    }

    void ContractModel::on_LEItem1_editingFinished(){
           qDebug() << "aslkdfal";
     }
    void ContractModel::on_LEItem2_editingFinished(){
         qDebug() << "aslkdfal";

    }

    void ContractModel::on_LEItem3_editingFinished(){
     qDebug() << "aslkdfal";
    }

    void ContractModel::on_LEItem4_editingFinished(){
         qDebug() << "aslkdfal";
    }
    /*void ContractModel::ContractModel::newAccount(){  // --> button Submit
        qDebug() << "Debug";
    }*/



























    /**
     *
     *      Random stuff from account models that was converted to ContractModel to get it to compile
     *
     *          I does however make sence for the conract objects tp act in a similar to accounts.....
     *
     *
     *
     */






    QHash<int, QByteArray> ContractModel::roleNames() const {
            QHash<int, QByteArray> roles;
            roles[HashRole] = "hash";
            roles[BalanceRole] = "balance";
            roles[TransCountRole] = "transactions";
            roles[LockedRole] = "locked";
            roles[SummaryRole] = "summary";
<<<<<<< HEAD
            roles[NameRole] = "names";
=======
>>>>>>> 099452e8305506f3f2695a486526fba629aa7a32
            return roles;
        }

        int ContractModel::rowCount(const QModelIndex & parent __attribute__ ((unused))) const {
            return fAccountList.size();
        }

        QVariant ContractModel::data(const QModelIndex & index, int role) const {
            const int row = index.row();

            return fAccountList.at(row).value(role);
        }

        // TODO: optimize with hashmap
        bool ContractModel::containsAccount(const QString& from, const QString& to, int& i1, int& i2) const {
            i1 = -1;
            i2 = -1;
            int i = 0;
            foreach ( const AccountInfo& a, fAccountList ) {
                const QString addr = a.value(HashRole).toString();
                if ( addr == from ) {
                    i1 = i;
                }

                if ( addr == to ) {
                    i2 = i;
                }
                i++;
            }

            return (i1 >= 0 || i2 >= 0);
        }

        const QString ContractModel::getTotal() const {
            BigInt::Rossi total;

            foreach ( const AccountInfo& info, fAccountList ) {
                const QString strVal = info.value(BalanceRole).toString();
                total += Helpers::etherStrToRossi(strVal);
            }

            const QString weiStr = QString(total.toStrDec().data());
            return Helpers::weiStrToEtherStr(weiStr);
        }



        void ContractModel::deleteAccount(const QString& pw, int index) {
            if ( index >= 0 && index < fAccountList.size() ) {
                const QString hash = fAccountList.at(index).value(HashRole).toString();
                fIpc.deleteAccount(hash, pw, index);
            } else {
                EtherLog::logMsg("Invalid account selection for delete", LS_Error);
            }
        }

        void ContractModel::unlockAccount(const QString& pw, int duration, int index) {
            if ( index >= 0 && index < fAccountList.size() && duration > 0 ) {
                const QString hash = fAccountList.at(index).value(HashRole).toString();
                fIpc.unlockAccount(hash, pw, duration, index);
            } else {
                EtherLog::logMsg("Invalid account selection for unlock");
            }
        }

        bool ContractModel::isLocked(int index) const {
            if ( index < 0 || index >= fAccountList.length() ) {
                return true;
            }

            return fAccountList.at(index).isLocked();
        }

        const QString ContractModel::getAccountHash(int index) const {
            if ( index >= 0 && fAccountList.length() > index ) {
                return fAccountList.at(index).value(HashRole).toString();
            }

            return QString();
        }

        void ContractModel::connectToServerDone() {
            fIpc.getAccounts();
        }

        void ContractModel::newAccountDone(const QString& hash, int index) {
            if ( !hash.isEmpty() ) {
                beginInsertRows(QModelIndex(), index, index);
                fAccountList.append(AccountInfo(hash, "0.00000000000000000", 0));
                endInsertRows();
                EtherLog::logMsg("New account created");
            } else {
                EtherLog::logMsg("Account create failure");
            }
        }
        void ContractModel::newAccount(const QString& pw) {
                const int index = fAccountList.size();
                fIpc.newAccount(pw, index);
            }

        void ContractModel::deleteAccountDone(bool result, int index) {
            if ( result ) {
                beginRemoveRows(QModelIndex(), index, index);
                fAccountList.removeAt(index);
                endRemoveRows();
                EtherLog::logMsg("Account deleted");
            } else {
                EtherLog::logMsg("Account delete failure");
            }
        }

        void ContractModel::unlockAccountDone(bool result, int index) {
            if ( result ) {
                QSettings settings;
                qint64 diff = settings.value("/ipc/accounts/lockduration", 300).toInt() * 1000;

                QTimer::singleShot(diff + 200, this, SLOT(checkAccountLocks()));
                fAccountList[index].unlock(QDateTime::currentMSecsSinceEpoch() + diff);
                const QModelIndex& modelIndex = QAbstractListModel::createIndex(index, 0);
                emit dataChanged(modelIndex, modelIndex, QVector<int>(1, LockedRole));
                emit accountLockedChanged(index);

                EtherLog::logMsg("Account unlocked");
            } else {
                EtherLog::logMsg("Account unlock failure");
            }
        }

        void ContractModel::checkAccountLocks() {
            int index = 0;
            foreach ( AccountInfo i, fAccountList ) {
                if ( i.value(LockedRole).toBool() != i.isLocked(true) ) {
                    i.lock();
                    const QModelIndex& modelIndex = QAbstractListModel::createIndex(index, 0);
                    emit dataChanged(modelIndex, modelIndex, QVector<int>(1, LockedRole));
                    emit accountLockedChanged(index);
                }
                index++;
            }
        }

        void ContractModel::getAccountsDone(const AccountList& list) {
            beginResetModel();
            fAccountList = list;
            endResetModel();

            int i = 0;
            foreach ( const AccountInfo& info, list ) {
                const QString& hash = info.value(HashRole).toString();
                fIpc.refreshAccount(hash, i++);
            }

            emit totalChanged();
        }

        void ContractModel::accountChanged(const AccountInfo& info) {
            int index = 0;
            const QString infoHash = info.value(HashRole).toString();
            foreach ( const AccountInfo& a, fAccountList ) {
                if ( a.value(HashRole).toString() == infoHash ) {
                    fAccountList[index] = info;
                    const QModelIndex& leftIndex = QAbstractListModel::createIndex(index, 0);
                    const QModelIndex& rightIndex = QAbstractListModel::createIndex(index, 4);
                    emit dataChanged(leftIndex, rightIndex);
                    emit totalChanged();
                    return;
                }
                index++;
            }

            const int len = fAccountList.length();
            beginInsertRows(QModelIndex(), len, len);
            fAccountList.append(info);
            endInsertRows();

            emit totalChanged();
        }

        void ContractModel::newBlock(const QJsonObject& block) {
            const QJsonArray transactions = block.value("transactions").toArray();
            const QString miner = block.value("miner").toString("bogus");
            int i1, i2;
            if ( containsAccount(miner, "bogus", i1, i2) ) {
                fIpc.refreshAccount(miner, i1);
            }

            foreach ( QJsonValue t, transactions ) {
                const QJsonObject to = t.toObject();
                const TransactionInfo info(to);
                const QString& sender = info.value(SenderRole).toString();
                const QString& receiver = info.value(ReceiverRole).toString();

                if ( containsAccount(sender, receiver, i1, i2) ) {
                    if ( i1 >= 0 ) {
                        fIpc.refreshAccount(sender, i1);
                    }

                    if ( i2 >= 0 ) {
                        fIpc.refreshAccount(receiver, i2);
                    }
                }
            }

            emit totalChanged();
        }


        /**
         * @brief ContractModel::getSelectedAccountRow
         * @return
         *
         *  TextField input Stuff.....
         *
         */


        QString ContractModel::getSelectedAccountRow() const {
            return fSelectedAccountRow;
        }

        QString ContractModel::getSelectedAccountRow1() const {
            return fSelectedAccountRow1;
        }
        QString ContractModel::getSelectedAccountRow2() const {
            return fSelectedAccountRow2;
        }

        QString ContractModel::getSelectedAccountRow3() const {
            return fSelectedAccountRow3;
        }


        void ContractModel::setSelectedAccountRow(QString item) {
            fSelectedAccountRow = item;
            //int input = atoi(row.c_str());
            //int test2 = item;
            //QString test = QString::number(test2);
            qDebug()<< "Debug: item1: -->" << fSelectedAccountRow;
            //emit accountSelectionChanged(item);
        }

        void ContractModel::setSelectedAccountRow1(QString item) {
            fSelectedAccountRow1 = item;
            //int input = atoi(row.c_str());
            qDebug()<< "Debug: item2: -->" << fSelectedAccountRow1;
            //emit accountSelectionChanged(item);
        }

        void ContractModel::setSelectedAccountRow2(QString item) {
            fSelectedAccountRow2 = item;
            //int input = atoi(row.3_str());
            qDebug()<< "Debug: item1: -->" << fSelectedAccountRow2;
           // emit accountSelectionChanged(item);
        }

        void ContractModel::setSelectedAccountRow3(QString item) {
            fSelectedAccountRow3 = item;
            //int input = atoi(row.c_str());

            qDebug()<< "Debug: item3: -->" << fSelectedAccountRow3;
           // emit accountSelectionChanged(item);
        }
        const QString ContractModel::getSelectedAccount() const {
            return fSelectedAccountRow;
        }
        const QString ContractModel::getSelectedAccount1() const {
            return fSelectedAccountRow1;
        }

        const QString ContractModel::getSelectedAccount2() const {
            return fSelectedAccountRow2;
        }
        const QString ContractModel::getSelectedAccount3() const {
            return fSelectedAccountRow3;
        }
        const QJsonArray ContractModel::getAccountsJsonArray() const {
            QJsonArray result;

            foreach ( const AccountInfo ai, fAccountList ) {
                const QString hash = ai.value(HashRole).toString();
                result.append(hash);
            }

            return result;
        }

  }
