#ifndef POMODOR_TIMER_H
#define POMODOR_TIMER_H

#include <QAction>
#include <QApplication>
#include <QIcon>
#include <QMenu>
#include <QObject>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QWidget>

#include <map>
#include <string>

class PomodoroTimer {
public:
	PomodoroTimer();
	void start();
	void stop();
	void toggle();

private:
	enum STATE{NORMAL, PAUSE, BREAK} state{NORMAL};
	int pomodoro_time{1500 * 1000}; //in ms. 1500s = 25minutes
	int pomodoro_pause{300 * 1000}; //in ms. 300s = 5minutes
	int pomodoro_big_pause{1200 * 1000}; //in ms. 1200s = 20minutes
	int pomodoros{3}; //pomodoros until bigger pause
	int pomodoro_cntr{0}; //current pomodoro intervall counter

	int timer_remaining{pomodoro_time}; //init with default pomodoro_time

	bool running{false};

	std::map<std::string, QAction> actions;

	//QAction startstop{"Toggle Start/Stop"};
	//QAction quitaction{"Exit"};
	QMenu menu;

	QSystemTrayIcon sti;

	QTimer timer, update_timer;

	//show somewhere
	//designed by Freepik from Flaticon
	QIcon icon_running{":/resources/tomato.png"};
	QIcon icon_paused{":/resources/tomato_pause.png"};
};

#endif // POMODOR_TIMER_H
