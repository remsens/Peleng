# проект типа subdirs
TEMPLATE = subdirs

# список каталогов проектов

#SUBDIRS = HDF5FormatLib ReadDataLib AvirisReadPlugin MainApp 2DHyperCube SpectrPlotter \
 #   LinePlotter HistPlotter 3DHyperCube\

SUBDIRS = MainApp SpectrPlotter LinePlotter 2DHyperCube 3DHyperCube HistPlotter\
    RgbImagePlotter \
    SpectralDistance \
    NoisePlugin \
    SpectralLibPlugin \
    ReadDataPlugin \
    AvirisReadPlugin
