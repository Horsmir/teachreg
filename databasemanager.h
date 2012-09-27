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

#include <QtCore/QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>


class DataBaseManager : public QObject
{
public:
	explicit DataBaseManager(QObject *parent = 0);
	virtual ~DataBaseManager();
	
	void setDbFilePath(const QString &filePath);
	bool openDb();
	void closeDb();
	bool createDb(const QString &about);
	QString getAboutDb() const;
	
	void addDisciplin(const QString &disciplinName);
	void addGroup(const QString &groupName);
	void addStudent(const QString &studentName, int groupId, int subgroup = 1);
	
	void editDisciplin(int disciplinId, const QString &newDisciplinName);
	void editGroup(int groupId, const QString &newGroupName);
	void editStudent(int studentId, const QString &newStudentName);
	
	void delDisciplin(int disciplinId);
	void delGroup(int groupId);
	void delStudent(int studentId);
	
	int getGroupIdByName(const QString &groupName) const;
	int getDisciplinIdByName(const QString &disciplinName) const;
	int getStudentIdByName(const QString &studentName) const;
	int getNumStudentsInGroup(int groupId) const;
	
	QStringList getDisciplinList() const;
	QStringList getGroupNamesList() const;
	QList<QStringList> getGroupsList() const;
	QStringList getGroupData(int groupId) const;
	QStringList getStudentList(int groupId, int numSubgroup = 0) const;
	
	void editNumStudentsInGroups(int groupId, int numStudents);
	void editNumSubgroupInGroups(int groupId, int numSubgroups);
	void setNumSubgroupForStudent(int studentId, int numSubgroup);
	
	QString getLectureDates(int groupId, int disciplinId) const;
	QString getPracticDates(int groupId, int disciplinId, int numSubgroup) const;
	QString getLectureResults(int groupId, int disciplinId, int studentId) const;
	QString getPracticResults(int groupId, int disciplinId, int numSubgroup, int studentId) const;
	
	void addLectureDates(int groupId, int disciplinId, const QString &dateList);
	void addPracticDates(int groupId, int disciplinId, int numSubgroup, const QString &dateList);
	void addLectureResults(int groupId, int disciplinId, int studentId, const QString &resultList);
	void addPracticResults(int groupId, int disciplinId, int numSubgroup, int studentId, const QString &resultList);
	
	void editLectureDates(int groupId, int disciplinId, const QString &dateList);
	void editPracticDates(int groupId, int disciplinId, int numSubgroup, const QString &dateList);
	void editLectureResults(int groupId, int disciplinId, int studentId, const QString &resultList);
	void editPracticResults(int groupId, int disciplinId, int numSubgroup, int studentId, const QString &resultList);
	
private:
	QSqlDatabase db;
	QString dbFilePath;
	QSqlQuery *query;
};

#endif // DATABASEMANAGER_H
