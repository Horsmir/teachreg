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


#ifndef SUBGROUPSDIALOG_H
#define SUBGROUPSDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QCloseEvent>
#include "ui_subgroupsdlg.h"
#include "teachregmanager.h"

const quint32 MAX_NUM_SUBGROUPS = 4;

namespace Ui
{
	class SubgroupsDlg;
}

class SubgroupsDialog : public QDialog
{
	Q_OBJECT
public:
	explicit SubgroupsDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);
	virtual ~SubgroupsDialog();
	
	void setTeachregManager(TeachRegManager *manager);
	void setGroupName(int groupId);
	
protected:
	void closeEvent(QCloseEvent *event);
	
private slots:
	void on_addSubgroupButton_clicked();
	void on_okButton_clicked();
	
private:
	Ui::SubgroupsDlg *ui;
	TeachRegManager *manager;
	int groupId;
	QListWidget *listSubgroups[MAX_NUM_SUBGROUPS];
	QLabel *subgroupLabels[MAX_NUM_SUBGROUPS];
	quint32 curNamSubgroup;
	
	void setSubgroups();
	void clearAll();
};

#endif // SUBGROUPSDIALOG_H
