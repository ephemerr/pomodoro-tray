#include "task_box.h"
#include "ui_task_box.h"
#include <QDebug>

TaskBox::TaskBox(QWidget *parent) :
    QDialog(parent), ui(new Ui::TaskBox)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnWidth(0,200);
    ui->tableWidget->setColumnWidth(1,70);
    ui->tableWidget->setColumnWidth(2,30);
}

void TaskBox::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Delete) {
      auto selected = ui->tableWidget->selectedItems();
        if(selected.count() > 0) {
            QMessageBox deleteRequest;
            auto task_name = selected.first()->data(Qt::DisplayRole).toString();
            deleteRequest.setText("Delete task: " + task_name);
            deleteRequest.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            deleteRequest.setDefaultButton(QMessageBox::Cancel);
            auto approved = deleteRequest.exec();
            if (approved) {
              ui->tableWidget->removeRow(selected.first()->row());
            }
        }
    }
    QDialog::keyPressEvent(event);
}


void TaskBox::populate(const TaskData &data) {
  for (auto task : data) {
    qDebug() << __func__ << task.first;
    auto row = ui->tableWidget->rowCount()-1;
    ui->tableWidget->insertRow(row);
    // ui->tableWidget->setItemDelegate(new TaskBoxDelegate);
    auto *text = new QTableWidgetItem(task.first);
    auto *pomodoras = new QTableWidgetItem();
    pomodoras->setIcon(QIcon(":/resources/red.png"));
    pomodoras->setText(QString("x%1").arg(task.second));
    pomodoras->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    ui->tableWidget->setItem(row, 0, text);
    ui->tableWidget->setItem(row, 1, pomodoras);
  }
}

TaskBox::~TaskBox()
{
    delete ui;
}
