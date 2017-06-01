#include "actionitemmodel.h"

actionitemmodel::actionitemmodel(questData & d) :
    d(d)
{
    model = new QStandardItemModel{0,1};
}

void actionitemmodel::setItemBackground(QStandardItem *item, action_type type)
{
    switch(type)
    {
    case action_type::pick_up_quest:
        item->setBackground(QBrush{QColor{225,225,0,100}});
        break;
    case action_type::return_quest:
        item->setBackground(QBrush{QColor{0,225,0,100}});
        break;
    case action_type::set_hearthstone:
        item->setBackground(QBrush{QColor{0,225,225,100}});
        break;
    case action_type::use_hearthstone:
        item->setBackground(QBrush{QColor{255,0,225,100}});
        break;
    case action_type::discover_flightmaster:
        item->setBackground(QBrush{QColor{128,0,0,100}});
        break;
    case action_type::use_flightmaster:
        item->setBackground(QBrush{QColor{255,0,0,100}});
        break;
    case action_type::vendor:
        item->setBackground(QBrush{QColor{128,128,128,100}});
        break;
    case action_type::training:
        item->setBackground(QBrush{QColor{128,64,255,100}});
        break;
    case action_type::reach_xp_breakpoint:
        item->setBackground(QBrush{QColor{128,0,128}});
    case action_type::next_comment:
        item->setBackground(QBrush{QColor{0,0,0,100}});
        break;
    }
}

void actionitemmodel::insertItemOnRow(int row, action a)
{
    QStandardItem *desc,*type,*questId,*hs,*qr,*fp,*fpt,*vend;
    desc = new QStandardItem{actionLabel(a,d)};
    desc->setEditable(false);
    setItemBackground(desc,a.type);
    type = new QStandardItem{};
    questId = new QStandardItem{};
    hs = new QStandardItem{};
    qr = new QStandardItem{};
    fp = new QStandardItem{};
    fpt = new QStandardItem{};
    vend = new QStandardItem{};
    type->setData((int)a.type);
    questId->setData(a.questId);
    hs->setData((int)a.hs);
    qr->setData(a.itemrewardchoice);
    fp->setData((int)a.flightmaster);
    fpt->setData((int)a.flightmaster_target);
    vend->setData(a.vendorName);
    QList<QStandardItem*> l;
    l.push_back(desc);
    l.push_back(type);
    l.push_back(questId);
    l.push_back(hs);
    l.push_back(qr);
    l.push_back(fp);
    l.push_back(fpt);
    l.push_back(vend);
    model->insertRow(row,l);
}

action actionitemmodel::getSelectedAction(const QModelIndex &index)
{
    action a;
    auto type = model->item(index.row(),1)->data().toInt();
    auto questid = model->item(index.row(),2)->data().toInt();
    auto hs = model->item(index.row(),3)->data().toInt();
    auto qr = model->item(index.row(),4)->data().toInt();
    auto fp = model->item(index.row(),5)->data().toInt();
    auto fpt = model->item(index.row(),6)->data().toInt();
    auto vend = model->item(index.row(),7)->data().toString();
    a.type = (action_type)type;
    a.questId = questid;
    a.hs = (hearthstone_location)hs;
    a.itemrewardchoice = qr;
    a.flightmaster = (flightmaster_location)fp;
    a.flightmaster_target = (flightmaster_location)fpt;
    a.vendorName = vend;

    return a;
}

void actionitemmodel::loadActionsFromFile(QFile &f)
{
    f.open(QIODevice::ReadOnly);
    size_t sz = f.size();
    alignas (sizeof(int)) char data[f.size()];
    f.read(data,f.size());
    f.close();
    int row = 0;
    int * ptr = reinterpret_cast<int*>(data);
    while(reinterpret_cast<ptrdiff_t>(ptr) < reinterpret_cast<ptrdiff_t>(data) + static_cast<ptrdiff_t>(sz))
    {
        action a;
        a.type = (action_type)(*ptr);
        switch(a.type)
        {
        case action_type::pick_up_quest:
            ++ptr;
            a.questId = *ptr;
            break;
        case action_type::return_quest:
            ++ptr;
            a.questId = *ptr;
            ++ptr;
            a.itemrewardchoice = *ptr;
            break;
        case action_type::set_hearthstone:
            ++ptr;
            a.hs = (hearthstone_location)(*ptr);
            break;
        case action_type::use_hearthstone:
            ++ptr;
            a.hs = (hearthstone_location)(*ptr);
            break;
        case action_type::discover_flightmaster:
            ++ptr;
            a.flightmaster = (flightmaster_location)(*ptr);
            break;
        case action_type::use_flightmaster:
            ++ptr;
            a.flightmaster = (flightmaster_location)(*ptr);
            ++ptr;
            a.flightmaster_target = (flightmaster_location)(*ptr);
            break;
        case action_type::vendor:
        case action_type::training:
        case action_type::next_comment:
        {
            ++ptr;
            std::size_t sz = *ptr;
            ++ptr;
            a.vendorName = "";
            char * cptr = reinterpret_cast<char*>(ptr);
            for(std::size_t i = 0; i < sz; ++i)
            {
                a.vendorName.append(*cptr);
                cptr++;
            }
            ptr += sz * sizeof(char) / sizeof(int);
        }
            break;
        case action_type::reach_xp_breakpoint:
            ++ptr;
            a.setLevel(*ptr);
            ++ptr;
            a.setXp(*ptr);
            break;
        }
        insertItemOnRow(row,a);
        ++ptr;
        ++row;
    }
}

void actionitemmodel::saveActionsToFile(QFile &f)
{
    // Actions are saved in format [action_type,X,X,X,X,X...] where action_type determines the remainder of the entry
    std::vector<int> vec;

    for(int i = 0; i < model->rowCount(); ++i)
    {
        int type = model->item(i,1)->data().toInt();
        vec.push_back(type);
        switch((action_type)type)
        {
        case action_type::pick_up_quest:
            vec.push_back(model->item(i,2)->data().toInt());
            break;
        case action_type::return_quest:
            vec.push_back(model->item(i,2)->data().toInt());
            vec.push_back(model->item(i,4)->data().toInt());
            break;
        case action_type::set_hearthstone:
            vec.push_back(model->item(i,3)->data().toInt());
            break;
        case action_type::use_hearthstone:
            vec.push_back(model->item(i,3)->data().toInt());
            break;
        case action_type::discover_flightmaster:
            vec.push_back(model->item(i,5)->data().toInt());
            break;
        case action_type::use_flightmaster:
            vec.push_back(model->item(i,5)->data().toInt());
            vec.push_back(model->item(i,6)->data().toInt());
            break;
        case action_type::vendor:
        case action_type::training:
        case action_type::next_comment:
        {
            QString str = model->item(i,7)->data().toString();
            std::size_t sz = str.size();
            vec.push_back((int)sz);
            std::size_t idx = vec.size();
            for(int i = 0; i < sz * sizeof(char)/sizeof(int) + 1; ++i)
            {
                vec.push_back(0);
            }
            memcpy(reinterpret_cast<char*>(vec.data() + idx),str.toStdString().c_str(),sz);
        }
            break;

        case action_type::reach_xp_breakpoint:
            vec.push_back(model->item(i,2)->data().toInt());
            vec.push_back(model->item(i,4)->data().toInt());
            break;
        }
    }
    f.open(QIODevice::WriteOnly);
    f.write(reinterpret_cast<char*>(vec.data()),vec.size()*sizeof(int)/sizeof(char));
    f.close();

}

