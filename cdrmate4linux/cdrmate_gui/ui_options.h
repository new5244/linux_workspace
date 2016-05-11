/********************************************************************************
** Form generated from reading ui file 'options.ui'
**
** Created: Mon Nov 30 19:51:23 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_OPTIONS_H
#define UI_OPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Options
{
public:
    QGridLayout *gridLayout_4;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_apply;
    QDialogButtonBox *buttonBox;
    QTabWidget *tabWidget;
    QWidget *tab_connection;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QRadioButton *radioButton;
    QSpacerItem *horizontalSpacer;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QComboBox *comboBox_system_id;
    QComboBox *comboBox_protocol_ver;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_3;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QLabel *label_3;
    QLabel *label_4;
    QComboBox *comboBox_speed;
    QComboBox *comboBox_port;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_5;
    QCheckBox *checkBox_auto_connect;
    QSpacerItem *verticalSpacer;
    QWidget *tab_receive_transfer;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_6;
    QLabel *label_5;
    QRadioButton *radio_export_on;
    QRadioButton *radio_export_off;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_6;
    QLineEdit *lineEdit_export;
    QPushButton *pushButton_export;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_7;
    QLabel *label_8;
    QLineEdit *lineEdit_import;
    QPushButton *pushButton_import;
    QLabel *label_7;
    QRadioButton *radio_transfer_on;
    QRadioButton *radio_transfer_off;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *verticalSpacer_4;

    void setupUi(QDialog *Options)
    {
        if (Options->objectName().isEmpty())
            Options->setObjectName(QString::fromUtf8("Options"));
        Options->setEnabled(true);
        Options->resize(402, 499);
        gridLayout_4 = new QGridLayout(Options);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalSpacer_4 = new QSpacerItem(106, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_4, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_apply = new QPushButton(Options);
        pushButton_apply->setObjectName(QString::fromUtf8("pushButton_apply"));
        pushButton_apply->setEnabled(false);
        pushButton_apply->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout->addWidget(pushButton_apply);

        buttonBox = new QDialogButtonBox(Options);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setLayoutDirection(Qt::LeftToRight);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout->addWidget(buttonBox);


        gridLayout_4->addLayout(horizontalLayout, 1, 1, 1, 1);

        tabWidget = new QTabWidget(Options);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab_connection = new QWidget();
        tab_connection->setObjectName(QString::fromUtf8("tab_connection"));
        verticalLayout_2 = new QVBoxLayout(tab_connection);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(tab_connection);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setStyleSheet(QString::fromUtf8(""));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        radioButton = new QRadioButton(groupBox);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setEnabled(false);
        radioButton->setMouseTracking(true);
        radioButton->setCheckable(true);
        radioButton->setChecked(true);
        radioButton->setAutoRepeat(false);
        radioButton->setAutoExclusive(true);

        gridLayout_2->addWidget(radioButton, 0, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(16, 61, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        comboBox_system_id = new QComboBox(groupBox);
        comboBox_system_id->setObjectName(QString::fromUtf8("comboBox_system_id"));

        gridLayout->addWidget(comboBox_system_id, 1, 1, 1, 1);

        comboBox_protocol_ver = new QComboBox(groupBox);
        comboBox_protocol_ver->setObjectName(QString::fromUtf8("comboBox_protocol_ver"));

        gridLayout->addWidget(comboBox_protocol_ver, 0, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 1, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 1, 2, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_3, 2, 1, 1, 1);


        verticalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(tab_connection);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setAutoFillBackground(false);
        groupBox_2->setStyleSheet(QString::fromUtf8(""));
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_3->addWidget(label_3, 0, 0, 1, 1);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_3->addWidget(label_4, 1, 0, 1, 1);

        comboBox_speed = new QComboBox(groupBox_2);
        comboBox_speed->setObjectName(QString::fromUtf8("comboBox_speed"));

        gridLayout_3->addWidget(comboBox_speed, 1, 1, 1, 1);

        comboBox_port = new QComboBox(groupBox_2);
        comboBox_port->setObjectName(QString::fromUtf8("comboBox_port"));
        comboBox_port->setEditable(true);

        gridLayout_3->addWidget(comboBox_port, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 0, 2, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_2, 2, 1, 1, 1);


        verticalLayout_2->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(tab_connection);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setStyleSheet(QString::fromUtf8(""));
        gridLayout_5 = new QGridLayout(groupBox_3);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        checkBox_auto_connect = new QCheckBox(groupBox_3);
        checkBox_auto_connect->setObjectName(QString::fromUtf8("checkBox_auto_connect"));

        gridLayout_5->addWidget(checkBox_auto_connect, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer, 1, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox_3);

        tabWidget->addTab(tab_connection, QString());
        tab_receive_transfer = new QWidget();
        tab_receive_transfer->setObjectName(QString::fromUtf8("tab_receive_transfer"));
        verticalLayout = new QVBoxLayout(tab_receive_transfer);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_4 = new QGroupBox(tab_receive_transfer);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_6 = new QGridLayout(groupBox_4);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        label_5 = new QLabel(groupBox_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_6->addWidget(label_5, 0, 0, 1, 2);

        radio_export_on = new QRadioButton(groupBox_4);
        radio_export_on->setObjectName(QString::fromUtf8("radio_export_on"));

        gridLayout_6->addWidget(radio_export_on, 0, 2, 1, 1);

        radio_export_off = new QRadioButton(groupBox_4);
        radio_export_off->setObjectName(QString::fromUtf8("radio_export_off"));
        radio_export_off->setChecked(true);

        gridLayout_6->addWidget(radio_export_off, 0, 3, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(92, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_5, 0, 4, 1, 2);

        label_6 = new QLabel(groupBox_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_6->addWidget(label_6, 1, 0, 1, 1);

        lineEdit_export = new QLineEdit(groupBox_4);
        lineEdit_export->setObjectName(QString::fromUtf8("lineEdit_export"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEdit_export->sizePolicy().hasHeightForWidth());
        lineEdit_export->setSizePolicy(sizePolicy);
        lineEdit_export->setReadOnly(true);

        gridLayout_6->addWidget(lineEdit_export, 1, 1, 1, 4);

        pushButton_export = new QPushButton(groupBox_4);
        pushButton_export->setObjectName(QString::fromUtf8("pushButton_export"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_export->sizePolicy().hasHeightForWidth());
        pushButton_export->setSizePolicy(sizePolicy1);
        pushButton_export->setMaximumSize(QSize(25, 16777215));
        pushButton_export->setBaseSize(QSize(0, 0));

        gridLayout_6->addWidget(pushButton_export, 1, 5, 1, 1);


        verticalLayout->addWidget(groupBox_4);

        groupBox_5 = new QGroupBox(tab_receive_transfer);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        gridLayout_7 = new QGridLayout(groupBox_5);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        label_8 = new QLabel(groupBox_5);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_7->addWidget(label_8, 0, 0, 1, 1);

        lineEdit_import = new QLineEdit(groupBox_5);
        lineEdit_import->setObjectName(QString::fromUtf8("lineEdit_import"));
        lineEdit_import->setReadOnly(true);

        gridLayout_7->addWidget(lineEdit_import, 0, 1, 1, 4);

        pushButton_import = new QPushButton(groupBox_5);
        pushButton_import->setObjectName(QString::fromUtf8("pushButton_import"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton_import->sizePolicy().hasHeightForWidth());
        pushButton_import->setSizePolicy(sizePolicy2);
        pushButton_import->setMinimumSize(QSize(25, 0));
        pushButton_import->setMaximumSize(QSize(25, 16777215));

        gridLayout_7->addWidget(pushButton_import, 0, 5, 1, 1);

        label_7 = new QLabel(groupBox_5);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_7->addWidget(label_7, 1, 0, 1, 2);

        radio_transfer_on = new QRadioButton(groupBox_5);
        radio_transfer_on->setObjectName(QString::fromUtf8("radio_transfer_on"));

        gridLayout_7->addWidget(radio_transfer_on, 1, 2, 1, 1);

        radio_transfer_off = new QRadioButton(groupBox_5);
        radio_transfer_off->setObjectName(QString::fromUtf8("radio_transfer_off"));
        radio_transfer_off->setChecked(true);

        gridLayout_7->addWidget(radio_transfer_off, 1, 3, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(92, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_6, 1, 4, 1, 2);


        verticalLayout->addWidget(groupBox_5);

        verticalSpacer_4 = new QSpacerItem(20, 193, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

        tabWidget->addTab(tab_receive_transfer, QString());

        gridLayout_4->addWidget(tabWidget, 0, 0, 1, 2);


        retranslateUi(Options);
        QObject::connect(buttonBox, SIGNAL(accepted()), Options, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Options, SLOT(reject()));
        QObject::connect(comboBox_protocol_ver, SIGNAL(textChanged(QString)), Options, SLOT(data_modified()));
        QObject::connect(comboBox_system_id, SIGNAL(textChanged(QString)), Options, SLOT(data_modified()));
        QObject::connect(comboBox_port, SIGNAL(textChanged(QString)), Options, SLOT(data_modified()));
        QObject::connect(radio_export_on, SIGNAL(toggled(bool)), Options, SLOT(data_modified()));
        QObject::connect(radio_export_off, SIGNAL(toggled(bool)), Options, SLOT(data_modified()));
        QObject::connect(lineEdit_export, SIGNAL(textChanged(QString)), Options, SLOT(data_modified()));
        QObject::connect(lineEdit_import, SIGNAL(textChanged(QString)), Options, SLOT(data_modified()));
        QObject::connect(radio_transfer_on, SIGNAL(toggled(bool)), Options, SLOT(data_modified()));
        QObject::connect(radio_transfer_off, SIGNAL(toggled(bool)), Options, SLOT(data_modified()));
        QObject::connect(pushButton_apply, SIGNAL(clicked()), Options, SLOT(conf_apply()));
        QObject::connect(comboBox_speed, SIGNAL(currentIndexChanged(int)), Options, SLOT(data_modified()));
        QObject::connect(pushButton_import, SIGNAL(clicked()), Options, SLOT(choose_dir()));
        QObject::connect(pushButton_export, SIGNAL(clicked()), Options, SLOT(choose_dir()));
        QObject::connect(checkBox_auto_connect, SIGNAL(toggled(bool)), Options, SLOT(data_modified()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Options);
    } // setupUi

    void retranslateUi(QDialog *Options)
    {
        Options->setWindowTitle(QApplication::translate("Options", "Dialog", 0, QApplication::UnicodeUTF8));
        pushButton_apply->setText(QApplication::translate("Options", "&Apply", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Options", "Connection Target", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("Options", "Digital Refractor (CDR)", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Options", "Protocol Version", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Options", "System ID", 0, QApplication::UnicodeUTF8));
        comboBox_system_id->clear();
        comboBox_system_id->insertItems(0, QStringList()
         << QApplication::translate("Options", "0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "7", 0, QApplication::UnicodeUTF8)
        );
        comboBox_protocol_ver->clear();
        comboBox_protocol_ver->insertItems(0, QStringList()
         << QApplication::translate("Options", "1.0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "1.1", 0, QApplication::UnicodeUTF8)
        );
        groupBox_2->setTitle(QApplication::translate("Options", "Port Connection", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Options", "Port", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Options", "Baud Rate", 0, QApplication::UnicodeUTF8));
        comboBox_speed->clear();
        comboBox_speed->insertItems(0, QStringList()
         << QApplication::translate("Options", "9600", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "14400", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "19200", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "38400", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "57600", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "115200", 0, QApplication::UnicodeUTF8)
        );
        comboBox_port->clear();
        comboBox_port->insertItems(0, QStringList()
         << QApplication::translate("Options", "/dev/ttyUSB0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "/dev/ttyUSB1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "/dev/ttyUSB2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "/dev/ttyUSB3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "/dev/ttyS0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "/dev/ttyS1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "/dev/ttyS2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Options", "/dev/ttyS3", 0, QApplication::UnicodeUTF8)
        );
        groupBox_3->setTitle(QApplication::translate("Options", "Startup && Connect", 0, QApplication::UnicodeUTF8));
        checkBox_auto_connect->setText(QApplication::translate("Options", "Connect to port when CDRMate starts up", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_connection), QApplication::translate("Options", "Connection", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("Options", "Data Receiving Option", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Options", "Automatic Export", 0, QApplication::UnicodeUTF8));
        radio_export_on->setText(QApplication::translate("Options", "ON", 0, QApplication::UnicodeUTF8));
        radio_export_off->setText(QApplication::translate("Options", "OFF", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Options", "Export Directory", 0, QApplication::UnicodeUTF8));
        pushButton_export->setText(QApplication::translate("Options", "...", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("Options", "Data Transfer Option", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Options", "Import Directory", 0, QApplication::UnicodeUTF8));
        pushButton_import->setText(QApplication::translate("Options", "...", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Options", "Automatic Transfer", 0, QApplication::UnicodeUTF8));
        radio_transfer_on->setText(QApplication::translate("Options", "ON", 0, QApplication::UnicodeUTF8));
        radio_transfer_off->setText(QApplication::translate("Options", "OFF", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_receive_transfer), QApplication::translate("Options", "Data Receiving && Transfer", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Options);
    } // retranslateUi

};

namespace Ui {
    class Options: public Ui_Options {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONS_H
