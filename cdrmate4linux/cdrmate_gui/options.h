#ifndef OPTIONS_H
#define OPTIONS_H

#include <QtGui/QDialog>
#include <QSettings>

namespace Ui
{
	class Options;
}

class Options:public QDialog
{
Q_OBJECT 
public:
	Options(QWidget * parent = 0);
	~Options();

	bool need_reconnect;
	bool modified;

	void set_conf(QSettings * pconf);
	void read_conf();
	void write_conf();

protected:
	void changeEvent(QEvent * e);
	QSettings *pconf;

private:
	Ui::Options * m_ui;

public slots: 
	void conf_apply();
	void data_modified();
	void choose_dir();
};

#endif // OPTIONS_H
