#include "./playstab/playslistwidget.h"
#include "./playstab/playsminicard.h"
#include "./playstab/newplaydialog.h"
#include "./general/usefulfunctions.h"
#include <QtSql>

PlaysListWidget::PlaysListWidget(QWidget *parent)
    : QWidget{parent}
{
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(10);

    //Header setup
    QHBoxLayout* header = new QHBoxLayout();
    header->addWidget(new QLabel("Дата"), 12, Qt::AlignHCenter);
    header->addWidget(new QLabel("Игра"), 16, Qt::AlignHCenter);
    header->addWidget(new QLabel("Игроки"), 21, Qt::AlignHCenter);
    header->addWidget(new QLabel("Победитель"), 28, Qt::AlignHCenter);
    header->addWidget(new QLabel("Комментарий"), 23, Qt::AlignHCenter);
    layout->addLayout(header);

    //New play button setup
    new_play_button = new QPushButton("Записать новую партию", this);
    layout->addWidget(new_play_button);

    //Scroll area setup
    plays_list_area = new QScrollArea(this);
    plays_list_area->setWidgetResizable(true); //IMPORTANT
    scrollAreaContent = new QWidget(plays_list_area);
    plays_list_layout = new QVBoxLayout();
    plays_list_layout->setContentsMargins(0,0,0,0);
    scrollAreaContent->setLayout(plays_list_layout);
    plays_list_area->setWidget(scrollAreaContent);
    plays_list_area->setFrameShape(QFrame::NoFrame);
    layout->addWidget(plays_list_area);

    update_plays_list();

    connect(new_play_button,SIGNAL(clicked()),this,SLOT(new_play_button_clicked()));
}

void PlaysListWidget::update_plays_list()
{
    clearLayout(plays_list_layout);
    QSqlQuery query;
    query.prepare("SELECT play_id "
                  "FROM play "
                  "ORDER BY play_date DESC;");
    if (!query.exec()) {
        qDebug() << query.lastError();
        throw std::invalid_argument("Unable to select from `play` table");
    }

    while(query.next()) {
        plays_list_layout->addWidget(new PlaysMiniCard(this,query.value(0).toInt()));
    }
    plays_list_layout->addStretch();
}

void PlaysListWidget::new_play_button_clicked()
{
    NewPlayDialog* new_play_form = new NewPlayDialog();
    if (new_play_form->exec() == QDialog::Accepted) {
        update_plays_list();
    }
}
