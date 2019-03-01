#include "pomodoro_timer.h"

#include <QDebug>
#include <QString>

PomodoroState state_data[] = {
    PomodoroState(State::WORK,    "work",  ":/resources/red.png",    25)
  , PomodoroState(State::BREAK,   "break", ":/resources/green.png",  5)
  , PomodoroState(State::REST,    "rest",  ":/resources/yellow.png", 20)
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

  setState(State::STOPPED);

  //left click
  QObject::connect(&tray_,
  &QSystemTrayIcon::activated, [=] (QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::DoubleClick) toggle();
  });
  tray_.show();
}

void PomodoroTimer::update() {
  int rt = timer.isActive() ? timer.remainingTime() : 0;
  int min = rt / 1000 / 60;
  int sec = rt / 1000  - min*60;
  //prepend '0' for single digit seconds (< 10) to have nice format (mm:ss)
  auto secstr = (sec < 10) ? '0' + QString::number(sec) : QString::number(sec);
  //update text
  auto time_str = QString("%1:%2 [%3/%4]")
                 .arg(min)
                 .arg(secstr)
                 .arg(pomodoro_cntr + 1)
                 .arg(pomodoros)
                 ;
  tray_.setToolTip(time_str);
}

void PomodoroTimer::toggle() {
  if (timer.isActive())
    setState(State::STOPPED);
  else
    setState(State::WORK);
}

int PomodoroTimer::showMessage(QString title, QString msg, int) {
  QMessageBox msgBox;
  msgBox.setText(msg);
  // msgBox.setInformativeText(msg);
  msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Ok);
  int ret = msgBox.exec();
  return ret;
}

State PomodoroTimer::selectNextState() {
  State res = State::STOPPED;
  if(state_ == State::WORK) {
    if(pomodoro_cntr >= pomodoros) {
      res = State::REST;
    } else {
      res = State::BREAK;
    }
  } else if(state_ == State::BREAK || state_ == State::REST) {
      res = State::WORK;
  } else {
    showMessage("ERROR", "Unknown state found!");
  }
  return res;
}

void PomodoroTimer::setState(State to_indx) {
  setState(state_data[to_indx]);
}

void PomodoroTimer::setState(const PomodoroState &to) {
  qDebug() << __func__ <<  to.state_;
  state_ = to.state_;
  tray_.setIcon(QIcon(to.icon_address_));
  if (state_ != State::STOPPED) {
    timer.start(to.minutes_*60*1000);
  }
}

void PomodoroTimer::askToNextState(State to_indx) {
  PomodoroState &to = state_data[to_indx];
  if (to_indx == State::STOPPED)
    goto FIN;
  if (to_indx == State::BREAK)
    ++pomodoro_cntr;
  if (to_indx == State::REST)
    pomodoro_cntr = 0;
  if (PomodoroTimer::showMessage("","Pomodoro calls to "+to.msg_) != QMessageBox::Ok)
    to = state_data[State::STOPPED];
FIN:
  qDebug() << __func__ << to.state_;
  setState(to);
}
