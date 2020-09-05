#ifndef GENERATEADDON_H
#define GENERATEADDON_H

#include <set>
#include <QFile>
#include <QString>
#include "actionitemmodel.h"
#include "vendorconditionmodel.h"
#include <vector>
#include "trainertablemodel.h"

class generateAddon
{
public:
    generateAddon(actiontablemodel & atm, vendorConditionModel & dontsell, vendorConditionModel & buy, std::vector<trainertablemodel*> & trainers);

    void generate112(QString path, QString addonName, QString author, QString notes, QString version);
    void generateClassic(QString path, QString addonName, QString author, QString notes, QString version);

private:
    actiontablemodel & atm;
    vendorConditionModel & dontsell;
    vendorConditionModel & buy;
    std::vector<trainertablemodel*> & trainers;

    QString replaceSymbols(QString s, QString title) const;
    QString replaceVendorSymbols(QString s, QString title) const;

    QString trainerData() const;

    QString loadActionsTemplate(QString version) const;
    void saveActionsToScript(QString version, QFile & f, QString addonName);
    void generateActionsXML(QFile & f, QString addonName);
    void generateActionsClassicXML(QFile & f, QString addonName);
    QString loadVendorActionsTemplate(QString version) const;
    void saveVendorActionsToScript(QString version, QFile & f, QString addonName);
    void generateVendorActionXML(QFile & f, QString addonName);
    void generateVendorActionClassicXML(QFile & f, QString addonName);
    void generateAddonToC(QFile & f, QString addonName, QString author, QString notes, QString version);
    void generateAddonClassicToC(QFile & f, QString addonName, QString author, QString notes, QString version);
    void scanForReferedQuests(std::set<int> &quests, vendorConditionModel & m) const;
    void scanForUsedQuests(std::map<int, int> &quests) const;
    QString generateVendorQuestReference() const;
    QString generateVendorModelData() const;
};

#endif // GENERATEADDON_H
