#ifndef ADDSPECTR_H
#define ADDSPECTR_H

#include "../Library/Spectr.h"

//! Класс, который совершает загрузку спектра из спектральной библиотеки без уведомления пользователя об ошибках
class AddSpectr
{
public:
    AddSpectr();
    virtual ~AddSpectr();
    static Spectr *AddSpectrFromSpectralLib(QString filePath, HyperCube* cube);
};

#endif // ADDSPECTR_H
