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
}

StudentListDialog::~StudentListDialog()
{
	delete ui;
}

void StudentListDialog::on_subgroupsButton_clicked()
{
	subgroupDlg->show();
}

#include "studentlistdialog.moc"