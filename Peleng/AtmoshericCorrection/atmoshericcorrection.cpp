#include "atmoshericcorrection.h"
#include <QDebug>
#include <QFile>
#include <vector>
#include <QProgressDialog>
#include <QElapsedTimer>

using namespace std;


AtmoshericCorrection::AtmoshericCorrection(QObject *parent) : QObject(parent)
{

}


// Линейная интерполяция данных. Массив данных должен быть отсортирован.

double LineInterpolation(double new_wl, const double* wl, const double* data, size_t size_of_data) {
    size_t index_between_element;



    if ((new_wl>wl[size_of_data-1]) || (new_wl<wl[0]))  return 0; /*throw GenericExc("Необходимо экстраполяция, а не интерполяция", -1);*/
    for (index_between_element = size_of_data-1; index_between_element >0; index_between_element--) {
        if ((new_wl <= wl[index_between_element]) && (new_wl >= wl[index_between_element-1])) break;
    }


    return data[index_between_element-1]+(data[index_between_element]-data[index_between_element-1])/(wl[index_between_element]-wl[index_between_element-1])*(new_wl-wl[index_between_element-1]);

}



int AtmoshericCorrection::doAtmCorr(HyperCube *cube, Attributes *attr)
{
    qint16 **cube_array = (qint16**)cube->GetDataCube();


    QList<double> wl_list= cube->GetListOfChannels();





    int sza=60;
    int zout=20;

    QFile file(QString("d:\\d\\%1_%2_1.out").arg(sza).arg(zout));
       if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
           return 1;


    vector<double> wl, uu, Edir, Edn;

    QTextStream in(&file);
    QString line = in.readLine();
       while (!line.isNull()) {
           wl.push_back(line.trimmed().split(' ')[0].toDouble());
           uu.push_back(line.trimmed().split(' ')[3].toDouble());
           Edir.push_back(line.trimmed().split(' ')[5].toDouble());
           Edn.push_back(line.trimmed().split(' ')[7].toDouble());
           line = in.readLine();
       }
    file.close();





    file.setFileName(QString("d:\\d\\%1_%2_2.out").arg(sza).arg(zout));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 1;
    in.reset();

    vector<double> Lu;
    line = in.readLine();
           while (!line.isNull()) {
               Lu.push_back(line.trimmed().split(' ')[0].toDouble());
               line = in.readLine();
           }
   file.close();



   file.setFileName(QString("d:\\d\\%1_%2_3.out").arg(sza).arg(zout));
   if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
       return 1;
   in.reset();

   vector<double> tau;
   line = in.readLine();
          while (!line.isNull()) {
              tau.push_back(line.trimmed().split(' ')[0].toDouble());
              line = in.readLine();
          }
  file.close();



  double *Edir_array = new double[cube->GetCountofChannels()];
  double *Edn_array = new double[cube->GetCountofChannels()];
  double *Lu_array = new double[cube->GetCountofChannels()];
  double *tau_array= new double[cube->GetCountofChannels()];



  //Интерполирую значения к данным прибора
  for (int i = 0 ; i < cube->GetCountofChannels(); i++) {
      Edir_array[i] = LineInterpolation(wl_list[i],wl.data(),Edir.data(),Edir.size());
      Edn_array[i] = LineInterpolation(wl_list[i],wl.data(),Edn.data(),Edn.size());
      Lu_array[i] = LineInterpolation(wl_list[i],wl.data(),Lu.data(),Lu.size());
      tau_array[i] = LineInterpolation(wl_list[i],wl.data(),tau.data(),tau.size());
  }

  //Основная формула для пересчета
  //Внимание!!! конечный результат умножаю на 1000, т.к. используется qint16 в качестве базового в гиперкубе

  QProgressDialog progress("Пожалуйста, подождите...", "Отменить", 0, cube->GetCountofChannels());
  progress.setWindowModality(Qt::WindowModal);

  QElapsedTimer timer;
  timer.start();

  progress.setValue(0);
  double albedo;
  for (unsigned int i  = 0; i < cube->GetCountofChannels(); i++) {
      if (timer.elapsed()>1000) {
        progress.setValue(i);
        timer.restart();
      }
     if (progress.wasCanceled()) {
         break;
     }
     for (unsigned int k =0; k < cube->GetSizeChannel(); k++) {

            albedo = (cube_array[i][k]-Lu_array[i])/(Edir_array[i]+Edn_array[i])*3.14/tau_array[i]*1000;
            if (albedo <0) albedo = 0;
            if (albedo>1000) albedo = 1000;
            cube_array[i][k]= albedo;

      }

   }
   progress.setValue(cube->GetCountofChannels());


   delete[] Edir_array;
   delete[] Edn_array;
   delete[] Lu_array;
   delete[] tau_array;


   qDebug() << "finish";

   return 0;



}

