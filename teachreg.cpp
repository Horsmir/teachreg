#include "teachreg.h"

TeachReg::TeachReg(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	ui->contentsWidget->setCurrentRow(0);
	ui->pagesWidget->setCurrentIndex(0);
	
	dlgStudents = new StudentListDialog(this);
	
	createIcons();
}

TeachReg::~TeachReg()
{
	delete ui;
}

void TeachReg::createIcons()
{
	QListWidgetItem *homeButton = new QListWidgetItem(ui->contentsWidget);
	homeButton->setIcon(QIcon(":/images/home.png"));
	homeButton->setText(trUtf8("Главная"));
	homeButton->setTextAlignment(Qt::AlignHCenter);
	
	QListWidgetItem *disciplinsButton = new QListWidgetItem(ui->contentsWidget);
	disciplinsButton->setIcon(QIcon(":/images/disciplins.png"));
	disciplinsButton->setText(trUtf8("Дисциплины"));
	disciplinsButton->setTextAlignment(Qt::AlignHCenter);
	
	QListWidgetItem *groupsButton = new QListWidgetItem(ui->contentsWidget);
	groupsButton->setIcon(QIcon(":/images/groups.png"));
	groupsButton->setText(trUtf8("Группы"));
	groupsButton->setTextAlignment(Qt::AlignHCenter);
	
	QListWidgetItem *lectureButton = new QListWidgetItem(ui->contentsWidget);
	lectureButton->setIcon(QIcon(":/images/lectures.png"));
	lectureButton->setText(trUtf8("Лекции"));
	lectureButton->setTextAlignment(Qt::AlignHCenter);
	
	QListWidgetItem *practicsButton = new QListWidgetItem(ui->contentsWidget);
	practicsButton->setIcon(QIcon(":/images/practics.png"));
	practicsButton->setText(trUtf8("Практика"));
	practicsButton->setTextAlignment(Qt::AlignHCenter);
}

void TeachReg::on_contentsWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	if(!current)
		current = previous;
	
	ui->pagesWidget->setCurrentIndex(ui->contentsWidget->row(current));
}

void TeachReg::on_studentListButton_clicked()
{
	dlgStudents->show();
}

#include "teachreg.moc"
