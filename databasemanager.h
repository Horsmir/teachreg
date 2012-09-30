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


#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QFile>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include "dbase/databaseteachreg.h"


class DataBaseManager : public QObject
{
public:
	explicit DataBaseManager(QObject *parent = 0);
	virtual ~DataBaseManager();
	
	void setDbFilePath(const QString &filePath);
	bool openDb(const QString &filePath);
	void closeDb();
	bool createDb(const QString &about);
	bool saveDb();
	QString getAboutDb() const;
	
	void addDisciplin(const QString &disciplinName);
	void addGroup(const QString &groupName);
	void addStudent(const QString &studentName, int groupId, int subgroup = 1);
	
	void editDisciplin(int disciplinId, const QString &newDisciplinName);
	void editGroup(const QString &groupName, const QString &newGroupName);
	void editStudent(int groupId, const QString &oldStudentName, const QString &newStudentName);
	
	void delDisciplin(int disciplinId);
	void delGroup(int groupId);
	void delStudent(int studentId);
	
	int getGroupIdByName(const QString &groupName) const;
	int getDisciplinIdByName(const QString &disciplinName) const;
	int getNumStudentsInGroup(int groupId) const;
	
	QStringList getDisciplinList() const;
	QStringList getGroupNamesList() const;
	QList<QStringList> getGroupsList() const;
	QStringList getGroupData(int groupId) const;
	QStringList getStudentList(int groupId, int numSubgroup = 0) const;
	
	void editNumSubgroupInGroups(int groupId, int numSubgroups);
	void setNumSubgroupForStudent(int groupId, const QString &studentName, int numSubgroup);
	
	QStringList getLectureDates(int groupId, int disciplinId) const;
	QStringList getPracticDates(int groupId, int disciplinId, int numSubgroup) const;
	QStringList getLectureResults(int groupId, int disciplinId, const QString &studentName) const;
	QStringList getPracticResults(int groupId, int disciplinId, const QString &studentName) const;
	
	void addLectureDate(int groupId, int disciplinId, const QString &date);
	void addPracticDate(int groupId, int disciplinId, int numSubgroup, const QString &date);
	void addLectureResult(int groupId, int disciplinId, const QString &studentName, const QString &result);
	void addPracticResult(int groupId, int disciplinId, const QString &studentName, const QString &result);
	
	void editLectureDates(int groupId, int disciplinId, const QString &dateList);
	void editPracticDates(int groupId, int disciplinId, int numSubgroup, const QString &dateList);
	void editLectureResults(int groupId, int disciplinId, int pos, const QString &studentName, const QString &result);
	void editPracticResults(int groupId, int disciplinId, int pos, const QString &studentName, const QString &result);
	
private:
	DataBaseTeachReg *db;
	QString dbFilePath;
	quint32 magicNumber;
};

#endif // DATABASEMANAGER_H
