#include <QtGui/QApplication>
#include <QDir>
#include <QtDebug>

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "mainwindow.h"

/// default ini 파일 생성
/// \param ini_path
static void create_ini(QString &conf_path, QString &ini_path)
{
	QSettings conf(ini_path, QSettings::IniFormat);
	QString tmp;

#if 0
	// DEBUG read test
        QString data;
        data = conf.value("port", "error").toString();

        qDebug() << "DEBUG port = " << data << conf.value("speed", "err").toString();
#endif 
        conf.setValue("port", "/dev/ttyS0");
        conf.setValue("speed", "9600");
        conf.setValue("protocol_ver", "1.0");
        conf.setValue("system_id", "0");
	tmp = conf_path + "/received";
        conf.setValue("received", tmp);
	tmp = conf_path + "/transfer";
        conf.setValue("transfer", tmp);
	tmp = conf_path + "/transferred";
        conf.setValue("transferred", tmp);

	conf.beginGroup("GUI");
        conf.setValue("auto_export", "off");
        conf.setValue("auto_transfer", "off");
        conf.setValue("auto_connect", "off");
	conf.endGroup();
        conf.sync();
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QStringList args = a.arguments();
	QString conf_path, cmd, ini;

	// 프로그램 경로를 반드시 지정해야 한다.
	if ( args.count() < 2) {
		conf_path = QDir::currentPath();
	} else {
		conf_path = args[1];
	}
	cmd = conf_path + "/cdrmate";
	ini = conf_path + "/cdrmate.ini";

	qDebug("conf_path = %s", qPrintable(conf_path));
	qDebug("cmd = %s", qPrintable(cmd));
	qDebug("ini = %s", qPrintable(ini));

	// cmd check
	QFile file(cmd);
	if ( !file.open(QIODevice::ReadOnly)) {
		fprintf(stderr, "cdrmate command not found.\n");
		fprintf(stderr, "Usage : %s program_directory\n", argv[0]);
		return 1;
	}
	file.close();

	// ini check
	file.setFileName(ini);
	if ( !file.open(QIODevice::ReadOnly)) {
		// create default ini
		create_ini(conf_path, ini);
		qDebug("ini file not found. %s created.\n", qPrintable(ini));
	} else {
		file.close();
	}


	// start main window
	MainWindow w;
	w.setup_cmd(cmd, ini);
	w.show();
	return a.exec();
}
