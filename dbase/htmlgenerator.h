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


#ifndef HTMLGENERATOR_H
#define HTMLGENERATOR_H

#include <QtCore/QObject>


class HtmlGenerator : public QObject
{
	Q_OBJECT
public:
	explicit HtmlGenerator(QObject *parent = 0);
	virtual ~HtmlGenerator();
	
	QString htmlHead();
	QString htmlHeader();
	QString htmlStyle();
	QString tableHead();
	QString tableRow();
	QString htmlEnd();
	
private:
	QString s_htmlHead;
	QString s_htmlHeader;
	QString s_htmlStyle;
	QString s_tableHead;
	QString s_tableRow;
	QString s_htmlEnd;
	
	void initHtmlStrings();
};

#endif // HTMLGENERATOR_H
