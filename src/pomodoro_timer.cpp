#include "pomodoro_timer.h"

#include <QDebug>
#include <QString>

PomodoroTimer::PomodoroTimer() {
	actions["startstop"].setText("Toggle Start/Stop");
	actions["restartcurr"].setText("Restart current Pomodoro");
	actions["skip"].setText("Skip current Pomodoro");
	actions["restart"].setText("Restart all");
	actions["extend"].setText("Extend current timer");
	actions["timer"].setText("25:00");
	actions["timer"].setDisabled(true);
	actions["exit"].setText("Exit");

	menu.addAction(&actions["startstop"]);
	menu.addAction(&actions["restartcurr"]);
	//menu.addAction(&actions["skip"]); //TODO
	//menu.addAction(&actions["restart"]); //TODO
	//menu.addAction(&actions["extend"]); //TODO
	menu.addSeparator();
	menu.addAction(&actions["timer"]);
	menu.addSeparator();
	menu.addAction(&actions["exit"]);

	QObject::connect(&actions["startstop"], &QAction::triggered, [=](){this->toggle();}); //"Toggle Start/Stop"
	QObject::connect(&actions["restartcurr"], &QAction::triggered, [=](){this->timer.start();}); //"Restart current Pomodoro"
	QObject::connect(&actions["exit"], &QAction::triggered, [=](){QApplication::quit();}); //"Exit"
	//QObject::connect(&actions["skip"], &QAction::triggered, [=](){timer.start(1000);}); //"Skip"

	QObject::connect(&timer, &QTimer::timeout, [=](){
		if(state == STATE::NORMAL) {
			++pomodoro_cntr;
			if(pomodoro_cntr >= pomodoros) {
				this->sti.showMessage("Timeout", "make a break now!");
				state = STATE::BREAK;
				timer.start(pomodoro_big_pause);
				pomodoro_cntr = 0;
			} else {
				this->sti.showMessage("Timeout", "make a small pause now!");
				state = STATE::PAUSE;
				timer.start(pomodoro_pause);
			}
		} else if(state == STATE::PAUSE || state == STATE::BREAK) {
			state = STATE::NORMAL;
			timer.start(pomodoro_time);
		} else {
			this->sti.showMessage("ERROR", "Unknown state found!", QSystemTrayIcon::MessageIcon::Critical);
		}
	});

	QObject::connect(&update_timer, &QTimer::timeout, [=](){
		int rt = running ? timer.remainingTime() : timer_remaining;

		int min = rt / 1000 / 60;
		int sec = rt / 1000  - min*60;

		//prepend '0' for single digit seconds (< 10) to have nice format (mm:ss)
		auto secstr = (sec < 10) ? '0' + QString::number(sec) : QString::number(sec);

		//update text
		actions["timer"].setText(QString::number(min) + ":" + secstr);

		update_timer.start(1000);
	});

	QObject::connect(&sti, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason reason){
		switch (reason) {
		case QSystemTrayIcon::Trigger:
			this->sti.showMessage("W", "look at me");
			break;
		default:
			break;
		}
	});

	sti.setContextMenu(&menu);
	sti.setIcon(icon_paused);
	sti.show();

	update_timer.start(1000); //update timer displayed in menu every second
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
