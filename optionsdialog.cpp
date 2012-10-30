/*
    Диалог настройки параметров.
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


#include "optionsdialog.h"

OptionsDialog::OptionsDialog(QWidget *parent, Qt::WindowFlags f): QDialog(parent, f), ui(new Ui::OptionsDlg)
{
	ui->setupUi(this);
}

OptionsDialog::~OptionsDialog()
{
	delete ui;
}

QColor OptionsDialog::getColor0() const
{
	return color0;
}

QColor OptionsDialog::getColor1() const
{
	return color1;
}

QFont OptionsDialog::getFont0() const
{
	return font0;
}

QFont OptionsDialog::getFont1() const
{
	return font1;
}

void OptionsDialog::setColor0(const QColor &c0)
{
	color0 = c0;
	ui->color0Label->setPalette(QPalette(color0));
	ui->color0Label->setAutoFillBackground(true);
}

void OptionsDialog::setColor1(const QColor &c1)
{
	color1 = c1;
	ui->color1Label->setPalette(QPalette(color1));
	ui->color1Label->setAutoFillBackground(true);
}

void OptionsDialog::setFont0(const QFont &f0)
{
	font0 = f0;
	ui->font0LineEdit->setText(QString("%1, %2").arg(font0.family()).arg(font0.pointSize()));
}

void OptionsDialog::setFont1(const QFont &f1)
{
	font1 = f1;
	ui->font1LineEdit->setText(QString("%1, %2").arg(font1.family()).arg(font1.pointSize()));
}

void OptionsDialog::on_font0ToolButton_clicked()
{
	bool ok;
	font0 = QFontDialog::getFont(&ok, font0, this);
	ui->font0LineEdit->setText(QString("%1, %2").arg(font0.family()).arg(font0.pointSize()));
}

void OptionsDialog::on_color0ToolButton_clicked()
{
	color0 = QColorDialog::getColor(color0, this);
	ui->color0Label->setPalette(QPalette(color0));
	ui->color0Label->setAutoFillBackground(true);
}

void OptionsDialog::on_color1ToolButton_clicked()
{
	color1 = QColorDialog::getColor(color1, this);
	ui->color1Label->setPalette(QPalette(color1));
	ui->color1Label->setAutoFillBackground(true);
}

void OptionsDialog::on_font1ToolButton_clicked()
{
	bool ok;
	font1 = QFontDialog::getFont(&ok, font1, this);
	ui->font1LineEdit->setText(QString("%1, %2").arg(font1.family()).arg(font1.pointSize()));
}

#include "optionsdialog.moc"