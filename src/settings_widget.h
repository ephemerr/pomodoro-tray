#ifndef SETTINGS_WIDGET_H
#define SETTINGS_WIDGET_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QSlider>

class SettingsWidget : public QMainWindow {
	Q_OBJECT
public:
	SettingsWidget(QWidget * parent) : QMainWindow(parent) {
		setWindowTitle("Annotation");
		mainLayout.addWidget(&slider);
		//setLayout(&mainLayout);
	}

	QSlider slider{Qt::Horizontal, this};
	QVBoxLayout mainLayout;
};

#endif // SETTINGS_WIDGET_H
