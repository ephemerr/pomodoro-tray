#include "task_box.h"
#include "ui_task_box.h"
#include <QDebug>

TaskBox::TaskBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskBox)
{
    ui->setupUi(this);
}

void TaskBox::populate(const TaskData &data) {
  for (auto task : data) {
    qDebug() << __func__ << task.first;
    auto row = ui->tableWidget->rowCount()-1;
    ui->tableWidget->insertRow(row);
    // ui->tableWidget->setItemDelegate(new TaskBoxDelegate);
    auto *text = new QTableWidgetItem(task.first);
    auto *pomodoras = new QTableWidgetItem(task.second);
    pomodoras->setIcon(QIcon(":/resources/red.png"));
    pomodoras->setText(QString("x%1").arg(task.second));
    ui->tableWidget->setItem(row, 0, text);
    ui->tableWidget->setItem(row, 1, pomodoras);
  }
}

TaskBox::~TaskBox()
{
    delete ui;
}
