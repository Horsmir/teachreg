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


#include "teachregmanager.h"

TeachRegManager::TeachRegManager(QObject *parent): QObject(parent)
{
	dbManager = new DataBaseManager(this);
}

TeachRegManager::~TeachRegManager()
{

}

bool TeachRegManager::loadDB(const QString &dbFilePath)
{
	dbManager->setDbFilePath(dbFilePath);
	return dbManager->openDb();
}

QString TeachRegManager::getAboutDB() const
{
	return dbManager->getAboutDb();
}

bool TeachRegManager::createDB(const QString &dbFilePath, const QString &aboutString)
{
	dbManager->setDbFilePath(dbFilePath);
	if(dbManager->openDb())
		return dbManager->createDb(aboutString);
	else
		return false;
}

QStringList TeachRegManager::getDisciplinsList() const
{
	return dbManager->getDisciplinList();
}

void TeachRegManager::addDisciplin(const QString &disciplinName)
{
	dbManager->addDisciplin(disciplinName);
}

bool TeachRegManager::editDisciplin(const QString &oldDisciplinName, const QString &newDisciplinName)
{
	int disciplinId = dbManager->getDisciplinIdByName(oldDisciplinName);
	dbManager->editDisciplin(disciplinId, newDisciplinName);
	return true;
}

bool TeachRegManager::delDisciplin(const QString &disciplinName)
{
	int disciplinId = dbManager->getDisciplinIdByName(disciplinName);
	dbManager->delDisciplin(disciplinId);
}

QList< QStringList > TeachRegManager::getGroupsList() const
{
	return dbManager->getGroupsList();
}

QStringList TeachRegManager::getGroupsNamesList() const
{
	return dbManager->getGroupNamesList();
}

QStringList TeachRegManager::getStudentsList(const QString &groupName) const
{
	int groupId = dbManager->getGroupIdByName(groupName);
	return dbManager->getStudentList(groupId);
}

QStringList TeachRegManager::getStudentsSubgroupList(const QString &groupName, quint32 numSubgroup) const
{
	int groupId = dbManager->getGroupIdByName(groupName);
	return dbManager->getStudentList(groupId, numSubgroup);
}

void TeachRegManager::addGroup(const QString &groupName)
{
	dbManager->addGroup(groupName);
	int g = dbManager->getGroupIdByName(groupName);
	QStringList disciplins = dbManager->getDisciplinList();
	for(int i = 0; i < disciplins.count(); i++)
	{
		int d = dbManager->getDisciplinIdByName(disciplins.at(i));
		dbManager->addLectureDates(g, d, QString());
	}
}

void TeachRegManager::editGroup(const QString &oldGroupName, const QString &newGroupName)
{
	int groupId = dbManager->getGroupIdByName(oldGroupName);
	dbManager->editGroup(groupId, newGroupName);
}

void TeachRegManager::editGroupNumStudents(const QString &groupName, quint32 numStudents)
{
	int groupId = dbManager->getGroupIdByName(groupName);
	dbManager->editNumStudentsInGroups(groupId, numStudents);
}

void TeachRegManager::editGroupNumSubgroups(const QString &groupName, quint32 numSubgroup)
{
	int groupId = dbManager->getGroupIdByName(groupName);
	
	quint32 oldNumSubgroups = dbManager->getGroupData(groupId).at(3).toInt();
	
	QStringList disciplins = dbManager->getDisciplinList();
	
	dbManager->editNumSubgroupInGroups(groupId, numSubgroup);
	
	for(quint32 i = oldNumSubgroups + 1; i <= numSubgroup; i++)
	{
		for(int j = 0; j < disciplins.count(); j++)
		{
			int d = dbManager->getDisciplinIdByName(disciplins.at(i));
			dbManager->addPracticDates(groupId, d, i, QString());
		}
	}
}

void TeachRegManager::delGroup(const QString &oldGroupName)
{
	int groupId = dbManager->getGroupIdByName(oldGroupName);
	dbManager->delGroup(groupId);
}

void TeachRegManager::changeSubgroup(quint32 numSubgroup, const QString &studentName)
{
	int studentId = dbManager->getStudentIdByName(studentName);
	dbManager->setNumSubgroupForStudent(studentId, numSubgroup);
}

void TeachRegManager::addStudentName(const QString &studentName, const QString &groupName)
{
	int groupId = dbManager->getGroupIdByName(groupName);
	dbManager->addStudent(studentName, groupId);
}

void TeachRegManager::editStudentName(const QString &oldStudentName, const QString &newStudentName)
{
	int studentId = dbManager->getStudentIdByName(oldStudentName);
	dbManager->editStudent(studentId, newStudentName);
}

void TeachRegManager::delStudent(const QString &studentName)
{
	int studentId = dbManager->getStudentIdByName(studentName);
	dbManager->delStudent(studentId);
}

QStringList TeachRegManager::getLecturesDateList(const QString &groupName, const QString &disciplinName) const
{
	int groupId = dbManager->getGroupIdByName(groupName);
	int disciplinId = dbManager->getDisciplinIdByName(disciplinName);
	return dbManager->getLectureDates(groupId, disciplinId).split(SEPAR);
}

QStringList TeachRegManager::getPracticsDateList(const QString &groupName, const QString &disciplinName, quint32 numSubgroup) const
{
	int groupId = dbManager->getGroupIdByName(groupName);
	int disciplinId = dbManager->getDisciplinIdByName(disciplinName);
	return dbManager->getPracticDates(groupId, disciplinId, numSubgroup).split(SEPAR);
}

void TeachRegManager::addLecture(const QString &groupName, const QString &disciplinName, const QString &date)
{
	int groupId = dbManager->getGroupIdByName(groupName);
	int disciplinId = dbManager->getDisciplinIdByName(disciplinName);
	QString dateList = dbManager->getLectureDates(groupId, disciplinId);
	if(!dateList.isEmpty())
		dateList += SEPAR;
	dateList += date;
	dbManager->editLectureDates(groupId, disciplinId, dateList);
	
	QStringList students = dbManager->getStudentList(groupId);
	for(int i = 0; i < students.count(); i++)
	{
		int studentId = dbManager->getStudentIdByName(students.at(i));
		QString ocenks = dbManager->getLectureResults(groupId, disciplinId, studentId);
		if(!ocenks.isEmpty())
			ocenks += SEPAR + "0";
		else
			ocenks = "0";
		dbManager->editLectureResults(groupId, disciplinId, studentId, ocenks);
	}
}

void TeachRegManager::addPractic(const QString &groupName, const QString &disciplinName, quint32 numSubgroup, const QString &date)
{
	int groupId = dbManager->getGroupIdByName(groupName);
	int disciplinId = dbManager->getDisciplinIdByName(disciplinName);
	QString dateList = dbManager->getPracticDates(groupId, disciplinId, numSubgroup);
	if(!dateList.isEmpty())
		dateList += SEPAR;
	dateList += date;
	dbManager->editPracticDates(groupId, disciplinId, numSubgroup, dateList);
}

QStringList TeachRegManager::getLecturesResultList(const QString &groupName, const QString &disciplinNam, const QString &studentName) const
{
	int groupId = dbManager->getGroupIdByName(groupName);
	int disciplinId = dbManager->getDisciplinIdByName(disciplinNam);
	int studentId = dbManager->getStudentIdByName(studentName);
	return dbManager->getLectureResults(groupId, disciplinId, studentId).split(SEPAR);
}

QStringList TeachRegManager::getPracticsResultList(const QString &groupName, const QString &disciplinNam, quint32 numSubgroup, const QString &studentName) const
{
	int groupId = dbManager->getGroupIdByName(groupName);
	int disciplinId = dbManager->getDisciplinIdByName(disciplinNam);
	int studentId = dbManager->getStudentIdByName(studentName);
	return dbManager->getPracticResults(groupId, disciplinId, numSubgroup, studentId).split(SEPAR);
}

void TeachRegManager::addLectureResult(const QString &groupName, const QString &disciplinNam, const QString &studentName, const QString &date, const QString &result)
{
	int groupId = dbManager->getGroupIdByName(groupName);
	int disciplinId = dbManager->getDisciplinIdByName(disciplinNam);
	int studentId = dbManager->getStudentIdByName(studentName);
	
	QStringList dateList = dbManager->getLectureDates(groupId, disciplinId).split(SEPAR);
	int dateId = dateList.indexOf(date);
	
	QStringList resultList = dbManager->getLectureResults(groupId, disciplinId, studentId).split(SEPAR);
	resultList[dateId] = result;
	
	dbManager->editLectureResults(groupId, disciplinId, studentId, resultList.join(SEPAR));
}

void TeachRegManager::addPracticResult(const QString &groupName, const QString &disciplinNam, const QString &studentName, const QString &date, quint32 numSubgroup, const QString &result)
{
	int groupId = dbManager->getGroupIdByName(groupName);
	int disciplinId = dbManager->getDisciplinIdByName(disciplinNam);
	int studentId = dbManager->getStudentIdByName(studentName);
	
	QStringList dateList = dbManager->getPracticDates(groupId, disciplinId, numSubgroup).split(SEPAR);
	int dateId = dateList.indexOf(date);
	
	QStringList resultList = dbManager->getPracticResults(groupId, disciplinId, numSubgroup, studentId).split(SEPAR);
	resultList[dateId] = result;
	
	dbManager->editPracticResults(groupId, disciplinId, numSubgroup, studentId, resultList.join(SEPAR));
}

QStringList TeachRegManager::getGroupData(const QString &groupName) const
{
	int groupId = dbManager->getGroupIdByName(groupName);
	return dbManager->getGroupData(groupId);
}
