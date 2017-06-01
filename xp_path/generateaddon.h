#ifndef GENERATEADDON_H
#define GENERATEADDON_H

#include <set>
#include <QFile>
#include <QString>
#include "actionitemmodel.h"
#include "vendorconditionmodel.h"

class generateAddon
{
public:
    generateAddon(actionitemmodel & am, vendorConditionModel & dontsell, vendorConditionModel & buy);

    void generate(QString path, QString addonName, QString author, QString notes, QString version);

private:
    actionitemmodel & am;
    vendorConditionModel & dontsell;
    vendorConditionModel & buy;

    QString replaceSymbols(QString s, QString title) const;
    QString replaceVendorSymbols(QString s, QString title) const;

    QString loadActionsTemplate() const;
    void saveActionsToScript(QFile & f, QString addonName);
    void generateActionsXML(QFile & f, QString addonName);
    QString loadVendorActionsTemplate() const;
    void saveVendorActionsToScript(QFile & f, QString addonName);
    void generateVendorActionXML(QFile & f, QString addonName);
    void generateAddonToC(QFile & f, QString addonName, QString author, QString notes, QString version);
    void scanForReferedQuests(std::set<int> &quests, vendorConditionModel & m) const;
    void scanForUsedQuests(std::map<int, int> &quests) const;
    QString generateVendorQuestReference() const;
    QString generateVendorModelData() const;
};

#endif // GENERATEADDON_H
