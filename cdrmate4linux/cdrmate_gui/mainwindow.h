#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QLabel>
#include <QProcess>
#include <QSettings>
#include <QStandardItemModel>

#include "options.h"
#include "cdrdata.h"
#include "exdata.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

        void update_StatusBar(void);
	void update_Menu(void);

        void setup_cmd(QString &cmd, QString &ini);



private:
	Ui::MainWindow *ui;

        bool connected;
        QLabel status_bar_label;
        QProcess console_program;
        QString cmd_path;
	QString ini_path;
        QSettings *pconf;

        QVector<CDRData> received_list;
	QVector<ExData> transfer_list;

        QStandardItemModel *received_model;
	QStandardItemModel *transfer_model;

        Options *poption;

	void handle_console_msg_line(QString &line);

	int handle_SLR(QString param);
	int handle_SLF(QString name, QString type_str);

	void handle_export();
	void handle_send(int idx);
	void handle_cdr_data_delete();
	void handle_clm_mrk_delete();
	void send_console(QString msg);


private slots:
        void on_action_Options_triggered();
	void on_action_Disconnect_triggered();
	void on_action_Connect_triggered();
	void on_action_About_CDRMate_triggered();
	void closeEvent(QCloseEvent *event);

        void slot_console_msg();
        void slot_console_output();

	void slot_item_view();
        void slot_export_send();
        void slot_item_delete();
	void slot_change_export_send_button(int tab_idx);
};

#endif // MAINWINDOW_H
