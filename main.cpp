#include <QApplication>

#include <iostream>

#include "pomodor_timer.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	PomodorTimer pt;

	return a.exec();
}
