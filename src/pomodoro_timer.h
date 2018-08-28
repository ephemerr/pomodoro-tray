#ifndef POMODOR_TIMER_H
#define POMODOR_TIMER_H

#include <QAction>
#include <QApplication>
#include <QIcon>
#include <QMenu>
#include <QObject>
#include <QSystemTrayIcon>
#include <QWidget>

class PomodoroTimer {
public:
	PomodoroTimer();
	void start();
	void stop();
	void toggle();

private:
	double time{0.0};
	bool running{false};

	QSystemTrayIcon sti;
	QMenu menu;
	QAction startstop{"Toggle Start/Stop"};
	QAction quitaction{"Exit"};

	//show somewhere
	//designed by Freepik from Flaticon
	QIcon icon_running{":/resources/tomato.png"};
	QIcon icon_paused{":/resources/tomato_pause.png"};
};

#endif // POMODOR_TIMER_H
