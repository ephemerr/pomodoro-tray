#include "pomodoro_timer.h"

#include <QDebug>

PomodoroTimer::PomodoroTimer() {
	actions["startstop"].setText("Toggle Start/Stop");
	actions["restartcurr"].setText("Restart current Pomodoro");
	actions["skip"].setText("Skip current Pomodoro");
	actions["restart"].setText("Restart all");
	actions["extend"].setText("Extend current timer");
	actions["exit"].setText("Exit");

	menu.addAction(&actions["startstop"]);
	menu.addAction(&actions["restartcurr"]);
	menu.addAction(&actions["skip"]); //TODO
	//menu.addAction(&actions["restart"]); //TODO
	//menu.addAction(&actions["extend"]); //TODO
	menu.addSeparator();
	menu.addAction(&actions["exit"]);

	QObject::connect(&actions["startstop"], &QAction::triggered, [=](){this->toggle();}); //"Toggle Start/Stop"
	QObject::connect(&actions["restartcurr"], &QAction::triggered, [=](){this->timer.start();}); //"Restart current Pomodoro"
	QObject::connect(&actions["exit"], &QAction::triggered, [=](){QApplication::quit();}); //"Exit"

	QObject::connect(&sti, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason reason){
		actions["skip"].setText(std::to_string(timer.remainingTime()).c_str());
	});

	timer.setInterval(pomodoro_time * 1000);

	QObject::connect(&timer, &QTimer::timeout, [=](){
		this->sti.showMessage("Timeout", "*ring* *ring*");
		timer.start(pomodoro_time * 1000);
	});

	sti.setContextMenu(&menu);
	sti.setIcon(icon_paused);
	sti.show();
	//sti.showMessage("TEST", "Hello World");
}

void PomodoroTimer::toggle() {
	if(running) {
		sti.setIcon(icon_paused);
		timer_remaining = timer.remainingTime();
		timer.stop();
	} else {
		sti.setIcon(icon_running);
		timer.start(timer_remaining);
	}
	running = !running;
}
