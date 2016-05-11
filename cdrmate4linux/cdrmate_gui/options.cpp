#include <QFileDialog>
#include <QtDebug>

#include "options.h"
#include "ui_options.h"

Options::Options(QWidget * parent):
QDialog(parent), m_ui(new Ui::Options)
{
	m_ui->setupUi(this);
}

Options::~Options()
{
	delete m_ui;
}

void Options::changeEvent(QEvent * e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void Options::read_conf()
{
	m_ui->comboBox_protocol_ver->setCurrentIndex(m_ui->comboBox_protocol_ver->findText(pconf->value("protocol_ver", "").toString(), Qt::MatchExactly));
	m_ui->comboBox_system_id->setCurrentIndex(m_ui->comboBox_system_id->findText(pconf->value("system_id", "").toString(), Qt::MatchExactly));
	m_ui->comboBox_port->setEditText(pconf->value("port", "").toString());
	m_ui->comboBox_speed->setCurrentIndex(m_ui->comboBox_speed->findText(pconf->value("speed", "").toString(), Qt::MatchExactly));

	m_ui->lineEdit_export->setText(pconf->value("received", "").toString());
	m_ui->lineEdit_import->setText(pconf->value("transfer", "").toString());

	pconf->beginGroup("GUI");
	if (pconf->value("auto_export", "").toString().toLower().compare("on") == 0) {
		m_ui->radio_export_on->setChecked(true);
	} else {
		m_ui->radio_export_off->setChecked(true);
	}


	if (pconf->value("auto_transfer", "").toString().toLower().compare("on") == 0) {
		m_ui->radio_transfer_on->setChecked(true);
	} else {
		m_ui->radio_transfer_off->setChecked(true);
	}

	if (pconf->value("auto_connect", "").toString().toLower().compare("on") == 0) {
		m_ui->checkBox_auto_connect->setChecked(true);
	} else {
		m_ui->checkBox_auto_connect->setChecked(false);
	}

	pconf->endGroup();

	modified = false;
	need_reconnect = false;
	m_ui->pushButton_apply->setEnabled(false);
}

void Options::write_conf()
{
	pconf->setValue("port", m_ui->comboBox_port->currentText());
	pconf->setValue("speed", m_ui->comboBox_speed->currentText());
	pconf->setValue("protocol_ver", m_ui->comboBox_protocol_ver->currentText());
	pconf->setValue("system_id", m_ui->comboBox_system_id->currentText());
	pconf->setValue("received", m_ui->lineEdit_export->text());
	pconf->setValue("transfer", m_ui->lineEdit_import->text());

	pconf->beginGroup("GUI");
	pconf->setValue("auto_export", m_ui->radio_export_on->isChecked()? "on" : "off");
	pconf->setValue("auto_transfer", m_ui->radio_transfer_on->isChecked()? "on" : "off");
	pconf->setValue("auto_connect", m_ui->checkBox_auto_connect->isChecked()? "on" : "off");
	pconf->endGroup();
	pconf->sync();

	modified = false;
	m_ui->pushButton_apply->setEnabled(false);
}

void Options::set_conf(QSettings * p)
{
	pconf = p;
}

void Options::data_modified()
{
	qDebug() << "conf modified.";
	modified = true;
	m_ui->pushButton_apply->setEnabled(true);
}

void Options::choose_dir()
{
	QString dir_name = QFileDialog::getExistingDirectory(this, tr("choose Directory"), "", 0);

	if (dir_name.isNull()) {
		return;
	}

	qDebug() << __func__ << __LINE__ << dir_name;

	if (QObject::sender() == m_ui->pushButton_export) {
		m_ui->lineEdit_export->setText(dir_name);
	} else {
		m_ui->lineEdit_import->setText(dir_name);
	}
}

void Options::conf_apply()
{
	write_conf();
	need_reconnect = true;
}
