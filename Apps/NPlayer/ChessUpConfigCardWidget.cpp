#include "ChessUpConfigCardWidget.h"

ChessUpConfigCardWidget::ChessUpConfigCardWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
}

ChessUpConfiguration::ChessUpConfigurationCard ChessUpConfigCardWidget::getCard() const
{
    return ChessUpConfiguration::ChessUpConfigurationCard(ui.spQueen->value(), ui.spKing->value());
}
