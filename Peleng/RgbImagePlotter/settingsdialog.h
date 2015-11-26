#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "../Library/HyperCube.h"



namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(HyperCube *cube, QWidget *parent = 0);
    ~SettingsDialog();

    QString getFileName() { return FileName;}
    qint32 getTypeOfMethod() { return TypeOfMethod;}
    qint32 getTypeOfStdProfile() {return TypeOfStdProfile;}
    qint32 getRCh() { return rCh;}
    qint32 getGCh() { return gCh;}
    qint32 getBCh() { return bCh;}





private slots:
    void on_radioButton_clicked(bool checked);

    void on_stdRadioBox_clicked(bool checked);

    void on_rndRadioBox_pressed();

    void on_rndRadioBox_clicked(bool checked);

    void on_chnRadioBox_clicked(bool checked);

    void on_findFileBtn_clicked();



    void on_StdProfileComboBox_currentIndexChanged(int index);

    void on_Ch1ComboBox_currentIndexChanged(int index);

    void on_Ch2ComboBox_activated(int index);

    void on_Ch3ComboBox_activated(int index);



private:
    qint32 TypeOfMethod;
    QString FileName;
    qint32 TypeOfStdProfile;
    qint32 rCh, gCh, bCh;

    Ui::SettingsDialog *ui;


};

#endif // SETTINGSDIALOG_H
