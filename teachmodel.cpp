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


#include "teachmodel.h"

TeachModel::TeachModel(QObject *parent): QAbstractTableModel(parent)
{
	dBase = 0;
	groupId = -1;
	disciplinId = -1;
	subgroupId = -1;
	
	ocenkList.append("");
	ocenkList.append(trUtf8("нб"));
	ocenkList.append("2");
	ocenkList.append("3");
	ocenkList.append("4");
	ocenkList.append("5");
}

int TeachModel::columnCount(const QModelIndex &parent) const
{
	if(groupId == -1)
		return 0;
	int ret = 1;
	if(subgroupId == 0)
		ret += dBase->getGroupPtr(groupId)->getLectureDate(disciplinId).count();
	else
		ret += dBase->getGroupPtr(groupId)->getPracticDate(disciplinId * 10 + subgroupId).count();
	return ret;
}

int TeachModel::rowCount(const QModelIndex &parent) const
{
	if(groupId == -1)
		return 0;
	return studentsList.count();
}

void TeachModel::setDataForQuest(const quint32 disciplinId, const quint32 groupId, const quint32 subgroupId)
{
	this->disciplinId = disciplinId;
	this->groupId = groupId;
	this->subgroupId = subgroupId;
	createStudentsList();
	reset();
}

void TeachModel::setTeachDb(DataBaseTeachReg *db)
{
	dBase = db;
}

QVariant TeachModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid())
		return QVariant();
	
	if(role == Qt::TextAlignmentRole)
	{
		if(index.column() == 0)
			return int(Qt::AlignLeft | Qt::AlignVCenter);
		else
			return int(Qt::AlignHCenter | Qt::AlignVCenter);
	}
	else if(role == Qt::DisplayRole)
	{
		if(groupId == -1)
			return QVariant();
		if(index.column() == 0)
			return studentsList.at(index.row())->getName();
		if(subgroupId == 0)
			return ocenkList.at(studentsList.at(index.row())->getLectureResults(disciplinId).at(index.column() - 1).toInt());
		else
			return ocenkList.at(studentsList.at(index.row())->getPracticResults(disciplinId).at(index.column() - 1).toInt());
	}
	else if(role == Qt::UserRole)
	{
		return index.row();
	}
	else if(role == Qt::ForegroundRole)
	{
		QBrush brush;
		if(index.column() == 0)
			brush.setColor(color0);
		else
			brush.setColor(color1);
		return brush;
	}
	else if(role == Qt::FontRole)
	{
		if(index.column() == 0)
			return font0;
		else
			return font1;
	}
	return QVariant();
}

QVariant TeachModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		if(section == 0)
			return QObject::trUtf8("Фамилия Имя");
		else if(subgroupId == 0)
			return dBase->getGroupPtr(groupId)->getLectureDate(disciplinId).at(section - 1);
		else
			return dBase->getGroupPtr(groupId)->getPracticDate(disciplinId * 10 + subgroupId).at(section - 1);
	}
	return QAbstractItemModel::headerData(section, orientation, role);
}

Qt::ItemFlags TeachModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags flags = QAbstractItemModel::flags(index);
	if(index.column() != 0)
		flags |=  Qt::ItemIsEditable;
	return flags;
}

bool TeachModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(index.isValid() && role == Qt::EditRole)
	{
		if(subgroupId == 0)
			studentsList[index.data(Qt::UserRole).toInt()]->repleceLectureResult(disciplinId, index.column() - 1, QString().setNum(ocenkList.indexOf(value.toString())));
		else
			studentsList[index.data(Qt::UserRole).toInt()]->replecePracticResult(disciplinId, index.column() - 1, QString().setNum(ocenkList.indexOf(value.toString())));
		return true;
	}
	return false;
}

bool caseInsensitiveLessThan(Student *st1, Student *st2)
{
	return st1->getName().toLower() < st2->getName().toLower();
}

void TeachModel::createStudentsList()
{
	studentsList.clear();
	Group *gr = dBase->getGroupPtr(groupId);
	for(int i = 0; i < gr->count(); i++)
	{
		Student *st = gr->getStudentPtr(i);
		if(subgroupId == 0)
			studentsList << st;
		else if(st->getSubgroupId() == subgroupId)
			studentsList << st;
	}
	qSort(studentsList.begin(), studentsList.end(), caseInsensitiveLessThan);
}

void TeachModel::setDecor(const QFont &f0, const QFont &f1, const QColor c0, const QColor c1)
{
	font0 = f0;
	font1 = f1;
	color0 = c0;
	color1 = c1;
	reset();
}


#include "teachmodel.moc"