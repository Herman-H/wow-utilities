#include "xp_vector.h"
#include <map>
#include <QRegularExpression>
#include <array>

// \[(\d+)\]\s*=\s*{([^}]+), regexp all entries
// \[\"(.+)\"\]\s*=\s*(\d+[\.[\d]*]?|\".+\"), regexp for each line

bool xp_data::parse_data(QString s)
{
    QRegularExpression re{"\\[\\\"(.+)\\\"\]\\s*=\\s*(\\d+[\\.[\\d]*]?|\\\".+\\\")"};
    std::map<QString, QString> map_;
    auto it = re.globalMatch(s);

    while(it.hasNext())
    {
        auto match = it.next();
        QString id = match.captured(1);
        QString data = match.captured(2);

        map_[id] = data;
    }

    QString id_xp{"XP"};
    QString id_time{"Time"};
    QString id_name{"Name"};
    QString id_lvl{"PlayerLevel"};
    QString id_map{"Map"};
    QString id_x{"X"};
    QString id_y{"Y"};
    QString id_is_quest{"IsQuest"};

    if(map_.find("XP") != map_.end()) { xp = map_[id_xp].toInt(); } else { xp = 0; }
    if(map_.find("Time") != map_.end()) { time = map_[id_time].toDouble(); } else { time = -1.0; }
    if(map_.find("Name") != map_.end()) { auto s = map_[id_name]; name = s.mid(1,s.length()-2); } else { name = "Unknown"; }
    if(map_.find("PlayerLevel") != map_.end()) { player_level = map_[id_lvl].toInt(); } else { player_level = 0; }
    if(map_.find("Map") != map_.end()) { map = map_[id_map].toInt(); } else { map = 0; }
    if(map_.find("X") != map_.end()) { x = map_[id_x].toDouble(); } else { x = 0.0; }
    if(map_.find("Y") != map_.end()) { y = map_[id_y].toDouble(); } else { y = 0.0; }
    if(map_.find("IsQuest") != map_.end()) { is_quest = map_[id_is_quest].toInt(); } else { is_quest = false; }

    return xp != 0 && time > 0 && player_level > 0;
}

xp_vector::xp_vector(QFile &f) :
    current_file(f.fileName())
{
    std::vector<QChar> data;
    unsigned int size = f.size();
    data.reserve(size);

    QString file_string{f.readAll()};

    QRegularExpression re{"\\[(\\d+)\\]\\s*=\\s*{([^}]+)"};
    std::map<unsigned int, xp_data> temp;
    auto it1 = re.globalMatch(file_string);
    while(it1.hasNext())
    {
        auto match = it1.next();
        unsigned int index = match.captured(1).toInt();
        xp_data d;
        d.parse_data(match.captured(2));
        temp[index] = d;
    }

    auto it2 = temp.begin();
    while(it2 != temp.end())
    {
        xp_data d = (*it2).second;
        vec.push_back(d);
        ++it2;
    }
}


void xp_vector::window_average(std::vector<type_t>& fill, double window_size, bool include_mobs, bool include_quests)
{
    fill.clear();

    fill.push_back(type_t{0.0,0.0});
    unsigned int front_idx = 0;
    unsigned int back_idx = 0;

    double acc = 0;

    auto next_idx = [&](unsigned int start) -> unsigned int
    {
        while(start < vec.size() && !(vec[start].is_quest && include_quests) && !(!(vec[start].is_quest) && include_mobs))
        {
            ++start;
        }
        return start;
    };

    auto time = [&](unsigned int idx) -> double
    {
        return vec[idx].time;
    };

    auto xp = [&](unsigned int idx) -> unsigned int
    {
        return vec[idx].xp;
    };

    auto wnd_sz = [&](unsigned int idx) -> double
    {
        return vec[idx].time < window_size ? vec[idx].time : window_size;
    };

    front_idx = next_idx(front_idx);

    while(front_idx < vec.size())
    {
        if(time(front_idx) - time(back_idx) < window_size)
        {
            acc += xp(front_idx);
            fill.push_back(type_t{time(front_idx),3600*acc/wnd_sz(front_idx)});
            front_idx = next_idx(++front_idx);
        }
        else
        {
            acc -= xp(back_idx);
            fill.push_back(type_t{time(back_idx)+window_size,3600*acc/window_size});
            back_idx = next_idx(++back_idx);
        }
    }

}
