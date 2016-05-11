#include <QMessageBox>
#include <QCloseEvent>
#include <QProcess>
#include <QtDebug>
#include <QHeaderView>
#include <QFileDialog>

#include <sys/types.h>
#include <signal.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget * parent)
:	QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setWindowTitle(tr("Huvitz CDR Mate"));

	// setup received list 
	received_model = new QStandardItemModel(0, 4, parent);
	received_model->setHeaderData(0, Qt::Horizontal, tr("No"));
	received_model->setHeaderData(1, Qt::Horizontal, tr("Date"));
	received_model->setHeaderData(2, Qt::Horizontal, tr("Patient ID"));
	received_model->setHeaderData(3, Qt::Horizontal, tr("Exported"));
	ui->treeView_receive->setModel(received_model);
	ui->treeView_receive->header()->resizeMode(QHeaderView::ResizeToContents);
	ui->treeView_receive->header()->resizeSection(0, 50);	// No 
	ui->treeView_receive->header()->resizeSection(2, 150);	// patient id

	// setup transfer list 
	transfer_model = new QStandardItemModel(0, 5, parent);
	transfer_model->setHeaderData(0, Qt::Horizontal, tr("No"));
	transfer_model->setHeaderData(1, Qt::Horizontal, tr("Date"));
	transfer_model->setHeaderData(2, Qt::Horizontal, tr("Data Number"));
	transfer_model->setHeaderData(3, Qt::Horizontal, tr("Type"));
	transfer_model->setHeaderData(4, Qt::Horizontal, tr("Transfered"));

	ui->treeView_transfer->setModel(transfer_model);
	ui->treeView_transfer->header()->resizeMode(QHeaderView::ResizeToContents);
	ui->treeView_transfer->header()->resizeSection(0, 50);	// No 
	ui->treeView_transfer->header()->resizeSection(1, 160);	// Date 

	connected = false;
	ui->statusBar->addPermanentWidget(&status_bar_label);

	update_Menu();
	update_StatusBar();

	poption = new Options(this);
}

MainWindow::~MainWindow()
{
	delete poption;
	delete received_model;
	delete transfer_model;
	delete pconf;
	delete ui;
}

void MainWindow::setup_cmd(QString & cmd, QString & ini)
{
	cmd_path = cmd;
	ini_path = ini;
	pconf = new QSettings(ini, QSettings::IniFormat);

	poption->set_conf(pconf);

	// auto connect
	pconf->beginGroup("GUI");
	QString tmp = pconf->value("auto_connect", "").toString();
	pconf->endGroup();
	if (tmp.toLower().compare("on") == 0) {
		qDebug() << "emit connect";
		emit on_action_Connect_triggered();
	}
}

void MainWindow::update_StatusBar(void)
{
	if (connected) {
		QString port = pconf->value("port", "").toString();
		QString speed = pconf->value("speed", "").toString();
		status_bar_label.setText("[" + port + "," + speed + "bps] Connected");
	} else {
		status_bar_label.setText(tr("Not Connected"));
	}
}

void MainWindow::update_Menu(void)
{
	if (connected) {
		ui->action_Connect->setEnabled(false);
		ui->action_Disconnect->setEnabled(true);
	} else {
		ui->action_Connect->setEnabled(true);
		ui->action_Disconnect->setEnabled(false);
	}
}

void MainWindow::on_action_About_CDRMate_triggered()
{
	QMessageBox::about(this, tr("About CDRMate Linux"),
			   tr("<p>This is not the official version.<br><br>"
				   "<h2 align=center>CDRMate Linux V0.1</h2>"
			      "<p>For any kind of support about CDR Mate, contact your local distributor or send email to following address" 
			      "<p align=center>E-mail: cdrmate@huvitz.com" 
			      "<p align=center>Copyright &copy; 2009 Huvitz Co., Ltd." "<p align=center>All rights reserved."));
}

void MainWindow::closeEvent(QCloseEvent * event)
{
	if (connected == false) {
		event->accept();
		return;
	}

	int r = QMessageBox::warning(this, tr("CDRMate"), tr("Quit CDRMate?"), QMessageBox::No, QMessageBox::Yes | QMessageBox::Default);

	if (r == QMessageBox::Yes) {
		on_action_Disconnect_triggered();
		event->accept();
	} else {
		event->ignore();
	}
}

void MainWindow::on_action_Connect_triggered()
{
	if (connected) {
		return;
	}
	QStringList args;
	args << "-s" << "-c" + ini_path;
	console_program.start(cmd_path, args);

	// TO-DO : 프로세스를 실행하지 못핼경우 (잘못된 경로 혹은 콘솔 명령이 없을 경우)
	if (!console_program.waitForStarted()) {
		qDebug("Qprocess start failed.\n");
		return;
	}
	// TO-DO : 콘솔 프로그램 에러 발생
	if (console_program.waitForFinished(1000)) {
		QString err_log = console_program.readAllStandardOutput().constData();

		qDebug("console terminated with error.\n");
		qDebug() << err_log;
		QMessageBox::critical(this, tr("Connection Error"), err_log, QMessageBox::Ok | QMessageBox::Default);

		return;
	}

	connect(&console_program, SIGNAL(readyReadStandardError()), this, SLOT(slot_console_msg()));

	connected = true;
	update_StatusBar();
	update_Menu();
}

void MainWindow::on_action_Disconnect_triggered()
{
	if (!connected) {
		return;
	}

	qDebug() << "console pid = " << console_program.pid() << endl;

	kill(console_program.pid(), SIGTERM);

	if (!console_program.waitForFinished()) {
		qDebug("Qprocess finish failed.\n");
		return;
	}

	disconnect(&console_program, SIGNAL(readyReadStandardOutput()), this, SLOT(slot_console_output()));

	connected = false;
	update_StatusBar();
	update_Menu();
}

/// received table 내용 update
/// \param model
/// \param row
/// \param p
static void update_received_table(QAbstractItemModel * model, int row, CDRData * p)
{
	// 현재는 exported 필드만 업데이트 가능하다.
	model->setData(model->index(row, 3), p->exported ? "Yes" : "No");
}

/// received 테이블에 항목 추가
/// \param model
/// \param data
static void add_received_table(QAbstractItemModel * model, CDRData & data)
{
	static int serial_no = 1;
	int row;

	row = model->rowCount();

	model->insertRow(row);
	model->setData(model->index(row, 0), serial_no++);
	model->setData(model->index(row, 1), data.date.toString(Qt::ISODate));
	model->setData(model->index(row, 2), data.patient_id);
	model->setData(model->index(row, 3), data.exported ? "Yes" : "No");
}

/// transfer table 내용 update
/// \param model
/// \param row
/// \param p
static void update_transfer_table(QAbstractItemModel * model, int row, ExData * p)
{
	// 현재는 transferred 필드만 업데이트 가능하다.
	model->setData(model->index(row, 4), p->transferred ? "Yes" : "No");
}

/// transfer 테이블에 항목 추가
/// \param model
/// \param data
static void add_transfer_table(QAbstractItemModel * model, ExData & data)
{
	static int serial_no = 1;
	int row;

	row = model->rowCount();

	model->insertRow(row);
	model->setData(model->index(row, 0), serial_no++);
	model->setData(model->index(row, 1), data.date.toString(Qt::ISODate));
	model->setData(model->index(row, 2), data.data_no);
	model->setData(model->index(row, 3), data.type ? "MRK" : "CLM");
	model->setData(model->index(row, 4), data.transferred ? "Yes" : "No");
}

/// SLR message 처리 
/// \param param filename
int MainWindow::handle_SLR(QString param)
{
	QString rcv_path = pconf->value("received", "").toString();

	if (rcv_path.isEmpty()) {
		qDebug() << "invalid received path.\n";
		return -1;
	}

	rcv_path = rcv_path + "/" + param;

	qDebug() << rcv_path;

	QFile file(rcv_path);
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << file.errorString() << rcv_path;
		return -1;
	}

	// auto export
	pconf->beginGroup("GUI");
	bool auto_export = pconf->value("auto_export", "").toString().toLower().compare("on") == 0 ? true : false;
	pconf->endGroup();

	CDRData *p = new CDRData();

	p->date = QDate::currentDate();
	p->filename = param;
	p->patient_id = param.section('_', 2, 2).section('.', 0, 0);
	if ( auto_export ) {
		p->exported = true;
	} else {
		p->exported = false;
	}

	QTextStream in(&file);
	p->contents = in.readLine();
	file.close();

	received_list.append(*p);

	for (int i = 0; i < received_list.count(); i++) {
		qDebug() << __func__ << __LINE__ << i << "date=" << received_list[i].date.toString(Qt::ISODate) << "patient_id=" << received_list[i].patient_id << "file=" << received_list[i].filename << "contents=" << received_list[i].contents;
	}

	add_received_table(received_model, *p);

	if ( !auto_export ) { 
		send_console("SLD " + param);
	}
	return 0;
}

/// ExData 생성 
static ExData *make_exdata(QString & filename, QString & contents, QString & type_str)
{
	QStringList all_field = contents.split(",");

	qDebug() << __func__ << __LINE__ << "field count " << all_field.count();
	if (all_field.count() < 14) {
		qDebug() << "insufficient data field.";
		return NULL;
	}

	ExData *p = new ExData();

	p->date = QDateTime::currentDateTime();
	p->data_no = all_field.at(0);
	p->data_no.remove('"');
	p->filename = filename;
	if (type_str.compare("CLM") == 0) {
		p->type = 0;
	} else {
		p->type = 1;
	}
	p->contents = contents;
	p->transferred = false;
	return p;
}

/// SLF message 처리 
/// \param filename
/// \param no
/// \param type
int MainWindow::handle_SLF(QString filename, QString type_str)
{
	QString tr_path = pconf->value("transfer", "").toString();
	if (tr_path.isEmpty()) {
		qDebug() << "invalid transfer path" << tr_path;
		return -1;
	}

	tr_path = tr_path + "/" + filename;

	qDebug() << tr_path;

	QFile file(tr_path);
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << file.errorString() << tr_path;
		return -2;
	}

	QTextStream in(&file);
	QString contents = in.readLine();
	file.close();



	ExData *p = make_exdata(filename, contents, type_str);
	if (p == NULL) {
		return -3;

	}
	// auto transfer 
	pconf->beginGroup("GUI");
	bool auto_transfer = pconf->value("auto_transfer", "").toString().toLower().compare("on") == 0 ? true : false;
	pconf->endGroup();
	if ( auto_transfer ) {
		p->transferred = true;
	}
	transfer_list.append(*p);
	add_transfer_table(transfer_model, *p);


	for (int i = 0; i < transfer_list.count(); i++) {
		qDebug() << __func__ << __LINE__ << i << "date=" << transfer_list[i].date.toString(Qt::ISODate) << "data_no=" << transfer_list[i].data_no << "file=" << transfer_list[i].filename << "type=" << transfer_list[i].type << "contents=" << transfer_list[i].contents;
	}

	if ( auto_transfer ) {
		handle_send(transfer_list.count() - 1);
	}

	return 0;

#if 0

	add_received_table(received_model, *p);

	//send_console("SLD " + param);
#endif
	return 0;
}

/// 메시지 한 줄 처리
/// \param line
void MainWindow::handle_console_msg_line(QString & line)
{
	line.trimmed();

	// 공백으로 key, value 분리 
	QStringList token_list = line.split(" ");

	if (token_list.value(0).isEmpty()) {
		return;
	}

	if (token_list.value(0).compare("SLR") == 0) {
		handle_SLR(token_list.value(1));
	} else if (token_list.value(0).compare("SLF") == 0) {
		handle_SLF(token_list.value(1), token_list.value(2));
	} else if (token_list.value(0).compare("SLE") == 0) {
		qDebug() << "SLE" << token_list.value(1);
	} else {
		qDebug() << "Invalid msg : " << line;
	}
}

void MainWindow::slot_console_msg()
{
	QString msg = console_program.readAllStandardError().constData();
	qDebug() << __func__ << __LINE__ << "msg = " << msg << endl;

	msg.trimmed();

	QStringList lines = msg.split("\n");

	for (int i = 0; i < lines.count(); i++) {
		QString line = lines.at(i);
		line.trimmed();
		handle_console_msg_line(line);
	}

	// 메시지 박스로 확인 
	//QMessageBox::information(this, tr("DEBUG console msg"), msg, QMessageBox::Ok | QMessageBox::Default);

}

void MainWindow::slot_console_output()
{
	qDebug() << "DEBUG handle console out" << endl;

	QString output = console_program.readAllStandardOutput().constData();
	QMessageBox::information(this, tr("DEBUG console output"), output, QMessageBox::Ok | QMessageBox::Default);
}

/// csv 파일 저장 
/// \return 0 success
/// \return -1 error
static int save_csv(QString & path, QString & buf)
{
	QFile file(path);

	if (!file.open(QIODevice::WriteOnly)) {
		qDebug() << file.errorString() << path;
		return -1;
	}

	QTextStream out(&file);
	out << buf;
	file.close();
	return 0;
}

/// export to csv file
void MainWindow::handle_export()
{
	QModelIndex index = ui->treeView_receive->currentIndex();
	qDebug() << __func__ << __LINE__ << index;
	if (index.row() < 0) {
		return;
	}
	QString export_filename = pconf->value("received", "").toString() + "/" + received_list[index.row()].patient_id + ".csv";

	export_filename = QFileDialog::getSaveFileName(this, tr("Export"), export_filename, tr("csv Files (*.csv)"));

	if (export_filename.isEmpty()) {
		return;
	}
	qDebug() << "save : " << export_filename;

	if (save_csv(export_filename, received_list[index.row()].contents) < 0) {
		QMessageBox::critical(this, tr("Export Error"), "File save error.\n" + export_filename, QMessageBox::Ok | QMessageBox::Default);

	} else {
		received_list[index.row()].exported = true;
		update_received_table(received_model, index.row(), &received_list[index.row()]);
	}
}

/// mrk/clm data send
void MainWindow::handle_send(int idx)
{
	if ( idx < 0 ) {
		QModelIndex index = ui->treeView_transfer->currentIndex();
		qDebug() << __func__ << __LINE__ << "row =" << index.row() << "QModelIndex=" << index;

		idx = index.row();
	} 

	if (idx < 0) {
		return;
	}

	if (!connected) {
		QMessageBox::warning(this, tr("CDRMate"), tr("CDR is not connected.\nPlease connect first."), QMessageBox::Ok | QMessageBox::Default);
		return;
	}

	send_console("SLT " + transfer_list[idx].filename);
	transfer_list[idx].transferred = true;
	update_transfer_table(transfer_model, idx, &transfer_list[idx]);

#if 0
	// test
	qDebug() << "DEBUG send msg : " << ui->lineEdit->text();
	console_program.write(qPrintable(ui->lineEdit->text()));
	console_program.write("\n");
#endif
}

/// export/send button 공통 slot
void MainWindow::slot_export_send()
{
	if (ui->tabWidget->currentIndex() == 0) {
		handle_export();
	} else {
		handle_send( -1);
	}
}

/// view button slot
void MainWindow::slot_item_view()
{
	qDebug() << __func__ << __LINE__ << "not implemented.";
}

/// cdr data delete 처리 
void MainWindow::handle_cdr_data_delete()
{
	QModelIndex index = ui->treeView_receive->currentIndex();

	qDebug() << __func__ << __LINE__ << "row =" << index.row() << "QModelIndex=" << index;
	if (index.row() < 0) {
		return;
	}
	received_model->removeRow(index.row());
	received_list.remove(index.row());

	for (int i = 0; i < received_list.count(); i++) {
		qDebug() << i << "date =" << received_list[i].date.toString(Qt::ISODate) << "patient_id =" << received_list[i].patient_id << "file =" << received_list[i].filename;
	}
}

/// clm/mrk  data delete 처리 
void MainWindow::handle_clm_mrk_delete()
{
	QModelIndex index = ui->treeView_transfer->currentIndex();
	qDebug() << __func__ << __LINE__ << "row =" << index.row() << "QModelIndex=" << index;
	if (index.row() < 0) {
		return;
	}
	transfer_model->removeRow(index.row());
	transfer_list.remove(index.row());

	for (int i = 0; i < transfer_list.count(); i++) {
		qDebug() << __func__ << __LINE__ << i << "date=" << transfer_list[i].date.toString(Qt::ISODate) << "data_no=" << transfer_list[i].data_no << "file=" << transfer_list[i].filename << "type=" << transfer_list[i].type << "contents=" << transfer_list[i].contents;
	}
}

/// delete button slot
void MainWindow::slot_item_delete()
{
	if (ui->tabWidget->currentIndex() == 0) {
		handle_cdr_data_delete();
	} else {
		handle_clm_mrk_delete();
	}
}

/// tab widget 변경에 따라 export/send button text 변경
void MainWindow::slot_change_export_send_button(int tab_idx)
{
	ui->pushButton_export_send->setText(tab_idx == 0 ? tr("Export") : tr("Send"));
}

void MainWindow::send_console(QString msg)
{
	console_program.write(qPrintable(msg));
	console_program.write("\n");
}

void MainWindow::on_action_Options_triggered()
{
	bool reconnect = false;

	poption->read_conf();
	if (poption->exec()) {
		if ( poption->modified) {
			poption->write_conf();
			reconnect = true;
		}
	} 
	if ( poption->need_reconnect ) {
		reconnect = true;
	}

	if ( connected && reconnect ) {

		ui->statusBar->showMessage(tr("Disconnecting..."), 4000);
		on_action_Disconnect_triggered();
		ui->statusBar->showMessage(tr("Connecting..."), 4000);
		on_action_Connect_triggered();
		ui->statusBar->showMessage(tr("Reconnected."), 4000);
	}
}

#if 0
			// restart 반응이 늦어 다른 방법으로.....

			// restart console
			qDebug() << "restart console, pid = " << console_program.pid();
			kill(console_program.pid(), SIGHUP);

			if (console_program.waitForFinished(5000)) {
				QString err_log = console_program.readAllStandardOutput().constData();

				qDebug("console terminated with error.\n");
				qDebug() << err_log;
				QMessageBox::critical(this, tr("Connection Error"), err_log, QMessageBox::Ok | QMessageBox::Default);

				disconnect(&console_program, SIGNAL(readyReadStandardOutput()), this, SLOT(slot_console_output()));

				connected = false;
				update_StatusBar();
				update_Menu();

				return;
			}
#endif

