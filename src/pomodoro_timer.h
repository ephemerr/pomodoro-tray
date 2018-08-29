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
	int pomodoro_time{1500}; //in seconds. 1500 = 25minutes
	int pomodoro_pause{300}; //in seconds. 300 = 5minutes
	int pomodoro_big_pause{1200}; //in seconds. 1200 = 20minutes
	int pomodoros{3}; //pomodoros until bigger pause
	int pomodoro_cntr{0}; //current pomodoro intervall counter

	int timer_remaining;

	bool running{false};

	std::map<std::string, QAction> actions;

	//QAction startstop{"Toggle Start/Stop"};
	//QAction quitaction{"Exit"};
	QMenu menu;

	QSystemTrayIcon sti;

	QTimer timer;

	//show somewhere
	//designed by Freepik from Flaticon
	QIcon icon_running{":/resources/tomato.png"};
	QIcon icon_paused{":/resources/tomato_pause.png"};
};

#endif // POMODOR_TIMER_H
