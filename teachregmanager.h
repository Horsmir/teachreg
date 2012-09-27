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


#ifndef TEACHREGMANAGER_H
#define TEACHREGMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include "databasemanager.h"

const QString SEPAR = "|";

class TeachRegManager : public QObject
{
public:
	explicit TeachRegManager(QObject *parent = 0);
	virtual ~TeachRegManager();
	
	bool loadDB(const QString &dbFilePath);
	QString getAboutDB() const;
	bool createDB(const QString &dbFilePath, const QString &aboutString);
	
	QStringList getDisciplinsList() const;
	void addDisciplin(const QString &disciplinName);
	bool editDisciplin(const QString &oldDisciplinName, const QString &newDisciplinName);
	bool delDisciplin(const QString &disciplinName);
	
	QStringList getGroupsNamesList() const;
	QList<QStringList> getGroupsList() const;
	QStringList getGroupData(const QString &groupName) const;
	QStringList getStudentsList(const QString &groupName) const;
	QStringList getStudentsSubgroupList(const QString &groupName, quint32 numSubgroup) const;
	void addGroup(const QString &groupName);
	void editGroup(const QString &oldGroupName, const QString &newGroupName);
	void editGroupNumStudents(const QString &groupName, quint32 numStudents);
	void editGroupNumSubgroups(const QString &groupName, quint32 numSubgroup);
	void delGroup(const QString &oldGroupName);
	void changeSubgroup(quint32 numSubgroup, const QString &studentName);
	void addStudentName(const QString &studentName, const QString &groupName);
	void editStudentName(const QString &oldStudentName, const QString &newStudentName);
	void delStudent(const QString &studentName);
	
	QStringList getLecturesDateList(const QString &groupName, const QString &disciplinName) const;
	QStringList getPracticsDateList(const QString &groupName, const QString &disciplinName, quint32 numSubgroup) const;
	void addLecture(const QString &groupName, const QString &disciplinName, const QString &date);
	void addPractic(const QString &groupName, const QString &disciplinName, quint32 numSubgroup, const QString &date);
	
	QStringList getLecturesResultList(const QString &groupName, const QString &disciplinNam, const QString &studentName) const;
	QStringList getPracticsResultList(const QString &groupName, const QString &disciplinNam, quint32 numSubgroup, const QString &studentName) const;
	void addLectureResult(const QString &groupName, const QString &disciplinNam, const QString &studentName, const QString &date, const QString &result);
	void addPracticResult(const QString &groupName, const QString &disciplinNam, const QString &studentName, const QString &date, quint32 numSubgroup, const QString &result);
	
private:
	DataBaseManager *dbManager;
};

#endif // TEACHREGMANAGER_H
