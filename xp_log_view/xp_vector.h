#ifndef XP_VECTOR_H
#define XP_VECTOR_H

#include <QFile>
#include <QString>
#include <vector>

struct xp_data
{
    unsigned int    xp;
    double          time;

    QString         name;
    unsigned int    player_level;

    int             map;
    double          x;
    double          y;

    bool            is_quest;

    bool parse_data(QString s);
};

class xp_vector
{
private:
    const QString current_file;

    std::vector<xp_data> vec;
public:
    xp_vector(QFile &f);

    unsigned int size() const { return vec.size(); }

    QString file_name() const { return current_file; }

    xp_data at(unsigned int idx) const { return vec[idx]; }

    typedef std::pair<double,double> type_t;
    void window_average(std::vector<type_t>& fill, double window_size, bool include_mobs, bool include_quests);

};

#endif // XP_VECTOR_H
