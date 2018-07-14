# ExeQt

## What is it?
A nifty little utility for pinning commands, applications or links to the system tray. ExeQt goes even further and provides network facilities so that your custom actions can be transfered, accesed and used from computers around the world or computers around your house.

![Main Image](./Images/main.png)

## How does it work?
ExeQt uses the system's native tray to display groups of actions with the things the user loves most such as their documents, the folder with pictures of cats or that site that lets you see how you would look in 50 years. Actions are cool little shortcut-like items that make your life easier. Anything you can run or execute on your machine can be put in one convenient menu that you can acces anywhere on your operating system.

## Features
- Ability to create shortcuts for shell commands, executable files, web links or any other file on the system
- Nice grouping of actions so the system tray is kept neat and tidy
- Support for action sets which make running multiple things together really easy by grouping the existing actions
- Actions that save locally in an XML format
- Cross platform (Windows, macOS and Linux) thanks to the cross platform Qt Toolkit
- Actions can be synced across computers using a free cloud service (andu.internals.ro) provided for the app
- Communication with other computers in LAN running ExeQt allows you to call actions on another computer in your house or office remotely from the comfort of your computer

## Some sugestive pictures
Easy to work with, featuring an easy to use user interface for manging your actions

![Actions](./Images/actions.png)

One intuitive application menu!

![Menu](./Images/menu.png)

The web platform from where you can log in to your account and keep actions synced across computers

![Cloud Login](./Images/web-login.png)

Accounts even for dog lovers!

![Cloud Account](./Images/web-account.png)

Accessing other computers on the LAN

![LAN1](./Images/lan1.png)

Running actions on another computer on the LAN

![LAN2](./Images/lan2.png)

## Compiling from Source

You must first have the GNU compiler (gcc) and the Qt Toolkit installed on your machine

- **Windows:** The project must be first built using the QtCreator IDE and then if you want to create a standalone executable you must use the tool [windeployqt](https://doc.qt.io/qt-5/windows-deployment.html) which creates a directory with your app and all of the required DLLs so that you can run ExeQt on a system without having Qt installed

- **macOS:** The project must be first built using the qMake script (or the QtCreator IDE) and then if you want to create a standalone executable you must use the tool [macdeployqt](https://doc.qt.io/qt-5/osx-deployment.html#macdeploy) which creates an App Bundle so that you can run ExeQt on a system without having Qt installed

- **Linux:** The project must be first built using the qMake script (or the QtCreator IDE) and then if you want to create a standalone executable you must use the tool [linuxdeployqt](https://github.com/probonopd/linuxdeployqt) which creates an AppImage so that you can run ExeQt on a system without having Qt installed

## Libraries used
- Qt Toolkit
- [qtsingleapplication](https://github.com/qtproject/qt-solutions/tree/master/qtsingleapplication) from the [qt-solutions](https://github.com/qtproject/qt-solutions) project

## Disclaimer
I do not own any of the icons used in this project. I thank the wonderful people at [findicons.com](findicons.com) and [iconarchive.com](iconarchive.com) for making these available to the public for free.
My thanks to the Canonical team for providing beautiful desktop wallpapers for their Ubuntu operating system.
Also on the list of things not entirely made by me is the nice ExeQt WordArt that I used as the picture at the top of this page. That image was made with a free tool from [WordArt.com](https://wordart.com/)
