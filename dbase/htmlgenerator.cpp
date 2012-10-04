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


#include "htmlgenerator.h"

HtmlGenerator::HtmlGenerator(QObject *parent): QObject(parent)
{
	initHtmlStrings();
}

HtmlGenerator::~HtmlGenerator()
{

}

void HtmlGenerator::initHtmlStrings()
{
	s_htmlHead = trUtf8("<html><head><meta name=\"qrichtext\" content=\"1\" /><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><title>Итоговый результат</title>%1</head>");
	s_htmlStyle = trUtf8("<style type=\"text/css\">\
	body {\
		background: #FFFFFF;\
		font: normal small sans-serif;\
		color: #5D5D5D;\
	}\
	h1, h2, h3 {\
		font-family: serif;\
		color: #385B88;\
	}\
	h1 {\
		font-size: 2.2em;\
		font-weight: bold;\
		text-align: center;\
	}\
	h2 {\
		margin-bottom: .8em;\
		font-size: 2em;\
		font-weight: normal;\
	}\
	h3 {\
		margin-bottom: 1em;\
		text-transform: uppercase;\
		letter-spacing: 2px;\
		font: bold x-small sans-serif;\
	}\
	table{\
		border: 1px solid;\
		/*width: 30\%;*/\
	}\
	div {\
		margin-top:0.3em;\
		margin-bottom:0.3em;\
	}\
	td {\
		border: 1px solid;\
		/*width: 50\%;*/\
	}\
	th {\
		border: 1px solid;\
		/*width: 50\%;*/\
		font-size: 1.2em;\
		font-weight: bold;\
		text-align: center;\
	}\
	</style>");
	s_htmlHeader = trUtf8("<body style=\"font-family: sans-serif; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">\
	<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:20pt; font-weight:600; color: #385B88;\">Итоговый результат</span></p>\
	<p style=\" margin-top:16px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:18pt; font-weight:600;\"><span style=\" font-size:16pt; color: #385B88;\">Дисциплина: \"%1\"</span></p>\
	<p style=\" margin-top:16px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:18pt; font-weight:600;\"><span style=\" font-size:16pt; color: #385B88;\">%2 группа %3</span></p>");
	s_tableHead = trUtf8("<table border=\"1\" align=\"left\" cellspacing=\"0\" cellpadding=\"4\" style=\"margin-top:16px; margin-bottom:12px; font-size:12pt;\">\
	<tr>\
	<td><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Ф.И.О.</span></p></td>\
	<td><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Балл</span></p></td>\
	</tr>");
	s_tableRow = trUtf8("<tr>\
	<td><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">%1</p></td>\
	<td><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-style:italic;\">%2</span></p></td>\
	</tr>");
	s_htmlEnd = trUtf8("</table>\
	<p align=\"justify\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">Средний балл: %1.</span></p>\
	<p align=\"justify\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">Качество знаний: %2\%\
	</span></p>\
	</body>\
	</html>");
}

QString HtmlGenerator::htmlEnd()
{
	return s_htmlEnd;
}

QString HtmlGenerator::htmlHead()
{
	return s_htmlHead;
}

QString HtmlGenerator::htmlHeader()
{
	return s_htmlHeader;
}

QString HtmlGenerator::htmlStyle()
{
	return s_htmlStyle;
}

QString HtmlGenerator::tableHead()
{
	return s_tableHead;
}

QString HtmlGenerator::tableRow()
{
	return s_tableRow;
}

#include "htmlgenerator.moc"