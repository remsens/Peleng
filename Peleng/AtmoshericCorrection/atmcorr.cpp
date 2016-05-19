#include "atmcorr.h"
#include "ui_atmcorr.h"\

#include <QFile>

AtmCorr::AtmCorr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AtmCorr)
{
    ui->setupUi(this);



}

AtmCorr::~AtmCorr()
{
    delete ui;
}


#ifdef CALC
QList<QByteArray> AtmCorr::calcLibRadTran(float albedo, float sza, float umu,
                                float zout, QString output_user, bool isTransmitance=false) {

    QProcess libRadTran;
    libRadTran.setWorkingDirectory("D:\\libRadtran-2.0\\example");
    libRadTran.start("uvspec.exe");

    libRadTran.write("atmosphere_file US-standard\n");
    libRadTran.write("source solar ../data/solar_flux/kurudz_1.0nm.dat\n");
    libRadTran.write(QString("albedo %1\n").arg(albedo).toStdString().c_str());
    libRadTran.write(QString("sza %1\n").arg(sza).toStdString().c_str());
    libRadTran.write("rte_solver sdisort\n");

    libRadTran.write("wavelength_grid_file calc_wl.txt\n");

    libRadTran.write("wavelength 365 2500\n");
    libRadTran.write("mol_abs_param LOWTRAN\n");
    libRadTran.write("number_of_streams 16\n");

    libRadTran.write("spline_file aviris_wl.txt\n");

    libRadTran.write(QString("umu %1\n").arg(umu).toStdString().c_str());
    libRadTran.write(QString("zout %1\n").arg(zout).toStdString().c_str());
//    if (isTransmitance) libRadTran.write("output_quantity transmittance\n");
    libRadTran.write("output_quantity transmittance\n");

    libRadTran.write(QString("output_user %1\n").arg(output_user).toStdString().c_str());

    libRadTran.closeWriteChannel();
    libRadTran.waitForFinished(300000);

    return libRadTran.readAll().split('\n');
}
#endif

int AtmCorr::readFromFile(QString FileName, QList<QByteArray> &arr) {
    QFile file(FileName);
       if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
           return 1;
    arr = file.readAll().split('\n');
    file.close();
    return 0;
}






int AtmCorr::makeCorrection() {


    float sza_i = 60;
    int zout = 20;

    QFile file(QString("d:\\data\\%1_%2_1.out").arg(sza_i).arg(zout));
       if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
           return 1;

    QList<double> wl, uu, Edir, Edn;

    QTextStream in(&file);
    QString line = in.readLine();
       while (!line.isNull()) {
           wl.append(line.trimmed().split(' ')[0].toDouble());
           uu.append(line.trimmed().split(' ')[3].toDouble());
           Edir.append(line.trimmed().split(' ')[5].toDouble());
           Edn.append(line.trimmed().split(' ')[7].toDouble());
           line = in.readLine();


       }
    file.close();


    qDebug() << Edn;

    QList<double> Lu;



    out_str = calcLibRadTran(albedo,sza,umu,zout,output_user);

    for (int i = 0; i < out_str.size(); i++) {
        QList<QByteArray> split_string = out_str[i].trimmed().split(' ');
        if (split_string[0].size()<1) continue;
        Lu.append(split_string[0].toDouble());
    }

    qDebug() << Lu;
    sza = 10;
    umu = -1;
    zout = 0.01;
    output_user = "edir";
     QList<double> tau;



    out_str = calcLibRadTran(albedo,sza,umu,zout,output_user,true);

    for (int i = 0; i < out_str.size(); i++) {
        QList<QByteArray> split_string = out_str[i].trimmed().split(' ');
        if (split_string[0].size()<1) continue;
        qDebug() << split_string;
        tau.append(split_string[0].toDouble());
    }

    qDebug() << tau;
    QList<double> r;
    for (int i =0; i < uuu.size(); i++) {
        r.append((uuu[i]-Lu[i])/(Edir[i]+Edn[i])*3.14/tau[i]);
    }
    qDebug() << r;
}
