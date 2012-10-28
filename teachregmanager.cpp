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
	dbFilePath = "default.dbtr";
	magicNumber = 0xAAFF452D;
}

TeachRegManager::~TeachRegManager()
{

}

bool TeachRegManager::loadDB(const QString &filePath)
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

QString TeachRegManager::getAboutDB() const
{
	return db->getDescription();
}

bool TeachRegManager::createDB(const QString &dbFilePath, const QString &aboutString)
{
	this->dbFilePath = dbFilePath;
	if(db)
	{
		if(!saveDb())
			return false;
		delete db;
		db = 0;
	}
	db = new DataBaseTeachReg(this);
	db->setDescription(aboutString);
	return true;
}

QStringList TeachRegManager::getDisciplinsList() const
{
	return db->getDisciplinList();
}

void TeachRegManager::addDisciplin(const QString &disciplinName)
{
	db->addDisciplin(disciplinName);
}

bool TeachRegManager::editDisciplin(int oldDisciplinId, const QString &newDisciplinName)
{
	db->editDisciplin(oldDisciplinId, newDisciplinName);
	return true;
}

bool TeachRegManager::delDisciplin(int disciplinId)
{
	db->delDisciplin(disciplinId);
}

QList< QStringList > TeachRegManager::getGroupsList() const
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

QStringList TeachRegManager::getGroupsNamesList() const
{
	QStringList ret;
	for(int i = 0; i < db->countGroups(); i++)
		ret << db->getGroup(i).getName();
	return ret;
}

QStringList TeachRegManager::getStudentsList(int groupId, int subgroupId) const
{
	QStringList ret;
	Group gr = db->getGroup(groupId);
	if(subgroupId == 0)
		for(int i = 0; i < gr.count(); i++)
		{
			ret << gr.getStudent(i).getName();
		}
		else
			for(int i = 0; i < gr.count(); i++)
			{
				if(gr.getStudent(i).getSubgroupId() == subgroupId)
					ret << gr.getStudent(i).getName();
			}
			
			return ret;
}

void TeachRegManager::addGroup(const QString &groupName)
{
	Group gr;
	gr.setParent(db);
	gr.setName(groupName);
	db->addGroup(gr);
}

void TeachRegManager::editGroup(int oldGroupId, const QString &newGroupName)
{
	db->getGroupPtr(oldGroupId)->setName(newGroupName);
}

void TeachRegManager::editGroupNumSubgroups(int groupId, quint32 numSubgroup)
{
	db->getGroupPtr(groupId)->setNumSubgroups(numSubgroup);
}

void TeachRegManager::delGroup(int groupId)
{
	db->delGroup(groupId);
}

void TeachRegManager::changeSubgroup(int groupId, quint32 numSubgroup, const QString &studentName)
{
	db->getGroupPtr(groupId)->getStudentPtr(studentName)->setSubgroupId(numSubgroup);
}

void TeachRegManager::addStudentName(const QString &studentName, int groupId)
{
	Student st;
	st.setName(studentName);
	st.setSubgroupId(1);
	
	Group *gr = db->getGroupPtr(groupId);
	st.setParent(gr);
	gr->addStudent(st);
}

void TeachRegManager::editStudentName(int groupId, const QString &oldStudentName, const QString &newStudentName)
{
	db->getGroupPtr(groupId)->getStudentPtr(oldStudentName)->setName(newStudentName);
}

void TeachRegManager::delStudent(const QString &studentName)
{
	// TODO
}

void TeachRegManager::addLecture(int groupId, int disciplinId, const QString &date)
{
	Group *gr = db->getGroupPtr(groupId);
	gr->addLectureDate(disciplinId, date);
	for(int i = 0; i < gr->count(); i++)
		gr->getStudentPtr(i)->addLectureResult(disciplinId, "0");
}

void TeachRegManager::addPractic(int groupId, int disciplinId, quint32 numSubgroup, const QString &date)
{
	Group *gr = db->getGroupPtr(groupId);
	gr->addPracticDate(disciplinId * 10 + numSubgroup, date);
	for(int i = 0; i < gr->count(); i++)
	{
		if(gr->getStudentPtr(i)->getSubgroupId() == numSubgroup)
			gr->getStudentPtr(i)->addPracticResult(disciplinId, "0");
	}
}

QStringList TeachRegManager::getGroupData(int groupId) const
{
	QStringList ret;
	Group gr = db->getGroup(groupId);
	ret << gr.getName() << QString().setNum(gr.count()) << QString().setNum(gr.getNumSubgroups());
	return ret;
}

QList< float > TeachRegManager::getLectureTotals(int groupId, int disciplinId) const
{
	return db->getGroup(groupId).getLectureTotals(disciplinId);
}

QList< float > TeachRegManager::getPracticTotals(int groupId, int disciplinId, int subgroupId) const
{
	return db->getGroup(groupId).getPracticTotals(disciplinId, subgroupId);
}

void TeachRegManager::setDisciplinView(QComboBox *dcb)
{
	QStringList disciplinList = db->getDisciplinList();
	for(int i = 0; i < disciplinList.count(); i++)
		dcb->addItem(disciplinList.at(i), i);
}

void TeachRegManager::setGroupView(QComboBox *gcb)
{
	QStringList groupList = getGroupsNamesList();
	for(int i = 0; i < groupList.count(); i++)
		gcb->addItem(groupList.at(i), i);
}

void TeachRegManager::setSubgroupView(QComboBox *scb, int groupId)
{
	int n = db->getGroupPtr(groupId)->getNumSubgroups();
	for(int i = 0; i < n; i++)
	{
		scb->addItem(QString().setNum(i + 1), i + 1);
	}
}

DataBaseTeachReg *TeachRegManager::getDb()
{
	return db;
}

bool TeachRegManager::saveDb()
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
