#ifndef teachreg_H
#define teachreg_H

#include <QtGui/QMainWindow>
#include <QtCore/QSettings>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <cmath>
#include "ui_main_window.h"
#include "studentlistdialog.h"
#include "createdbdialog.h"
#include "teachregmanager.h"
#include "adddatedialog.h"
#include "dbase/htmlgenerator.h"
#include "help_browser/helpbrowser.h"
#include "teachmodel.h"
#include "optionsdialog.h"
#include "teachregconfigure.h"

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
	void showPracticsTable();
	
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
	void on_groupComboBox_activated(int index);
	void on_disciplinComboBox_activated(int index);
	void on_practicGroupComboBox_activated(int index);
	void on_subgroupComboBox_activated(int index);
	void on_practicDisciplinComboBox_activated(int index);
	void on_addPracticButton_clicked();
	void on_actionSave_triggered();
	void on_actionCreate_triggered();
	void on_actionOpen_triggered();
	void on_actionAboutQt_triggered();
	void on_showResultButton_clicked();
	void on_setTypeComboBox_activated(int index);
	void on_disciplinResultComboBox_activated(int index);
	void on_groupResultComboBox_activated(int index);
	void on_actionHelp_triggered();
	void on_actionOptions_triggered();
	void on_actionAbout_triggered();
	
private:
	Ui::MainWindow *ui;
	StudentListDialog *dlgStudents;
	CreateDbDialog *dlgCreateDb;
	AddDateDialog *dlgAddDate;
	TeachRegManager *manager;
	QSettings *settings;
	QString dbFileName;
	QString dbDateDir;
	bool onChangedLectureView;	// флаг запрета изменения ячейки false - не изменять, true - изменять можно
	bool onChangedPracticView;	// флаг запрета изменения ячейки false - не изменять, true - изменять можно
	HtmlGenerator *htmlGenerator;
	int currentPageIndex;
	QString docDir;
	TeachModel *lecPracModel;
	OptionsDialog *dlgOptions;
	QFont font0;				// шрифт списка студентов
	QFont font1;				// шрифт баллов
	QColor color0;				// цвет текста списка студентов
	QColor color1;				// цвет текста баллов
	QString appName;			// название приложения
	
	void createIcons();
	void writeSettings();
	void readSettings();
	void showGroupData(int groupId);
	QString roundResult(float num, int pers = 0);
	void setSubgroupsListTotals();
};

#endif // teachreg_H
