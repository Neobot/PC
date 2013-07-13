#include "AX12Playground.h"
#include "ui_AX12Playground.h"

using namespace Tools;

AX12Playground::AX12Playground(QWidget *parent) :
	QDialog(parent), ui(new Ui::AX12Playground)
{
	ui->setupUi(this);

	_logger = new StdoutLogger();
	_logger->setBase(16);

	_commManager = new Comm::AX12CommManager("/dev/ttyS3", BAUD115200, _logger);
	_commManager->open();


	_um6 = new Comm::UM6("/dev/ttyS2", BAUD115200, _logger);
	_um6->open();
	_um6->calibrateGyros();

/*
	_arm = new ThreeAxisArm(_logger, _commManager,
							33.0, 68.0, 35.0,
							1, 50.0, 250.0,
							2, 50.0, 150.0,
							3, 50.0, 250.0);

*/
	_hexapod = new Hexapod(_logger, _commManager, _um6,
						   107.0, 53.0, 83.0, 138.0,
						   70.0, 140.0, 40,
						   65, 235, 60, 240, 15, 285);

	_commManager->setReadingLoopMode(Comm::AX12CommManager::AUTO_MODE);
	_commManager->askStartReadingLoop();

	_timerRefresh = new QTimer(this);
	connect(_timerRefresh, SIGNAL(timeout()), this, SLOT(timerRefreshTick()));
	_timerRefresh->start(500);

	_hexapod->setGoal(300, 0, 30);
}

AX12Playground::~AX12Playground()
{
	delete _logger;
	delete _commManager;
	delete _hexapod;
	delete ui;
}

void AX12Playground::on_btnSend1_clicked()
{
	float p = (float)ui->sbPosition1->value();
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	_commManager->setGoal(1, p, s, t, true);
}

void AX12Playground::on_btnSend2_clicked()
{
	float p = (float)ui->sbPosition2->value();
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	_commManager->setGoal(2, p, s, t, true);
}

void AX12Playground::on_btnSend3_clicked()
{
	float p = (float)ui->sbPosition3->value();
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	_commManager->setGoal(3, p, s, t, true);
}

void AX12Playground::on_btnSend4_clicked()
{
	float p = (float)ui->sbPosition4->value();
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	_commManager->setGoal(4, p, s, t, true);
}

void AX12Playground::on_btnSend5_clicked()
{
	float p = (float)ui->sbPosition5->value();
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	_commManager->setGoal(5, p, s, t, true);
}

void AX12Playground::on_btnSend6_clicked()
{
	float p = (float)ui->sbPosition6->value();
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	_commManager->setGoal(6, p, s, t, true);
}

void AX12Playground::on_btnSend7_clicked()
{
	float p = (float)ui->sbPosition7->value();
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	_commManager->setGoal(7, p, s, t, true);
}

void AX12Playground::on_btnSend8_clicked()
{
	float p = (float)ui->sbPosition8->value();
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	_commManager->setGoal(8, p, s, t, true);
}

void AX12Playground::on_btnSend9_clicked()
{
	float p = (float)ui->sbPosition9->value();
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	_commManager->setGoal(9, p, s, t, true);
}

void AX12Playground::on_btnSend10_clicked()
{
	float p = (float)ui->sbPosition10->value();
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	_commManager->setGoal(10, p, s, t, true);
}

void AX12Playground::on_btnSend11_clicked()
{
	float p = (float)ui->sbPosition11->value();
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	_commManager->setGoal(11, p, s, t, true);
}

void AX12Playground::on_btnSend12_clicked()
{
	float p = (float)ui->sbPosition12->value();
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	_commManager->setGoal(12, p, s, t, true);
}

void AX12Playground::on_btnSend13_clicked()
{
	float p = (float)ui->sbPosition13->value();
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	_commManager->setGoal(13, p, s, t, true);
}

void AX12Playground::on_btnSend14_clicked()
{
	float p = (float)ui->sbPosition14->value();
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	_commManager->setGoal(14, p, s, t, true);
}

void AX12Playground::on_btnSend15_clicked()
{
	float p = (float)ui->sbPosition15->value();
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	_commManager->setGoal(15, p, s, t, true);
}

void AX12Playground::on_btnSend16_clicked()
{
	float p = (float)ui->sbPosition16->value();
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	_commManager->setGoal(16, p, s, t, true);
}

void AX12Playground::on_btnSend17_clicked()
{
	float p = (float)ui->sbPosition17->value();
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	_commManager->setGoal(17, p, s, t, true);
}

void AX12Playground::on_btnSend18_clicked()
{
	float p = (float)ui->sbPosition18->value();
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	_commManager->setGoal(18, p, s, t, true);
}


void AX12Playground::on_btnSendAll_clicked()
{
	float s = (float)ui->sbSpeed->value();
	float t = ui->sbMaxTorque->value();

	float p1 = (float)ui->sbPosition1->value();
	float p2 = (float)ui->sbPosition2->value();
	float p3 = (float)ui->sbPosition3->value();
	float p4 = (float)ui->sbPosition4->value();
	float p5 = (float)ui->sbPosition5->value();
	float p6 = (float)ui->sbPosition6->value();
	float p7 = (float)ui->sbPosition7->value();
	float p8 = (float)ui->sbPosition8->value();
	float p9 = (float)ui->sbPosition9->value();
	float p10 = (float)ui->sbPosition10->value();
	float p11 = (float)ui->sbPosition11->value();
	float p12 = (float)ui->sbPosition12->value();
	float p13 = (float)ui->sbPosition13->value();
	float p14 = (float)ui->sbPosition14->value();
	float p15 = (float)ui->sbPosition15->value();
	float p16 = (float)ui->sbPosition16->value();
	float p17 = (float)ui->sbPosition17->value();
	float p18 = (float)ui->sbPosition18->value();

	_commManager->setGoal(1, p1, s, t, false);
	_commManager->setGoal(2, p2, s, t, false);
	_commManager->setGoal(3, p3, s, t, false);
	_commManager->setGoal(4, p4, s, t, false);
	_commManager->setGoal(5, p5, s, t, false);
	_commManager->setGoal(6, p6, s, t, false);
	_commManager->setGoal(7, p7, s, t, false);
	_commManager->setGoal(8, p8, s, t, false);
	_commManager->setGoal(9, p9, s, t, false);
	_commManager->setGoal(10, p10, s, t, false);
	_commManager->setGoal(11, p11, s, t, false);
	_commManager->setGoal(12, p12, s, t, false);
	_commManager->setGoal(13, p13, s, t, false);
	_commManager->setGoal(14, p14, s, t, false);
	_commManager->setGoal(15, p15, s, t, false);
	_commManager->setGoal(16, p16, s, t, false);
	_commManager->setGoal(17, p17, s, t, false);
	_commManager->setGoal(18, p18, s, t, false);

	_commManager->synchronize();
}

void AX12Playground::on_btnGoCoord_clicked()
{
	move();
	_hexapod->initOrientation();
}

void AX12Playground::on_btnAuto_clicked()
{
	_hexapod->setAutoWalk(ui->btnAuto->isChecked());
}

void AX12Playground::on_btnAction_clicked()
{
}

void AX12Playground::on_btnCalibrateGyros_clicked()
{
	_um6->calibrateGyros();
}

void AX12Playground::timerRefreshTick()
{
	ui->lblPosition1->setText(QString::number(_commManager->getServoPosition(1)));
	ui->lblLoad1->setText(QString::number(_commManager->getServoLoad(1)));
	ui->lblPosition2->setText(QString::number(_commManager->getServoPosition(2)));
	ui->lblLoad2->setText(QString::number(_commManager->getServoLoad(2)));
	ui->lblPosition3->setText(QString::number(_commManager->getServoPosition(3)));
	ui->lblLoad3->setText(QString::number(_commManager->getServoLoad(3)));
	ui->lblPosition4->setText(QString::number(_commManager->getServoPosition(4)));
	ui->lblLoad4->setText(QString::number(_commManager->getServoLoad(4)));
	ui->lblPosition5->setText(QString::number(_commManager->getServoPosition(5)));
	ui->lblLoad5->setText(QString::number(_commManager->getServoLoad(5)));
	ui->lblPosition6->setText(QString::number(_commManager->getServoPosition(6)));
	ui->lblLoad6->setText(QString::number(_commManager->getServoLoad(6)));
	ui->lblPosition7->setText(QString::number(_commManager->getServoPosition(7)));
	ui->lblLoad7->setText(QString::number(_commManager->getServoLoad(7)));
	ui->lblPosition8->setText(QString::number(_commManager->getServoPosition(8)));
	ui->lblLoad8->setText(QString::number(_commManager->getServoLoad(8)));
	ui->lblPosition9->setText(QString::number(_commManager->getServoPosition(9)));
	ui->lblLoad9->setText(QString::number(_commManager->getServoLoad(9)));
	ui->lblPosition10->setText(QString::number(_commManager->getServoPosition(10)));
	ui->lblLoad10->setText(QString::number(_commManager->getServoLoad(10)));
	ui->lblPosition11->setText(QString::number(_commManager->getServoPosition(11)));
	ui->lblLoad11->setText(QString::number(_commManager->getServoLoad(11)));
	ui->lblPosition12->setText(QString::number(_commManager->getServoPosition(12)));
	ui->lblLoad12->setText(QString::number(_commManager->getServoLoad(12)));
	ui->lblPosition13->setText(QString::number(_commManager->getServoPosition(13)));
	ui->lblLoad13->setText(QString::number(_commManager->getServoLoad(13)));
	ui->lblPosition14->setText(QString::number(_commManager->getServoPosition(14)));
	ui->lblLoad14->setText(QString::number(_commManager->getServoLoad(14)));
	ui->lblPosition15->setText(QString::number(_commManager->getServoPosition(15)));
	ui->lblLoad15->setText(QString::number(_commManager->getServoLoad(15)));
	ui->lblPosition16->setText(QString::number(_commManager->getServoPosition(16)));
	ui->lblLoad16->setText(QString::number(_commManager->getServoLoad(16)));
	ui->lblPosition17->setText(QString::number(_commManager->getServoPosition(17)));
	ui->lblLoad17->setText(QString::number(_commManager->getServoLoad(17)));
	ui->lblPosition18->setText(QString::number(_commManager->getServoPosition(18)));
	ui->lblLoad18->setText(QString::number(_commManager->getServoLoad(18)));

	RPoint coord = _hexapod->getRobotCoord();
	ui->lblCoordX->setText(QString::number(coord.getX(), 'f', 0));
	ui->lblCoordY->setText(QString::number(coord.getY(), 'f', 0));
	ui->lblCoordTheta->setText(QString::number(coord.getThetaDegree(), 'f', 0));

	ui->lblUM6Phi->setText(QString::number(_um6->getPhi(), 'f', 2));
	ui->lblUM6Theta->setText(QString::number(_um6->getTheta(), 'f', 2));
	ui->lblUM6Psi->setText(QString::number(_um6->getPsi(), 'f', 2));
	ui->lblGyrosCalibrated->setText(QString::number(_um6->gyrosCalibrated()));

	/*ui->lblUM6Phi->setText(QString::number(_um6->get16BitRegister(0x62, 0)));
	ui->lblUM6Theta->setText(QString::number(_um6->get16BitRegister(0x62, 1)));*/
}

void AX12Playground::move()
{
	float x = ui->sbMoveCoordX->value();
	float y = ui->sbMoveCoordY->value();
	float z = ui->sbMoveCoordZ->value();
	float xr = ui->sbMoveRotX->value();
	float yr = ui->sbMoveRotY->value();
	float zr = ui->sbMoveRotZ->value();
	float speed = ui->sbMoveCoordSpeed->value();

	_hexapod->moveCenterTo(x, y, z, xr, yr, zr, 100.0);
	_commManager->calculateSmoothServosSpeed(speed);
	_commManager->synchronize();
}
