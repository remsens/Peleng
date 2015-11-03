#ifndef ADDSPECTR_H
#define ADDSPECTR_H

#include <QWidget>
#include <../Library/Attributes/Attributes.h>
#include <../Library/HyperCube.h>

namespace Ui {
class AddSpectr;
}

class AddSpectr : public QWidget
{
    Q_OBJECT

public:
    explicit AddSpectr(HyperCube* cube, Attributes* attr, QWidget *parent = 0);
    virtual ~AddSpectr();

public slots:
    void OnPushButtonImportClicked();
private:
    void ParseFile(QStringList& possibleTitles, QString& filePath);

private:
    Ui::AddSpectr *m_ui;
    HyperCube* m_cube;
    Attributes* m_attributes;
};

#endif // ADDSPECTR_H
