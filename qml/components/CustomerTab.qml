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
    You should have received a copy of the GNU General Publicdepth License
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
    id: transactionsTab
    enabled: !ipc.busy && (ipc.connectionState > 0)
    title: qsTr("Transaction")


    Column {
        id: input1
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        GridLayout {
            id: gridLayout
            columns: 4
            width: parent.width

                /**

                  From Field starts here

                 */

            //From label

            Label {
                id: fromLabel
                text: qsTr("From: ")
            }

            Row {
                Layout.columnSpan: 3
                Layout.minimumWidth: 600

                PasswordDialog {
                    id: accountUnlockDialog
                    //standardButtons: StandardButton.Ok | StandardButton.Cancel

                    onAccepted: {
                        accountModel.unlockAccount(password, settings.value("ipc/accounts/lockduration", 300), fromField.currentIndex)
                    }
                }

                   //Button to unlock the selected account
                ToolButton {
                    id: lockTool
                    iconSource: accountModel.isLocked(fromField.currentIndex) ? "/images/locked" : "/images/unlocked"
                    width: 24
                    height: 24

                    Connections {
                        target: accountModel
                        onAccountLockedChanged: {
                            lockTool.iconSource = accountModel.isLocked(fromField.currentIndex) ? "/images/locked" : "/images/unlocked"
                            transactionWarning.refresh()
                        }
                    }

                    onClicked: {
                        accountModel.selectedAccountRow = fromField.currentIndex
                        accountUnlockDialog.openFocused("Unlock " + accountModel.selectedAccount)
                    }
                }
                //Drop down to select the account
                ComboBox {
                    id: fromField
                    width: parent.width - lockTool.width
                    model: accountModel
                    textRole: "summary"
                    onCurrentIndexChanged: transactionWarning.refresh()
                }
            }

                /**

                  TO label starts Here

                  */
            //To label


            Label {
                id: toLabel
                text: qsTr("To: ")
            }

            //Text field for the receipt account
           TextField {
                id: toField
                validator: RegExpValidator {
                    regExp: /0x[a-f,0-9]{40}/
                }

                maximumLength: 42
                Layout.minimumWidth: 600
                Layout.columnSpan: 3

                onTextChanged: transactionWarning.refresh()
            }




            }

            Row {
                Layout.columnSpan: 1

                Label {
                    text: qsTr("Value: ")
                }

                TextField {
                    id: valueField
                    validator: DoubleValidator {
                        bottom: 0.000000000000000001 // should be 1 wei
                        decimals: 18
                        locale: "en_US"
                    }

                    maximumLength: 50
                    width: 200
                    onTextChanged: transactionWarning.refresh()
                }

                //Send button--> on click invokes the ConfirmDialog to send the information to transaction model for processing
                Button {
                    id: sendButton
                    text: "Send"

                    onClicked: {
                        var result = transactionWarning.check()
                        if ( result.error !== null ) {
                            errorDialog.msg = result.error
                            errorDialog.open()
                            return
                        }

                        confirmDialog.open()
                    }
                }

            // -- estimate is broken in geth 1.0.1- must wait for later release
           Row {
                Layout.columnSpan: 2
                Layout.minimumWidth: 450

                Label {
                    text: qsTr("Gas: ")
                }

                TextField {
                    id: gasField
                    width: 80
                    text: settings.value("gas", "90000")
                    validator: IntValidator {
                        bottom: 0
                        locale: "en_US"
                    }

                    onTextChanged: {
                        settings.setValue("gas", text)
                    }
                }

                Label {
                    text: qsTr("Total: ")
                }

                TextField {
                    id: valueTotalField
                    readOnly: true
                    maximumLength: 50
                    width: 200
                    validator: DoubleValidator {
                        bottom: 0.000000000000000001 // should be 1 wei
                        decimals: 18
                        locale: "en_US"
                    }

                    text: transactionModel.estimateTotal(valueField.text, gasField.text)
                }

            }

        }
            //Checks for various "invalid " things befroe it will allow the transaction to go through
            Row {
                ToolButton {
                    id: transactionWarning
                    //iconSource: "/images/warning"
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
                        /*if ( !result.to.match(/0x[a-f,0-9]{40}/) ) {
                            result.error = qsTr("Recipient account invalid")
                            return result
                        }*/

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

                   //ConfirmDialog --> prompts for confirmation that you want to put the transaction through
                ConfirmDialog {
                    id: confirmDialog
                    msg: qsTr("Confirm transaction send?")

                    //If yes--> double check that the infomration is valid before you  putthe transaction through
                    onYes: {
                        var result = transactionWarning.check()
                        if ( result.error !== null ) {
                            errorDialog.msg = result.error
                            errorDialog.open()
                            return
                        }

                           /**

                                    If all systems are a go, call the transactionModel class to pass the information from the GUI for processing

                                    Note: this is called "prepSendTransaction: this basically stores the information regarding the to from and amount
                                    in order to wait for the distributor to proviode his information before actually invoking the transaction

                                        prepSendTransaction takes as parametets: the to address, the from adddres, the amount to be sent, and the gas limit
                             */
                        transactionModel.prepSendTransaction(result.from, result.to, result.txtVal, gasField.text.toString());

                    }
                }

            }

        TransactionDetails {
            id: details
        }

        /**

                    This begins the Table where the informration stored.
                      Each table column has role and a title
                      The Title simply provide a name to be displayed in order make the list meaning full and readable
                      The role is the id for that column that can be reference in other code files inorder to add and update column values



          */


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
                title: qsTr("To")
                width: 70
            }
            TableViewColumn {
                horizontalAlignment: Text.AlignRight
                role: "item"
                title: qsTr("Date")
                width: 70
            }
            TableViewColumn {
                horizontalAlignment: Text.AlignRight
                role: "seriel"
                title: qsTr("Product Order Number")
                width: 200
            }
            TableViewColumn {
                horizontalAlignment: Text.AlignRight
                role: "company"
                title: qsTr("Product Name")
                width: 200
            }
            TableViewColumn {
                horizontalAlignment: Text.AlignRight
                role: "desc"
                title: qsTr("Notes")
                width: 200
            }
            model: transactionModel


            /**
                Each row can be right clicked to view the deatils of each compoent in a nice pop dialog


              */



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
