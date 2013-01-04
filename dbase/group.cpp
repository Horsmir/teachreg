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


#include "group.h"

Group::Group(QObject *parent): QObject(parent), name(QString()), numSubgroups(0)
{

}

Group::Group(const Group &group): QObject()
{
	name = group.name;
	numSubgroups = group.numSubgroups;
	lectureDatesMap = group.lectureDatesMap;
	practicDatesMap = group.practicDatesMap;
	studentList = group.studentList;
}

Group::~Group()
{

}

void Group::addLectureDate(int disciplinId, const QString &date)
{
	QStringList ret = lectureDatesMap.value(disciplinId);
	ret << date;
	lectureDatesMap.insert(disciplinId, ret);
}

void Group::addPracticDate(int disciplinIdsubId, const QString &date)
{
	QStringList ret = practicDatesMap.value(disciplinIdsubId);
	ret << date;
	practicDatesMap.insert(disciplinIdsubId, ret);
}

void Group::addStudent(const Student &student)
{
	studentList << student;
}

QStringList Group::getLectureDate(int disciplinId) const
{
	return lectureDatesMap.value(disciplinId);
}

QString Group::getName() const
{
	return name;
}

int Group::getNumSubgroups() const
{
	return numSubgroups;
}

QStringList Group::getPracticDate(int disciplinIdsubId) const
{
	return practicDatesMap.value(disciplinIdsubId);
}

Student Group::getStudent(int studentId) const
{
	Q_ASSERT(studentId >= 0 && studentId < studentList.count());
	return studentList.at(studentId);
}

Student Group::getStudent(const QString &studentName) const
{
	QListIterator<Student> curStudent(studentList);
	while(curStudent.hasNext())
	{
		Student ret = curStudent.next();
		if(ret.getName() == studentName)
			return ret;
	}
	return Student();
}

Group &Group::operator=(const Group &group)
{
	name = group.name;
	numSubgroups = group.numSubgroups;
	lectureDatesMap = group.lectureDatesMap;
	practicDatesMap = group.practicDatesMap;
	studentList = group.studentList;
	return *this;
}

void Group::setName(const QString &groupName)
{
	name = groupName;
}

void Group::setNumSubgroups(int numSubgroups)
{
	this->numSubgroups = numSubgroups;
}

int Group::count() const
{
	return studentList.count();
}

Student *Group::getStudentPtr(const QString &studentName)
{
	for(int i = 0; i < studentList.count(); i++)
	{
		if(studentList.at(i).getName() == studentName)
			return &(studentList[i]);
	}
	return 0;
}

Student *Group::getStudentPtr(int studentId)
{
	Q_ASSERT(studentId >= 0 && studentId < studentList.count());
	return &(studentList[studentId]);
}

QMap< int, QStringList > Group::getLectureDates() const
{
	return lectureDatesMap;
}

QMap< int, QStringList > Group::getPracticDates() const
{
	return practicDatesMap;
}

QList< Student > Group::getStudents() const
{
	return studentList;
}

void Group::setLectureDates(QMap< int, QStringList > ld)
{
	lectureDatesMap = ld;
}

void Group::setPracticDates(QMap< int, QStringList > pd)
{
	practicDatesMap = pd;
}

void Group::setStudents(QList< Student > students)
{
	studentList = students;
}

int Group::getStudentIdByName(const QString &studentName)
{
	for(int i = 0; i < studentList.count(); i++)
	{
		if(studentList.at(i).getName() == studentName)
			return i;
	}
	return -1;
}

void Group::replaceStudent(int studentId, const Student &student)
{
	studentList.replace(studentId, student);
}

void Group::repleceLectureResult(int studentId, int disciplinId, int pos, const QString &newResult)
{
	(&studentList[studentId])->repleceLectureResult(disciplinId, pos, newResult);
}

void Group::replecepracticResult(int studentId, int disciplinId, int pos, const QString &newResult)
{
	(&studentList[studentId])->replecePracticResult(disciplinId, pos, newResult);
}

QList< float > Group::getLectureTotals(int disciplinId)
{
	QList<float> ret;
	for(int i = 0; i < count(); i++)
	{
		getStudentPtr(i)->calculateLectureTotal(disciplinId);
		ret << studentList.at(i).getLectureTotal(disciplinId);
	}
	return ret;
}

QList< float > Group::getPracticTotals(int disciplinId, int subgroupId)
{
	QList<float> ret;
	for(int i = 0; i < count(); i++)
	{
		if(studentList.at(i).getSubgroupId() == subgroupId)
		{
			getStudentPtr(i)->calculatePracticTotal(disciplinId);
			ret << studentList.at(i).getPracticTotal(disciplinId);
		}
	}
	return ret;
}

int Group::countInSubgroup(int subgroupId) const
{
	int stl = 0;
	QListIterator<Student> itr(studentList);
	while(itr.hasNext())
	{
		if(itr.next().getSubgroupId() == subgroupId)
			stl++;
	}
	return stl;
}

void Group::delStudent(int studentId)
{
	studentList.removeAt(studentId);
}

//---------------------------------------------------------------------------------------------------------------------

QDataStream &operator<<(QDataStream &out, const Group &gr)
{
	out << gr.getName() << gr.getNumSubgroups() << gr.getLectureDates() << gr.getPracticDates() << gr.getStudents();
	return out;
}

QDataStream &operator>>(QDataStream &in, Group &gr)
{
	QString name;
	int numSubgroups;
	QMap<int, QStringList> lectureDatesMap;
	QMap<int, QStringList> practicDatesMap;
	QList<Student> studentList;
	
	in >> name >> numSubgroups >> lectureDatesMap >> practicDatesMap >> studentList;
	gr.setName(name);
	gr.setNumSubgroups(numSubgroups);
	gr.setLectureDates(lectureDatesMap);
	gr.setPracticDates(practicDatesMap);
	gr.setStudents(studentList);
	
	return in;
}

#include "group.moc"