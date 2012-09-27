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


#include "subgroupsdialog.h"

SubgroupsDialog::SubgroupsDialog(QWidget *parent, Qt::WindowFlags f): QDialog(parent, f), ui(new Ui::SubgroupsDlg)
{
	ui->setupUi(this);
	
	listSubgroups[0] = ui->subgroupsWidget1;
	listSubgroups[1] = ui->subgroupsWidget2;
	listSubgroups[2] = ui->subgroupsWidget3;
	listSubgroups[3] = ui->subgroupsWidget4;
	
	subgroupLabels[0] = ui->label;
	subgroupLabels[1] = ui->label_2;
	subgroupLabels[2] = ui->label_3;
	subgroupLabels[3] = ui->label_4;
	
	clearAll();
}

SubgroupsDialog::~SubgroupsDialog()
{
	delete ui;
}

void SubgroupsDialog::closeEvent(QCloseEvent *event)
{
	clearAll();
	QDialog::closeEvent(event);
}

void SubgroupsDialog::setGroupName(const QString &groupName)
{
	this->groupName = groupName;
}

void SubgroupsDialog::setTeachregManager(TeachRegManager *manager)
{
	this->manager = manager;
	setSubgroups();
}

void SubgroupsDialog::setSubgroups()
{
	int numSubgroups = manager->getGroupData(groupName).at(3).toInt();
	if(!numSubgroups)
		ui->subgroupsWidget1->addItems(manager->getStudentsList(groupName));
	else
	{
		for(int i = 0; i < numSubgroups; i++)
		{
			listSubgroups[i]->addItems(manager->getStudentsSubgroupList(groupName, i + 1));
			listSubgroups[i]->setVisible(true);
			subgroupLabels[i]->setVisible(true);
		}
		curNamSubgroup = numSubgroups;
	}
}

void SubgroupsDialog::on_addSubgroupButton_clicked()
{
	if(curNamSubgroup >= MAX_NUM_SUBGROUPS)
	{
		QMessageBox::information(this, trUtf8("Подгруппы"), trUtf8("Установлено максимальное количество подгрупп."), QMessageBox::Ok);
		return;
	}
	
	listSubgroups[curNamSubgroup]->setVisible(true);
	subgroupLabels[curNamSubgroup]->setVisible(true);
	curNamSubgroup++;
}

void SubgroupsDialog::clearAll()
{
	listSubgroups[0]->clear();
	curNamSubgroup = 1;
	for(int i = 1; i < MAX_NUM_SUBGROUPS; i++)
	{
		listSubgroups[i]->clear();
		listSubgroups[i]->setVisible(false);
		subgroupLabels[i]->setVisible(false);
	}
}

void SubgroupsDialog::on_okButton_clicked()
{
	manager->editGroupNumSubgroups(groupName, curNamSubgroup);
	for(int i = 0; i < curNamSubgroup; i++)
	{
		for(int j = 0; j < listSubgroups[i]->count(); j++)
			manager->changeSubgroup(i + 1, listSubgroups[i]->item(j)->text());
	}
	close();
}

#include "subgroupsdialog.moc"