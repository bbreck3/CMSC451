/**
    This is the Contracts tab of the interface.
    The purpose of the Contracts tab is to endable the user to input data from the GUI and either insert this into the contract before it
    deployed or assign the input to variable name t oenable the info querrable on the blockchain.


 */


import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

Tab{

    //Sets up the tab
    id: contractTab
    enabled: !ipc.busy && (ipc.connectionState > 0)
    title: qsTr("Contacts")

    //adds  tableView to view the reciept elements...
    //elements will be added at a later date

    TableView {
               id: recieptView
               anchors.left: parent.left
               anchors.right: parent.right
               height: parent.height /*newAccountButton.height*/ - parent.spacing

               //TableViewColumn: adds elements to the tablView column by column
               TableViewColumn {
                   horizontalAlignment: Text.AlignHCenter
                   role: "test1"
                   title: qsTr("test1")
                   width: 70

                   /*delegate: ToolButton {
                       iconSource: (styleData.value === true) ? "/images/locked" : "/images/unlocked"
                       enabled: (styleData.value === true)
                       onClicked: {
                           if ( styleData.value === true ) {
                               accountView.currentRow = styleData.row
                               accountModel.selectedAccountRow = accountView.currentRow
                               accountUnlockDialog.openFocused("Unlock " + accountModel.selectedAccount)
                           }
                       }
                   }*/
               }
               TableViewColumn {
                   role: "test2"
                   title: qsTr("test2")
                   width: 150
               }
               TableViewColumn {
                   horizontalAlignment: Text.AlignRight
                   role: "test3"
                   title: qsTr("test3")
                   width: 150
               }
               TableViewColumn {
                   horizontalAlignment: Text.AlignRight
                   role: "test4"
                   title: qsTr("test4")
                   width: 150
               }

               //this should be changed to contractsModel but that does not exist yet and the application seems to fine as is so will leave for the time being
               model: accountModel




    }//End TableView
}//End Receipts
