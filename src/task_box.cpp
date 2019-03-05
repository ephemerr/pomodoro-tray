#include "task_box.h"
#include "ui_task_box.h"

TaskBox::TaskBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskBox)
{
    ui->setupUi(this);
}

TaskBox::~TaskBox()
{
    delete ui;
}
