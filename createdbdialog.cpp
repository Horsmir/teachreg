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


#include "createdbdialog.h"

CreateDbDialog::CreateDbDialog(QWidget *parent, Qt::WindowFlags f): QDialog(parent, f), ui(new Ui::CreateDbDlg)
{
	ui->setupUi(this);
}

CreateDbDialog::~CreateDbDialog()
{
	delete ui;
}

QString CreateDbDialog::getAbout() const
{
	return ui->textEdit->toPlainText();
}

QString CreateDbDialog::getFilePath() const
{
	return ui->filePathEdit->text();
}

void CreateDbDialog::on_newFileButton_clicked()
{
	QFileDialog::Options options;
	options |= QFileDialog::DontUseNativeDialog;
	QString selectedFilter;
	
	QString fileName = QFileDialog::getSaveFileName(this, trUtf8("Файл базы данных"), QDir::homePath(), trUtf8("Файлы базы данных (*.dbtr)"), &selectedFilter, options);
	if(!fileName.isEmpty())
		ui->filePathEdit->setText(fileName);
}

#include "createdbdialog.moc"