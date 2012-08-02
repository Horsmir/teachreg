#ifndef teachreg_H
#define teachreg_H

#include <QtGui/QMainWindow>
#include "ui_main_window.h"
#include "studentlistdialog.h"

namespace Ui
{
	class MainWindow;
}

class TeachReg : public QMainWindow
{
Q_OBJECT
public:
	explicit TeachReg(QWidget *parent = 0);
	virtual ~TeachReg();
	
private slots:
	void on_contentsWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
	void on_studentListButton_clicked();
	
private:
	Ui::MainWindow *ui;
	StudentListDialog *dlgStudents;
	
	void createIcons();
};

#endif // teachreg_H
