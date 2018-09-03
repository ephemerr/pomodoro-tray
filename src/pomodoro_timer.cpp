#include "pomodoro_timer.h"

#include <QDebug>
#include <QString>

#include <QWidgetAction>

#include <QSpinBox>

PomodoroTimer::PomodoroTimer() {
	actions["startstop"].setText("Toggle Start/Stop");
	actions["restartcurr"].setText("Restart current Pomodoro");
	actions["skip"].setText("Skip current Pomodoro");
	actions["restart"].setText("Restart all");
	actions["extend"].setText("Extend current timer");
	actions["timer"].setText("25:00");
	actions["timer"].setDisabled(true);
	actions["exit"].setText("Exit");

	//SliderAction requires qstring at construction
	//resulting in a deleted default ctor and deleted copy ctor
	//thus we need to forward the args
	wdgtactions.emplace(std::piecewise_construct,
						std::forward_as_tuple("pomodoro_time"),
						std::forward_as_tuple("Pomodoro Time", pomodoro_time/1000 / 60));

	wdgtactions.emplace(std::piecewise_construct,
						std::forward_as_tuple("pomodoro_pause"),
						std::forward_as_tuple("Pomodoro Pause", pomodoro_pause/1000 / 60));

	wdgtactions.emplace(std::piecewise_construct,
						std::forward_as_tuple("pomodoro_big_pause"),
						std::forward_as_tuple("Pomodoro Break", pomodoro_big_pause/1000 / 60));

	wdgtactions.emplace(std::piecewise_construct,
						std::forward_as_tuple("pomodoros"),
						std::forward_as_tuple("Pomodoros", pomodoros, ""));

	wdgtactions.at("pomodoro_time").slider()->setMaximum(60);
	wdgtactions.at("pomodoro_pause").slider()->setMaximum(60);
	wdgtactions.at("pomodoro_big_pause").slider()->setMaximum(2*60);
	wdgtactions.at("pomodoros").slider()->setMaximum(10);
	wdgtactions.at("pomodoros").slider()->setMaximumWidth(100);

	//create menu
	menu.addSeparator();
	menu.addAction(&actions.at("startstop"));
	menu.addAction(&actions.at("restartcurr"));
	//menu.addAction(&actions.at("skip")); //TODO
	//menu.addAction(&actions.at("restart")); //TODO
	//menu.addAction(&actions.at("extend")); //TODO
	menu.addSeparator();
	menu.addAction(&actions.at("timer"));
	menu.addSeparator();
	menu.addAction(&wdgtactions.at("pomodoro_time"));
	menu.addAction(&wdgtactions.at("pomodoro_pause"));
	menu.addAction(&wdgtactions.at("pomodoro_big_pause"));
	menu.addAction(&wdgtactions.at("pomodoros"));
	menu.addSeparator();
	menu.addAction(&actions.at("exit"));

	QObject::connect(&actions.at("startstop"), &QAction::triggered, [=](){this->toggle();}); //"Toggle Start/Stop"
	QObject::connect(&actions.at("restartcurr"), &QAction::triggered, [=](){this->timer.start();}); //"Restart current Pomodoro"
	QObject::connect(&actions.at("exit"), &QAction::triggered, [=](){QApplication::quit();}); //"Exit"
	//QObject::connect(&actions.at("skip"), &QAction::triggered, [=](){timer.start(1000);}); //"Skip"

	//pomodoro timer logic
	QObject::connect(&timer, &QTimer::timeout, [=](){
		if(state == STATE::NORMAL) {
			++pomodoro_cntr;
			if(pomodoro_cntr >= pomodoros) {
				this->sti.showMessage("Big Break!", "make a break now!");
				state = STATE::BREAK;
				timer.start(pomodoro_big_pause);
				pomodoro_cntr = 0;
			} else {
				this->sti.showMessage("Small Pause", "make a small pause now!");
				state = STATE::PAUSE;
				timer.start(pomodoro_pause);
			}
		} else if(state == STATE::PAUSE || state == STATE::BREAK) {
			this->sti.showMessage("BACK", "back to work!");
			state = STATE::NORMAL;
			timer.start(pomodoro_time);
		} else {
			this->sti.showMessage("ERROR", "Unknown state found!", QSystemTrayIcon::MessageIcon::Critical);
		}
	});

	//update timer to show remaining time in menu
	QObject::connect(&update_timer, &QTimer::timeout, [=](){
		int rt = running ? timer.remainingTime() : timer_remaining;

		int min = rt / 1000 / 60;
		int sec = rt / 1000  - min*60;

		//prepend '0' for single digit seconds (< 10) to have nice format (mm:ss)
		auto secstr = (sec < 10) ? '0' + QString::number(sec) : QString::number(sec);

		//update text
		actions["timer"].setText(QString::number(min) + ":" + secstr + "  [" + QString::number(pomodoro_cntr + 1) + "/" + QString::number(pomodoros) + "]");

		update_timer.start(1000);
	});

	//left click
	QObject::connect(&sti, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason reason){
		switch (reason) {
		case QSystemTrayIcon::Trigger:
			//this->settings.show();
			break;
		default:
			break;
		}
	});

	//slider signals
	QObject::connect(wdgtactions.at("pomodoro_time").slider(), &QSlider::valueChanged, [=](int newvalue){
		pomodoro_time = newvalue * 60 * 1000;
		timer_remaining = pomodoro_time;
	});
	QObject::connect(wdgtactions.at("pomodoro_pause").slider(), &QSlider::valueChanged, [=](int newvalue){
		pomodoro_pause = newvalue * 60 * 1000;
	});
	QObject::connect(wdgtactions.at("pomodoro_big_pause").slider(), &QSlider::valueChanged, [=](int newvalue){
		pomodoro_big_pause = newvalue * 60 * 1000;
	});
	QObject::connect(wdgtactions.at("pomodoros").slider(), &QSlider::valueChanged, [=](int newvalue){
		pomodoros = newvalue;
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
