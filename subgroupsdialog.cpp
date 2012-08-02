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
	
	ui->label->setVisible(false);
	ui->label_2->setVisible(false);
	ui->label_3->setVisible(false);
	ui->label_4->setVisible(false);
	ui->subgroupsWidget1->setVisible(false);
	ui->subgroupsWidget2->setVisible(false);
	ui->subgroupsWidget3->setVisible(false);
	ui->subgroupsWidget4->setVisible(false);
}

SubgroupsDialog::~SubgroupsDialog()
{
	delete ui;
}

void SubgroupsDialog::on_subgroupWidget1_changed()
{
	ui->subgroupsWidget1->sortItems();
}

#include "subgroupsdialog.moc"