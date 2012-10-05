#include "teachreg.h"

TeachReg::TeachReg(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	dbFileName = "";
	dbDateDir = "";
	
	dlgStudents = new StudentListDialog(this);
	dlgCreateDb = new CreateDbDialog(this);
	dlgAddDate = new AddDateDialog(this);
	
	manager = new TeachRegManager(this);
	
	settings = new QSettings("../config/teahreg.conf", QSettings::NativeFormat, this);
	readSettings();
	
	if(!dbFileName.isEmpty())
	{
		if(manager->loadDB(dbFileName))
		{
			ui->dbFilePathEdit->setText(dbFileName);
			ui->aboutDbTextEdit->setPlainText(manager->getAboutDB());
			ui->label_14->setVisible(false);
			setWindowTitle("TeachReg - " + dbFileName);
		}
		else
		{
			QMessageBox::warning(this, trUtf8("Загрузка базы данных"), trUtf8("Не возможно загрузить файл ") + dbFileName);
			dbFileName.clear();
		}
	}
	
	createIcons();
	ui->subgroupResultComboBox->setVisible(false);
	ui->label_20->setVisible(false);
	
	ocenkList = new QStringList;
	ocenkList->append("");
	ocenkList->append(trUtf8("нб"));
	ocenkList->append("2");
	ocenkList->append("3");
	ocenkList->append("4");
	ocenkList->append("5");
	
	onChangedLectureView = false;
	onChangedPracticView = false;
	
	htmlGenerator = new HtmlGenerator(this);
	
	ui->contentsWidget->setCurrentRow(currentPageIndex);
	ui->pagesWidget->setCurrentIndex(currentPageIndex);
}

TeachReg::~TeachReg()
{
	delete ui;
	delete ocenkList;
}

void TeachReg::createIcons()
{
	QListWidgetItem *homeButton = new QListWidgetItem(ui->contentsWidget);
	homeButton->setIcon(QIcon(":/images/home.png"));
	homeButton->setText(trUtf8("Главная"));
	homeButton->setTextAlignment(Qt::AlignHCenter);
	
	QListWidgetItem *disciplinsButton = new QListWidgetItem(ui->contentsWidget);
	disciplinsButton->setIcon(QIcon(":/images/disciplins.png"));
	disciplinsButton->setText(trUtf8("Дисциплины"));
	disciplinsButton->setTextAlignment(Qt::AlignHCenter);
	
	QListWidgetItem *groupsButton = new QListWidgetItem(ui->contentsWidget);
	groupsButton->setIcon(QIcon(":/images/groups.png"));
	groupsButton->setText(trUtf8("Группы"));
	groupsButton->setTextAlignment(Qt::AlignHCenter);
	
	QListWidgetItem *lectureButton = new QListWidgetItem(ui->contentsWidget);
	lectureButton->setIcon(QIcon(":/images/lectures.png"));
	lectureButton->setText(trUtf8("Лекции"));
	lectureButton->setTextAlignment(Qt::AlignHCenter);
	
	QListWidgetItem *practicsButton = new QListWidgetItem(ui->contentsWidget);
	practicsButton->setIcon(QIcon(":/images/practics.png"));
	practicsButton->setText(trUtf8("Практика"));
	practicsButton->setTextAlignment(Qt::AlignHCenter);
	
	QListWidgetItem *resultsButton = new QListWidgetItem(ui->contentsWidget);
	resultsButton->setIcon(QIcon(":/images/results.png"));
	resultsButton->setText(trUtf8("Итог"));
	resultsButton->setTextAlignment(Qt::AlignHCenter);
}

void TeachReg::on_contentsWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	if(!current)
		current = previous;
	
	ui->pagesWidget->setCurrentIndex(ui->contentsWidget->row(current));
	
	switch(ui->pagesWidget->currentIndex())
	{
		case 0:
			break;
		case 1:
			showDisciplinList();
			break;
		case 2:
			showGroupslinList();
			break;
		case 3:
			ui->groupComboBox->clear();
			ui->disciplinComboBox->clear();
			ui->groupComboBox->addItems(manager->getGroupsNamesList());
			ui->disciplinComboBox->addItems(manager->getDisciplinsList());
			showLecturesTable();
			break;
		case 4:
			ui->practicGroupComboBox->clear();
			ui->practicDisciplinComboBox->clear();
			ui->practicGroupComboBox->addItems(manager->getGroupsNamesList());
			ui->practicDisciplinComboBox->addItems(manager->getDisciplinsList());
			on_practicGroupComboBox_activated(0);
			break;
		case 5:
			ui->groupResultComboBox->addItems(manager->getGroupsNamesList());
			ui->groupResultComboBox->setCurrentIndex(0);
			ui->disciplinResultComboBox->addItems(manager->getDisciplinsList());
			ui->disciplinResultComboBox->setCurrentIndex(0);
			break;
		default:
			break;
	}
}

void TeachReg::on_studentListButton_clicked()
{
	dlgStudents->setGroupName(ui->viewGroupsWidget->currentItem()->text());
	dlgStudents->setManager(manager);
	dlgStudents->show();
}

void TeachReg::on_createDbButton_clicked()
{
	if(dlgCreateDb->exec() == QDialog::Accepted)
	{
		dbFileName = dlgCreateDb->getFilePath();
		manager->createDB(dbFileName, dlgCreateDb->getAbout());
		ui->dbFilePathEdit->setText(dbFileName);
		ui->aboutDbTextEdit->setPlainText(manager->getAboutDB());
		ui->label_14->setVisible(false);
		setWindowTitle("TeachReg - " + dbFileName);
	}
}

void TeachReg::writeSettings()
{
	settings->setValue("general/CurrentDBFilePath", dbFileName);
	settings->setValue("general/CurrentDBDateDir", dbDateDir);
	settings->setValue("view/LastPage", ui->pagesWidget->currentIndex());
}

void TeachReg::readSettings()
{
	dbFileName = settings->value("general/CurrentDBFilePath", QString()).toString();
	dbDateDir = settings->value("general/CurrentDBDateDir", QDir::homePath()).toString();
	currentPageIndex = settings->value("view/LastPage", 0).toInt();
}

void TeachReg::on_selectDbFilePathButton_clicked()
{
	QFileDialog::Options options;
	options |= QFileDialog::DontUseNativeDialog;
	
	QString selectedFilter;
	
	QString fileName = QFileDialog::getOpenFileName(this, trUtf8("Файл базы данных"), dbDateDir, trUtf8("Файлы базы данных (*.dbtr);;Все файлы (*.*)"), &selectedFilter, options);
	if(!fileName.isEmpty())
	{
		dbFileName = fileName;
		manager->loadDB(dbFileName);
		ui->dbFilePathEdit->setText(dbFileName);
		ui->aboutDbTextEdit->setPlainText(manager->getAboutDB());
		ui->label_14->setVisible(false);
		setWindowTitle("TeachReg - " + dbFileName);
	}
}

void TeachReg::showDisciplinList()
{
	ui->viewDisciplinsWidget->clear();
	QStringList disciplins = manager->getDisciplinsList();
	ui->viewDisciplinsWidget->addItems(disciplins);
	ui->viewDisciplinsWidget->sortItems();
	ui->viewDisciplinsWidget->setCurrentItem(0);
}

void TeachReg::on_addDisciplinButton_clicked()
{
	bool ok;
	
	QString text = QInputDialog::getText(this, trUtf8("Добавить дисциплину"), trUtf8("Название:"), QLineEdit::Normal, QString(), &ok);
	if(ok && !text.isEmpty())
	{
		manager->addDisciplin(text);
		showDisciplinList();
	}
}

void TeachReg::on_editDisciplinButton_clicked()
{
	QString curDisciplin = ui->viewDisciplinsWidget->currentItem()->text();
	bool ok;
	
	QString text = QInputDialog::getText(this, trUtf8("Изменить дисциплину"),
												  trUtf8("Название:"), QLineEdit::Normal,
												  curDisciplin, &ok);
	if (ok && !text.isEmpty())
	{
		manager->editDisciplin(curDisciplin, text);
		showDisciplinList();
	}
}

void TeachReg::on_delDisciplinButton_clicked()
{
	QString curDisciplin = ui->viewDisciplinsWidget->currentItem()->text();
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, trUtf8("Удаление дисциплины"), QString(trUtf8("Удалить дисциплину \"") + curDisciplin + "\"?"), QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes)
	{
		manager->delDisciplin(curDisciplin);
		showDisciplinList();
	}
}

void TeachReg::closeEvent(QCloseEvent *event)
{
	manager->saveDB();
	writeSettings();
	QWidget::closeEvent(event);
}

void TeachReg::on_addGroupButton_clicked()
{
	bool ok;
	
	QString text = QInputDialog::getText(this, trUtf8("Добавить группу"), trUtf8("Название:"), QLineEdit::Normal, QString(), &ok);
	if(ok && !text.isEmpty())
	{
		manager->addGroup(text);
		showGroupslinList();
	}
}

void TeachReg::showGroupslinList()
{
	ui->viewGroupsWidget->clear();
	
	QStringList groups = manager->getGroupsNamesList();
	ui->viewGroupsWidget->addItems(groups);
	ui->viewGroupsWidget->sortItems();
	ui->viewGroupsWidget->setCurrentItem(0);
}

void TeachReg::showGroupData(const QString &groupName)
{
	QStringList groupData = manager->getGroupData(groupName);
	ui->groupNameEdit->setText(groupData.at(0));
	ui->labelNumStudents->setText(groupData.at(1));
	ui->labelNumSubGroups->setText(groupData.at(2));
}

void TeachReg::on_viewGroupsWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	if(!current)
		current = previous;
	
	QString curGroupName = current->text();
	showGroupData(curGroupName);
}

void TeachReg::on_editGroupButton_clicked()
{
	if(!ui->viewGroupsWidget->currentItem())
	{
		QMessageBox::information(this, trUtf8("Группы"), trUtf8("Не выбрана группа.\nВыберите группу из списка и нажмите кнопку \"Изменить\"."), QMessageBox::Ok);
		return;
	}
	ui->setEditGroupButton->setEnabled(true);
}

void TeachReg::on_setEditGroupButton_clicked()
{
	manager->editGroup(ui->viewGroupsWidget->currentItem()->text(), ui->groupNameEdit->text());
	ui->setEditGroupButton->setDisabled(true);
	showGroupslinList();
}

void TeachReg::on_delGroupButton_clicked()
{
	if(!ui->viewGroupsWidget->currentItem())
	{
		QMessageBox::information(this, trUtf8("Группы"), trUtf8("Не выбрана группа.\nВыберите группу из списка и нажмите кнопку \"Удалить\"."), QMessageBox::Ok);
		return;
	}
	
	QString curGroupName = ui->viewGroupsWidget->currentItem()->text();
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, trUtf8("Удаление группы"), QString(trUtf8("Удалить группу \"") + curGroupName + "\"?"), QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes)
	{
		manager->delGroup(curGroupName);
		showGroupslinList();
	}
}

void TeachReg::showLecturesTable()
{
	onChangedLectureView = false;
	ui->lectureViewWidget->clear();
	QString curGroupName = ui->groupComboBox->currentText();
	QString curDisciplin = ui->disciplinComboBox->currentText();
	
	QStringList dates;
	dates << trUtf8("Ф.И. студента");
	dates.append( manager->getLecturesDateList(curGroupName, curDisciplin));
	
	QStringList sts = manager->getStudentsList(curGroupName);
	
	int datesCount = dates.count();
	int studentsCount = sts.count();
	
	ui->lectureViewWidget->setColumnCount(datesCount);
	ui->lectureViewWidget->setRowCount(studentsCount);
	ui->lectureViewWidget->setHorizontalHeaderLabels(dates);
	for(int i = 0; i < studentsCount; i++)
	{
		ui->lectureViewWidget->setItem(i, 0, new QTableWidgetItem(sts.at(i)));
	}
	
	for(int i = 0; i < studentsCount; i++)
	{
		QStringList results = manager->getLecturesResultList(curGroupName, curDisciplin, sts.at(i));
		if(results.isEmpty())
			break;
		for(int j = 0; j < results.count(); j++)
		{
			ui->lectureViewWidget->setItem(i, j + 1, new QTableWidgetItem(ocenkList->at(results.at(j).toInt())));
		}
		
	}
	ui->lectureViewWidget->sortItems(0);
	onChangedLectureView = true;
}

void TeachReg::on_addLectureButton_clicked()
{
	if(dlgAddDate->exec() == QDialog::Accepted)
	{
		manager->addLecture(ui->groupComboBox->currentText(), ui->disciplinComboBox->currentText(), dlgAddDate->getDate());
		showLecturesTable();
	}
}

void TeachReg::on_lectureViewWidget_cellChanged(int row, int column)
{
	if(!onChangedLectureView)
		return;
	QString studentName = ui->lectureViewWidget->item(row, 0)->text();
	int posDate = column - 1;
	QString result = ui->lectureViewWidget->item(row, column)->text();
	
	int ocencId = ocenkList->indexOf(result);
	if(ocencId == -1)
		ocencId = 0;
	
	manager->addLectureResult(ui->groupComboBox->currentText(), ui->disciplinComboBox->currentText(), studentName, posDate, QString().setNum(ocencId));
}

void TeachReg::on_disciplinComboBox_activated(int index)
{
	showLecturesTable();
}

void TeachReg::on_groupComboBox_activated(int index)
{
	showLecturesTable();
}

void TeachReg::showPracticsTable()
{
	onChangedPracticView = false;
	
	QString curGroupName = ui->practicGroupComboBox->currentText();
	QString curDisciplinName = ui->practicDisciplinComboBox->currentText();
	int curSubgroup = ui->subgroupComboBox->currentText().toInt();
	
	QStringList dates;
	dates << trUtf8("Ф.И. студента");
	dates.append(manager->getPracticsDateList(curGroupName, curDisciplinName, curSubgroup));
	
	QStringList sts = manager->getStudentsSubgroupList(curGroupName, curSubgroup);
	
	int datesCount = dates.count();
	int studentsCount = sts.count();
	
	ui->practicViewWidget->setColumnCount(datesCount);
	ui->practicViewWidget->setRowCount(studentsCount);
	ui->practicViewWidget->setHorizontalHeaderLabels(dates);
	for(int i = 0; i < studentsCount; i++)
	{
		ui->practicViewWidget->setItem(i, 0, new QTableWidgetItem(sts.at(i)));
	}
	
	for(int i = 0; i < studentsCount; i++)
	{
		QStringList results = manager->getPracticsResultList(curGroupName, curDisciplinName, curSubgroup, sts.at(i));
		if(results.isEmpty())
			break;
		for(int j = 0; j < results.count(); j++)
		{
			ui->practicViewWidget->setItem(i, j + 1, new QTableWidgetItem(ocenkList->at(results.at(j).toInt())));
		}
		
	}
	ui->practicViewWidget->sortItems(0);
	onChangedPracticView = true;
}

void TeachReg::on_practicGroupComboBox_activated(int index)
{
	int countSubgroups = manager->getGroupData(ui->practicGroupComboBox->currentText()).at(2).toInt();
	ui->subgroupComboBox->clear();
	for(int i = 0; i < countSubgroups; i++)
		ui->subgroupComboBox->addItem(QString().setNum(i + 1));
	showPracticsTable();
}

void TeachReg::on_subgroupComboBox_activated(int index)
{
	showPracticsTable();
}

void TeachReg::on_practicDisciplinComboBox_activated(int index)
{
	showPracticsTable();
}

void TeachReg::on_addPracticButton_clicked()
{
	if(dlgAddDate->exec() == QDialog::Accepted)
	{
		manager->addPractic(ui->practicGroupComboBox->currentText(), ui->practicDisciplinComboBox->currentText(), ui->subgroupComboBox->currentText().toInt(), dlgAddDate->getDate());
		showPracticsTable();
	}
}

void TeachReg::on_practicViewWidget_cellChanged(int row, int column)
{
	if(!onChangedPracticView)
		return;
	
	QString studentName = ui->practicViewWidget->item(row, 0)->text();
	int posDate = column - 1;
	QString result = ui->practicViewWidget->item(row, column)->text();
	
	int ocencId = ocenkList->indexOf(result);
	if(ocencId == -1)
		ocencId = 0;
	
	manager->addPracticResult(ui->practicGroupComboBox->currentText(), ui->practicDisciplinComboBox->currentText(), studentName, posDate, QString().setNum(ocencId));
}

void TeachReg::on_actionSave_triggered()
{
	manager->saveDB();
}

void TeachReg::on_actionAboutQt_triggered()
{
	qApp->aboutQt();
}

void TeachReg::on_actionCreate_triggered()
{
	on_createDbButton_clicked();
}

void TeachReg::on_actionOpen_triggered()
{
	on_selectDbFilePathButton_clicked();
	ui->pagesWidget->setCurrentIndex(0);
	ui->contentsWidget->setCurrentRow(0);
}

QString TeachReg::roundResult(float num, int pers)
{
	float ret = round(num * pow10(pers));
	QString tmp = QString().setNum(ret / pow10(pers), 'f', pers);
	return tmp;
}

void TeachReg::on_showResultButton_clicked()
{
	QList<float> totals;
	QStringList students;
	QMap<QString, QString> totalsForSort;
	QString totalText = htmlGenerator->htmlHead();
	int i = 0;
	float midTotal = 0.0;
	int quelTotal = 0;
	
	if(ui->setTypeComboBox->currentIndex() == 0)
	{
		totals = manager->getLectureTotals(ui->groupResultComboBox->currentText(), ui->disciplinResultComboBox->currentText());
		students = manager->getStudentsList(ui->groupResultComboBox->currentText());
		totalText += htmlGenerator->htmlHeader().arg(ui->disciplinResultComboBox->currentText()).arg(ui->groupResultComboBox->currentText()).arg(QString());
	}
	else
	{
		totals = manager->getPracticTotals(ui->groupResultComboBox->currentText(), ui->disciplinResultComboBox->currentText(), ui->subgroupResultComboBox->currentText().toInt());
		students = manager->getStudentsSubgroupList(ui->groupResultComboBox->currentText(), ui->subgroupResultComboBox->currentText().toInt());
		totalText += htmlGenerator->htmlHeader().arg(ui->disciplinResultComboBox->currentText()).arg(ui->groupResultComboBox->currentText()).arg(ui->subgroupResultComboBox->currentText());
	}
	
	for(i = 0; i < students.count(); i++)
	{
		totalsForSort.insert(students.at(i), roundResult(totals.at(i)));
	}
	
	students.sort();
	totalText += htmlGenerator->tableHead();
	for(i = 0; i < students.count(); i++)
	{
		totalText += htmlGenerator->tableRow().arg(students.at(i)).arg(totalsForSort.value(students.at(i)));
	}
	
	totals.clear();
	QMapIterator<QString, QString> iter(totalsForSort);
	while(iter.hasNext())
	{
		iter.next();
		totals.append(iter.value().toFloat());
	}
	for(i = 0; i < totals.count(); i++)
	{
		midTotal += totals.at(i);
		if(totals.at(i) < 4.0)
			quelTotal++;
	}
	midTotal /= float(totals.count());
	
	totalText += htmlGenerator->htmlEnd().arg(roundResult(midTotal, 1)).arg(roundResult((1 - float(quelTotal) / float(totals.count())) * 100.0, 1));
	
	ui->resultTextEdit->setHtml(totalText);
}

void TeachReg::on_setTypeComboBox_activated(int index)
{
	if(index == 1)
	{
		ui->subgroupResultComboBox->setVisible(true);
		setSubgroupsListTotals();
	}
	else
	{
		ui->subgroupResultComboBox->setVisible(false);
	}
}

void TeachReg::on_disciplinResultComboBox_activated(int index)
{
	setSubgroupsListTotals();
}

void TeachReg::on_groupResultComboBox_activated(int index)
{
	setSubgroupsListTotals();
}

void TeachReg::setSubgroupsListTotals()
{
	int countSubgroups = manager->getGroupData(ui->groupResultComboBox->currentText()).at(2).toInt();
	ui->subgroupResultComboBox->clear();
	for(int i = 0; i < countSubgroups; i++)
		ui->subgroupResultComboBox->addItem(QString().setNum(i + 1));
}

#include "teachreg.moc"
