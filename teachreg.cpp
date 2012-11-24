#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include "teachreg.h"

TeachReg::TeachReg(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	dbFileName = "";
	dbDateDir = "";
	
	dlgStudents = new StudentListDialog(this);
	dlgCreateDb = new CreateDbDialog(this);
	dlgAddDate = new AddDateDialog(this);
	dlgOptions = new OptionsDialog(this);
	
	manager = new TeachRegManager(this);
	
	settings = new QSettings( QSettings::NativeFormat, QSettings::UserScope, "TeachReg", QString(), this);
	readSettings();
	
	if(!dbFileName.isEmpty())
	{
		if(manager->loadDB(dbFileName))
		{
			ui->dbFilePathEdit->setText(dbFileName);
			ui->aboutDbTextEdit->setPlainText(manager->getAboutDB());
			ui->label_14->setVisible(false);
			setWindowTitle(appName + " - " + dbFileName);
		}
		else
		{
			QMessageBox::warning(this, trUtf8("Загрузка базы данных"), trUtf8("Не возможно загрузить файл ") + dbFileName);
			dbFileName.clear();
		}
	}
	
	createIcons();
	if(!dbFileName.isEmpty())
	{
		if(manager->getGroupsNamesList().isEmpty() || manager->getDisciplinsList().isEmpty())
		{
			for(int i = 3; i <= 5; i++)
			{
				Qt::ItemFlags flags =  ui->contentsWidget->item(i)->flags();
				ui->contentsWidget->item(i)->setFlags(flags ^ Qt::ItemIsEnabled);
			}
		}
	}
	else
	{
		for(int i = 1; i <= 5; i++)
		{
			Qt::ItemFlags flags =  ui->contentsWidget->item(i)->flags();
			ui->contentsWidget->item(i)->setFlags(flags ^ Qt::ItemIsEnabled);
		}
	}
	ui->subgroupResultComboBox->setVisible(false);
	ui->label_20->setVisible(false);
	
	onChangedLectureView = false;
	onChangedPracticView = false;
	
	htmlGenerator = new HtmlGenerator(this);
	
	lecPracModel = new TeachModel(this);
	lecPracModel->setTeachDb(manager->getDb());
	lecPracModel->setDecor(font0, font1, color0, color1);
	ui->lectureViewWidget->setModel(lecPracModel);
	ui->practicViewWidget->setModel(lecPracModel);
	
	ui->contentsWidget->setCurrentRow(currentPageIndex);
	ui->pagesWidget->setCurrentIndex(currentPageIndex);
}

TeachReg::~TeachReg()
{
	delete ui;
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
			manager->setDisciplinView(ui->disciplinComboBox);
			manager->setGroupView(ui->groupComboBox);
			showLecturesTable();
			break;
		case 4:
			ui->practicGroupComboBox->clear();
			ui->practicDisciplinComboBox->clear();
			manager->setDisciplinView(ui->practicDisciplinComboBox);
			manager->setGroupView(ui->practicGroupComboBox);
			on_practicGroupComboBox_activated(0);
			break;
		case 5:
			ui->groupResultComboBox->clear();
			ui->disciplinResultComboBox->clear();
			manager->setDisciplinView(ui->disciplinResultComboBox);
			manager->setGroupView(ui->groupResultComboBox);
			ui->groupResultComboBox->setCurrentIndex(0);
			ui->disciplinResultComboBox->setCurrentIndex(0);
			break;
		default:
			break;
	}
}

void TeachReg::on_studentListButton_clicked()
{
	dlgStudents->setGroupName(ui->viewGroupsWidget->currentItem()->data(Qt::UserRole).toInt());
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
		setWindowTitle(appName + " - " + dbFileName);
		lecPracModel->setTeachDb(manager->getDb());
		
		for(int i = 1; i <= 2; i++)
		{
			Qt::ItemFlags flags =  ui->contentsWidget->item(i)->flags();
			ui->contentsWidget->item(i)->setFlags(flags | Qt::ItemIsEnabled);
		}
	}
}

void TeachReg::writeSettings()
{
	settings->setValue("general/ApplicationName", appName);
	settings->setValue("general/CurrentDBFilePath", dbFileName);
	settings->setValue("general/CurrentDBDateDir", dbDateDir);
	settings->setValue("general/docDir", docDir);
	settings->setValue("view/LastPage", ui->pagesWidget->currentIndex());
	settings->setValue("view/Font0", font0);
	settings->setValue("view/Font1", font1);
	settings->setValue("view/Color0", color0);
	settings->setValue("view/Color1", color1);
	settings->setValue("view/Geometry", saveGeometry());
}

void TeachReg::readSettings()
{
	appName = settings->value("general/ApplicationName", "TeachReg").toString();
	dbFileName = settings->value("general/CurrentDBFilePath", QString()).toString();
	dbDateDir = settings->value("general/CurrentDBDateDir", QDir::homePath()).toString();
	docDir = settings->value("general/docDir", QApplication::applicationDirPath() + "/../doc").toString();
	currentPageIndex = settings->value("view/LastPage", 0).toInt();
	font0 = settings->value("view/Font0", font()).value<QFont>();
	font1 = settings->value("view/Font1", font()).value<QFont>();
	color0 = settings->value("view/Color0", palette().text().color()).value<QColor>();
	color1 = settings->value("view/Color1", palette().text().color()).value<QColor>();
	restoreGeometry(settings->value("view/Geometry", saveGeometry()).toByteArray());
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
		lecPracModel->setTeachDb(manager->getDb());
		for(int i = 1; i <= 2; i++)
		{
			Qt::ItemFlags flags =  ui->contentsWidget->item(i)->flags();
			ui->contentsWidget->item(i)->setFlags(flags | Qt::ItemIsEnabled);
		}
		if(!manager->getGroupsNamesList().isEmpty() && !manager->getDisciplinsList().isEmpty())
		{
			for(int i = 3; i <= 5; i++)
			{
				Qt::ItemFlags flags =  ui->contentsWidget->item(i)->flags();
				ui->contentsWidget->item(i)->setFlags(flags | Qt::ItemIsEnabled);
			}
		}
		else
		{
			for(int i = 3; i <= 5; i++)
			{
				Qt::ItemFlags flags =  ui->contentsWidget->item(i)->flags();
				ui->contentsWidget->item(i)->setFlags(flags ^ Qt::ItemIsEnabled);
			}
		}
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
		if(!manager->getGroupsNamesList().isEmpty())
		{
			for(int i = 3; i <= 5; i++)
			{
				Qt::ItemFlags flags =  ui->contentsWidget->item(i)->flags();
				ui->contentsWidget->item(i)->setFlags(flags | Qt::ItemIsEnabled);
			}
		}
	}
}

void TeachReg::on_editDisciplinButton_clicked()
{
	int curDisciplin = ui->viewDisciplinsWidget->currentItem()->data(Qt::UserRole).toInt();
	bool ok;
	
	QString text = QInputDialog::getText(this, trUtf8("Изменить дисциплину"),
												  trUtf8("Название:"), QLineEdit::Normal,
												  ui->viewDisciplinsWidget->currentItem()->text(), &ok);
	if (ok && !text.isEmpty())
	{
		manager->editDisciplin(curDisciplin, text);
		showDisciplinList();
	}
}

void TeachReg::on_delDisciplinButton_clicked()
{
	int curDisciplin = ui->viewDisciplinsWidget->currentItem()->data(Qt::UserRole).toInt();
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
	manager->saveDb();
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
		if(!manager->getDisciplinsList().isEmpty())
		{
			for(int i = 3; i <= 5; i++)
			{
				Qt::ItemFlags flags =  ui->contentsWidget->item(i)->flags();
				ui->contentsWidget->item(i)->setFlags(flags | Qt::ItemIsEnabled);
			}
		}
	}
}

void TeachReg::showGroupslinList()
{
	ui->viewGroupsWidget->clear();
	
	QStringList groups = manager->getGroupsNamesList();
	for(int i = 0; i < groups.count(); i++)
	{
		QListWidgetItem *item = new QListWidgetItem;
		item->setText(groups.at(i));
		item->setData(Qt::UserRole, i);
		ui->viewGroupsWidget->addItem(item);
	}
	
	ui->viewGroupsWidget->sortItems();
	ui->viewGroupsWidget->setCurrentItem(0);
}

void TeachReg::showGroupData(int groupId)
{
	QStringList groupData = manager->getGroupData(groupId);
	ui->groupNameEdit->setText(groupData.at(0));
	ui->labelNumStudents->setText(groupData.at(1));
	ui->labelNumSubGroups->setText(groupData.at(2));
}

void TeachReg::on_viewGroupsWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	if(!current)
		current = previous;
	
	int curGroupName = current->data(Qt::UserRole).toInt();
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
	manager->editGroup(ui->viewGroupsWidget->currentItem()->data(Qt::UserRole).toInt(), ui->groupNameEdit->text());
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
	
	int curGroupName = ui->viewGroupsWidget->currentItem()->data(Qt::UserRole).toInt();
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
	
	int curGroupName = ui->groupComboBox->itemData(ui->groupComboBox->currentIndex()).toInt();
	int curDisciplin = ui->disciplinComboBox->itemData(ui->disciplinComboBox->currentIndex()).toInt();
	
	lecPracModel->setDataForQuest(curDisciplin, curGroupName);
	ui->lectureViewWidget->sortByColumn(0, Qt::AscendingOrder);
	
	onChangedLectureView = true;
}

void TeachReg::on_addLectureButton_clicked()
{
	if(dlgAddDate->exec() == QDialog::Accepted)
	{
		manager->addLecture(ui->groupComboBox->itemData(ui->groupComboBox->currentIndex()).toInt(), ui->disciplinComboBox->itemData(ui->disciplinComboBox->currentIndex()).toInt(), dlgAddDate->getDate());
		showLecturesTable();
	}
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
	
	int curGroupName = ui->practicGroupComboBox->itemData(ui->practicGroupComboBox->currentIndex()).toInt();
	int curDisciplinName = ui->practicDisciplinComboBox->itemData(ui->practicDisciplinComboBox->currentIndex()).toInt();
	int curSubgroup = ui->subgroupComboBox->itemData(ui->subgroupComboBox->currentIndex()).toInt();
	
	lecPracModel->setDataForQuest(curDisciplinName, curGroupName, curSubgroup);
	
	onChangedPracticView = true;
}

void TeachReg::on_practicGroupComboBox_activated(int index)
{
	ui->subgroupComboBox->clear();
	manager->setSubgroupView(ui->subgroupComboBox, ui->practicGroupComboBox->itemData(index).toInt());
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
		manager->addPractic(ui->practicGroupComboBox->itemData(ui->practicGroupComboBox->currentIndex()).toInt(), ui->practicDisciplinComboBox->itemData(ui->practicDisciplinComboBox->currentIndex()).toInt(), ui->subgroupComboBox->currentText().toInt(), dlgAddDate->getDate());
		showPracticsTable();
	}
}

void TeachReg::on_actionSave_triggered()
{
	manager->saveDb();
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
		totals = manager->getLectureTotals(ui->groupResultComboBox->itemData(ui->groupResultComboBox->currentIndex()).toInt(), ui->disciplinResultComboBox->itemData(ui->disciplinResultComboBox->currentIndex()).toInt());
		students = manager->getStudentsList(ui->groupResultComboBox->itemData(ui->groupResultComboBox->currentIndex()).toInt());
		totalText += htmlGenerator->htmlHeader().arg(ui->disciplinResultComboBox->currentText()).arg(ui->groupResultComboBox->currentText()).arg(QString());
	}
	else
	{
		totals = manager->getPracticTotals(ui->groupResultComboBox->itemData(ui->groupResultComboBox->currentIndex()).toInt(), ui->disciplinResultComboBox->itemData(ui->disciplinResultComboBox->currentIndex()).toInt(), ui->subgroupResultComboBox->currentText().toInt());
		students = manager->getStudentsList(ui->groupResultComboBox->itemData(ui->groupResultComboBox->currentIndex()).toInt(), ui->subgroupResultComboBox->currentText().toInt());
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
	int countSubgroups = manager->getGroupData(ui->groupResultComboBox->itemData(ui->groupResultComboBox->currentIndex()).toInt()).at(2).toInt();
	ui->subgroupResultComboBox->clear();
	for(int i = 0; i < countSubgroups; i++)
		ui->subgroupResultComboBox->addItem(QString().setNum(i + 1));
}

void TeachReg::on_actionHelp_triggered()
{
	HelpBrowser::showPage(docDir, "index.html");
}

void TeachReg::on_actionOptions_triggered()
{
	dlgOptions->setColor0(color0);
	dlgOptions->setColor1(color1);
	dlgOptions->setFont0(font0);
	dlgOptions->setFont1(font1);
	if(dlgOptions->exec() == QDialog::Accepted)
	{
		color0 = dlgOptions->getColor0();
		color1 = dlgOptions->getColor1();
		font0 = dlgOptions->getFont0();
		font1 = dlgOptions->getFont1();
		lecPracModel->setDecor(font0, font1, color0, color1);
	}
}

void TeachReg::on_actionAbout_triggered()
{
	QString str1, str2, str3, str4;
	
	str1 = trUtf8("<h2>%1 %2</h2><p><b>%1</b> - предназначена для учёта успеваемости студентов на теоритических и практических занятиях по различным дисциплинам, а также вычисления и вывода итоговых результатов.</p><p>Copyright &copy;  2012 Роман Браун</p>").arg(appName).arg(VERSION);
	str2 = trUtf8("<p>Это программа является свободным программным обеспечением. Вы можете распространять и/или модифицировать её согласно условиям Стандартной Общественной Лицензии GNU, опубликованной Фондом Свободного Программного Обеспечения, версии 3 или, по Вашему желанию, любой более поздней версии.</p>");
	str3 = trUtf8("<p>Эта программа распространяется в надежде, что она будет полезной, но БЕЗ ВСЯКИХ ГАРАНТИЙ, в том числе подразумеваемых гарантий ТОВАРНОГО СОСТОЯНИЯ ПРИ ПРОДАЖЕ и ГОДНОСТИ ДЛЯ ОПРЕДЕЛЁННОГО ПРИМЕНЕНИЯ. Смотрите Стандартную Общественную Лицензию GNU для получения дополнительной информации.</p>");
	str4 = trUtf8("<p>Вы должны были получить копию Стандартной Общественной Лицензии GNU вместе с программой. В случае её отсутствия, посмотрите <a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses/</a>.</p><p>E-Mail: <a href=\"mailto:firdragon76@gmail.com\">firdragon76@gmail.com</a><br>Сайт программы: <a href=\"github.com/Horsmir/teachreg\">github.com/Horsmir/teachreg</a></p>");
	
	QMessageBox::about(this, trUtf8("О программе"), str1 + str2 + str3 + str4);
}

void TeachReg::on_printResultButton_clicked()
{
	QPrinter *printer = new QPrinter;
	QPrintDialog *printDialog = new QPrintDialog(printer, this);
	if(printDialog->exec() == QDialog::Accepted)
	{
		ui->resultTextEdit->print(printer);
	}
}

#include "teachreg.moc"
