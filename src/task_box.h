#ifndef TASK_BOX_H
#define TASK_BOX_H

#include <QtWidgets>
#include <QtCore>

namespace Ui {
class TaskBox;
}

typedef QList<QPair<QString,int>> TaskData;

class TaskBox : public QDialog
{
    Q_OBJECT

public:
    explicit TaskBox(QWidget *parent = nullptr);
    void populate(const TaskData&);
    void keyPressEvent(QKeyEvent *event);
    ~TaskBox();

private:
    Ui::TaskBox *ui;
};

#endif // TASK_BOX_H
