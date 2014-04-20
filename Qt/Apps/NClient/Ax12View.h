#ifndef AX12VIEW_H
#define AX12VIEW_H

#include "AbstractView.h"

#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QSignalMapper>

namespace Ui {
	class Ax12View;
}

class Ax12DisplayWidget : public QWidget
{
	Q_OBJECT

signals:
	void send();
	void increase();
	void decrease();
	void remove();

public:
	explicit Ax12DisplayWidget(QWidget *parent = 0);

	void setId(int id);
	void setAngle(float angle);
	void setSpeed(float speed);
	void setTorque(float torque);

	int getId() const;
	float getAngle() const;
	float getSpeed() const;
	float getTorque() const;

	void setSendEnabled(bool value);

private:
	QSpinBox* _spId;
	QDoubleSpinBox* _spAngle;
	QDoubleSpinBox* _spSpeed;
	QDoubleSpinBox* _spTorque;

	QPushButton* _btnSend;
	QPushButton* _btnIncrease;
	QPushButton* _btnDecrease;
};

class Ax12View : public AbstractView, public NetworkClientCommListener
{
	Q_OBJECT
	
public:
	explicit Ax12View(NetworkConnection *connection, QWidget *parent = 0);
	~Ax12View();

	void loadSettings(QSettings *settings);
	void saveSettings(QSettings *settings);

	void connectionStatusChanged(NetworkConnection::ConnectionStatus status);
	
private:
	Ui::Ax12View *ui;
	QHash<int, Ax12DisplayWidget*> _ax12Widgets;
	QSignalMapper* _removeAx12Mapper;
	QSignalMapper* _sendAx12Mapper;
	QSignalMapper* _increaseAx12Mapper;
	QSignalMapper* _decreaseAx12Mapper;

	void addAx12(int id, float speed, float torque);

	void setAx12Positions(const QList<int> ids, const QList<float> &positions, const QList<float>& loads);
	void setAx12MovementsFile(const QByteArray &data);

	QMap<int, float> getAllPositions() const;

private slots:
	void addAx12();
	void removeAx12(QWidget* ax12Widget);
	void sendAx12(QWidget* ax12Widget);
	void increaseAx12(QWidget* ax12Widget);
	void decreaseAx12(QWidget* ax12Widget);

	void refreshAx12Positions();
	void lockAll();
	void releaseAll();

	void addRecord();

	void reloadMovements();
	void saveMovements();

	void exportMovements();
	void importMovements();
	
	void movementEditorTabChanged(int index);
    void runMovement();

    void runCurrentMovementUntil(int positionIndex);
    void moveToCurrentMovementPosition(int positionIndex);

};

#endif // AX12VIEW_H
