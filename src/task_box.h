#ifndef TASK_BOX_H
#define TASK_BOX_H

#include <QDialog>

namespace Ui {
class TaskBox;
}

class TaskBox : public QDialog
{
    Q_OBJECT

public:
    explicit TaskBox(QWidget *parent = nullptr);
    ~TaskBox();

private:
    Ui::TaskBox *ui;
};

#endif // TASK_BOX_H
