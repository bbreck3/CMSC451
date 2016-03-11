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
/** @file TransactionsTab.qml
 * @author Ales Katona <almindor@gmail.com>
 * @date 2015
 *
 * Transactions tab
 */

import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0

Tab {
    id:receiptTab
    enabled: !ipc.busy && (ipc.connectionState > 0)
    title: qsTr("Distributer")


    Column {
        id: input1
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

                     TextField {
                      id: name


                       }
                       TextField {
                           id: prod_id

                       }
                       TextField {
                           id: sNum

                       }
                       TextField {
                           id: company_name
                       }
                       TextField {
                           id: desc
                       }


        GridLayout {
            id: gridLayout
            columns: 4
            width: parent.width

           /* Label {
                id: fromLabel
                text: qsTr("From: ")
            }*/

            Row {
                Layout.columnSpan: 3
                Layout.minimumWidth: 600


            }



            Row {
                ToolButton {
                    id: transactionWarning
                    iconSource: "/images/warning"
                    tooltip: qsTr("Sending checks", "before sending a transaction")
                    width: sendButton.height
                    height: sendButton.height

                    function check() {

                        //results array of info:

                        //This is where you need to define all the value
                        var result = {
                            error: null,
                            from: null,
                            to: null,
                            //the amount
                            value: 0, //--> was this :-1
                            name: null,
                            item: null,
                            description: null,
                        }

                        if ( fromField.currentIndex < 0 ) {
                            result.error = qsTr("Sender account not selected")
                            return result
                        }
                        var index = fromField.currentIndex
                        result.from = accountModel.getAccountHash(index) || ""

                        if ( !result.from.match(/0x[a-f,0-9]{40}/) ) {
                            result.error = qsTr("Sender account invalid")
                            return result
                        }

                        if ( accountModel.isLocked(index) ) {
                            result.error = qsTr("From account is locked")
                            return result
                        }

                        result.to = toField.text || ""
                        if ( !result.to.match(/0x[a-f,0-9]{40}/) ) {
                            result.error = qsTr("Recipient account invalid")
                            return result
                        }

                        result.txtVal = valueField.text.trim() || ""
                        result.value = result.txtVal.length > 0 ? Number(result.txtVal) : NaN
                        if ( isNaN(result.value) || result.value <= 0.0 ) {
                            result.error = qsTr("Invalid value")
                            return result
                        }

                        result.txtGas = gasField.text


                        return result;
                    }

                    function refresh() {
                        var result = check()
                        if ( result.error !== null ) {
                            tooltip = result.error
                        }

                        enabled = (result.error !== null)
                    }

                    onClicked: {
                        refresh()

                        if ( enabled ) {
                            errorDialog.msg = tooltip
                            errorDialog.open()
                        }
                    }
                }

                ConfirmDialog {
                    id: confirmDialog
                    msg: qsTr("Confirm Deploy Contract info?")

                    onYes: {
                        //var result = transactionWarning.check()
                       /* if ( result.error !== null ) {
                            errorDialog.msg = result.error
                            errorDialog.open()
                            return
                        }*/
                        /*contractModel.selectedAccountRow = items5.text.toString(); //this needs be changed to make sence
                        contractModel.selectedAccountRow1 = items6.text.toString();
                        contractModel.selectedAccountRow2 = items7.text.toString();
                        contractModel.selectedAccountRow3 = items8.text.toString();*/

                        /*transactionModel.sendTransaction(result.from, result.to, result.txtVal, result.txtGas) */

                          //This will be important later on...

                        //transactionModel.sendTransaction(result.from, result.to, result.txtVal, result.txtGas);// items5.text.toString(), items6.text.toString(),items6.text.toString());
                        transactionModel.sendTransaction(name.text.toString(), prod_id.text.toString(),sNum.text.toString(), company_name.text.toString(),desc.text.toString());


                    }
                }


            }

            Row {
                Layout.columnSpan: 1
                Button {
                    id: sendButton
                    text: "Deploy"

                    onClicked: {
                        /*var result = transactionWarning.check()
                        if ( result.error !== null ) {
                            errorDialog.msg = result.error
                            errorDialog.open()
                            return
                        }*/

                        confirmDialog.open()
                    }
                }

            }



        }

        TransactionDetails {
            id: details
        }

        TableView {
            id: transactionView
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height - gridLayout.height - parent.spacing

            TableViewColumn {
                horizontalAlignment: Text.AlignRight
                role: "blocknumber"
                title: qsTr("Block#")
                width: 70
            }
            TableViewColumn {
                role: "sender"
                title: qsTr("Sender")
                width: 200
            }
            TableViewColumn {
                role: "receiver"
                title: qsTr("Receiver")
                width: 200
            }
            TableViewColumn {
                horizontalAlignment: Text.AlignRight
                role: "value"
                title: qsTr("Value (Ether)")
                width: 150
            }
            TableViewColumn {
                horizontalAlignment: Text.AlignRight
                role: "depth"
                title: qsTr("Depth")
                width: 70
            }
            TableViewColumn {
                horizontalAlignment: Text.AlignRight
                role: "name"
                title: qsTr("Name")
                width: 70
            }
            TableViewColumn {
                horizontalAlignment: Text.AlignRight
                role: "item"
                title: qsTr("Item")
                width: 200
            }
            TableViewColumn {
                horizontalAlignment: Text.AlignRight
                role: "seriel"
                title: qsTr("Seriel#")
                width: 200
            }
            TableViewColumn {
                horizontalAlignment: Text.AlignRight
                role: "company"
                title: qsTr("Company")
                width: 200
            }
            TableViewColumn {
                horizontalAlignment: Text.AlignRight
                role: "desc"
                title: qsTr("Description")
                width: 200
            }
            model: transactionModel

            Menu {
                id: rowMenu

                MenuItem {
                    text: qsTr("Details")
                    onTriggered: {
                        details.open(transactionModel.getJson(transactionView.currentRow, true))
                    }
                }

                MenuItem {
                    text: qsTr("Copy Sender")
                    onTriggered: {
                        clipboard.setText(transactionModel.getSender(transactionView.currentRow))
                    }
                }

                MenuItem {
                    text: qsTr("Copy Receiver")
                    onTriggered: {
                        clipboard.setText(transactionModel.getReceiver(transactionView.currentRow))
                    }
                }
            }

            rowDelegate: Item {
                SystemPalette {
                    id: osPalette
                    colorGroup: SystemPalette.Active
                }

                Rectangle {
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }
                    height: parent.height
                    color: styleData.selected ? osPalette.highlight : (styleData.alternate ? osPalette.alternateBase : osPalette.base)
                    MouseArea {
                        anchors.fill: parent
                        propagateComposedEvents: true
                        acceptedButtons: Qt.RightButton

                        onReleased: {
                            if ( transactionView.currentRow >= 0 ) {
                                rowMenu.popup()
                            }
                        }
                    }
                }
            }
        }

    }
}
