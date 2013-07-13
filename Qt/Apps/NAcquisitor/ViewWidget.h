#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <QWidget>

namespace Ui {
    class ViewWidget;
}

class ViewInterface;

class ViewWidget : public QWidget
{
    Q_OBJECT

public:
	explicit ViewWidget(QWidget *parent = 0, QWidget* view = 0, QWidget* settings = 0);
    ~ViewWidget();

    void setView(QWidget* widget);
	void setSettings(QWidget* widget);

    QWidget* getView() const;

private:
    Ui::ViewWidget *ui;
    QWidget* _currentView;
	QWidget* _currentSettings;
	ViewInterface* _curentViewInterface;

    bool _isFullScreen;

private slots:
    void fullscreen();
    void fullscreenStoped(QWidget* w);
	void switchSettings();

};

#endif // VIEWWIDGET_H
