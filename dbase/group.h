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


#ifndef GROUP_H
#define GROUP_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QStringList>
#include "student.h"


class Group : public QObject
{
	Q_OBJECT
public:
	explicit Group(QObject *parent = 0);
	Group(const Group &group);
	virtual ~Group();
	
	void setName(const QString &groupName);
	void setNumSubgroups(int numSubgroups);
	
	QString getName() const;
	int getNumSubgroups() const;
	
	void addLectureDate(int disciplinId, const QString &date);
	void addPracticDate(int disciplinIdsubId, const QString &date); // disciplinIdsubId = disciplinId * 10 + subgroupId
	void addStudent(const Student &student);
	
	QStringList getLectureDate(int disciplinId) const;
	QStringList getPracticDate(int disciplinIdsubId) const;
	Student getStudent(int studentId) const;
	Student getStudent(const QString &studentName) const;
	Student *getStudentPtr(const QString &studentName);
	Student *getStudentPtr(int studentId);
	int getStudentIdByName(const QString &studentName);
	int count() const;
	
	QMap<int, QStringList> getLectureDates() const;
	QMap<int, QStringList> getPracticDates() const;
	QList<Student> getStudents() const;
	
	void setLectureDates(QMap<int, QStringList> ld);
	void setPracticDates(QMap<int, QStringList> pd);
	void setStudents(QList<Student> students);
	
	void replaceStudent(int studentId, const Student &student);
	void repleceLectureResult(int studentId, int disciplinId, int pos, const QString &newResult);
	void replecepracticResult(int studentId, int disciplinId, int pos, const QString &newResult);
	
	QList<float> getLectureTotals(int disciplinId);
	QList<float> getPracticTotals(int disciplinId, int subgroupId);
	
	Group &operator=(const Group &group);
	
private:
	QString name;
	int numSubgroups;
	QMap<int, QStringList> lectureDatesMap;
	QMap<int, QStringList> practicDatesMap;
	QList<Student> studentList;
};

QDataStream &operator<<(QDataStream &out, const Group &gr);
QDataStream &operator>>(QDataStream &in, Group &gr);

#endif // GROUP_H
