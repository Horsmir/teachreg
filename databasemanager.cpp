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


#include "databasemanager.h"

DataBaseManager::DataBaseManager(QObject *parent): QObject(parent)
{
	db = QSqlDatabase::addDatabase("QSQLITE");
	dbFilePath = ":memorry:";
	query = 0;
}

DataBaseManager::~DataBaseManager()
{
	delete query;
}

void DataBaseManager::setDbFilePath(const QString &filePath)
{
	dbFilePath = filePath;
}

bool DataBaseManager::openDb()
{
	db.setDatabaseName(dbFilePath);
	if(!db.open())
	{
		return false;
	}
	query = new QSqlQuery(db);
	return true;
}

void DataBaseManager::closeDb()
{
	db.close();
}

bool DataBaseManager::createDb(const QString &about)
{
	query->clear();
	query->exec("CREATE TABLE \"info\" (\"id\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \"about\" TEXT NOT NULL)");
	query->exec("CREATE TABLE \"disciplins\" (\"id\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \"name\" TEXT NOT NULL)");
	query->exec("CREATE TABLE \"groups\" (\"id\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \"name\" TEXT NOT NULL, \"num_students\" INTEGER DEFAULT (0), \"num_subgroups\" INTEGER DEFAULT (0))");
	query->exec("CREATE TABLE \"students\" (\"id\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \"name\" TEXT NOT NULL, \"group_id\" INTEGER NOT NULL, \"num_subgroup\" INTEGER DEFAULT (0))");
	query->exec("CREATE TABLE \"lectures_dates\" (\"id\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \"group_id\" INTEGER NOT NULL, \"disciplin_id\" INTEGER NOT NULL, \"date_list\" TEXT)");
	query->exec("CREATE TABLE \"practics_dates\" (\"id\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \"group_id\" INTEGER NOT NULL, \"num_subgroup\" INTEGER, \"disciplin_id\" INTEGER NOT NULL, \"date_list\" TEXT)");
	query->exec("CREATE TABLE \"lectures_results\" (\"id\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \"student_id\" INTEGER NOT NULL, \"group_id\" INTEGER NOT NULL, \"disciplin_id\" INTEGER NOT NULL, \"result_list\" TEXT)");
	query->exec("CREATE TABLE \"practics_results\" (\"id\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \"student_id\" INTEGER NOT NULL, \"group_id\" INTEGER NOT NULL, \"disciplin_id\" INTEGER NOT NULL, \"num_subgroup\" INTEGER NOT NULL, \"result_list\" TEXT)");
	query->exec("CREATE TABLE \"results\" (\"id\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \"student_id\" INTEGER NOT NULL, \"group_id\" INTEGER NOT NULL, \"disciplin_id\" INTEGER NOT NULL, \"lecture_result\" INTEGER, \"practic_result\" INTEGER, \"all_result\" TEXT)");
	
	query->exec("INSERT INTO info (about) VALUES('" + about + "')");
	
	return true;
}

QString DataBaseManager::getAboutDb() const
{
	query->exec("SELECT about FROM info");
	if(query->next())
		return query->value(0).toString();
	else
		return db.lastError().text();
}

void DataBaseManager::addDisciplin(const QString &disciplinName)
{
	query->exec("INSERT INTO disciplins(name) VALUES('" + disciplinName + "')" );
}

void DataBaseManager::addGroup(const QString &groupName)
{
	query->exec("INSERT INTO groups(name) VALUES('" + groupName + "')");
}

void DataBaseManager::addStudent(const QString &studentName, int groupId, int subgroup)
{
	query->exec(QString("INSERT INTO students(name, group_id, num_subgroup) VALUES('%1', %2, %3)").arg(studentName).arg(groupId).arg(subgroup));
	editNumStudentsInGroups(groupId, getNumStudentsInGroup(groupId) + 1);
}

int DataBaseManager::getGroupIdByName(const QString &groupName) const
{
	query->exec("SELECT id FROM groups WHERE name='" + groupName + "'");
	if(query->next())
		return query->value(0).toInt();
	else
		return -1;
}

int DataBaseManager::getDisciplinIdByName(const QString &disciplinName) const
{
	query->exec("SELECT id FROM disciplins WHERE name='" + disciplinName + "'");
	if(query->next())
		return query->value(0).toInt();
	else
		return -1;
}

int DataBaseManager::getStudentIdByName(const QString &studentName) const
{
	query->exec("SELECT id FROM students WHERE name='" + studentName + "'");
	if(query->next())
		return query->value(0).toInt();
	else
		return -1;
}

QStringList DataBaseManager::getDisciplinList() const
{
	QStringList ret;
	query->exec("SELECT name FROM disciplins");
	while(query->next())
		ret << query->value(0).toString();
	return ret;
}

QStringList DataBaseManager::getGroupNamesList() const
{
	QStringList ret;
	query->exec("SELECT name FROM groups");
	while(query->next())
		ret << query->value(0).toString();
	return ret;
}

QStringList DataBaseManager::getStudentList(int groupId, int numSubgroup) const
{
	QStringList ret;
	QString queryString = QString("SELECT name FROM students WHERE group_id=%1").arg(groupId);
	if(numSubgroup != 0)
		queryString += QString(" AND num_subgroup=%1").arg(numSubgroup);
	
	query->exec(queryString);
	while(query->next())
		ret << query->value(0).toString();
	return ret;
}

void DataBaseManager::editNumStudentsInGroups(int groupId, int numStudents)
{
	query->exec(QString("UPDATE groups SET num_students=%1 WHERE id=%2").arg(numStudents).arg(groupId));
}

int DataBaseManager::getNumStudentsInGroup(int groupId) const
{
	query->exec(QString("SELECT num_students FROM groups WHERE id=%1").arg(groupId));
	if(query->next())
		return query->value(0).toInt();
	return 0;
}

void DataBaseManager::editNumSubgroupInGroups(int groupId, int numSubgroups)
{
	query->exec(QString("UPDATE groups SET num_subgroups=%1 WHERE id=%2").arg(numSubgroups).arg(groupId));
}

void DataBaseManager::setNumSubgroupForStudent(int studentId, int numSubgroup)
{
	query->exec(QString("UPDATE students SET num_subgroup=%1 WHERE id=%2").arg(numSubgroup).arg(studentId));
}

QString DataBaseManager::getLectureDates(int groupId, int disciplinId) const
{
	query->exec(QString("SELECT date_list FROM lectures_dates WHERE group_id=%1 AND disciplin_id=%2").arg(groupId).arg(disciplinId));
	if(query->next())
		return query->value(0).toString();
	else
		return QString();
}

QString DataBaseManager::getPracticDates(int groupId, int disciplinId, int numSubgroup) const
{
	query->exec(QString("SELECT date_list FROM practics_dates WHERE group_id=%1 AND disciplin_id=%2 AND num_subgroup=%3").arg(groupId).arg(disciplinId).arg(numSubgroup));
	if(query->next())
		return query->value(0).toString();
	else
		return QString();
}

QString DataBaseManager::getLectureResults(int groupId, int disciplinId, int studentId) const
{
	query->exec(QString("SELECT result_list FROM lectures_results WHERE group_id=%1 AND disciplin_id=%2 AND student_id=%3").arg(groupId).arg(disciplinId).arg(studentId));
	if(query->next())
		return query->value(0).toString();
	else
		return QString();
}

QString DataBaseManager::getPracticResults(int groupId, int disciplinId, int numSubgroup, int studentId) const
{
	query->exec(QString("SELECT result_list FROM practics_results WHERE group_id=%1 AND disciplin_id=%2 AND num_subgroup=%3 AND student_id=%4").arg(groupId).arg(disciplinId).arg(numSubgroup).arg(studentId));
	if(query->next())
		return query->value(0).toString();
	else
		return QString();
}

void DataBaseManager::addLectureDates(int groupId, int disciplinId, const QString &dateList)
{
	query->exec(QString("INSERT INTO lectures_dates(group_id, disciplin_id, date_list) VALUES(%1, %2, '%3')").arg(groupId).arg(disciplinId).arg(dateList));
}

void DataBaseManager::addPracticDates(int groupId, int disciplinId, int numSubgroup, const QString &dateList)
{
	query->exec(QString("INSERT INTO practics_dates(group_id, disciplin_id, num_subgroup, date_list) VALUES(%1, %2, %3, '%4')").arg(groupId).arg(disciplinId).arg(numSubgroup).arg(dateList));
}

void DataBaseManager::addLectureResults(int groupId, int disciplinId, int studentId, const QString &resultList)
{
	query->exec(QString("INSERT INTO lectures_results(group_id, disciplin_id, result_list) VALUES(%1, %2, '%3')").arg(groupId).arg(disciplinId).arg(resultList));
}

void DataBaseManager::addPracticResults(int groupId, int disciplinId, int numSubgroup, int studentId, const QString &resultList)
{
	query->exec(QString("INSERT INTO practics_results(group_id, disciplin_id, num_subgroup, result_list) VALUES(%1, %2, %3, '%4')").arg(groupId).arg(disciplinId).arg(numSubgroup).arg(resultList));
}

void DataBaseManager::editLectureDates(int groupId, int disciplinId, const QString &dateList)
{
	query->exec(QString("UPDATE lectures_dates SET date_list='%1' WHERE group_id=%2 AND disciplin_id=%3").arg(dateList).arg(groupId).arg(disciplinId));
}

void DataBaseManager::editPracticDates(int groupId, int disciplinId, int numSubgroup, const QString &dateList)
{
	query->exec(QString("UPDATE practicd-dates SET date_list=%1 WHERE group_id=%2 AND disciplin_id=%3 AND num_subgroup=%4").arg(dateList).arg(groupId).arg(disciplinId).arg(numSubgroup));
}

void DataBaseManager::editLectureResults(int groupId, int disciplinId, int studentId, const QString &resultList)
{
	query->exec(QString("UPDATE lectures_results SET result_list=%1 WHERE group_id=%2 AND disciplin_id=%3 AND student_id=%4").arg(resultList).arg(groupId).arg(disciplinId).arg(studentId));
}

void DataBaseManager::editPracticResults(int groupId, int disciplinId, int numSubgroup, int studentId, const QString &resultList)
{
	query->exec(QString("UPDATE practics_results SET result_list=%1 WHERE group_id=%2 AND disciplin_id=%3 AND num_subgroup=%4 AND student_id=%5").arg(resultList).arg(groupId).arg(disciplinId).arg(numSubgroup).arg(studentId));
}

void DataBaseManager::delDisciplin(int disciplinId)
{
	query->exec(QString("DELETE FROM disciplins WHERE id=%1").arg(disciplinId));
}

void DataBaseManager::delGroup(int groupId)
{
	query->exec(QString("DELETE FROM groups WHERE id=%1").arg(groupId));
}

void DataBaseManager::delStudent(int studentId)
{
	int groupId = -1;
	query->exec(QString("SELECT group_id FROM students WHERE id=%1").arg(studentId));
	if(query->next())
	{
		groupId = query->value(0).toInt();
	}
	
	query->exec(QString("DELETE FROM students WHERE id=%1").arg(studentId));
	editNumStudentsInGroups(groupId, getNumStudentsInGroup(groupId) - 1);
}

void DataBaseManager::editDisciplin(int disciplinId, const QString &newDisciplinName)
{
	query->exec(QString("UPDATE disciplins SET name='%1' WHERE id=%2").arg(newDisciplinName).arg(disciplinId));
}

void DataBaseManager::editGroup(int groupId, const QString &newGroupName)
{
	query->exec(QString("UPDATE groups SET name='%1' WHERE id=%2").arg(newGroupName).arg(groupId));
}

void DataBaseManager::editStudent(int studentId, const QString &newStudentName)
{
	query->exec(QString("UPDATE students SET name='%1' WHERE id=%2").arg(newStudentName).arg(studentId));
}

QList< QStringList > DataBaseManager::getGroupsList() const
{
	QList<QStringList> ret;
	query->exec("SELECT name, num_students, num_subgroups FROM groups");
	
	while(query->next())
	{
		QStringList group;
		group << query->value(0).toString();
		group << QString().setNum(query->value(1).toInt());
		group << QString().setNum(query->value(2).toInt());
		ret << group;
	}
	return ret;
}

QStringList DataBaseManager::getGroupData(int groupId) const
{
	QStringList ret;
	query->exec(QString("SELECT * FROM groups WHERE id=%1").arg(groupId));
	
	if(query->next())
	{
		ret << QString().setNum(query->value(0).toInt());
		ret << query->value(1).toString();
		ret << QString().setNum(query->value(2).toInt());
		ret << QString().setNum(query->value(3).toInt());
	}
	return ret;
}
