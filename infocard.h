#ifndef INFOCARD_H
#define INFOCARD_H

#include <QWidget>
#include <QtWidgets>

class InfoCard : public QWidget
{
    Q_OBJECT

private:
    QVBoxLayout* cardLayout;
    QHBoxLayout* tableLayout;

protected:
    QHBoxLayout* headerLayout;



public:
    QPushButton* backButton;
    QLineEdit* name;
    QLabel* idLabel;
    QSpacerItem* headerSpacer;
    QVBoxLayout* leftColumn;
    QVBoxLayout* rightColumn;

    explicit InfoCard(QWidget *parent = nullptr);

};

#endif // INFOCARD_H
