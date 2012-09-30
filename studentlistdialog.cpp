/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  Роман Браун <firdragon76@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "studentlistdialog.h"

StudentListDialog::StudentListDialog(QWidget *parent, Qt::WindowFlags f): QDialog(parent, f), ui(new Ui::StudentListDlg)
{
	ui->setupUi(this);
	
	subgroupDlg = new SubgroupsDialog(this);
	manager = 0;
	groupName = "";
}

StudentListDialog::~StudentListDialog()
{
	delete ui;
}

void StudentListDialog::on_subgroupsButton_clicked()
{
	subgroupDlg->setGroupName(groupName);
	subgroupDlg->setTeachregManager(manager);
	subgroupDlg->show();
}

void StudentListDialog::setStudentList()
{
	QStringList studentList;
	
	if(manager)
	{
		studentList = manager->getStudentsList(groupName);
	
		ui->listStudentsWidget->clear();
		ui->listStudentsWidget->addItems(studentList);
		ui->listStudentsWidget->sortItems();
	}
}

void StudentListDialog::setManager(TeachRegManager *tmanager)
{
	manager = tmanager;
	setStudentList();
}

void StudentListDialog::setGroupName(const QString &groupName)
{
	this->groupName = groupName;
}

void StudentListDialog::on_addStudentButton_clicked()
{
	bool ok;
	
	QString text = QInputDialog::getText(this, trUtf8("Добавить студента"), trUtf8("Фамилия Имя:"), QLineEdit::Normal, QString(), &ok);
	if(ok && !text.isEmpty())
	{
		manager->addStudentName(text, groupName);
		setStudentList();
	}
}

void StudentListDialog::on_editStudentButton_clicked()
{
	if(!ui->listStudentsWidget->currentItem())
	{
		QMessageBox::information(this, trUtf8("Список студентов"), trUtf8("Не выбран студент.\nВыберите студента из списка и нажмите кнопку \"Изменить\"."), QMessageBox::Ok);
		return;
	}
	
	QString curStudentName = ui->listStudentsWidget->currentItem()->text();
	bool ok;
	
	QString text = QInputDialog::getText(this, trUtf8("Изменить студента"), trUtf8("Фамилия Имя:"), QLineEdit::Normal, curStudentName, &ok);
	if(ok && !text.isEmpty())
	{
		manager->editStudentName(groupName, curStudentName, text);
		setStudentList();
	}
}

void StudentListDialog::on_delStudentButton_clicked()
{
	if(!ui->listStudentsWidget->currentItem())
	{
		QMessageBox::information(this, trUtf8("Список студентов"), trUtf8("Не выбран студент.\nВыберите студента из списка и нажмите кнопку \"Удалить\"."), QMessageBox::Ok);
		return;
	}
	
	QString curStudentName = ui->listStudentsWidget->currentItem()->text();
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, trUtf8("Удаление студента"), QString(trUtf8("Удалить студента \"") + curStudentName + "\"?"), QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes)
	{
		manager->delStudent(curStudentName);
		setStudentList();
	}
}

#include "studentlistdialog.moc"