#include <QApplication>
#include "pomodoro_timer.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
  a.setQuitOnLastWindowClosed(false);
	PomodoroTimer pt;
	return a.exec();
}
