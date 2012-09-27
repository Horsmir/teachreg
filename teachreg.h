#ifndef teachreg_H
#define teachreg_H

#include <QtGui/QMainWindow>
#include <QtCore/QSettings>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include "ui_main_window.h"
#include "studentlistdialog.h"
#include "createdbdialog.h"
#include "teachregmanager.h"
#include "adddatedialog.h"

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
	
	void showDisciplinList();
	void showGroupslinList();
	void showLecturesTable();
	
protected:
	void closeEvent(QCloseEvent *event);
	
private slots:
	void on_contentsWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
	void on_studentListButton_clicked();
	void on_createDbButton_clicked();
	void on_selectDbFilePathButton_clicked();
	void on_addDisciplinButton_clicked();
	void on_editDisciplinButton_clicked();
	void on_delDisciplinButton_clicked();
	void on_addGroupButton_clicked();
	void on_viewGroupsWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
	void on_editGroupButton_clicked();
	void on_setEditGroupButton_clicked();
	void on_delGroupButton_clicked();
	void on_addLectureButton_clicked();
	void on_lectureViewWidget_cellChanged(int row, int column);
	
private:
	Ui::MainWindow *ui;
	StudentListDialog *dlgStudents;
	CreateDbDialog *dlgCreateDb;
	AddDateDialog *dlgAddDate;
	TeachRegManager *manager;
	QSettings *settings;
	QString dbFileName;
	QString dbDateDir;
	QStringList *ocenkList;
	
	void createIcons();
	void writeSettings();
	void readSettings();
	void showGroupData(const QString &groupName);
};

#endif // teachreg_H
