#ifndef POMODOR_TIMER_H
#define POMODOR_TIMER_H

#include <QAction>
#include <QApplication>
#include <QIcon>
#include <QObject>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QtWidgets>

enum State{WORK=0, BREAK, REST, STOPPED};

class PomodoroState;

class PomodoroTimer {
public:

  PomodoroTimer();
  void update();
  int showMessage(QString,QString,int level=0);
  State selectNextState();
  void askToNextState(State );
  void setState(const PomodoroState &);
  void setState(State);

  int pomodoros{3}; //pomodoros until bigger rest
  int pomodoro_cntr{0}; //current pomodoro intervall counter

  QSystemTrayIcon tray_;
  QTimer timer, update_timer;

  State state_;
};

class PomodoroState {
  public:
  PomodoroState(State state,QString msg,QString icon_address,int minutes) :
    state_(state), msg_(msg), icon_address_(icon_address), minutes_(minutes) {};
  State state_{STOPPED};
  QString msg_;
  QString icon_address_;
  int minutes_;
};
#endif // POMODOR_TIMER_H
