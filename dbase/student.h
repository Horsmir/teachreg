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


#ifndef STUDENT_H
#define STUDENT_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QStringList>

class Student : public QObject
{
	Q_OBJECT
public:
	explicit Student(QObject *parent = 0);
	Student(const Student &student);
	virtual ~Student();
	
	QString getName() const;
	int getSubgroupId() const;
	QStringList getLectureResults(int disciplinId) const;
	QStringList getPracticResults(int disciplinId) const;
	float getLectureTotal(int disciplinId) const;
	float getPracticTotal(int disciplinId) const;
	
	void setName(const QString &studentName);
	void setSubgroupId(int subgroupId);
	void setLectureTotal(int disciplinId, float total);
	void setPracticTotal(int disciplinId, float total);
	
	void addLectureResult(int disciplinId, const QString &result);
	void addPracticResult(int disciplinId, const QString &result);
	
	void calculateLectureTotal(int disciplinId);
	void calculatePracticTotal(int disciplinId);
	
	QMap<int, QStringList> getLectureResultsMap() const;
	QMap<int, QStringList> getPracticResultsMap() const;
	QMap<int, float> getLectureTotalMap() const;
	QMap<int, float> getPracticTotalMap() const;
	
	void setLectureResultsMap(QMap<int, QStringList> lr);
	void setPracticResultsMap(QMap<int, QStringList> pr);
	void setLectureTotalMap(QMap< int, float > lt);
	void setPracticTotalMap(QMap< int, float > pt);
	
	void repleceLectureResult(int disciplinId, int pos, const QString &newResult);
	void replecePracticResult(int disciplinId, int pos, const QString &newResult);
	
	Student &operator=(const Student &other);
	
private:
	QString name;
	int subgroupId;
	QMap<int, QStringList> lectureResultsMap; // список списков оценок на лекциях по каждой дисциплине
	QMap<int, QStringList> practicResultsMap; // список списков оценок на практике по каждой дисциплине
	QMap<int, float> lectureTotalMap;       // список итоговых оценок по лекциям по каждой дисциплине
	QMap<int, float> practicTotalMap;       // список итоговых оценок по практике по каждой дисциплине
};

QDataStream &operator<<(QDataStream &out, const Student &student);
QDataStream &operator>>(QDataStream &in, Student &student);

#endif // STUDENT_H
