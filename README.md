
#Instructions

To build / rum--> make.sh 
You may need to install some prequisites
Follow the errors, it will tell you...just google them and install

to clean and rebuilt --> clean.sh

#Overview

3 tabs:
1) Distributor
2) Customer
3) Accounts

2) THe Customer selects the where he wants his funds to go, which account he is going to use to pay and enters an amount. Once the transaction information is recorded, is then sent to a tempory Queue that is waiting for the Distributor to plugin needed information abotu the product purchase.

1) The Distributor inputs the needed information and then clicks deploy. Once deployed the distributor information is then combined with the ciustomer information and deployed on the blockchain to which all onformation is accessible publically for display.

3) Accounts is just there to see an overview of all accounts

Working prototype.....bugs will occur, errors will arrise...


 
# Credits Below
# etherwall

Ethereum QT5 Wallet

Etherwall is a free software wallet/front-end for Ethereum Geth*.

## Usage

Geth 1.0.2+ is required to be running for Etherwall to work.

Etherwall should auto-detect geth's IPC file/name and work "out of the box" as long as geth is running.

If Etherwall fails to detect the IPC file/name you can specify it in the settings panel.

Do not run Ethwerwall while geth is syncing, it will just lock down processing all the blocks until syncing is done.

## License

Etherwall is licensed under the GPLv3 license. See LICENSE for more info.

## Donations

#### Flattr
[![Flattr this git repo](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=Almindor&url=https://github.com/almindor/etherwall&title=Etherwall&language=&tags=github&category=software)

#### Bitcoin
`1NcJoao879C1pSKvFqnUKD6wKtFpCMppP6`

#### Litecoin
`LcTfGmqpXCiG7UikBDTa4ZiJMS5cRxSXHm`

#### Ether
`0xc64b50db57c0362e27a32b65bd29363f29fdfa59`

## Development

### Requirements

Geth 1.0.2+ (eth and others might be supported)

Qt5.2+ with qmake

### Building

qmake -config release && make

### Roadmap

- 1.1+ add eth support
- 1.0 add geth account backup and restore
- 0.9 add transaction history support [done]
- 0.8 initial release [done]

### Caveats & bugs

Only supported client at the moment is Geth. Eth and others should work if you go to settings and set the IPC path/name properly.
# CMSC451
