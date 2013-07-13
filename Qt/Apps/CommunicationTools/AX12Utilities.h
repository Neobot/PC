#ifndef AX12UTILITIES_H
#define AX12UTILITIES_H

#include <QDialog>
#include "AX12CommManager.h"
#include "TextEditLogger.h"

namespace Ui {
	class AX12Utilities;
}

class AX12Utilities : public QDialog, Tools::LoggerInterface
{
	Q_OBJECT
	
public:
	explicit AX12Utilities(QWidget *parent = 0);
	~AX12Utilities();
	
	void closeConnection();

private:
	Ui::AX12Utilities *ui;
	Comm::AX12CommManager* _manager;
	Tools::PlainTextEditLogger* _logger;

	QStringList _remainingBaudrateToScan;

	bool openConnection(const QString &baudrate);

private slots:
	void scan();
	void scanNextBaudrate();
	void scanServosStatusUpdated(quint8 id);
	void scanRequestTimeoutReceived(quint8 id);
	void allIdReceived();
};

#endif // AX12UTILITIES_H
