#include <QApplication>

#include <iostream>

#include "pomodoro_timer.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	PomodoroTimer pt;

	return a.exec();
}
