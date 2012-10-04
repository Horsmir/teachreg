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

DataBaseManager::DataBaseManager(QObject *parent): QObject(parent), db(0)
{
	dbFilePath = "default.dbtr";
	magicNumber = 0xAAFF452D;
}

DataBaseManager::~DataBaseManager()
{
}

void DataBaseManager::setDbFilePath(const QString &filePath)
{
	dbFilePath = filePath;
}

bool DataBaseManager::openDb(const QString &filePath)
{
	quint32 magic;
	quint16 streamVersion;
	
	if(db)
	{
		if(!saveDb())
			return false;
		delete db;
		db = 0;
	}
	dbFilePath = filePath;
	
	QFile dbFile(dbFilePath);
	if(!dbFile.exists())
	{
		qDebug() << "DataBase file " << dbFilePath << " no exist.";
		return false;
	}
	if(!dbFile.open(QIODevice::ReadOnly))
	{
		qDebug() << "Can not open file " << dbFilePath << " for reading. Error: " << dbFile.errorString();
		return false;
	}
	
	QDataStream in(&dbFile);
	in >> magic >> streamVersion;
	if(magic != magicNumber)
	{
		qDebug() << "ERROR: File is not recognized by this application";
		return false;
	}
	else if(streamVersion > in.version())
	{
		qDebug() << "ERROR: File is from a more recent version of the application";
		return false;
	}
	
	db = new DataBaseTeachReg(this);
	in >> *db;
	return true;
}

bool DataBaseManager::saveDb()
{
	QFile file(dbFilePath);
	
	if(!file.open(QIODevice::WriteOnly))
	{
		qDebug() << "ERROR: Cannot open file" << dbFilePath << " for writing: " << file.errorString();
		return false;
	}
	
	QDataStream out(&file);
	out << quint32(magicNumber) << quint16(out.version());
	out << *db;
	return true;
}

bool DataBaseManager::createDb(const QString &about)
{
	if(db)
	{
		if(!saveDb())
			return false;
		delete db;
		db = 0;
	}
	db = new DataBaseTeachReg(this);
	db->setDescription(about);
	return true;
}

QString DataBaseManager::getAboutDb() const
{
	return db->getDescription();
}

void DataBaseManager::addDisciplin(const QString &disciplinName)
{
	db->addDisciplin(disciplinName);
}

void DataBaseManager::addGroup(const QString &groupName)
{
	Group gr;
	gr.setParent(db);
	gr.setName(groupName);
	db->addGroup(gr);
}

void DataBaseManager::addStudent(const QString &studentName, int groupId, int subgroup)
{
	Student st;
	st.setName(studentName);
	st.setSubgroupId(subgroup);
	
	Group *gr = db->getGroupPtr(groupId);
	st.setParent(gr);
	gr->addStudent(st);
}

int DataBaseManager::getGroupIdByName(const QString &groupName) const
{
	return db->getGroupId(groupName);
}

int DataBaseManager::getDisciplinIdByName(const QString &disciplinName) const
{
	return db->getDisciplinId(disciplinName);
}

QStringList DataBaseManager::getDisciplinList() const
{
	return db->getDisciplinList();
}

QStringList DataBaseManager::getGroupNamesList() const
{
	QStringList ret;
	for(int i = 0; i < db->countGroups(); i++)
		ret << db->getGroup(i).getName();
	return ret;
}

QStringList DataBaseManager::getStudentList(int groupId, int numSubgroup) const
{
	QStringList ret;
	Group gr = db->getGroup(groupId);
	if(numSubgroup == 0)
		for(int i = 0; i < gr.count(); i++)
		{
			ret << gr.getStudent(i).getName();
		}
	else
		for(int i = 0; i < gr.count(); i++)
		{
			if(gr.getStudent(i).getSubgroupId() == numSubgroup)
				ret << gr.getStudent(i).getName();
		}
	
	return ret;
}

int DataBaseManager::getNumStudentsInGroup(int groupId) const
{
	return db->getGroup(groupId).count();
}

void DataBaseManager::editNumSubgroupInGroups(int groupId, int numSubgroups)
{
	Group *gr = db->getGroupPtr(groupId);
	gr->setNumSubgroups(numSubgroups);
}

void DataBaseManager::setNumSubgroupForStudent(int groupId, const QString &studentName, int numSubgroup)
{
	Group *gr = db->getGroupPtr(groupId);
	Student *st = gr->getStudentPtr(studentName);
	st->setSubgroupId(numSubgroup);
}

QStringList DataBaseManager::getLectureDates(int groupId, int disciplinId) const
{
	return db->getGroup(groupId).getLectureDate(disciplinId);
}

QStringList DataBaseManager::getPracticDates(int groupId, int disciplinId, int numSubgroup) const
{
	return db->getGroup(groupId).getPracticDate(disciplinId * 10 + numSubgroup);
}

QStringList DataBaseManager::getLectureResults(int groupId, int disciplinId, const QString &studentName) const
{
	return db->getGroup(groupId).getStudent(studentName).getLectureResults(disciplinId);
}

QStringList DataBaseManager::getPracticResults(int groupId, int disciplinId, const QString &studentName) const
{
	return db->getGroup(groupId).getStudent(studentName).getPracticResults(disciplinId);
}

void DataBaseManager::addLectureDate(int groupId, int disciplinId, const QString &date)
{
	Group *gr = db->getGroupPtr(groupId);
	gr->addLectureDate(disciplinId, date);
	for(int i = 0; i < gr->count(); i++)
		gr->getStudentPtr(i)->addLectureResult(disciplinId, "0");
}

void DataBaseManager::addPracticDate(int groupId, int disciplinId, int numSubgroup, const QString &date)
{
	Group *gr = db->getGroupPtr(groupId);
	gr->addPracticDate(disciplinId * 10 + numSubgroup, date);
	for(int i = 0; i < gr->count(); i++)
	{
		if(gr->getStudentPtr(i)->getSubgroupId() == numSubgroup)
			gr->getStudentPtr(i)->addPracticResult(disciplinId, "0");
	}
}

void DataBaseManager::addLectureResult(int groupId, int disciplinId, const QString &studentName, const QString &result)
{
	db->getGroupPtr(groupId)->getStudentPtr(studentName)->addLectureResult(disciplinId, result);
}

void DataBaseManager::addPracticResult(int groupId, int disciplinId, const QString &studentName, const QString &result)
{
	db->getGroupPtr(groupId)->getStudentPtr(studentName)->addPracticResult(disciplinId, result);
}

void DataBaseManager::editLectureDates(int groupId, int disciplinId, const QString &dateList)
{
	// TODO: 
}

void DataBaseManager::editPracticDates(int groupId, int disciplinId, int numSubgroup, const QString &dateList)
{
	// TODO
}

void DataBaseManager::editLectureResults(int groupId, int disciplinId, int pos, const QString &studentName, const QString &result)
{
	db->editLectureResult(groupId, disciplinId, studentName, pos, result);
}

void DataBaseManager::editPracticResults(int groupId, int disciplinId, int pos, const QString &studentName, const QString &result)
{
	db->editPracticResult(groupId, disciplinId, studentName, pos, result);
}

void DataBaseManager::delDisciplin(int disciplinId)
{
	// TODO
}

void DataBaseManager::delGroup(int groupId)
{
	// TODO
}

void DataBaseManager::delStudent(int studentId)
{
	// TODO
}

void DataBaseManager::editDisciplin(int disciplinId, const QString &newDisciplinName)
{
	db->editDisciplin(disciplinId, newDisciplinName);
}

void DataBaseManager::editGroup(const QString &groupName, const QString &newGroupName)
{
	db->getGroupPtr(groupName)->setName(newGroupName);
}

void DataBaseManager::editStudent(int groupId, const QString &oldStudentName, const QString &newStudentName)
{
	db->getGroupPtr(groupId)->getStudentPtr(oldStudentName)->setName(newStudentName);
}

QList< QStringList > DataBaseManager::getGroupsList() const
{
	QList<QStringList> ret;
	for(int i = 0; i < db->countGroups(); i++)
	{
		QStringList group;
		Group gr = db->getGroup(i);
		group << gr.getName() << QString().setNum(gr.count()) << QString().setNum(gr.getNumSubgroups());
		ret << group;
	}
	return ret;
}

QStringList DataBaseManager::getGroupData(int groupId) const
{
	QStringList ret;
	Group gr = db->getGroup(groupId);
	ret << gr.getName() << QString().setNum(gr.count()) << QString().setNum(gr.getNumSubgroups());
	return ret;
}

QList< float > DataBaseManager::getLectureTotals(int groupId, int disciplinId) const
{
	return db->getGroup(groupId).getLectureTotals(disciplinId);
}

QList< float > DataBaseManager::getPracticTotals(int groupId, int disciplinId, int subgroupId) const
{
	return db->getGroup(groupId).getPracticTotals(disciplinId, subgroupId);
}
