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

#include <QWidgetAction>
#include <QLabel>


#include "settings_widget.h"


class SliderAction : public QWidgetAction {
public:
	SliderAction(const QString& title, int default_value, QString postfix = " min") : QWidgetAction(nullptr) {
		this->postfix = postfix;
		QWidget* pWidget = new QWidget(nullptr);
		QHBoxLayout* pLayout = new QHBoxLayout();

		pLabel = new QLabel("     " + title);

		pSlider = new QSlider(Qt::Horizontal, nullptr);
		pSlider->setMinimum(1);
		pSlider->setMaximum(1000);
		pSlider->setValue(default_value);
		pSlider->setSliderPosition(default_value);

		pLabelSlider = new QLabel(QString::number(this->pSlider->value()) + postfix);

		pLayout->addWidget(pLabel);
		pLayout->addWidget(pSlider);
		pLayout->addWidget(pLabelSlider);

		pWidget->setLayout(pLayout);

		setDefaultWidget(pWidget);

		QObject::connect(pSlider, &QSlider::valueChanged, [=](){
			this->pLabelSlider->setText(QString::number(this->pSlider->value()) + postfix);
		});
	}

	QSlider * slider() {
		return pSlider;
	}

	const QSlider * slider() const {
		return pSlider;
	}

	void setLabel(const QString& text) {
		if(pLabel != nullptr) {
			pLabel->setText(text);
		}
	}

private:
	QSlider * pSlider;
	QLabel * pLabel;
	QLabel * pLabelSlider;
	QString postfix;
};

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
	std::map<std::string, SliderAction> wdgtactions;

	QMenu menu;

	QSystemTrayIcon sti;

	QTimer timer, update_timer;

	//show somewhere
	//designed by Freepik from Flaticon
	QIcon icon_running{":/resources/tomato.png"};
	QIcon icon_paused{":/resources/tomato_pause.png"};
};

#endif // POMODOR_TIMER_H
