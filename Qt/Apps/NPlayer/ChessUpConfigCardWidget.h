#ifndef CHESSUPCONFIGCARDWIDGET_H
#define CHESSUPCONFIGCARDWIDGET_H

#include "ui_ChessUpConfigCardWidget.h"
#include "ChessUpConfiguration.h"

class ChessUpConfigCardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChessUpConfigCardWidget(QWidget *parent = 0);

    ChessUpConfiguration::ChessUpConfigurationCard getCard() const;

private:
    Ui::ChessUpConfigCardWidget ui;
};

#endif // CHESSUPCONFIGCARDWIDGET_H
