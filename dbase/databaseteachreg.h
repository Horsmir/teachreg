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


#ifndef DATABASETEACHREG_H
#define DATABASETEACHREG_H

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/qmetatype.h>
#include "group.h"

class DataBaseTeachReg : public QObject
{
	Q_OBJECT
public:
	explicit DataBaseTeachReg(QObject *parent = 0);
	DataBaseTeachReg(const DataBaseTeachReg &db);
	virtual ~DataBaseTeachReg();
	
	void setCreateDate(QDate createDate);
	void setDescription(const QString &description);
	void setDisciplins(QStringList disciplins);
	void setGroups(QList<Group> groups);
	
	QDate getCreateDate() const;
	QString getDescription() const;
	Group getGroup(int groupId) const;
	Group *getGroupPtr(int groupId);
	Group *getGroupPtr(const QString &groupName);
	Group getGroup(const QString &groupName) const;
	int getGroupId(const QString &groupName) const;
	Student getStudent(int groupId, int studentId) const;
	Student getStudent(int groupId, const QString &studentName) const;
	Student getStudent(const QString &groupName, const QString &studentName) const;
	Student *getStudentPtr(int groupId, const QString &studentName);
	QStringList getDisciplinList() const;
	int getDisciplinId(const QString &disciplin) const;
	QList<Group> getGroups() const;
	
	void addGroup(Group group);
	void addStudent(int groupId, Student student);
	void addDisciplin(const QString &disciplin);
	
	int countGroups() const;
	
	void editDisciplin(int disciplinId, const QString &newDisciplinName);
	void editLectureResult(int groupId, int disciplinId, const QString &studentName, int pos, const QString &newResult);
	void editPracticResult(int groupId, int disciplinId, const QString &studentName, int pos, const QString &newResult);
	
	void delDisciplin(int disciplinId);
	void delGroup(int groupId);
	void delStudent(int groupId, int studentId);
	
	DataBaseTeachReg &operator=(const DataBaseTeachReg &db);
	
private:
	QDate createDate;
	QString description;
	QStringList disciplins;
	QList<Group> groups;
};

QDataStream &operator<<(QDataStream &out, const DataBaseTeachReg &db);
QDataStream &operator>>(QDataStream &in, DataBaseTeachReg &db);

#endif // DATABASETEACHREG_H
