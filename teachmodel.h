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


#ifndef TEACHMODEL_H
#define TEACHMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtGui/QBrush>
#include <QtGui/QFont>
#include "dbase/databaseteachreg.h"

class TeachModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	explicit TeachModel(QObject *parent = 0);
	
	void setTeachDb(DataBaseTeachReg *db);
	void setDataForQuest(const quint32 disciplinId, const quint32 groupId, const quint32 subgroupId = 0);
	void setDecor(const QFont &f0, const QFont &f1, const QColor c0, const QColor c1);
	
	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	bool setData(const QModelIndex &index, const QVariant &value,int role);
	QVariant headerData(int section, Qt::Orientation orientation,int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	
	void createStudentsList();
	
private:
	DataBaseTeachReg *dBase;
	quint32 disciplinId;
	quint32 groupId;
	quint32 subgroupId;
	QStringList ocenkList;
	QList<Student*> studentsList;
	QFont font0;
	QFont font1;
	QColor color0;
	QColor color1;
	
	//bool caseInsensitiveLessThan(Student *st1, Student *st2);
};

#endif // TEACHMODEL_H
