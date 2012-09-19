#include "popup_neco.h"
#include "ui_popup_neco.h"

Popup_neco::Popup_neco(QWidget *parent,int pos_) :
    QWidget(parent),
    ui(new Ui::Popup_neco)
{
    ui->setupUi(this);

    //ui->edit_name->setFrameStyle(0);
    //ui->edit_zblytek->setFrameStyle(0);

    setAttribute(Qt::WA_ShowWithoutActivating);
    setWindowFlags( Qt::WindowStaysOnTopHint | Qt::ToolTip );

    connect(ui->edit_name,SIGNAL(clicked()),this,SLOT(clicked()));
    connect(ui->edit_zblytek,SIGNAL(clicked()),this,SLOT(clicked()));

    timer = new QTimer();

#ifdef QT_NO_DEBUG
    connect(timer, SIGNAL(timeout()), this, SLOT(clicked()));
#endif

    timer->setInterval(8000);
    timer->setSingleShot(true);
    timer->start();

    pos = pos_;
}

Popup_neco::~Popup_neco()
{
    delete ui;
}

void Popup_neco::mousePressEvent(QMouseEvent *evt)
{
    clicked();
}


void Popup_neco::clicked()
{
    emit wantDestroy(this);
}

void Popup_neco::SetText(QString &header, QString &text)
{
    ui->edit_name->setText(header);
    ui->edit_zblytek->setText(text);
}
