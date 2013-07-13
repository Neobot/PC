#ifndef AX12PLAYGROUND_H
#define AX12PLAYGROUND_H

#include <QMainWindow>
#include <QTimer>
#include <QtGui>

#include "ui_AX12Playground.h"
#include "AX12CommManager.h"
#include "UM6.h"
#include "AbstractLogger.h"
#include "Hexapod.h"

class AX12Playground : public QDialog
{
	Q_OBJECT
	
public:
	explicit AX12Playground(QWidget *parent = 0);
	~AX12Playground();
	
private:
	Ui::AX12Playground * ui;
	Comm::AX12CommManager * _commManager;
	Tools::AbstractLogger * _logger;
	Hexapod* _hexapod;
	QTimer* _timerRefresh;
	void move();

	Comm::UM6 * _um6;

private slots:
	void on_btnSend1_clicked();
	void on_btnSend2_clicked();
	void on_btnSend3_clicked();
	void on_btnSend4_clicked();
	void on_btnSend5_clicked();
	void on_btnSend6_clicked();
	void on_btnSend7_clicked();
	void on_btnSend8_clicked();
	void on_btnSend9_clicked();
	void on_btnSend10_clicked();
	void on_btnSend11_clicked();
	void on_btnSend12_clicked();
	void on_btnSend13_clicked();
	void on_btnSend14_clicked();
	void on_btnSend15_clicked();
	void on_btnSend16_clicked();
	void on_btnSend17_clicked();
	void on_btnSend18_clicked();
	void on_btnSendAll_clicked();
	void on_btnGoCoord_clicked();
	void on_btnAuto_clicked();
	void on_btnAction_clicked();
	void on_btnCalibrateGyros_clicked();
	void timerRefreshTick();
};

#endif // AX12PLAYGROUND_H
