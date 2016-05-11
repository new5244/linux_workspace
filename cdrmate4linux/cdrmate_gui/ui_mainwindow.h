/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Mon Nov 30 19:51:23 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Connect;
    QAction *action_Disconnect;
    QAction *actionE_xit;
    QAction *action_Options;
    QAction *action_About_CDRMate;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_2;
    QTreeView *treeView_receive;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_3;
    QTreeView *treeView_transfer;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_view;
    QPushButton *pushButton_export_send;
    QPushButton *pushButton_delete;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menu_Setup;
    QMenu *menu_Help;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 400);
        action_Connect = new QAction(MainWindow);
        action_Connect->setObjectName(QString::fromUtf8("action_Connect"));
        action_Disconnect = new QAction(MainWindow);
        action_Disconnect->setObjectName(QString::fromUtf8("action_Disconnect"));
        actionE_xit = new QAction(MainWindow);
        actionE_xit->setObjectName(QString::fromUtf8("actionE_xit"));
        action_Options = new QAction(MainWindow);
        action_Options->setObjectName(QString::fromUtf8("action_Options"));
        action_About_CDRMate = new QAction(MainWindow);
        action_About_CDRMate->setObjectName(QString::fromUtf8("action_About_CDRMate"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout_2 = new QHBoxLayout(tab);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setMargin(11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        treeView_receive = new QTreeView(tab);
        treeView_receive->setObjectName(QString::fromUtf8("treeView_receive"));
        treeView_receive->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeView_receive->setSortingEnabled(true);

        horizontalLayout_2->addWidget(treeView_receive);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        horizontalLayout_3 = new QHBoxLayout(tab_2);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setMargin(11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        treeView_transfer = new QTreeView(tab_2);
        treeView_transfer->setObjectName(QString::fromUtf8("treeView_transfer"));
        treeView_transfer->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeView_transfer->setSortingEnabled(true);
        treeView_transfer->setAnimated(false);

        horizontalLayout_3->addWidget(treeView_transfer);

        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setMargin(11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_view = new QPushButton(groupBox);
        pushButton_view->setObjectName(QString::fromUtf8("pushButton_view"));
        pushButton_view->setEnabled(false);

        horizontalLayout->addWidget(pushButton_view);

        pushButton_export_send = new QPushButton(groupBox);
        pushButton_export_send->setObjectName(QString::fromUtf8("pushButton_export_send"));

        horizontalLayout->addWidget(pushButton_export_send);

        pushButton_delete = new QPushButton(groupBox);
        pushButton_delete->setObjectName(QString::fromUtf8("pushButton_delete"));

        horizontalLayout->addWidget(pushButton_delete);


        verticalLayout->addWidget(groupBox);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 25));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menu_Setup = new QMenu(menuBar);
        menu_Setup->setObjectName(QString::fromUtf8("menu_Setup"));
        menu_Help = new QMenu(menuBar);
        menu_Help->setObjectName(QString::fromUtf8("menu_Help"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menu_Setup->menuAction());
        menuBar->addAction(menu_Help->menuAction());
        menu_File->addAction(action_Connect);
        menu_File->addAction(action_Disconnect);
        menu_File->addSeparator();
        menu_File->addAction(actionE_xit);
        menu_Setup->addAction(action_Options);
        menu_Help->addAction(action_About_CDRMate);

        retranslateUi(MainWindow);
        QObject::connect(actionE_xit, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(pushButton_view, SIGNAL(clicked()), MainWindow, SLOT(slot_item_view()));
        QObject::connect(pushButton_export_send, SIGNAL(clicked()), MainWindow, SLOT(slot_export_send()));
        QObject::connect(pushButton_delete, SIGNAL(clicked()), MainWindow, SLOT(slot_item_delete()));
        QObject::connect(tabWidget, SIGNAL(currentChanged(int)), MainWindow, SLOT(slot_change_export_send_button(int)));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        action_Connect->setText(QApplication::translate("MainWindow", "&Connect", 0, QApplication::UnicodeUTF8));
        action_Disconnect->setText(QApplication::translate("MainWindow", "&Disconnect", 0, QApplication::UnicodeUTF8));
        actionE_xit->setText(QApplication::translate("MainWindow", "E&xit", 0, QApplication::UnicodeUTF8));
        action_Options->setText(QApplication::translate("MainWindow", "&Options", 0, QApplication::UnicodeUTF8));
        action_About_CDRMate->setText(QApplication::translate("MainWindow", "&About CDRMate...", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Received List", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Transferred List", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        pushButton_view->setText(QApplication::translate("MainWindow", "&View", 0, QApplication::UnicodeUTF8));
        pushButton_export_send->setText(QApplication::translate("MainWindow", "&Export", 0, QApplication::UnicodeUTF8));
        pushButton_delete->setText(QApplication::translate("MainWindow", "&Delete", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menu_Setup->setTitle(QApplication::translate("MainWindow", "&Setup", 0, QApplication::UnicodeUTF8));
        menu_Help->setTitle(QApplication::translate("MainWindow", "&Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
