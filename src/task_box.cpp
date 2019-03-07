#include "task_box.h"
#include "ui_task_box.h"
#include <QDebug>

TaskItem::TaskItem(QString text, int pomodoras)
  : QListWidgetItem(QIcon(":/resources/red.png"),text)
    ,pomodoras_(pomodoras)
{}

QVariant TaskItem::data(int role) const {
  if (role == Qt::DisplayRole) {
    return QString("x%1 %2")
      .arg(pomodoras_)
      .arg(QListWidgetItem::data(role).toString());
  } else return QListWidgetItem::data(role);
}

TaskBox::TaskBox(QWidget *parent) :
    QDialog(parent), ui(new Ui::TaskBox)
{
    ui->setupUi(this);
}

void TaskBox::keyPressEvent(QKeyEvent *event) {
    auto taskWidget = ui->listWidget;
    if(event->key() == Qt::Key_Delete) {
      auto selected = taskWidget->selectedItems();
        if(selected.count() > 0) {
            QMessageBox deleteRequest;
            auto task_name = selected.first()->data(Qt::DisplayRole).toString();
            deleteRequest.setText("Delete task: " + task_name);
            deleteRequest.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            deleteRequest.setDefaultButton(QMessageBox::Cancel);
            auto approved = deleteRequest.exec();
            if (approved) {
              taskWidget->takeItem(taskWidget->currentRow());
            }
        }
    }
    QDialog::keyPressEvent(event);
}

void TaskBox::populate(const TaskData &data) {
  for (auto task : data) {
    qDebug() << __func__ << task.first;
    auto taskWidget = ui->listWidget;
    auto *item = new TaskItem(task.first, task.second);
    item->setIcon(QIcon(":/resources/red.png"));
    item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    taskWidget->addItem(item);
  }
}

TaskBox::~TaskBox()
{
    delete ui;
}
