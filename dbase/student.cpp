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


#include "student.h"

Student::Student(QObject *parent): QObject(parent), name(QString()), subgroupId(1)
{
}

Student::Student(const Student &student): QObject()
{
	name = student.name;
	subgroupId = student.subgroupId;
	lectureResultsMap = student.lectureResultsMap;
	practicResultsMap = student.practicResultsMap;
	lectureTotalMap = student.lectureTotalMap;
	practicTotalMap = student.practicTotalMap;
}
Student::~Student()
{
}

QString Student::getName() const
{
	return name;
}

void Student::setName(const QString &studentName)
{
	name = studentName;
}

void Student::addLectureResult(int disciplinId, const QString &result)
{
	QStringList ret = lectureResultsMap.value(disciplinId);
	ret << result;
	lectureResultsMap.insert(disciplinId, ret);
}

void Student::addPracticResult(int disciplinId, const QString &result)
{
	QStringList ret = practicResultsMap.value(disciplinId);
	ret << result;
	practicResultsMap.insert(disciplinId, ret);
}

void Student::calculateLectureTotal(int disciplinId)
{
	QStringList ret = lectureResultsMap.value(disciplinId);
	if(!ret.isEmpty())
	{
		float res  = 0;
		int iNum = 0;
		for(int i = 0; i < ret.count(); i++)
		{
			if(ret.at(i) == "0" || ret.at(i) == "1")
				continue;
			res += ret.at(i).toFloat();
			iNum++;
		}

		res /= float(iNum);
		QString strRes = QString().setNum(res, 'f', 1);
		lectureTotalMap.insert(disciplinId, strRes);
	}
}

void Student::calculatePracticTotal(int disciplinId)
{
	QStringList ret = practicResultsMap.value(disciplinId);
	if(!ret.isEmpty())
	{
		float res  = 0;
		int iNum = 0;
		for(int i = 0; i < ret.count(); i++)
		{
			if(ret.at(i) == "0" || ret.at(i) == "1")
				continue;
			res += ret.at(i).toFloat();
			iNum++;
		}

		res /= float(iNum);
		QString strRes = QString().setNum(res, 'f', 1);
		practicTotalMap.insert(disciplinId, strRes);
	}
}

QStringList Student::getLectureResults(int disciplinId) const
{
	return lectureResultsMap.value(disciplinId);
}

QString Student::getLectureTotal(int disciplinId) const
{
	return lectureTotalMap.value(disciplinId);
}

QStringList Student::getPracticResults(int disciplinId) const
{
	return practicResultsMap.value(disciplinId);
}

QString Student::getPracticTotal(int disciplinId) const
{
	return practicTotalMap.value(disciplinId);
}

int Student::getSubgroupId() const
{
	return subgroupId;
}

void Student::setLectureTotal(int disciplinId, const QString &total)
{
	lectureTotalMap.insert(disciplinId, total);
}

void Student::setPracticTotal(int disciplinId, const QString &total)
{
	practicTotalMap.insert(disciplinId, total);
}

void Student::setSubgroupId(int subgroupId)
{
	this->subgroupId = subgroupId;
}

Student &Student::operator= (const Student &other)
{
	name = other.name;
	subgroupId = other.subgroupId;
	lectureResultsMap = other.lectureResultsMap;
	practicResultsMap = other.practicResultsMap;
	lectureTotalMap = other.lectureTotalMap;
	practicTotalMap = other.practicTotalMap;
	return *this;
}

QMap< int, QStringList > Student::getLectureResultsMap() const
{
	return lectureResultsMap;
}

QMap< int, QString > Student::getLectureTotalMap() const
{
	return lectureTotalMap;
}

QMap< int, QStringList > Student::getPracticResultsMap() const
{
	return practicResultsMap;
}

QMap< int, QString > Student::getPracticTotalMap() const
{
	return practicTotalMap;
}

void Student::setLectureResultsMap(QMap< int, QStringList > lr)
{
	lectureResultsMap = lr;
}

void Student::setLectureTotalMap(QMap< int, QString > lt)
{
	lectureTotalMap = lt;
}

void Student::setPracticResultsMap(QMap< int, QStringList > pr)
{
	practicResultsMap = pr;
}

void Student::setPracticTotalMap(QMap< int, QString > pt)
{
	practicTotalMap = pt;
}

void Student::repleceLectureResult(int disciplinId, int pos, const QString &newResult)
{
	(&lectureResultsMap[disciplinId])->replace(pos, newResult);
	/*QStringList ret = lectureResultsMap.value(disciplinId);
	ret.replace(pos, newResult);
	lectureResultsMap.insert(disciplinId, ret);*/
}

void Student::replecePracticResult(int disciplinId, int pos, const QString &newResult)
{
	QStringList ret = practicResultsMap.value(disciplinId);
	ret.replace(pos, newResult);
	practicResultsMap.insert(disciplinId, ret);
}

//----------------------------------------------------------------------------------

QDataStream &operator<<(QDataStream &out, const Student &student)
{
	out << student.getName() << student.getSubgroupId() << student.getLectureResultsMap() << student.getPracticResultsMap() << student.getLectureTotalMap() << student.getPracticTotalMap();
	return out;
}

QDataStream &operator>>(QDataStream &in, Student &student)
{
	QString name;
	int subgroupId;
	QMap<int, QStringList> lectureResultsMap;
	QMap<int, QStringList> practicResultsMap;
	QMap<int, QString> lectureTotalMap;
	QMap<int, QString> practicTotalMap;

	in >> name >> subgroupId >> lectureResultsMap >> practicResultsMap >> lectureTotalMap >> practicTotalMap;
	student.setName(name);
	student.setSubgroupId(subgroupId);
	student.setLectureResultsMap(lectureResultsMap);
	student.setPracticResultsMap(practicResultsMap);
	student.setLectureTotalMap(lectureTotalMap);
	student.setPracticTotalMap(practicTotalMap);

	return in;
}

#include "student.moc"
