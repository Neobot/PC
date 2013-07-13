#ifndef RAWDATAVIEW_H
#define RAWDATAVIEW_H

#include <QWidget>
#include <QItemSelection>

#include "ViewInterface.h"

namespace Ui {
    class RawDataView;
}

class RawDataView : public QWidget, public ViewInterface
{
    Q_OBJECT
    
public:
    explicit RawDataView(QWidget *parent = 0);
    ~RawDataView();

	void addPositionAcquisition(int t, const PositionSample &sample, int ct, const CommandSample &csample);
	void addCommandAcquisition(int t, const CommandSample &sample);
	void addAvoidingSharpAcquisition(int t, const SharpSample &sample);
	void setAcquisition(const Acquisition* acquisition);
	void clear();
    
private:
    Ui::RawDataView *ui;
	bool _selectLast;

private slots:
    void robotSelectionChanged();
    void consigneSelectionChanged();
};

#endif // RAWDATAVIEW_H
