#ifndef SPECTRPLUGIN_H
#define SPECTRPLUGIN_H

#include <QObject>
#include "spectrplugin_global.h"
#include "../Library/HyperCube.h"
#include "../Library/PluginAttributes/SpectrPluginAttributes.h"

class SPECTRLIB_EXPORT SpectrPlugin
{
public:
    SpectrPlugin();
    virtual ~SpectrPlugin();

    void Execute(HyperCube* cube, IAttributes* attr);
};

#endif // SPECTRPLUGIN_H
