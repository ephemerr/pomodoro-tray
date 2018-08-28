#include "pomodor_timer.h"

PomodorTimer::PomodorTimer() {
	menu.addAction(&startstop);
	menu.addSeparator();
	menu.addAction(&quitaction);

	QObject::connect(&startstop, &QAction::triggered, [=](){this->toggle();});
	QObject::connect(&quitaction, &QAction::triggered, [=](){QApplication::quit();});

	sti.setContextMenu(&menu);
	sti.setIcon(icon_paused);
	sti.show();
	sti.showMessage("TEST", "Hello World");
}

void PomodorTimer::toggle() {
	if(running) {
		sti.setIcon(icon_paused);
	} else {
		sti.setIcon(icon_running);
	}
	running = !running;
}
