/**
    This is the Contracts tab of the interface.
    The purpose of the Contracts tab is to endable the user to input data from the GUI and either insert this into the contract before it
    deployed or assign the input to variable name t oenable the info querrable on the blockchain.


 */


import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
//import QtQuick.Dialogs 1.2

//import QtQuick.Controls.Styles 1.4
Tab{

    //Sets up the tab
    id: contractTab
    enabled: !ipc.busy && (ipc.connectionState > 0)
    title: qsTr("Contacts")

   /**Button and TextField area: Continas the buttons and textfields for the items tha
   that the distrubutor wants to sell

    */

    Column {
           id: col1
           anchors.margins: 20
           anchors.fill: parent

    //Columms that holds the Labels for the items that the distributer wants to sell
    ColumnLayout {
               Label {
                   id: item1
                   text: qsTr("Item 1")

               }
               Label {
                   id: items2
                   text: qsTr("Item 2")

               }
               Label {
                   id: items3
                   text: qsTr("Item 3")

               }
               Label {
                   id: items4
                   text: qsTr("Item 4")

               }

           }//End ColumnLayout


    //Column that holds the TextFields where the employer can input information to send to to contract
    ColumnLayout {

               TextField {
                   id: items5


               }
               TextField {
                   id: items6

               }
               TextField {
                   id: items7


               }
               TextField {
                   id: items8
               }
             Button{
                 id: newAccountButton
                 //id: accountNewDialog //works: changed id
                 text: qsTr("Debug")
                 onClicked: {
                    //accountNewDialog.openFocused("New account password")

                    contractModel.selectedAccountRow = items5.text.toString(); //this needs be changed to make sence
                    contractModel.selectedAccountRow1 = items6.text.toString();
                    contractModel.selectedAccountRow2 = items7.text.toString();
                    contractModel.selectedAccountRow3 = items8.text.toString();





                 }
               }




         /*PasswordDialog {
             id: accountNewDialog //works: changed id

             acceptEmpty: false

             onAccepted: {
                 accountModel.newAccount(password)
             }
         }*/

        /*MessageDialog {
             id: accountNewDialog //works: changed id

             title: "May I have your attention please"
             text: "It's so cool that you are using Qt Quick."
             onAccepted: {
                 console.log("And of course you could only agree.")
                // Qt.quit()
             }
             Component.onCompleted: visible = true
         }*/


           }//End ColumnLayout





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



    }//End Column
    }

    //End TableView
}//End Receipts
