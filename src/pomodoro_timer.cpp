#include "pomodoro_timer.h"
#include "task_box.h"

#include <QDebug>
#include <QString>

PomodoroState state_data[] = {
    PomodoroState(State::WORK,    "work",  ":/resources/red.png",    25)
  , PomodoroState(State::BREAK,   "break", ":/resources/green.png",  5)
  , PomodoroState(State::STOPPED, "",      ":/resources/grey.png",   0)
};

PomodoroTimer::PomodoroTimer() {
  qDebug() << "PomodoroTimer" ;
  //pomodoro timer logic
  timer.setSingleShot(true);
  QObject::connect(&timer, &QTimer::timeout, [=]() {
    auto next = this->selectNextState();
    askToNextState(next);
  });

  //update timer to show remaining time
  QObject::connect(&update_timer, &QTimer::timeout, [=] { this->update(); });
  update_timer.setInterval(1000);
  update_timer.start();

  //left click
  QObject::connect(&tray_,
  &QSystemTrayIcon::activated, [=] (QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::DoubleClick) {
      askToNextState(timer.isActive() ? State::STOPPED : State::WORK);
    }
  });
  askToNextState(State::WORK);
  tray_.show();
}

void PomodoroTimer::update() {
  int rt = timer.remainingTime();
  int min = rt / 1000 / 60;
  int sec = rt / 1000 - min*60;
  auto zero = (sec < 10) ? "0" : "";
  auto time_str = QString("%1:%2%3").arg(min).arg(zero).arg(sec);
  tray_.setToolTip(time_str);
}

int PomodoroTimer::showMessage(QString, QString , int) {
  TaskBox msgBox;
  TaskData data = {{"Work",99}};
  msgBox.populate(data);
  // msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
  // msgBox.setDefaultButton(QMessageBox::Ok);
  // QSpacerItem* horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
  // QGridLayout* layout = (QGridLayout*)msgBox.layout();
  // layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

  int ret = msgBox.exec();
  return ret;
}

State PomodoroTimer::selectNextState() {
  State res = State::STOPPED;
  if(state_ == State::WORK) {
      res = State::BREAK;
  } else if(state_ == State::BREAK) {
      res = State::WORK;
  } else {
    showMessage("ERROR", "Unknown state found!");
  }
  return res;
}

void PomodoroTimer::setState(State state) {
  auto to = state_data[state];
  qDebug() << __func__ <<  to.state_ << to.icon_address_;
  state_ = state;
  tray_.setIcon(QIcon(to.icon_address_));
  if (state_ != State::STOPPED) {
    timer.start(to.minutes_*60*1000);
  } else {
    timer.stop();
  }
}

void PomodoroTimer::askToNextState(State state) {
  auto msg = state_data[state].msg_;
  if (state != State::STOPPED) {
    if (PomodoroTimer::showMessage("","Pomodoro calls to " + msg) != QDialog::Accepted)
      state = State::STOPPED;
  }
  qDebug() << __func__ << state;
  setState(state);
}
