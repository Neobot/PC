#ifndef CHESSUPCONFIGURATIONSELECTOR_H
#define CHESSUPCONFIGURATIONSELECTOR_H

#include "ui_ChessUpConfigurationSelector.h"
#include "ChessUpConfiguration.h"

class ChessUpConfigurationSelector : public QDialog
{
    Q_OBJECT

public:
    explicit ChessUpConfigurationSelector(QWidget *parent = 0);

    ChessUpConfiguration::ChessUpConfigurationCard getCard1() const;
    ChessUpConfiguration::ChessUpConfigurationCard getCard2() const;
    ChessUpConfiguration::ChessUpConfigurationCard getCard3() const;

private:
    Ui::ChessUpConfigurationSelector ui;
};

#endif // CHESSUPCONFIGURATIONSELECTOR_H
