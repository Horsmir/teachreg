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
#include <QtCore/QFile>
#include <QtGui/QComboBox>
#include "dbase/databaseteachreg.h"

const QString SEPAR = "|";

class TeachRegManager : public QObject
{
public:
	explicit TeachRegManager(QObject *parent = 0);
	virtual ~TeachRegManager();
	
	bool loadDB(const QString &filePath);
	QString getAboutDB() const;
	bool createDB(const QString &dbFilePath, const QString &aboutString);
	
	QStringList getDisciplinsList() const;
	void addDisciplin(const QString &disciplinName);
	bool editDisciplin(int oldDisciplinId, const QString &newDisciplinName);
	bool delDisciplin(int disciplinId);
	
	QStringList getGroupsNamesList() const;
	QList<QStringList> getGroupsList() const;
	QStringList getGroupData(int groupId) const;
	QStringList getStudentsList(int groupId, int subgroupId = 0) const;
	void addGroup(const QString &groupName);
	void editGroup(int oldGroupId, const QString &newGroupName);
	void editGroupNumSubgroups(int groupId, quint32 numSubgroup);
	void delGroup(int groupId);
	void changeSubgroup(int groupId, quint32 numSubgroup, const QString &studentName);
	void addStudentName(const QString &studentName, int groupId);
	void editStudentName(int groupId, const QString &oldStudentName, const QString &newStudentName);
	void delStudent(const QString &studentName);
	
	void addLecture(int groupId, int disciplinId, const QString &date);
	void addPractic(int groupId, int disciplinId, quint32 numSubgroup, const QString &date);
	
	QList<float> getLectureTotals(int groupId, int disciplinId) const;
	QList<float> getPracticTotals(int groupId, int disciplinId, int subgroupId) const;
	
	void setDisciplinView(QComboBox *dcb);
	void setGroupView(QComboBox *gcb);
	void setSubgroupView(QComboBox *scb, int groupId);
	
	DataBaseTeachReg *getDb();
	bool saveDb();
	
private:
	//DataBaseManager *dbManager;
	DataBaseTeachReg *db;
	QString dbFilePath;
	quint32 magicNumber;
};

#endif // TEACHREGMANAGER_H
