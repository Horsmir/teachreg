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
	return dbManager->openDb(dbFilePath);
}

QString TeachRegManager::getAboutDB() const
{
	return dbManager->getAboutDb();
}

bool TeachRegManager::createDB(const QString &dbFilePath, const QString &aboutString)
{
	if(!dbManager->createDb(aboutString))
		return false;
	dbManager->setDbFilePath(dbFilePath);
	return true;
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
}

void TeachRegManager::editGroup(const QString &oldGroupName, const QString &newGroupName)
{
	dbManager->editGroup(oldGroupName, newGroupName);
}

void TeachRegManager::editGroupNumSubgroups(const QString &groupName, quint32 numSubgroup)
{
	int groupId = dbManager->getGroupIdByName(groupName);
	dbManager->editNumSubgroupInGroups(groupId, numSubgroup);
}

void TeachRegManager::delGroup(const QString &oldGroupName)
{
	int groupId = dbManager->getGroupIdByName(oldGroupName);
	dbManager->delGroup(groupId);
}

void TeachRegManager::changeSubgroup(const QString &groupName, quint32 numSubgroup, const QString &studentName)
{
	int groupId = dbManager->getGroupIdByName(groupName);
	dbManager->setNumSubgroupForStudent(groupId, studentName, numSubgroup);
}

void TeachRegManager::addStudentName(const QString &studentName, const QString &groupName)
{
	int groupId = dbManager->getGroupIdByName(groupName);
	dbManager->addStudent(studentName, groupId);
}

void TeachRegManager::editStudentName(const QString &groupName, const QString &oldStudentName, const QString &newStudentName)
{
	int groupId = dbManager->getGroupIdByName(groupName);
	dbManager->editStudent(groupId, oldStudentName, newStudentName);
}

void TeachRegManager::delStudent(const QString &studentName)
{
	// TODO
}

QStringList TeachRegManager::getLecturesDateList(const QString &groupName, const QString &disciplinName) const
{
	int groupId = dbManager->getGroupIdByName(groupName);
	int disciplinId = dbManager->getDisciplinIdByName(disciplinName);
	return dbManager->getLectureDates(groupId, disciplinId);
}

QStringList TeachRegManager::getPracticsDateList(const QString &groupName, const QString &disciplinName, quint32 numSubgroup) const
{
	int groupId = dbManager->getGroupIdByName(groupName);
	int disciplinId = dbManager->getDisciplinIdByName(disciplinName);
	return dbManager->getPracticDates(groupId, disciplinId, numSubgroup);
}

void TeachRegManager::addLecture(const QString &groupName, const QString &disciplinName, const QString &date)
{
	int groupId = dbManager->getGroupIdByName(groupName);
	int disciplinId = dbManager->getDisciplinIdByName(disciplinName);
	dbManager->addLectureDate(groupId, disciplinId, date);
}

void TeachRegManager::addPractic(const QString &groupName, const QString &disciplinName, quint32 numSubgroup, const QString &date)
{
	int groupId = dbManager->getGroupIdByName(groupName);
	int disciplinId = dbManager->getDisciplinIdByName(disciplinName);
	dbManager->addPracticDate(groupId, disciplinId, numSubgroup, date);
}

QStringList TeachRegManager::getLecturesResultList(const QString &groupName, const QString &disciplinNam, const QString &studentName) const
{
	int groupId = dbManager->getGroupIdByName(groupName);
	int disciplinId = dbManager->getDisciplinIdByName(disciplinNam);
	return dbManager->getLectureResults(groupId, disciplinId, studentName);
}

QStringList TeachRegManager::getPracticsResultList(const QString &groupName, const QString &disciplinNam, quint32 numSubgroup, const QString &studentName) const
{
	int groupId = dbManager->getGroupIdByName(groupName);
	int disciplinId = dbManager->getDisciplinIdByName(disciplinNam);
	return dbManager->getPracticResults(groupId, disciplinId, studentName);
}

void TeachRegManager::addLectureResult(const QString &groupName, const QString &disciplinNam, const QString &studentName, int pos, const QString &result)
{
	int groupId = dbManager->getGroupIdByName(groupName);
	int disciplinId = dbManager->getDisciplinIdByName(disciplinNam);
	dbManager->editLectureResults(groupId, disciplinId, pos, studentName, result);
}

void TeachRegManager::addPracticResult(const QString &groupName, const QString &disciplinNam, const QString &studentName, int pos, const QString &result)
{
	int groupId = dbManager->getGroupIdByName(groupName);
	int disciplinId = dbManager->getDisciplinIdByName(disciplinNam);
	dbManager->editPracticResults(groupId, disciplinId, pos, studentName, result);
}

QStringList TeachRegManager::getGroupData(const QString &groupName) const
{
	int groupId = dbManager->getGroupIdByName(groupName);
	return dbManager->getGroupData(groupId);
}

bool TeachRegManager::saveDB()
{
	dbManager->saveDb();
}

QList< float > TeachRegManager::getLectureTotals(const QString &groupName, const QString &disciplinName) const
{
	int groupId = dbManager->getGroupIdByName(groupName);
	int disciplinId = dbManager->getDisciplinIdByName(disciplinName);
	return dbManager->getLectureTotals(groupId, disciplinId);
}

QList< float > TeachRegManager::getPracticTotals(const QString &groupName, const QString &disciplinName, int subgroupId) const
{
	int groupId = dbManager->getGroupIdByName(groupName);
	int disciplinId = dbManager->getDisciplinIdByName(disciplinName);
	return dbManager->getPracticTotals(groupId, disciplinId, subgroupId);
}
