#include "xp_plot.h"
#include "ui_xp_plot.h"

#include <QChartView>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QSplineSeries>
#include "qcustomplot/qcustomplot.h"

XP_Plot::XP_Plot(xp_vector* v, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XP_Plot),
    vec(v)
{
    ui->setupUi(this);
    initialize();
}

XP_Plot::~XP_Plot()
{
    delete ui;
}

void print_rect(QRectF r)
{
    qDebug(QString{QString::number(r.x()) + QString{", "} + QString::number(r.y())
            + QString{", "} + QString::number(r.width()) + QString{", "} + QString::number(r.height())}
           .toLocal8Bit());
}

QCustomPlot * XP_Plot::plot() const { return ui->widget; }

/*
1	Ashenvale 	1	Ashenvale 	1	Ashenvale
2	Azshara 	2	Azshara 	2	Azshara
3	Darkshore 	17	Sombrivage 	6	Dunkelküste
4	Darnassus 	4	Darnassus 	4	Darnassus
5	Desolace 	5	Desolace 	5	Desolace
6	Durotar 	6	Durotar 	7	Durotar
7	Dustwallow Marsh 	11	Marais de Dustwallow 	9	Marschen von Dustwallow
8	Felwood 	8	Gangrebois 	18	Teufelswald
9	Feralas 	7	Feralas 	8	Feralas
10	Moonglade 	15	Reflet-de-Lune 	10	Moonglade
11	Mulgore 	13	Mulgore 	11	Mulgore
12	Orgrimmar 	14	Orgrimmar 	12	Orgrimmar
13	Silithus 	16	Silithus 	13	Silithus
14	Stonetalon Mountains 	9	Les Serres-Rocheuses 	14	Steinkrallengebirge
15	Tanaris 	18	Tanaris 	15	Tanaris
16	Teldrassil 	19	Teldrassil 	17	Teldrassil
17	The Barrens 	10	Les Tarides 	3	Brachland
18	Thousand Needles 	12	Mille pointes 	16	Tausend Nadeln
19	Thunder Bluff	20	Thunder Bluff 	19	Thunder Bluff
20	Un'Goro Crater 	3	Cratère d'Un'Goro 	20	Un'Goro-Krater
21	Winterspring 	21	Winterspring 	21	Winterspring
Eastern Kingdoms
(ger: Östliche Königreiche)
1	Alterac Mountains 	19	Montagnes d'Alterac 	1	Alteracgebirge
2	Arathi Highlands 	11	Hautes-terres d'Arathi 	2	Arathihochland
3	Badlands 	1	Badlands 	24	Ödland
4	Blasted Lands 	2	Blasted Lands 	19	Verwüstete Lande
5	Burning Steppes 	23	Steppes ardentes 	3	Brennende Steppe
6	Deadwind Pass 	5	Col de Deadwind 	6	Gebirgspass der Totenwinde
7	Dun Morogh 	7	Dun Morogh 	4	Dun Morogh
8	Duskwood 	3	Bois de la pénombre 	5	Dämmerwald
9	Eastern Plaguelands 	20	Plaguelands de l'est 	25	Östliche Pestländer
10	Elwynn Forest 	8	Forêt d'Elwynn 	21	Wald von Elwynn
11	Hillsbrad Foothills 	6	Contreforts d'Hillsbrad 	20	Vorgebirge von Hillsbrad
12	Ironforge 	12	Ironforge 	8	Ironforge
13	Loch Modan 	16	Loch Modan 	9	Loch Modan
14	Redridge Mountains 	13	Les Carmines 	10	Rotkammgebirge
15	Searing Gorge 	10	Gorge des Vents brûlants 	12	Sengende Schlucht
16	Silverpine Forest 	9	Forêt des Pins argentés 	13	Silberwald
17	Stormwind City 	4	Cité de Stormwind 	14	Stormwind
18	Stranglethorn Vale 	25	Vallée de Stranglethorn 	11	Schlingendorntal
19	Swamp of Sorrows 	17	Marais des lamentations 	16	Sümpfe des Elends
20	The Hinterlands 	14	Les Hinterlands 	7	Hinterland
21	Tirisfal Glades 	22	Prairies de Tirisfal 	17	Tirisfal
22	Undercity 	24	Undercity 	18	Undercity
23	Western Plaguelands 	21	Plaguelands de l'ouest 	23	Westliche Pestländer
24	Westfall 	18	Marche de l'Ouest 	22	Westfall
25	Wetlands 	15	Les Paluns 	15	Sumpfland
*/

QString map_to_string(int map)
{
    switch(map)
    {
    case 1:
        return QString{"Ashenvale/Alterac Mountains"};
    case 2:
        return QString{"Azshara/Arathi Highlands"};
    case 3:
        return QString{"Darkshore/Badlands"};
    case 4:
        return QString{"Darnassus/Blasted Lands"};
    case 5:
        return QString{"Desolace/Burning Steppes"};
    case 6:
        return QString{"Durotar/Deadwind Pass"};
    case 7:
        return QString{"Dustwallow Marsh/Dun Morogh"};
    case 8:
        return QString{"Felwood/Duskwood"};
    case 9:
        return QString{"Feralas/Eastern Plaguelands"};
    case 10:
        return QString{"Moonglade/Elwynn Forest"};
    case 11:
        return QString{"Mulgore/Hillsbrad Foothills"};
    case 12:
        return QString{"Orgrimmar/Ironforge"};
    case 13:
        return QString{"Silithus/Loch Modan"};
    case 14:
        return QString{"Stonetalon Mountains/Redridge Mountains"};
    case 15:
        return QString{"Tanaris/Searing Gorge"};
    case 16:
        return QString{"Teldrassil/Silverpine Forest"};
    case 17:
        return QString{"The Barrens/Stormwind City"};
    case 18:
        return QString{"Thousand Needles/Stranglethorn Vale"};
    case 19:
        return QString{"Thunder Bluff/Swamp of Sorrows"};
    case 20:
        return QString{"Un'goro Crater/The Hinterlands"};
    case 21:
        return QString{"Winterspring/Tirisfal Glades"};
    case 22:
        return QString{"Undercity"};
    case 23:
        return QString{"Western Plaguelands"};
    case 24:
        return QString{"Westfall"};
    case 25:
        return QString{"Wetlands"};
    default:
        break;
    }
    return QString{"Zone "} + QString::number(map);
}


void XP_Plot::initialize()
{
    plot()->addGraph();
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    plot()->xAxis->setTicker(timeTicker);
    timeTicker->setTimeFormat("%d:%h:%m:%s");
    timeTicker->setTickCount(8);
    QCPBars * mobBars = new QCPBars(plot()->xAxis,plot()->yAxis);
    mobBars->removeFromLegend();
    mobBars->setBrush(QBrush(Qt::green));
    mobBars->setPen(QPen(QColor(Qt::green)));

    QCPBars * questBars = new QCPBars(plot()->xAxis,plot()->yAxis);
    questBars->removeFromLegend();
    questBars->setBrush(QBrush(Qt::blue));
    questBars->setPen(QPen(QColor(Qt::blue)));

    qreal lastEntry = 0.0;
    qreal highest = 0.0;
    QVector<double> xMobData;
    QVector<double> yMobData;

    QVector<double> xQuestData;
    QVector<double> yQuestData;

    for(unsigned int i = 0; i < vec->size(); ++i)
    {
        const xp_data d = vec->at(i);
        qreal t = d.time;
        highest = d.xp > highest ? d.xp : highest;
        if(d.is_quest)
        {
            xQuestData.push_back(d.time);
            yQuestData.push_back(d.xp);
        }
        else
        {
            xMobData.push_back(d.time);
            yMobData.push_back(d.xp);
        }
        lastEntry = t;
    }

    auto zoneData = generateZoneBreakpoints();
    const unsigned int resolution = 30000;
    plot()->addGraph(plot()->xAxis2,plot()->yAxis);
    QCPColorMap * zoneColorMap = new QCPColorMap(plot()->xAxis2,plot()->yAxis);
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    plot()->xAxis2->setTicker(textTicker);
    for(unsigned int i = 0; i < zoneData.size(); ++i)
    {
        textTicker->addTick(zoneData[i].second,map_to_string(zoneData[i].first));
    }
    plot()->xAxis2->setRange(0.0,lastEntry+10);
    plot()->xAxis2->setVisible(true);

    zoneColorMap->data()->setSize(resolution,1);
    zoneColorMap->data()->setRange(QCPRange(0,lastEntry+10),QCPRange(0,highest+100));
    int min = 0;
    int max = 0;
    for(unsigned int i = 0, j = 0; i < resolution; ++i)
    {
        auto t = zoneData[j].second;
        auto c = zoneData[j].first;
        zoneColorMap->data()->setCell(i,0,c);
        while(t < i*(lastEntry+10)/resolution && j < zoneData.size())
        {
            c = zoneData[j].first;
            zoneColorMap->data()->setCell(i,0,c);
            min = c < min ? c : min;
            max = c < max ? max : c;
            ++j;
            if(j >= zoneData.size()){ --j; break; }
            t = zoneData[j].second;
        }
    }
    zoneColorMap->data()->fillAlpha(50);
    zoneColorMap->setDataRange(QCPRange(min,max));
    zoneColorMap->setGradient(QCPColorGradient::gpThermal);
    zoneColorMap->setInterpolate(false);
    zoneColorMap->rescaleDataRange(true);

    mobBars->setData(xMobData,yMobData,true);
    questBars->setData(xQuestData,yQuestData,true);
    plot()->xAxis->setRange(0,lastEntry+10);
    plot()->yAxis->setRange(0,highest+100);
    plot()->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plot()->axisRect()->setRangeDrag(Qt::Horizontal);
    plot()->axisRect()->setRangeZoom(Qt::Horizontal);
    QObject::connect(plot()->xAxis,SIGNAL(rangeChanged(QCPRange)),plot()->xAxis2,SLOT(setRange(QCPRange)));
    plot()->graph()->rescaleAxes();
    plot()->replot();



    plot()->addGraph(plot()->xAxis,plot()->yAxis2);

    std::vector<std::pair<double,double>> longwnd;
    vec->window_average(longwnd,3600*2,true,true);
    QVector<double> xAxisData;
    QVector<double> yAxisData;
    double max2 = 0.0;
    double current = 0.0;
    for(auto dpair : longwnd)
    {
        max2 = max2 < dpair.second ? dpair.second : max2;
        xAxisData.push_back(dpair.first-0.1);
        yAxisData.push_back(current);
        xAxisData.push_back(dpair.first);
        yAxisData.push_back(dpair.second);
        current = dpair.second;
    }
    plot()->graph(2)->setData(xAxisData,yAxisData);
    //plot()->graph(2)->setBrush(QBrush(Qt::yellow));
    plot()->graph(2)->setPen(QPen(QColor(Qt::yellow)));


    plot()->addGraph(plot()->xAxis,plot()->yAxis2);

    plot()->addGraph(plot()->xAxis,plot()->yAxis2);

    std::vector<std::pair<double,double>> mobwnd;
    vec->window_average(mobwnd,900,true,false);
    QVector<double> xAxisData2;
    QVector<double> yAxisData2;
    QVector<double> xAxisData4;
    QVector<double> yAxisData4;
    current = 0.0;
    double prevmax = 0.0;
    double max3 = 0.0;
    for(auto dpair : mobwnd)
    {
        max2 = max2 < dpair.second ? dpair.second : max2;
        max3 = max3 < dpair.second ? dpair.second : max3;
        xAxisData4.push_back(dpair.first-0.1);
        yAxisData4.push_back(prevmax);
        xAxisData4.push_back(dpair.first);
        yAxisData4.push_back(max3);

        xAxisData2.push_back(dpair.first-0.1);
        yAxisData2.push_back(current);
        xAxisData2.push_back(dpair.first);
        yAxisData2.push_back(dpair.second);
        current = dpair.second;
        prevmax = max3;
    }
    plot()->graph(3)->setData(xAxisData2,yAxisData2);
    plot()->graph(3)->setPen(QPen(QColor(Qt::green)));
    plot()->graph(4)->setData(xAxisData4,yAxisData4);
    plot()->graph(4)->setPen(QPen(QColor(Qt::red)));


    plot()->addGraph(plot()->xAxis,plot()->yAxis2);

    std::vector<std::pair<double,double>> questwnd;
    vec->window_average(questwnd,900,false,true);
    QVector<double> xAxisData3;
    QVector<double> yAxisData3;
    current = 0.0;
    for(auto dpair : questwnd)
    {
        xAxisData3.push_back(dpair.first-0.1);
        yAxisData3.push_back(current);
        xAxisData3.push_back(dpair.first);
        yAxisData3.push_back(dpair.second);
        current = dpair.second;
    }
    plot()->graph(5)->setData(xAxisData3,yAxisData3);
    plot()->graph(5)->setPen(QPen(QColor(Qt::darkCyan)));

    plot()->addGraph(plot()->xAxis,plot()->yAxis2);

    QVector<double> xReference;
    QVector<double> yReference;
    unsigned int curLvl = 0;
    for(unsigned int i = 0; i < vec->size(); ++i)
    {
        const xp_data d = vec->at(i);
        bool is_dif = curLvl != d.player_level;
        if(is_dif)
        {
            curLvl = d.player_level;
            QCPItemText *levelText = new QCPItemText(plot());
            levelText->position->setAxes(plot()->xAxis,plot()->yAxis2);
            levelText->position->setType(QCPItemPosition::ptPlotCoords);
            levelText->position->setCoords(d.time,3*60*(45+5*d.player_level));
            levelText->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
            levelText->setText(QString::number(d.player_level));
            levelText->setFont(QFont(font().family(), 10));
        }
        qreal t = d.time;
        xReference.push_back(t);
        yReference.push_back(3*60*(45+5*d.player_level));  // 3 mobs per minute an hour
    }
    plot()->graph(6)->setData(xReference,yReference);
    plot()->graph(6)->setPen(QPen(QBrush(Qt::darkYellow),1,Qt::DashLine));

    plot()->yAxis2->setRange(0,max2);
    plot()->yAxis2->setVisible(true);
    plot()->yAxis2->setLabel("XP/hr");
    plot()->yAxis2->ticker()->setTickCount(15);


}

std::vector<std::pair<int,double>> XP_Plot::generateZoneBreakpoints()
{
    typedef std::pair<int,double> pair_t;
    std::vector<pair_t> ret;

    int current_map = vec->at(0).map;
    pair_t first{current_map,0.0};
    ret.push_back(first);
    for(unsigned int i = 0; i < vec->size(); ++i)
    {
        const xp_data d = vec->at(i);
        if(d.map != current_map)
        {
            current_map = d.map;
            pair_t p{current_map,d.time};
            ret.push_back(p);
        }
    }
    return ret;
}
