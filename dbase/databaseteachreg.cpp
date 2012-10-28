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


#include "databaseteachreg.h"

DataBaseTeachReg::DataBaseTeachReg(QObject *parent): QObject(parent), description(QString())
{
	createDate = QDate::currentDate();
}

DataBaseTeachReg::DataBaseTeachReg(const DataBaseTeachReg &db): QObject()
{
	createDate = db.createDate;
	description = db.description;
	disciplins = db.disciplins;
	groups = db.groups;
}

DataBaseTeachReg::~DataBaseTeachReg()
{

}

void DataBaseTeachReg::addGroup(Group group)
{
	groups.append(group);
}

void DataBaseTeachReg::addStudent(int groupId, Student student)
{
	Group *gr = getGroupPtr(groupId);
	gr->addStudent(student);
}

QDate DataBaseTeachReg::getCreateDate() const
{
	return createDate;
}

QString DataBaseTeachReg::getDescription() const
{
	return description;
}

Group DataBaseTeachReg::getGroup(int groupId) const
{
	Q_ASSERT(groupId >= 0 && groupId < groups.count());
	return groups.at(groupId);
}

Group DataBaseTeachReg::getGroup(const QString &groupName) const
{
	for(int i = 0; i < groups.count(); i++)
	{
		if(groups.at(i).getName() == groupName)
			return groups.at(i);
	}
	return Group();
}

Group *DataBaseTeachReg::getGroupPtr(int groupId)
{
	Q_ASSERT(groupId >= 0 && groupId < groups.count());
	return &(groups[groupId]);
}

Group *DataBaseTeachReg::getGroupPtr(const QString &groupName)
{
	int groupId = getGroupId(groupName);
	if(groupId != -1)
		return &(groups[groupId]);
	return 0;
}

Student DataBaseTeachReg::getStudent(int groupId, int studentId) const
{
	Q_ASSERT(groupId >= 0 && groupId < groups.count());
	
	Group gr = getGroup(groupId);
	Q_ASSERT(studentId >= 0 && studentId < gr.count());
	
	return gr.getStudent(studentId);
}

Student DataBaseTeachReg::getStudent(int groupId, const QString &studentName) const
{
	Q_ASSERT(groupId >= 0 && groupId < groups.count());
	
	Group gr = getGroup(groupId);
	return gr.getStudent(studentName);
}

Student DataBaseTeachReg::getStudent(const QString &groupName, const QString &studentName) const
{
	return getGroup(groupName).getStudent(studentName);
}

Student *DataBaseTeachReg::getStudentPtr(int groupId, const QString &studentName)
{
	Q_ASSERT(groupId >= 0 && groupId < groups.count());
	
	return groups[groupId].getStudentPtr(studentName);
}

void DataBaseTeachReg::setCreateDate(QDate createDate)
{
	this->createDate = createDate;
}

void DataBaseTeachReg::setDescription(const QString &description)
{
	this->description = description;
}

DataBaseTeachReg &DataBaseTeachReg::operator=(const DataBaseTeachReg &db)
{
	createDate = db.createDate;
	description = db.description;
	disciplins = db.disciplins;
	groups = db.groups;
	return *this;
}

QStringList DataBaseTeachReg::getDisciplinList() const
{
	return disciplins;
}

void DataBaseTeachReg::addDisciplin(const QString &disciplin)
{
	disciplins << disciplin;
}

int DataBaseTeachReg::countGroups() const
{
	return groups.count();
}

void DataBaseTeachReg::editDisciplin(int disciplinId, const QString &newDisciplinName)
{
	Q_ASSERT(disciplinId >= 0 && disciplinId < disciplins.count());
	disciplins[disciplinId] = newDisciplinName;
}

int DataBaseTeachReg::getDisciplinId(const QString &disciplin) const
{
	return disciplins.indexOf(disciplin);
}

int DataBaseTeachReg::getGroupId(const QString &groupName) const
{
	for(int i = 0; i < groups.count(); i++)
	{
		if(groups.at(i).getName() == groupName)
			return i;
	}
	return -1;
}

QList< Group > DataBaseTeachReg::getGroups() const
{
	return groups;
}

void DataBaseTeachReg::setDisciplins(QStringList disciplins)
{
	this->disciplins = disciplins;
}

void DataBaseTeachReg::setGroups(QList< Group > groups)
{
	this->groups = groups;
}

void DataBaseTeachReg::editLectureResult(int groupId, int disciplinId, const QString &studentName, int pos, const QString &newResult)
{
	int studentId = -1;
	for(int i = 0; i < groups.at(groupId).count(); i++)
	{
		if(groups.at(groupId).getStudent(i).getName() == studentName)
		{
			studentId = i;
			break;
		}
	}
	
	getGroupPtr(groupId)->repleceLectureResult(studentId, disciplinId, pos, newResult);
}

void DataBaseTeachReg::editPracticResult(int groupId, int disciplinId, const QString &studentName, int pos, const QString &newResult)
{
	int studentId = -1;
	for(int i = 0; i < groups.at(groupId).count(); i++)
	{
		if(groups.at(groupId).getStudent(i).getName() == studentName)
		{
			studentId = i;
			break;
		}
	}
	
	getGroupPtr(groupId)->replecepracticResult(studentId, disciplinId, pos, newResult);
}

void DataBaseTeachReg::delDisciplin(int disciplinId)
{
	//TODO
}

void DataBaseTeachReg::delGroup(int groupId)
{
	//TODO
}

void DataBaseTeachReg::delStudent(int groupId, int studentId)
{
	//TODO
}

//-----------------------------------------------------------------------------------
QDataStream &operator<<(QDataStream &out, const DataBaseTeachReg &db)
{
	out << db.getCreateDate() << db.getDescription() << db.getDisciplinList() << db.getGroups();
	return out;
}

QDataStream &operator>>(QDataStream &in, DataBaseTeachReg &db)
{
	QDate createDate;
	QString description;
	QStringList disciplins;
	QList<Group> groups;
	
	in >> createDate >> description >> disciplins >> groups;
	db.setCreateDate(createDate);
	db.setDescription(description);
	db.setDisciplins(disciplins);
	db.setGroups(groups);
	
	return in;
}

Q_DECLARE_METATYPE(DataBaseTeachReg);

#include "databaseteachreg.moc"