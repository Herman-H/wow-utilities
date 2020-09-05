#include "banktransactionselecter.h"
#include "ui_banktransactionselecter.h"

#include "itemselecter.h"
#include <QSpinBox>
#include <QPushButton>

BankTransactionSelecter::BankTransactionSelecter(QSqlDatabase& db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BankTransactionSelecter),
    m_db(db),
    m_isSelected(false)
{
    ui->setupUi(this);
    this->setWindowTitle("Set Banking Transaction");

    auto toBankLayout = ui->toBank->layout();
    auto fromBankLayout = ui->fromBank->layout();

    m_currentAddToBank = new QPushButton();
    m_currentAddToBank->setText("+");
    m_currentAddToBank->setContentsMargins(0,0,0,0);
    m_currentAddToBank->setStyleSheet(QString{"border: none;"});
    toBankLayout->addWidget(m_currentAddToBank);

    m_currentAddFromBank = new QPushButton();
    m_currentAddFromBank->setText("+");
    m_currentAddFromBank->setContentsMargins(0,0,0,0);
    m_currentAddFromBank->setStyleSheet(QString{"border: none;"});
    fromBankLayout->addWidget(m_currentAddFromBank);

    QObject::connect(m_currentAddToBank,SIGNAL(clicked(bool)),this,SLOT(onAddToButtonPushed()));
    QObject::connect(m_currentAddFromBank,SIGNAL(clicked(bool)),this,SLOT(onAddFromButtonPushed()));
    QObject::connect(ui->bankerNameEdit,&QLineEdit::textChanged,[=]()
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isComplete());
    });
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    QObject::connect(this,&BankTransactionSelecter::accepted,[=]()
    {
        this->m_isSelected = true;
    });
}

BankTransactionSelecter::~BankTransactionSelecter()
{
    for (auto it = m_tos.begin(); it != m_tos.end(); ++it)
    {
        delete (*it);
    }
    for (auto it = m_froms.begin(); it != m_froms.end(); ++it)
    {
        delete (*it);
    }
    delete ui;
}

widget_entry_to *BankTransactionSelecter::onAddToButtonPushed()
{
    QHBoxLayout* toBankLayout = static_cast<QHBoxLayout*>(ui->toBank->layout());

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    widget_entry_to * entry = new widget_entry_to{};
    entry->itemId = -1;
    entry->widget = new QWidget{};
    entry->filter = new BankFilterToFunctionSelecter{};
    entry->spinbox = new QSpinBox{};
    entry->item = new QPushButton{"Item..."};
    entry->remove = new QPushButton{};
    entry->remove->setIcon(QApplication::style()->standardIcon(QStyle::SP_LineEditClearButton));
    entry->widget->setLayout(new QHBoxLayout{});
    entry->widget->layout()->setSpacing(0);
    entry->widget->layout()->setMargin(0);
    entry->widget->layout()->addWidget(entry->filter);
    entry->widget->layout()->addWidget(entry->spinbox);
    entry->widget->layout()->addWidget(new QLabel{" of "});
    entry->widget->layout()->addWidget(entry->item);
    entry->widget->layout()->addWidget(entry->remove);
    entry->remove->setStyleSheet({"border:none;"});
    toBankLayout->insertWidget(toBankLayout->count()-1,entry->widget);
    this->m_tos.insert(entry);
    entry->spinbox->hide();
    QObject::connect(
        entry->filter,
        static_cast<void (BankFilterToFunctionSelecter::*)(int)>(&BankFilterToFunctionSelecter::currentIndexChanged),
        [=]()
    {
        if(entry->filter->currentIndex() == 0)
        {
            entry->spinbox->hide();
        }
        else
        {
            entry->spinbox->show();
        }
    });

    QObject::connect(
        entry->item,
        &QPushButton::clicked,
        [=]()
    {
        itemselecter is{m_db};
        is.exec();
        if(is.isSelected())
        {
            entry->itemId = is.selectedItemId();
            entry->item->setText(is.getItemName(entry->itemId));
        }
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isComplete());
    });

    QObject::connect(
        entry->remove,
        &QPushButton::clicked,
        [=]()
    {
        toBankLayout->removeWidget(entry->widget);
        delete entry->widget;
        this->m_tos.erase(entry);
        delete entry;
    });
    return entry;
}

widget_entry_from * BankTransactionSelecter::onAddFromButtonPushed()
{
    QHBoxLayout* fromBankLayout = static_cast<QHBoxLayout*>(ui->fromBank->layout());

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    widget_entry_from * entry = new widget_entry_from{};
    entry->itemId = -1;
    entry->widget = new QWidget{};
    entry->filter = new BankFilterFromFunctionSelecter{};
    entry->spinbox = new QSpinBox{};
    entry->item = new QPushButton{"Item..."};
    entry->remove = new QPushButton{};
    entry->remove->setIcon(QApplication::style()->standardIcon(QStyle::SP_LineEditClearButton));
    entry->widget->setLayout(new QHBoxLayout{});
    entry->widget->layout()->setSpacing(0);
    entry->widget->layout()->setMargin(0);
    entry->widget->layout()->addWidget(entry->filter);
    entry->widget->layout()->addWidget(entry->spinbox);
    entry->widget->layout()->addWidget(new QLabel{" of "});
    entry->widget->layout()->addWidget(entry->item);
    entry->widget->layout()->addWidget(entry->remove);
    entry->remove->setStyleSheet({"border:none;"});
    fromBankLayout->insertWidget(fromBankLayout->count()-1,entry->widget);
    this->m_froms.insert(entry);
    entry->spinbox->hide();
    QObject::connect(
        entry->filter,
        static_cast<void (BankFilterFromFunctionSelecter::*)(int)>(&BankFilterFromFunctionSelecter::currentIndexChanged),
        [=]()
    {
        if(entry->filter->currentIndex() == 0)
        {
            entry->spinbox->hide();
        }
        else
        {
            entry->spinbox->show();
        }
    });

    QObject::connect(
        entry->item,
        &QPushButton::clicked,
        [=]()
    {
        itemselecter is{m_db};
        is.exec();
        if(is.isSelected())
        {
            entry->itemId = is.selectedItemId();
            entry->item->setText(is.getItemName(entry->itemId));
        }

        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isComplete());
    });

    QObject::connect(
        entry->remove,
        &QPushButton::clicked,
        [=]()
    {
        fromBankLayout->removeWidget(entry->widget);
        delete entry->widget;
        this->m_froms.erase(entry);
        delete entry;
    });
    return entry;
}

bool BankTransactionSelecter::isComplete() const
{
    bool r = ui->bankerNameEdit->text().length() > 0;
    for (auto it = m_tos.begin(); it != m_tos.end(); ++it)
    {
        r = r && ((*it)->itemId > 0);
    }
    for (auto it = m_froms.begin(); it != m_froms.end(); ++it)
    {
        r = r && ((*it)->itemId > 0);
    }

    return r;
}

bank_transaction BankTransactionSelecter::selectedTransaction() const
{
    bank_transaction transaction;
    transaction.banker_name = ui->bankerNameEdit->text();
    for (auto it = m_tos.begin(); it != m_tos.end(); ++it)
    {
        bank_atom_to atom;
        atom.itemId = (*it)->itemId;
        atom.param = (*it)->spinbox->value();
        atom.to = (bank_filter_function_to)(*it)->filter->currentIndex();
        transaction.to_bank.push_back(atom);
    }
    for (auto it = m_froms.begin(); it != m_froms.end(); ++it)
    {
        bank_atom_from atom;
        atom.itemId = (*it)->itemId;
        atom.param = (*it)->spinbox->value();
        atom.from = (bank_filter_function_from)(*it)->filter->currentIndex();
        transaction.from_bank.push_back(atom);
    }
    return transaction;
}

void BankTransactionSelecter::setTransaction(const bank_transaction* transaction)
{
    // should probably be a constructor instead

    for(unsigned int i = 0; i < transaction->to_bank.size(); ++i)
    {
        bank_atom_to atom = transaction->to_bank[i];
        widget_entry_to * entry = onAddToButtonPushed();
        entry->filter->setCurrentIndex(static_cast<int>(atom.to));
        entry->spinbox->setValue(atom.param);
        itemselecter is{m_db};
        entry->item->setText(is.getItemName(atom.itemId));
        entry->itemId = atom.itemId;

    }
    for(unsigned int i = 0; i < transaction->from_bank.size(); ++i)
    {
        bank_atom_from atom = transaction->from_bank[i];
        widget_entry_from * entry = onAddFromButtonPushed();
        entry->filter->setCurrentIndex(static_cast<int>(atom.from));
        entry->spinbox->setValue(atom.param);
        itemselecter is{m_db};
        entry->item->setText(is.getItemName(atom.itemId));
        entry->itemId = atom.itemId;
    }

    ui->bankerNameEdit->setText(transaction->banker_name);
}
