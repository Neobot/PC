#include "ChessUpConfigurationSelector.h"

ChessUpConfigurationSelector::ChessUpConfigurationSelector(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
}

ChessUpConfiguration::ChessUpConfigurationCard ChessUpConfigurationSelector::getCard1() const
{
    return ui.widgetCard1->getCard();
}

ChessUpConfiguration::ChessUpConfigurationCard ChessUpConfigurationSelector::getCard2() const
{
    return ui.widgetCard2->getCard();
}

ChessUpConfiguration::ChessUpConfigurationCard ChessUpConfigurationSelector::getCard3() const
{
    return ui.widgetCard3->getCard();
}
