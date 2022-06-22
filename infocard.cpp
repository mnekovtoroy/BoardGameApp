#include "infocard.h"

//Constructor
InfoCard::InfoCard(QWidget *parent)
    : QWidget{parent}
{
    cardLayout = new QVBoxLayout(this);

    backButton = new QPushButton("< Назад", this);
    backButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    cardLayout->addWidget(backButton);

    headerLayout = new QHBoxLayout(this);
    name = new QLineEdit(this);
    name->setReadOnly(true);
    name->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Fixed);
    headerLayout->addWidget(name);

    idLabel = new QLabel("0",this);
    idLabel->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);
    headerLayout->addWidget(idLabel);
    headerSpacer = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    headerLayout->addSpacerItem(headerSpacer);

    cardLayout->addLayout(headerLayout);

    tableLayout = new QHBoxLayout(this);
    leftColumn = new QVBoxLayout(this);
    rightColumn = new QVBoxLayout(this);
    tableLayout->addLayout(leftColumn);
    tableLayout->addLayout(rightColumn);
    cardLayout->addLayout(tableLayout);

    this->setLayout(cardLayout);
}
