
////
//  #include "stdafx.h"
////
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <Windows.h>

#include "Ellipsoid.h"

#ifdef    TO_BUILDER
   #ifndef min
      #define min(A,B) (((A)<(B))?(A):(B))
   #endif
   #ifndef max
      #define max(A,B) (((A)>(B))?(A):(B))
   #endif
#endif

#ifndef WMin
  #define WMin(A,B) (((A)<(B))?(A):(B))
#endif
#ifndef WMax
  #define WMax(A,B) (((A)>(B))?(A):(B))
#endif
//______________________________________________________________________________.
//

#ifndef    KOEF_UTM
   #define KOEF_UTM        0.9996
   #define KOEF_UTM_BACK   1.00040016006402561  //  1./0.9996
#endif
// Угловая скорость вращения Земли         ( rad/s ) [ПЗ-90, с.10 и М.С.Урмаев -> Ю.В.Плахов]
//.#define We_Earth double(7.2921150e-5)   /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^ */
// #define We_Earth double(7.2921151467e-5)/* В.Л.Пантелеев, сайт "Урания" теория фигуры Земли*/
// Геоцентрическая гравитационная постоянная: (m^3/s^2) [ПЗ-90, с.10]
// #define fM_Earth double(398600.44e9)    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^ */
// #define fM_Earth double(398600.43996e9) /* m^3/s^2) М.С.Урмаев -> Ю.В.Плахов] */
   #define fM_Sun   double(13271244.06e9)  /* m^3/s^2) М.С.Урмаев -> Ю.В.Плахов] */
   #define fM_Moon  double(4902.799e9)     /* m^3/s^2) М.С.Урмаев -> Ю.В.Плахов] */
// Скорость света                          (  m/s ) [ПЗ-90, с.10] )
   #define M_Light  double(299792458.0)
// extern
   double  ANGLE_TOPO=(-rwPi/2);
   #define DELTA_DOUBLE_GEOD double(0.1e-12)
///____________________________________________________________________________.
//              extern

const  double TO_DEGREE= (180.0/RWpi);
const  double TO_RADIAN= (RWpi/180.0);
// inline double RadToDeg(double radians) { return( radians * TO_DEGREE   ); };
// inline double DegToRad(double degrees) { return( degrees * TO_RADIAN   ); };
const  double FROM_SECOND= (RWpi/648000.0);
const  double FROM_RADIAN= (648000.0/RWpi);
// inline double SecToRad(double seconds) { return( seconds * FROM_SECOND ); };
// inline double RadToSec(double radians) { return( radians * FROM_RADIAN ); };


TValueEllipsoid VEllipsoid[] =
// ...         6378137   1.0/298.257223123           "    GRS 1980"
{  {"WGS-84^", 6378137, (1.0/298.257223563)        , " 0) WGS-84             "} // e^2=0.00669438
  ,{"WGS-84 ", 6378137, (1.0/298.257223563)        , " 1) WGS-84"             } // e^2=0.00669438
  ,{"ПЗ-90  ", 6378136, (1.0/298.257839303)        , " 2) ПЗ-90"              } // e^2=
  ,{"Крас-42", 6378245, (1.0/298.3        )        , " 3) Kras(CK-42)"        } // e^2=0.006693422
  ,{"Крас-95", 6378245, (1.0/298.3        )        , " 4) Kras(CK-95)"        } // e^2=0.006693422
//================================================== тот же, но с ориентацией для CK-95
  ,{"Bessel ", 6377397.155, (1.0/299.1528128  )    , " 5) Bessel 1841"        } // e^2=0.006674372
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Полярное сжатие =    (1.0-sqrt(1.0-e^2        )) = (a-b)/a
  ,{"BesselN", 6377484, (1.0-sqrt(1.0-0.006674372)), "Bessel 1841 (Namibia)"  } // =299.1528128
  ,{"Airy"   , 6377563, (1.0-sqrt(1.0-0.00667054 )), "Airy"                   }
  ,{"AustNat", 6378160, (1.0-sqrt(1.0-0.006694542)), "Australian National"    }
};
long  VEllipsoid_N= (sizeof(VEllipsoid) / sizeof(TValueEllipsoid));

//
//  CK-42 >> CK-95:                   " преобразовать в Radian`s, [1]
//  X0=   -1.80;                   Ax=  +0.02";
//  Y0=   +9.00;   dm= 0.15*10^-6; Ay=  -0.38";
//  Z0=   -6.80;                   Az=  -0.85";
//
//  CK-95                      ck-42
//  [X`]         [  1 +Az -Ay ] [X]   [X0]   X`= X0 +(+ 1*X +Az*Y -Ay*Z)*(1+dm)
//  [Y`] = (1+dm)[-Az   1 +Ax ] [Y] + [Y0];  Y`= Y0 +(-Az*X + 1*Y +Ax*Z)*(1+dm)
//  [Z`]         [+Ay -Ax   1 ] [Z]   [Z0]   Z`= Z0 +(+Ay*X -Ax*Y + 1*Z)*(1+dm)
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  CK-95 >> ПЗ-90
//  Dx=  +25.90;  X`пз-90`= X`ck-95` + Dx;
//  Dy= -130.94;  Y`пз-90`= Y`ck-95` + Dy;
//  Dz=  -81.76;  Z`пз-90`= Z`ck-95` + Dz;
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  CK-42 >> ПЗ-90:                            " преобразовать в Radian`s, [1]
//  X0=  +25.00 +-2;                        Ax= (+0.00 +-0.1)";
//  Y0= -141.00 +-2;  dm=(0 +- 0.25)*10^-6; Ay= (-0.35 +-0.1)";
//  Z0=  -80.00 +-3;                        Az= (-0.66 +-0.1)";
//                              +-2    +-2     +-3    +-0.1 +-0.1 +-0.1  +-0.25


///____________________________________________________________________________.
///              + | -
// (радиан/sec), We угловая скорость вращения Земли
// double   Calc_We()  { return rwRadian(360.98564/24.0);};// ==(7.2921157e-5)
   double   Calc_We()  { return       + (   We_Earth     );};
///____________________________________________________________________________.
///____________________________________________________________________________.
///
// (m     /sec), Mo линейная скорость спутника
   double Calc_Mo(double Ro)  { return sqrt (fM_Earth/Ro);      };
// (       sec), Период орбиты
   double Calc_To(double Ro)  { return 2.0*rwPi*Ro/Calc_Mo(Ro); };
// (радиан/sec), Wo угловая скорость спутника
   double Calc_Wo(double Ro)  { return Calc_Mo(Ro)/Ro;};// = sqrt(fM/pow((Re+Ho),3));

// Средняя аномалия=     Tv в секундах от перигея [Tv= Ti-Tп]
   double Calc_Mt(double Ro// Ro= Большая полуось орбиты
                 ,double Tv)  { return Tv*Calc_Wo(Ro);};

///Отношение линейной скорости спутника на орбите к ее проекции на Землю
   double Calc_OE(double Re
                 ,double Ho)  { return (Re+Ho) / Re;  };
///Отношение проекции линейной скорости спутника на Землю к скорости на орбите
   double Calc_EO(double Re
                 ,double Ho)  { return Re / (Re+Ho);  };
   double Calc_Rgc(double  Xc // Для любого значения долготы,  значение не зависит от эллипсоида
                  ,double  Yc
                  ,double  Zc){ return sqrt(Xc*Xc+Yc*Yc+Zc*Zc);};
///____________________________________________________________________________.
///
///____________________________________________________________________________.
///
long   NZona_GsK(double L, long Wz)//= 6);// L(Radian), Wz={ 6 | 3 }
{  int j;
   j=3; while((j--) && (L <      0)) L+= rwPi*2;
   j=3; while((j--) && (L > rwPi*2)) L-= rwPi*2;
   double Ld= RadToDeg(L);
   if(Wz==6)  return  long(floor(Ld/Wz    )) + 1;//    1,2,3,... 60 [L>= 0]
   else       return  long(floor(Ld/Wz+0.5));    // 0, 1,2,3,       [L>=-0.5*Wz]
};
double LZona_GsK(long  NZ, long Wz)//= 6);// return L(Radian) -- Долгота осевого мерилиана
{
// if(Wz==6) return  rwRadian( double((Wz>>1)*3600*(NZ*2-1)) );
// else      return  rwRadian( double( Wz    *3600*(NZ    )) );
   double Lz;
   if(Wz==6) Lz= DegToRad( double(Wz*NZ-(Wz>>1)) );
   else      Lz= DegToRad( double(Wz*NZ        ) );
   if(Lz > rwPi) Lz-= rwPi*2;
   return Lz;
};
///____________________________________________________________________________.
///
xyREAL rwSolveQ_2(double a,double b,double c, long* aOK)// a*x*x+b*x+c= 0
{  xyREAL pm={0,0};
   long   ok;
   char   sz[65];
   double d= b*b-4.0*a*c;
   if ( d < 0 )
   {  ok= 0;
   //.sprintf(sz,"a,b,c=%9.6f,%9.6f,%9.6f\n((D=b*b-4.0*a*c) < 0) ?",a,b,c);
   //.MessageBox(NULL,sz,"SolveQ_2",MB_OK|MB_ICONSTOP);
   //^if(LBox(MB_OKCANCEL|MB_ICONSTOP,"rwSolveQ_2","a=%9.6f\nb=%9.6f\nc=%9.6f\n\n((D=b*b-4.0*a*c) < 0)  ?...",a,b,c) != IDOK)
      exit(0);
   }
   else
   {  ok= 1;
      d= sqrt(d);
      pm.x= 0.5*(-b+d)/a;
      pm.y= 0.5*(-b-d)/a;
   }
   if(aOK)    aOK[0]= ok;
   return(pm);
}

xyREAL SolveQ_2L(LDouble aq,LDouble bq,LDouble cq, long* aOK)// a*x*x+b*x+c=0 //
{   xyREAL  pm={0,0};
    long    ok;
    char    sz[96];
    LDouble dq= bq*bq - aq*cq*4.0;
    if( dq<= 0 )
    {   ok = 0;
        sprintf(sz,"a=%9.6f\nb=%9.6f\nc=%9.6f\nd=%9.6f:  (b*b-4.0*a*c) <= 0 ?..."
               ,double(aq),double(bq),double(cq),double(dq));
        //MessageBox(NULL,sz,"SolveQ_2L",MB_OK|MB_ICONSTOP);
    // ...
    }
    else
    {   ok= 1;
        dq = sqrtl(dq);
        pm.x= double((-bq+dq)/aq)*0.5;
        pm.y= double((-bq-dq)/aq)*0.5;
    }
    if(aOK)   aOK[0]= ok;
    return(pm);
}// xyREAL SolveQ_2L(LDouble a,LDouble b,LDouble c, long* aOK=NULL)// a*x*x+b*x+c=0 //

xyzREAL TEllipsoid::VectorToEllipsoid(xyzREAL p1,xyzREAL p2, double * ADgc)
{  long    ok;
   xyREAL  tq;
   xyzREAL c1,c2;
   double  a,b,c, r1,r2;
   double  Eq= ELL_a/ELL_b;
           Eq= Eq*Eq;
   p2.x-=p1.x;// double  Vx= p2.x-p1.x;
   p2.y-=p1.y;// double  Vy= p2.y-p1.y;
   p2.z-=p1.z;// double  Vz= p2.z-p1.z;
   a=   (p2.x*p2.x + p2.y*p2.y + p2.z*p2.z*Eq);// * t^2      //  a=   (Vx*Vx+Vy*Vy+Vz*Vz*Eq);// * t^2
   b= 2*(p1.x*p2.x + p1.y*p2.y + p1.z*p2.z*Eq);// * t        //  b= 2*(Xs*Vx+Ys*Vy+Zs*Vz*Eq);// * t
   c=   (p1.x*p1.x + p1.y*p1.y + p1.z*p1.z*Eq) - ELL_a*ELL_a;//  c=   (Xs*Xs+Ys*Ys+Zs*Zs*Eq) - Ea*Ea;
   tq= rwSolveQ_2(a,b,c, &ok);// a*x^2 + b*x + c = 0
   //          tq.x=  (+sqrt(b^2-4*a*c) - b) / (2*a)
   //          tq.y=  (-sqrt(b^2-4*a*c) - b) / (2*a)
//+c1   = p1;
   c1.x = p2.x*tq.x;// Полные координаты точки пересечения(1) - p1.x
   c1.y = p2.y*tq.x;// Полные координаты точки пересечения(1) - p1.y
   c1.z = p2.z*tq.x;// Полные координаты точки пересечения(1) - p1.z
//+c2   = p1;
   c2.x = p2.x*tq.y;// Полные координаты точки пересечения(2) - p1.x
   c2.y = p2.y*tq.y;// Полные координаты точки пересечения(2) - p1.y
   c2.z = p2.z*tq.y;// Полные координаты точки пересечения(2) - p1.z
   r1= (c1.x*c1.x + c1.y*c1.y + c1.z*c1.z);
   r2= (c2.x*c2.x + c2.y*c2.y + c2.z*c2.z);
   if(r2 < r1) { c1= c2; r1= r2; }
   if(ADgc)  *ADgc= sqrt(r1);
   c1.x+= p1.x;// Полные координаты точки пересечения(Minimum distance from p1)
   c1.y+= p1.y;// Полные координаты точки пересечения(Minimum distance from p1)
   c1.z+= p1.z;// Полные координаты точки пересечения(Minimum distance from p1)
   return( c1);
}
// Наклонение орбиты Io
// return(X,Y) X=ELL_a, Y=ELL_b`(ПРИ СЕЧЕНИИ ЭЛЛИПСОИДА ПЛОСКОСТЬЮ ОРБИТЫ)
// BMax= максимальная широта, достигаемая подспутниковой точкой,
//       по напрравлению к центру масс
xyREAL  TEllipsoid::Calc_Cut(double Io, double * ABMax)//  = NULL)
{  xyREAL Pab;
   Pab.x= ELL_a;
   Pab.y= ELL_b;
   xyzREAL Po;
           Po.x= fabs(ELL_a*cos(Io));   // Io= [0..Pi]
           Po.y=                 0  ;
           Po.z= fabs(ELL_a*sin(Io));
   double  Dgc;                   // Отклонение от сферы R= ELL_a;
   xyzREAL Pc  = TEllipsoid::Calc_Dgc(Po, &Dgc);// = NULL);
   Pab.y       = sqrt(Pc.x*Pc.x + Pc.y*Pc.y + Pc.z*Pc.z);
   double  Bf  = asin(Pc.z/Pab.y);// Геоцентрическая широта для точки на поверхности
   double  Bgd = TEllipsoid::Calc_Bgd(Bf);
   if(ABMax)    *ABMax= Bgd;

   xyzREAL BLH = TEllipsoid::XYZ_To_BLH(Pc.x,Pc.y,Pc.z);// return xyzREAL(B,L,H) in radian, m
   if(fabs(Bgd-BLH.x)>DELTA_DOUBLE_GEOD)
   {
//,   LPW("? ЖЖ__Bgd) Bf  = %s b-b`= %13.3f", rwStrRadian(Bf),ELL_b-Pab.y);
//,   LPW("? ЖЖ__Bgd) BMax= %s H   = %13.3f D= %17.15f`` Seconds", rwStrRadian(BLH.x),BLH.z,rwSecond(fabs(Bgd-BLH.x)));
   }

   return(Pab);
};

xyzREAL VectorToEllipsoid(xyzREAL p1,xyzREAL p2,double Ea,double Eb, double * ADgc)
{  long    ok;
   xyREAL  tq;
   xyzREAL c1,c2;
   double  a,b,c, r1,r2;
   double  Eq= Ea/Eb;
           Eq= Eq*Eq;
   p2.x-=p1.x;// double  Vx= p2.x-p1.x;
   p2.y-=p1.y;// double  Vy= p2.y-p1.y;
   p2.z-=p1.z;// double  Vz= p2.z-p1.z;
   a=   (p2.x*p2.x + p2.y*p2.y + p2.z*p2.z*Eq);// * t^2  //  a=   (Vx*Vx+Vy*Vy+Vz*Vz*Eq);// * t^2
   b= 2*(p1.x*p2.x + p1.y*p2.y + p1.z*p2.z*Eq);// * t    //  b= 2*(Xs*Vx+Ys*Vy+Zs*Vz*Eq);// * t
   c=   (p1.x*p1.x + p1.y*p1.y + p1.z*p1.z*Eq) - Ea*Ea;  //  c=   (Xs*Xs+Ys*Ys+Zs*Zs*Eq) - Ea*Ea;
   tq= rwSolveQ_2(a,b,c, &ok);// a*x^2 + b*x + c = 0
   //          tq.x=  (+sqrt(b^2-4*a*c) - b) / (2*a)
   //          tq.y=  (-sqrt(b^2-4*a*c) - b) / (2*a)
//+c1   = p1;
   c1.x = p2.x*tq.x;// Полные координаты точки пересечения(1) - p1.x
   c1.y = p2.y*tq.x;// Полные координаты точки пересечения(1) - p1.y
   c1.z = p2.z*tq.x;// Полные координаты точки пересечения(1) - p1.z
//+c2   = p1;
   c2.x = p2.x*tq.y;// Полные координаты точки пересечения(2) - p1.x
   c2.y = p2.y*tq.y;// Полные координаты точки пересечения(2) - p1.y
   c2.z = p2.z*tq.y;// Полные координаты точки пересечения(2) - p1.z
   r1= (c1.x*c1.x + c1.y*c1.y + c1.z*c1.z);
   r2= (c2.x*c2.x + c2.y*c2.y + c2.z*c2.z);
   if(r2 < r1)
   {
     c1= c2;  //  для   N  &  S
     r1= r2;
   }
   else
   {
     c2= c1;
     r2= r1;
   }
   if(ADgc)  *ADgc= sqrt(r1);
   c1.x+= p1.x;// Полные координаты точки пересечения(Minimum distance from p1)
   c1.y+= p1.y;// Полные координаты точки пересечения(Minimum distance from p1)
   c1.z+= p1.z;// Полные координаты точки пересечения(Minimum distance from p1)
   return( c1);
}
xyREAL  VectorToEllipse  (xyREAL  p1,xyREAL  p2,double Ea,double Eb, double * ADgc=NULL);
xyREAL  VectorToEllipse  (xyREAL  p1,xyREAL  p2,double Ea,double Eb, double * ADgc)
{  long    ok;            // D:\ABC_BC64\Diff_ATRW\Kepler_Ellips\Topo-Geoc.C  17-01-2007
   xyREAL  tq;            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   xyREAL  c1,c2;
   double  a,b,c, r1,r2;
   double  Eq= Ea/Eb; 
           Eq= Eq*Eq;
   p2.x-=p1.x;// double  Vx= p2.x-p1.x;
   p2.y-=p1.y;// double  Vy= p2.y-p1.y;
   a=   (p2.x*p2.x + p2.y*p2.y*Eq);// * t^2  //  a=   (Vx*Vx+Vy*Vy+Vz*Vz*Eq);// * t^2
   b= 2*(p1.x*p2.x + p1.y*p2.y*Eq);// * t    //  b= 2*(Xs*Vx+Ys*Vy+Zs*Vz*Eq);// * t
   c=   (p1.x*p1.x + p1.y*p1.y*Eq) - Ea*Ea;  //  c=   (Xs*Xs+Ys*Ys+Zs*Zs*Eq) - Ea*Ea;
   tq= rwSolveQ_2(a,b,c, &ok);// a*x^2 + b*x + c = 0
   //          tq.x=  (+sqrt(b^2-4*a*c) - b) / (2*a)
   //          tq.y=  (-sqrt(b^2-4*a*c) - b) / (2*a)
//+c1   = p1;
   c1.x = p2.x*tq.x;// Полные координаты точки пересечения(1) - p1.x
   c1.y = p2.y*tq.x;// Полные координаты точки пересечения(1) - p1.y
//+c2   = p1;
   c2.x = p2.x*tq.y;// Полные координаты точки пересечения(2) - p1.x
   c2.y = p2.y*tq.y;// Полные координаты точки пересечения(2) - p1.y
   r1= (c1.x*c1.x + c1.y*c1.y);
   r2= (c2.x*c2.x + c2.y*c2.y);
   if(r2 < r1) { c1= c2; r1= r2; }
   if(ADgc)  *ADgc= sqrt(r1);
   c1.x+= p1.x;// Полные координаты точки пересечения(Minimum distance from p1)
   c1.y+= p1.y;// Полные координаты точки пересечения(Minimum distance from p1)
   return( c1);
}
// Исход.точка с координатами в ГЕОЦЕНТР.С.К. Ph(X,Y,Z)
// return(X,Y,Z) точки на поверхности эллипсоида, лежащей на прямой от(Ph) к центру С.К.
// Dgc= расстояние от(Ph) до поверхности эллипсоида по направлению к центру масс
xyzREAL TEllipsoid::Calc_Dgc(xyzREAL Ph, double * ADgc)// = NULL);
{  xyzREAL  Pc={0,0,0};
   if(ELL_i<=0)
   {  double L   = ATan2(Ph.y,Ph.x);
      double sinL= sin(L);
      double cosL= cos(L);
      double B   = ATan2(Ph.z,Ph.x*cosL+Ph.y*sinL);
      double sinB= sin(B);
      double cosB= cos(B);
      Pc.x= ELL_a*cosB*cosL;
      Pc.y= ELL_a*cosB*sinL;
      Pc.z= ELL_a*sinB;
      if(ADgc) *ADgc= Dist3D(Ph,Pc);
   } else
   {
      Pc=  VectorToEllipsoid(Ph,Pc, ADgc);
   }
   return ( Pc );
};
xyREAL TEllipsoid::Calc_Dgc( xyREAL Ph, double * ADgc)//= NULL);// то же для эллипса
{  xyREAL  Pc={0,0};
   if(ELL_i<=0)
   {  double B   = ATan2(Ph.y,Ph.x);
      double sinB= sin(B);
      double cosB= cos(B);
      Pc.x= ELL_a*cosB;
      Pc.y= ELL_a*sinB;
      return(Pc);
   } else
   {
      Pc= VectorToEllipse(Ph,Pc, ELL_a,ELL_b);
   }
   if(ADgc) *ADgc= Dist2D(Ph,Pc);
   return ( Pc );
};


// Исход.точка с координатами в ГЕОЦЕНТР.С.К. Ph(X,Y,Z) -> Геодезич.высота и XYZ
// return  (X,Y,Z) точки пересечения нормали от (Ph) и поверхности эллипсоида
// Dgc= расстояние от(Ph) до поверхности эллипсоида  по нормали к поверхности
xyzREAL TEllipsoid::Calc_Hgd(xyzREAL Ph, double * AHgd)// = NULL);
{  xyzREAL  BLH,Pc={0,0,0};
   if(ELL_i<=0) return Calc_Dgc(Ph,AHgd); 
   BLH= TEllipsoid::XYZ_To_BLH(Ph.x,Ph.y,Ph.z);// return xyzREAL(B,L,H) in radian, m
   Pc = TEllipsoid::BLH_To_XYZ(BLH.x,BLH.y, 0);
   if(AHgd) *AHgd= BLH.z;
//.OK   
//.if(fabs(Dist3D(Ph,Pc)-BLH.z)>DELTA_DOUBLE_GEOD) 
//.   LPW("?__Hgd__ B= %s H= %13.3f D= %17.15f", rwStrRadian(BLH.x),BLH.z,fabs(Dist3D(Ph,Pc)-BLH.z));
   return(Pc);
};
xyREAL  TEllipsoid::Calc_Hgd( xyREAL Ph, double * AHgd)//= NULL);// то же для эллипса
{  xyzREAL  BLH,Pc={0,0,0};
   if(ELL_i<=0) return Calc_Dgc(Ph,AHgd); 
   BLH= TEllipsoid::XYZ_To_BLH(Ph.x, 0,Ph.y);// return xyzREAL(B,L,H) in radian, m
   Pc = TEllipsoid::BLH_To_XYZ(BLH.x,BLH.y, 0);
   if(AHgd) *AHgd= BLH.z;// Dist3D(Ph,Pc);
   xyREAL pd;
          pd.x= Pc.x;
          pd.y= Pc.z;
//.if(fabs(Dist2D(Ph,pd)-BLH.z)>DELTA_DOUBLE_GEOD)
//.   LPW("?__Hgd__ (B= %s H= %13.3f) D= %17.15f", rwStrRadian(BLH.x),BLH.z,fabs(Dist2D(Ph,pd)-BLH.z));
   return(pd);
};

void TEllipsoid::Init(long iELL, double DeltaOrRSphere)
{
// if(iELL<=0) { Init_Sphere           (DeltaOrRSphere); return; }
/* +
   ELL_i= iELL;
   double a_2, b_2;
   switch(ELL_i)
   {CASE  1:   ELL_a= 6378137; ELL_p= 1/298.257223563;//WGS-84
   _CASE  2:   ELL_a= 6378136; ELL_p= 1/298.257839303;// PZ-90
   _CASE  3:   ELL_a= 6378245; ELL_p= 1/298.3        ;//CK-42
   _CASE  4:   ELL_a= 6378245; ELL_p= 1/298.3        ;//CK-95
   _CASE  5:   ELL_a= 6377397; ELL_p= 1/299.1528128  ;//BESSEL 1841
            // ELL_a= 6377484; ELL_e1E2= 0.006674372 ;//Bessel 1841 (Nambia)  ?...
   _DEFAULT:// ... Для всех положительных значений
            ELL_i = 1;
            ELL_a = 6378137; ELL_p= 1/298.257223563;//WGS-84
   }
+ */
   if(iELL >= VEllipsoid_N) iELL=1;
   ELL_a=  VEllipsoid[iELL].VELL_a;
   ELL_p=  VEllipsoid[iELL].VELL_p;
   ELL_b=  ELL_a*(1.0-ELL_p);
   if(DeltaOrRSphere!=0)
   {           ELL_b+=     DeltaOrRSphere;
               ELL_a+=     DeltaOrRSphere;
   //...^      ELL_p =  1.0 - ELL_b/ELL_a;
   }
   Init_Ellipsoid(ELL_a, ELL_b, iELL);
   return;
};
void  TEllipsoid::Init_Ellipsoid(double a, double b, long iELL)// = 0);
{  double  a_2, b_2, ab;
   ELL_i= (!iELL)?VEllipsoid_N:iELL;
   ELL_a=    a;
   ELL_b=    b;
   ELL_p=  1.0 - ELL_b/ELL_a;
// ELL_beta=          1/ELL_p;// Не определено для сферы
// ELL_b     = ELL_a*(1-ELL_p);
   a_2       = ELL_a * ELL_a;
   b_2       = ELL_b * ELL_b;
   ELL_ab    = ELL_a / ELL_b; // =         a/b
   ELL_ba    = 1.0   - ELL_p; // = (1-p) = b/a
   ELL_c     = ELL_a * ELL_ab;// = a_2 / ELL_b;
   ELL_ef    = sqrt(a_2-b_2);
   ELL_pf    = ELL_ba * ELL_b;  // b^2 / a
   ELL_ra    = ELL_a  + ELL_ef;
   ELL_rp    = ELL_a  - ELL_ef;
   ELL_e1E2  = 1.0    - ELL_ba*ELL_ba;//= 1 - b_2/a_2;
   ELL_e2E2  = ELL_ab * ELL_ab -  1.0;//= a_2/b_2 - 1;
   ELL_e1    = sqrt(ELL_e1E2);
   ELL_e2    = sqrt(ELL_e2E2);
// +
// ELL_e0    = 1.0/(1-ELL_p) ;// [ EM0 ] 1/(1-p)= beta/(beta-1) = a/b
//^ELL_ab    = 1.0/(1-ELL_p) ;// [ EM0 ] 1/(1-p)= beta/(beta-1) = a/b
   ELL_c1    = ELL_b*ELL_e2E2;// [ EC1 ] b*e2E2       = (ELL_b*ELL_e2E2)  =  b * e'^2
   ELL_c2    = ELL_a*ELL_e1E2;// [ EC2 ] a*e1E2       = (ELL_a*ELL_e1E2)  =  a * e ^2

// Совместимость( UTM, GsK):
   AELL=     ELL_a;       // +
   BETA= 1.0/ELL_p;       // +
   BELL=     ELL_b;       // +
   EM0 =     ELL_ab;      // +
   EM1 =     ELL_e1E2;    // +
   EM2 =     ELL_e2E2;    // +
   EC0 =     ELL_a*ELL_ab;// +
   EC1 =     ELL_c1;      // +
   EC2 =     ELL_c2;      // +
//~> Морозов, cтр.21-22,28
   double  m [11]={0};
   double  ka[11]={0};
   double  pq[ 7]={0};
   m[ 0]= AELL*(1.0-EM1);
   m[ 2]= EM1 * m[0]* 3.0/ 2.0;
   m[ 4]= EM1 * m[2]* 5.0/ 4.0;
   m[ 6]= EM1 * m[4]* 7.0/ 6.0;
   m[ 8]= EM1 * m[6]* 9.0/ 8.0;
   m[10]= EM1 * m[8]*11.0/10.0;

   ka[ 0]= m[0]+m[2]/2.0+m[4]*3.0/8.0+m[6]* 5.0/16.0+m[8]*35.0/128.0+m[10]* 63.0/256.0;
   ka[ 2]=      m[2]/2.0+m[4]    /2.0+m[6]*15.0/32.0+m[8]* 7.0/ 16.0+m[10]*105.0/256.0;
   ka[ 4]=               m[4]    /8.0+m[6]* 3.0/16.0+m[8]* 7.0/ 32.0+m[10]* 15.0/ 64.0;
   ka[ 6]=                            m[6]     /32.0+m[8]     / 16.0+m[10]* 45.0/512.0;
   ka[ 8]=                                           m[8]     /128.0+m[10]*  5.0/256.0;
   ka[10]=                                                           m[10]      /510.0;
// TEllipsoid::
   Ka0  = ka[0];
   wk246=(ka[2]-ka[4]+ka[6]);
   wk6  =(ka[6]*(16.0/3.0));
   wk46 =(ka[4]*2.0-wk6);

// коэф. р[]
   pq[2]= -ka[2]/2.0; pq[2]/=ka[0];
   pq[4]= +ka[4]/4.0; pq[4]/=ka[0];
   pq[6]= -ka[6]/6.0; pq[6]/=ka[0];
// коэф. q[] - обращение триг.ряда стр.292(приложение)
   double pq2_2=pq[2]*pq[2];
   double pq2_3=pq2_2*pq[2];
   double pq24 =pq[2]*pq[4];
   pq[1]= -pq[2]    -pq24    +pq2_3/2.0; //q[2]
   pq[3]= -pq[4]             +pq2_2;     //q[4]
   pq[5]= -pq[6]+3.0*pq24-3.0*pq2_3/2.0; //q[6]
// TEllipsoid::
   wq246= (2.0*pq[1]+4.0*pq[3]+6.0*pq[5]);
   wq6  = (pq[5]*32.0);
   wq46 = (pq[3]*8.0+wq6);
//
   ZonaUTM[0]=0;
};
//_____________________________________________________________________________________.
//

double TEllipsoid::AMeridian(double B, double L, long Wz)// ДЛЯ 6|3^ГРАДУСНОЙ ЗОНЫ
{
   long    Nz= NZona_GsK( L,Wz);
   double  Lz= LZona_GsK(Nz,Wz);
   return  LzOwn_AMeridian( Lz, B,L);
}
double TEllipsoid::LzOwn_AMeridian( double Lz, double B,double L)
{  double  L1,L2, sinB,cosB2,tanB2, n2, Mi;
   L1 = L  - Lz;
   L2 = L1 * L1;
   sinB = sin(B);
   tanB2= tan(B);  tanB2= tanB2*tanB2;
   cosB2= cos(B);  cosB2= cosB2*cosB2;
      n2= ELL_e2E2*cosB2;
//.Mi= (1.0+(0.33333+0.00674*cosB2+(0.2*cosB2-0.067)*L2)*L2*cosB2)*L1*sin(B);
//.LPW("|-- Meri_1=%s",rwStrRadian(Mi));
// ==
// И.Ф.Куштин, Геодезия. Уч-практ.пособие, М., "Издательство ПРИОР", 2001, -448 с.
// DM < 0.001'' при L1<=3.5°
   Mi= L1*sinB*(1 + L2*(1+n2*3+n2*n2*2)*cosB2/3 + L2*L2*(2-tanB2)*cosB2*cosB2/15);
//.LPW("|-- Meri_2=%s",rwStrRadian(Mi));
   return(Mi);
};

xyzREAL TEllipsoid::BLH_To_XYZ  (double  B,double  L,double  H) // (B,L,H) in radian, return( X,Y,Z )
{  double sinB,cosB,sinL,cosL;
   xyzREAL  gc;
   cosB=cos(B);
   sinB=sin(B);
   cosL=cos(L);
   sinL=sin(L);
   double N= ELL_a  / sqrt(1.0-ELL_e1E2*sinB*sinB);
// ok:    N= ELL_c0 / sqrt(1.0+ELL_e2E2*cosB*cosB);
   gc.x=  (H+N)       *  cosB * cosL;
   gc.y=  (H+N)       *  cosB * sinL;
   gc.z=  (H+N*(1.0-ELL_e1E2))* sinB;// = ((H+N)        - N*ELL_e1E2)* sinB;
   return( gc );                     // =  (H+N) * sinB - N*ELL_e1E2 * sinB;
                                     // H= Z/sinB - N*(1-e*e)
// "геодезия и картография" # 12, 2002, c.11-13, Л.В.Огородова
// L= ATan2(Y,X);
// R= ( X*cos(L) + Y*sin(L) );  // R==0 -> B= 90 'градусов'
// B= ATan2(Z+N*e^2*sib(B),R);  // Ниже исп. еще более подходящий вариант
// ...                          // примерно аналог.: -->> B=[0..+-90]
//[H= R*cosB + Z*sinB - N*(1-e^2*sinB^2);]      //   -->> B=[0..+-90]
// H= R*cosB + Z*sinB - a*(1-e^2*sinB^2)^(1/2); //   ^ ==
};
xyzREAL TEllipsoid::XYZ_To_BLH  (double  X,double  Y,double  Z) // return xyzREAL(B,L,H) in radian, m
{   xyzREAL  blh;
    double   zt,N;
    double   sinB,cosB, sinL,cosL;
    double   L= ATan2(Y, X);
    sinL=    sin(L);
    cosL=    cos(L);
  //~~~~~~~~~~~~~~~^
  //В.П.Морозов, стр.23-24,191-195: dB <= 0.0001" при 0<=H<=700км : OK
  //R =  sqrt( X*X + Y*Y );
    double  R =( X*cosL + Y*sinL );  // R==0 -> B= 90 'градусов'
    double  q = ATan2(ELL_ab*Z ,R);
    double  q1= sin(q);
    double  q2= cos(q);
////////////////////////////////////////////////////////////////////////////////////////////////
    double  B= ATan2(Z + ELL_c1*q1*q1*q1, R - ELL_c2*q2*q2*q2 );////   при B~45° |D|<(0.3e-9 )``
////////////////////////////////////////////////////////////////////////////////////////////////
//  Ошибка появляется только в 10-ом знаке после точки для секунд >> ПРИ ЭТОМ 1 ИТЕРАЦИЯ:
//  В.П.Морозов, стр.193 >> Боуринг (Survey Review, XXIII, 181, July 1976    и   |D|<(0.1e-15)``
////////////////////////////////////////////////////////////////////////////////////////////////
    long    p = 0;
    double  bt= B;
//. double  x2= X*X;
//. double  y2= Y*Y;
  //+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^
  //В.Ф.Чекалин,1986г.стр.45-46 -> А.Н.Лобанов,АФ,1972г.
  //Первое приближение   B=^
    double  sq;
    for(p=1;p<=1;p++)// [1] is OK..., ЕСЛИ [0] то макс.отклонение на широте ~50° < 0.27*10^-9``
    {  bt= B;
       sinB= sin(bt);
       N = ELL_a/sqrt(1.0-ELL_e1E2*sinB*sinB);
       zt= Z + N*ELL_e1E2*sinB;
//     sq= (x2 + y2 + zt*zt);
       sq= (R*R + zt*zt);
       if(sq>0) B= asin(zt/sqrt(sq));
       else   { B= 0;      break;  }
    // q=         (fabs(B-bt));
    // if(q <= DELTA_DOUBLE_GEOD) break;
    }//for(p=1;p<=7;p++)
/// ОПРЕДЕЛЕНИЕ ВЫСОТЫ ...
    sinB=  sin(B);
    cosB=  cos(B);
    double ue=  (1.0-ELL_e1E2*sinB*sinB);
//  N   =  ELL_a/sqrt(ue);
//  double H= Z/sinB - N*(1-ELL_e1E2);//  ? (B==0)
// "геодезия и картография" # 12, 2002, c.11-13, Л.В.Огородова
//  double H= (X*cosL+Y*sinL)*cosB + Z*sinB - N*(1-e^2*sinB^2);//  -->> B=[0..+-90]
    double H= R*cosB + Z*sinB - ELL_a*sqrt(ue);
    blh.x=  B;
    blh.y=  L;
    blh.z=  H;
    return( blh );
}
//_____________________________________________________________________________________.
//
//      return:    Radian`s ...
double  LZona_UTM (long  Nz_UTM) //             1..60
{
	return  DegToRad((Nz_UTM - 1)*6 - 180 + 3);  //+3 puts origin in middle of zone
}
double  LZona_UTM (char * ZoneIn) // ZoneIn[4]= "1..60'C'"
{
	char*  ZoneLetter;
	int    ZoneNumber = strtoul(ZoneIn ,&ZoneLetter, 10);
/*
	if((long(WORD(*ZoneLetter)) - long(WORD('N'))) >= 0)
		    NorthernHemisphere = 1;//point is in northern hemisphere
	else {  NorthernHemisphere = 0;//point is in southern hemisphere
	       	y -= 10000000.0;//remove 10,000,000 meter offset used for southern hemisphere
	}
*/
    return  LZona_UTM (ZoneNumber);
}
//      return  1..60
long    NZona_UTM (double Lrad) // L(Radian), Lz= 6: Геометрия ОК, но не соотв.разграфке
{
//  Make sure the longitude is between -180.00 .. 179.9
    int j=0;
    while(fabs(Lrad)  >  rwPi)// -180..+180
    {  j++;
       if(Lrad<0) Lrad+= rwPiM2;
       else       Lrad-= rwPiM2;
       if(j==2) break;
    };
    long     L = (long)RadToDeg(Lrad);
	return( (L + 180)/6 + 1 );
}
long    NZona_UTM (double Lrad, double Brad)             // L(Radian), Lz= 6
{
//  Make sure the longitude is between -180.00 .. 179.9
    int j=0;
    while(fabs(Lrad)  >  rwPi)// -180..+180
    {  j++;
       if(Lrad<0) Lrad+= rwPiM2;
       else       Lrad-= rwPiM2;
       if(j==2) break;
    };
    double          B= RadToDeg(Brad);
    double          L= RadToDeg(Lrad);
	long Nz= ((long(L) + 180)/6 + 1 );
    /////////////////////////////////////////////////////////////////////////
	if( (B >= 56.0)&&(B < 64.0)&&
        (L >= 3.0 )&&(L < 12.0) )        Nz= 32;
    else
	if( (B >= 72.0)&&(B < 84.0) )//  Special zones for Svalbard
	{
        if(     (L >= 0.0 )&&(L <  9.0)) Nz= 31;
        else if((L >= 9.0 )&&(L < 21.0)) Nz= 33;
        else if((L >= 21.0)&&(L < 33.0)) Nz= 35;
        else if((L >= 33.0)&&(L < 42.0)) Nz= 37;
	}
    return Nz;
}
long  IsUTMZ(double X_East)// return  NZona_UTM[1..60] if UTMZ-Format, else <=0
{
    return long(floor(X_East/1000000));
}
xyREAL  UTM_To_UTMZ(double XEast, double YNorth, char szZonaUTM_In [4])
{
    xyREAL utmz={XEast,YNorth};
	char*  ZoneLetter;
	int    ZoneNumber = strtoul(szZonaUTM_In ,&ZoneLetter, 10);
    int    NorthernHemisphere=((long(WORD(*ZoneLetter)) - long(WORD('N'))) < 0) ?0 :1;
	if(  ! NorthernHemisphere) utmz.y-= 10000000.0;//remove 10,000,000 meter offset used for southern hemisphere
    utmz.x+= double(ZoneNumber)*1000000;
    return   utmz;
}
xyzREAL UTM_To_UTMZ(double XEast, double YNorth, double H, char szZonaUTM_In [4])
{
    xyREAL  pu  = UTM_To_UTMZ(XEast, YNorth, szZonaUTM_In);
    xyzREAL utmz={pu.x,pu.y, H};
    return  utmz;
}

xyREAL  TEllipsoid::UTMZ_To_UTM(double XEastPlus,double YNorthMinus, char szZonaUTM_Out[4])
{
    xyREAL utm={XEastPlus,YNorthMinus};
    long   Nz = long(floor(XEastPlus/1000000));
    utm.x-=               double(Nz)*1000000;
//. ТРЕБУЕТСЯ ВКЛЮЧЕНИЕ В ОБЪЕКТ ДЛЯ ФОРМИРОВАНИЯ  B_Degrees
//. хотя далее достаточно укзания на северное или южное полушарие
//. char          charUTM= UTMLetterDesignator(B_Degrees);//  +/-B - градусы
    char          charUTM='N';
    if(utm.y<0) { charUTM='M'; utm.y+= 10000000.0;}

    xyzREAL blh= UTMZ_To_BLH(XEastPlus, YNorthMinus, 0, szZonaUTM_Out);// return xyzREAL(B,L,H) in radian, m
//. charUTM= UTMLetterDesignator(blh.x*RadToDeg);//  +/-B - градусы
//. sprintf(szZonaUTM_Out, "%ld%c", Nz, charUTM);
    return  utm;
}
xyzREAL TEllipsoid::UTMZ_To_UTM (double XEastPlus,double YNorthMinus,double H, char szZonaUTM_Out[4])
{
    xyREAL  pu = UTMZ_To_UTM(XEastPlus, YNorthMinus, szZonaUTM_Out);
    xyzREAL utm= {pu.x,pu.y, H};
    return  utm;
}

// return xyzREAL(B,L,H) in radian,   m,...                     [Геоценгрическая->Трансляция]
xyzREAL TEllipsoid::UTM_To_BLH(double UTMEasting,double UTMNorthing,double h, char ZoneIn [4])
{  xyzREAL blh= {0};
           blh.z= h;
   double  Lat,Long;
// converts UTM coords to lat/long.  Equations from USGS Bulletin 1532
// East Longitudes are positive, West longitudes are negative
// North latitudes are positive, South latitudes are negative
// Written by Chuck Gantz- chuck.gantz@globalstar.com
	double k0 = KOEF_UTM;
	double a          = ELL_a;   //ellipsoid[ReferenceEllipsoid].EquatorialRadius;
	double eccSquared = ELL_e1E2;//ellipsoid[ReferenceEllipsoid].eccentricitySquared;
	double eccPrimeSquared;
	double e1 = (1-sqrt(1-eccSquared))/(1+sqrt(1-eccSquared));
	double N1, T1, C1, R1, D, M;
	double LongOrigin;
	double mu, phi1, phi1Rad;
	double x, y;
	int    NorthernHemisphere; //1 for northern hemispher, 0 for southern
	x = UTMEasting - 500000.0; //remove 500,000 meter offset for longitude
	y = UTMNorthing;
	char*  ZoneLetter;
	int    ZoneNumber = strtoul(ZoneIn ,&ZoneLetter, 10);
	if((long(WORD(*ZoneLetter)) - long(WORD('N'))) >= 0)
		NorthernHemisphere = 1;//point is in northern hemisphere
	else
	{  NorthernHemisphere = 0;//point is in southern hemisphere
		y -= 10000000.0;//remove 10,000,000 meter offset used for southern hemisphere
	}
	LongOrigin = (ZoneNumber - 1)*6 - 180 + 3;  //+3 puts origin in middle of zone
	eccPrimeSquared = (eccSquared)/(1-eccSquared);

    M = y / k0;
	mu = M/(a*(1 -                         eccSquared/4
                -            3*eccSquared*eccSquared/64
                - 5*eccSquared*eccSquared*eccSquared/256));

	phi1Rad = mu +     (3*e1/2  -    27*e1*e1*e1/32)*sin(2*mu)
				    + (21*e1*e1/16 - 55*e1*e1*e1*e1/32)*sin(4*mu)
				    +                 (151*e1*e1*e1/96)*sin(6*mu);
	phi1 = RadToDeg(phi1Rad);

	N1 = a/sqrt(1-eccSquared*sin(phi1Rad)*sin(phi1Rad));
	T1 = tan(phi1Rad)*tan(phi1Rad);
	C1 = eccPrimeSquared*cos(phi1Rad)*cos(phi1Rad);
	R1 = a*(1-eccSquared)/pow(1-eccSquared*sin(phi1Rad)*sin(phi1Rad), 1.5);
	D  = x/(N1*k0);

	Lat = phi1Rad -(N1*tan(phi1Rad)/R1)
                  *(D*D/2
                    -(5+3*T1+10*C1-4*C1*C1-9*eccPrimeSquared)*D*D*D*D/24
                    +(61+90*T1+298*C1+45*T1*T1-252*eccPrimeSquared-3*C1*C1)*D*D*D*D*D*D/720);
//	Lat  = Lat * RadToDeg;
//  blh.x= Lat * DegToRad;
    blh.x= Lat;

	Long = (D - (1+2*T1+C1)*D*D*D/6
             + (5-2*C1+28*T1-3*C1*C1+8*eccPrimeSquared+24*T1*T1)*D*D*D*D*D/120)/cos(phi1Rad);
//	Long = LongOrigin + Long * RadToDeg;
//  blh.y= Long*DegToRad;
    blh.y= DegToRad(LongOrigin) + Long;
    return( blh );
}//xyzREAL TEllipsoid::UTM_To_BLH(double x,double y,double h, char ZoneIn [4])// =NULL);

//-> xyzREAL TEllipsoid::BLH_To_UTM(...
char UTMLetterDesignator(double  Lat) //  B - градусы
{  //This routine determines the correct UTM letter designator for the given latitude
   //returns 'Z' if latitude is outside the UTM limits of 84N to 80S
   //Written by Chuck Gantz- chuck.gantz@globalstar.com
	char LetterDesignator;
	if     (( 84 >=Lat) && (Lat >=  72)) LetterDesignator = 'X';
	else if(( 72 > Lat) && (Lat >=  64)) LetterDesignator = 'W';
	else if(( 64 > Lat) && (Lat >=  56)) LetterDesignator = 'V';
	else if(( 56 > Lat) && (Lat >=  48)) LetterDesignator = 'U';
	else if(( 48 > Lat) && (Lat >=  40)) LetterDesignator = 'T';
	else if(( 40 > Lat) && (Lat >=  32)) LetterDesignator = 'S';
	else if(( 32 > Lat) && (Lat >=  24)) LetterDesignator = 'R';
	else if(( 24 > Lat) && (Lat >=  16)) LetterDesignator = 'Q';
	else if(( 16 > Lat) && (Lat >=   8)) LetterDesignator = 'P';
	else if((  8 > Lat) && (Lat >=   0)) LetterDesignator = 'N';
	else if((  0 > Lat) && (Lat >=  -8)) LetterDesignator = 'M';
	else if(( -8 > Lat) && (Lat >= -16)) LetterDesignator = 'L';
	else if((-16 > Lat) && (Lat >= -24)) LetterDesignator = 'K';
	else if((-24 > Lat) && (Lat >= -32)) LetterDesignator = 'J';
	else if((-32 > Lat) && (Lat >= -40)) LetterDesignator = 'H';
	else if((-40 > Lat) && (Lat >= -48)) LetterDesignator = 'G';
	else if((-48 > Lat) && (Lat >= -56)) LetterDesignator = 'F';
	else if((-56 > Lat) && (Lat >= -64)) LetterDesignator = 'E';
	else if((-64 > Lat) && (Lat >= -72)) LetterDesignator = 'D';
	else if((-72 > Lat) && (Lat >= -80)) LetterDesignator = 'C';
	else   LetterDesignator = 'Z'; //This is here as an error flag to show that the Latitude is outside the UTM limits
	return LetterDesignator;
};
xyREAL UTMLetter_Lat1Lat2 (char  Cutm) //  return .x= Lat1(min) ,y=Lat2(max) в градусах
{   xyREAL b1b2={0,0};
	if     ((Cutm=='X')||(Cutm=='x')) {b1b2.y= 84; b1b2.x=  72;}
	else if((Cutm=='W')||(Cutm=='w')) {b1b2.y= 72; b1b2.x=  64;}
	else if((Cutm=='V')||(Cutm=='v')) {b1b2.y= 64; b1b2.x=  56;}
	else if((Cutm=='U')||(Cutm=='u')) {b1b2.y= 56; b1b2.x=  48;}
	else if((Cutm=='T')||(Cutm=='t')) {b1b2.y= 48; b1b2.x=  40;}
	else if((Cutm=='S')||(Cutm=='s')) {b1b2.y= 40; b1b2.x=  32;}
	else if((Cutm=='R')||(Cutm=='r')) {b1b2.y= 32; b1b2.x=  24;}
	else if((Cutm=='Q')||(Cutm=='o')) {b1b2.y= 24; b1b2.x=  16;}
	else if((Cutm=='P')||(Cutm=='p')) {b1b2.y= 16; b1b2.x=   8;}
	else if((Cutm=='N')||(Cutm=='n')) {b1b2.y=  8; b1b2.x=   0;}
	else if((Cutm=='M')||(Cutm=='m')) {b1b2.y=  0; b1b2.x=  -8;}
	else if((Cutm=='L')||(Cutm=='l')) {b1b2.y= -8; b1b2.x= -16;}
	else if((Cutm=='K')||(Cutm=='k')) {b1b2.y=-16; b1b2.x= -24;}
	else if((Cutm=='J')||(Cutm=='j')) {b1b2.y=-24; b1b2.x= -32;}
	else if((Cutm=='H')||(Cutm=='h')) {b1b2.y=-32; b1b2.x= -40;}
	else if((Cutm=='G')||(Cutm=='g')) {b1b2.y=-40; b1b2.x= -48;}
	else if((Cutm=='F')||(Cutm=='f')) {b1b2.y=-48; b1b2.x= -56;}
	else if((Cutm=='E')||(Cutm=='e')) {b1b2.y=-56; b1b2.x= -64;}
	else if((Cutm=='D')||(Cutm=='d')) {b1b2.y=-64; b1b2.x= -72;}
	else if((Cutm=='C')||(Cutm=='c')) {b1b2.y=-72; b1b2.x= -80;}
	return b1b2;
};
//                                                                          Target 1..60
xyzREAL TEllipsoid::BLH_To_UTM(double Brad,double Lrad,double H, char ZoneOut[4], long  Nz_UTM)// (B,L,H) in radian, return(x, y, H)
{   xyzREAL utm={0,0,H};
/*
   long    NZgk;
   double  Lz;
   if(!((Wz==6)||(Wz==3))) Wz= 6;
   if(  (Nz< 0)||(Nz>60) ) Nz= 0;
   if(!Nz) { NZgk= NZona_GsK(L,Wz); Lz = LZona_GsK(NZgk, Wz);}
   else    { NZgk= Nz;              Lz = LZona_GsK(NZgk, Wz);}
   xyREAL  pt = BLH_To_GsK_LzOwn(Lz, B, L);//  ->  dL= (L - Lz)
   utm.y = pt.y;
   utm.x = pt.x + double(NZgk)*1000000 + 500000;
*/
    if((Nz_UTM< 0)||(Nz_UTM>60)) Nz_UTM= 0;
    long NzUTM;
    long NzGsK;
    if(!Nz_UTM) NzUTM= NZona_UTM(Lrad);//, Brad);// L(Radian), Wz= 6: Без УЧЕТа РАЗГРАФКи на СЕВЕРЕ
    else        NzUTM= Nz_UTM;

//. NzGsK  = NZona_GsK(Lrad,6);// Для автомат.определения NzGsK
    NzGsK = (NzUTM>30)?(NzUTM-30):(NzUTM+30); // +

    double  Lz= LZona_GsK(NzGsK, 6);
    xyREAL  pt= BLH_To_GsK_LzOwn(Lz, Brad, Lrad);//  ->  dL= (L - Lz)

    utm.x= pt.x*KOEF_UTM + 500000;//   + double(NzUTM)*1000000
    utm.y= pt.y*KOEF_UTM; if(utm.y<0) utm.y+= 10000000;
    if(ZoneOut)
    {  char szUTM= UTMLetterDesignator(RadToDeg(Brad));//  +/-B - градусы
       sprintf(ZoneOut,"%ld%c",NzUTM, szUTM);
    }
    return( utm );
}
// АНАЛОГ GsK:
// (B,L,H) in radian, return( x,y,H )
// .Номер зоны UTM включается в координату (xEast) в виде: +Nz*1000000+500000+x
// yNorth>=0: North, yNorth<0: Sourth без прибавления 10000000.0                   Target 1..60
xyzREAL TEllipsoid::BLH_To_UTMZ(double Brad ,double Lrad,double H, char ZoneOut[4], long Nz_UTM)// =NULL);// (B,L,H) in radian, return(x, y, H)
{   xyzREAL utmz={0,0,H};
    if((Nz_UTM< 0)||(Nz_UTM>60)) Nz_UTM= 0;
    long NzUTM;
    long NzGsK;
    if(!Nz_UTM) NzUTM= NZona_UTM(Lrad);//, Brad);// L(Radian), Wz= 6: Без УЧЕТа РАЗГРАФКи на СЕВЕРЕ
    else        NzUTM= Nz_UTM;

//. NzGsK  = NZona_GsK(Lrad,6);// Для автомат.определения NzGsK
    NzGsK = (NzUTM>30)?(NzUTM-30):(NzUTM+30); // +

    double  Lz= LZona_GsK(NzGsK, 6);
    xyREAL  pt= BLH_To_GsK_LzOwn(Lz, Brad, Lrad);//  ->  dL= (L - Lz)

    utmz.x= pt.x*KOEF_UTM + double(NzUTM)*1000000 + 500000;
    utmz.y= pt.y*KOEF_UTM;
    if(ZoneOut)
    {  char szUTM= UTMLetterDesignator(RadToDeg(Brad));//  +/-B - градусы
       sprintf(ZoneOut,"%ld%c",NzUTM, szUTM);
    }
    return( utmz );
}

// Номер зоны UTM в координате (x) в виде: +Nz*1000000+500000+x
// yNorth>=0: North, yNorth<0: Sourth без прибавления 10000000.0
xyzREAL TEllipsoid::UTMZ_To_BLH(double xEast,double yNorth,double H, char ZoneOut[4])// =NULL);// return xyzREAL(B,L,H) in radian, m
{   xyzREAL  blh={0,0,H};
    long     NzUTM= long(floor(xEast/1000000));
    xEast-=         double(  NzUTM )*1000000;

    long NzGSK = (NzUTM>30)?(NzUTM-30):(NzUTM+30);
//. long NzUTM = (NzGSK>30)?(NzGSK-30):(NzGSK+30);

    double  Lz  = LZona_GsK(NzGSK  ,6);

    xyREAL  gk={(xEast-500000)/KOEF_UTM, yNorth/KOEF_UTM};
// .x= EastCoord->(Yг) не ссодержит: Nz и + 500000.0  и обратно,       B,L: полные
//                             Lz in Radians
    xyREAL BL= LzOwn_GsK_To_BLH(Lz, gk.x, gk.y);// return xyREAL(B,L) in radian
    blh.x= BL.x;
    blh.y= BL.y;
    if(ZoneOut)
    {
       char szUTM= UTMLetterDesignator(RadToDeg(BL.x));//  +/-B - градусы
       sprintf(ZoneOut,"%ld%c",NzUTM, szUTM);
    }
    return blh;
}

// .x= EastCoord не ссодержит: Nz и + 500000.0  и обратно
// .y= (+/-) без добавление +10000000 при .y<0
xyREAL TEllipsoid::BLH_To_UTMZ_NzOwn(long   NzUTM// Номер зоны [1..60] UTM, как правило соседней
                                    ,double Brad // Radian`s: полные
                                    ,double Lrad)// Radian`s: полные
{   xyREAL utmz ={0,0};
//. long NzGSK  = NZona_GsK(Lrad,6);
    long NzGSK  = (NzUTM>30)?(NzUTM-30):(NzUTM+30);
    double  Lz  = LZona_GsK(NzGSK  ,6);
    xyREAL  pt  = BLH_To_GsK_LzOwn(Lz, Brad, Lrad);//  ->  dL= (L - Lz)

    utmz.x= pt.x*KOEF_UTM;// + double(NzUTM)*1000000 + 500000;
    utmz.y= pt.y*KOEF_UTM;
    return  utmz;
}
xyREAL TEllipsoid::NzOwn_UTMZ_To_BLH(long   NzUTM// Номер зоны [1..60] UTM, как правило соседней
                                    ,double xREast
                                    ,double yRNorth) // return xyREAL(B,L) in radian, m
{   xyREAL  BL={0,0};
    long NzGSK = (NzUTM>30)?(NzUTM-30):(NzUTM+30);
//. long NzUTM = (NzGSK>30)?(NzGSK-30):(NzGSK+30);

    double  Lz  = LZona_GsK(NzGSK  ,6);

    xyREAL  gk={(xREast       )/KOEF_UTM, yRNorth/KOEF_UTM};
// .x= EastCoord->(Yг) не ссодержит: Nz и + 500000.0  и обратно,       B,L: полные
//                             Lz in Radians
    BL= LzOwn_GsK_To_BLH(Lz, gk.x, gk.y);// return xyREAL(B,L) in radian
    return BL;
}

xyzREAL TEllipsoid::BLH_To_UTM (double Brad,double Lrad,double Hm, char ZoneOut[4])
{
//converts lat/long to UTM coords.  Equations from USGS Bulletin 1532
//East Longitudes are positive, West longitudes are negative.
//North latitudes are positive, South latitudes are negative
//Lat and Long are in decimal degrees
//Written by Chuck Gantz- chuck.gantz@globalstar.com
    char szZoneOut[4]="";
    xyzREAL utm= {0};
            utm.z= Hm;
    double  Lat = RadToDeg(Brad);
    double  Long= RadToDeg(Lrad);
    double  UTMEasting ;
    double  UTMNorthing;
	double  LongOrigin;
	double  eccPrimeSquared;
	double  N, T, C, A, M;
	double a         = ELL_a;   //ellipsoid[ReferenceEllipsoid].EquatorialRadius;
	double eccSquared= ELL_e1E2;//ellipsoid[ReferenceEllipsoid].eccentricitySquared;
	double k0 = KOEF_UTM;

//Make sure the longitude is between -180.00 .. 179.9
	double LongTemp = (Long+180)-int((Long+180)/360)*360-180; // -180.00 .. 179.9;

	double LatRad  = DegToRad(Lat     );
	double LongRad = DegToRad(LongTemp);
	double LongOriginRad;
	int    ZoneNumber;

	ZoneNumber = int((LongTemp + 180)/6) + 1;

	if( Lat >= 56.0 && Lat < 64.0 && LongTemp >= 3.0 && LongTemp < 12.0 ) ZoneNumber = 32;

  // Special zones for Svalbard
	if( Lat >= 72.0 && Lat < 84.0 )
	{
        if(      LongTemp >= 0.0  && LongTemp <  9.0 ) ZoneNumber = 31;
        else if( LongTemp >= 9.0  && LongTemp < 21.0 ) ZoneNumber = 33;
        else if( LongTemp >= 21.0 && LongTemp < 33.0 ) ZoneNumber = 35;
        else if( LongTemp >= 33.0 && LongTemp < 42.0 ) ZoneNumber = 37;
	 }
	LongOrigin = (ZoneNumber - 1)*6 - 180 + 3;  //+3 puts origin in middle of zone
	LongOriginRad = DegToRad(LongOrigin);

//  compute the UTM Zone from the latitude and longitude
	sprintf(szZoneOut, "%ld%c", ZoneNumber, UTMLetterDesignator(Lat));

	eccPrimeSquared = (eccSquared)/(1-eccSquared);

	N = a/sqrt(1-eccSquared*sin(LatRad)*sin(LatRad));
	T = tan(LatRad)*tan(LatRad);
	C = eccPrimeSquared*cos(LatRad)*cos(LatRad);
	A = cos(LatRad)*(LongRad-LongOriginRad);

	M = a*((1- eccSquared/4		- 3*eccSquared*eccSquared/64  -  5*eccSquared*eccSquared*eccSquared/256)*LatRad
				- (3*eccSquared/8	+ 3*eccSquared*eccSquared/32  + 45*eccSquared*eccSquared*eccSquared/1024)*sin(2*LatRad)
									+ (15 *eccSquared*eccSquared/256 + 45*eccSquared*eccSquared*eccSquared/1024)*sin(4*LatRad)
									- (35 *eccSquared*eccSquared*eccSquared/3072)*sin(6*LatRad));

	UTMEasting = (double)(k0*N*(A+(1-T+C)*A*A*A/6
					+ (5-18*T+T*T+72*C-58*eccPrimeSquared)*A*A*A*A*A/120)
					+ 500000.0);

	UTMNorthing = (double)(k0*(M+N*tan(LatRad)*(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24
				 + (61-58*T+T*T+600*C-330*eccPrimeSquared)*A*A*A*A*A*A/720)));

   if(Lat < 0)  UTMNorthing += 10000000.0; //10000000 meter offset for southern hemisphere
   utm.x= UTMEasting ;
   utm.y= UTMNorthing;
   if( ZoneOut ) strcpy(ZoneOut, szZoneOut);
   return( utm );
}//xyzREAL TEllipsoid::BLH_To_UTM(double B,double L,double H, char ZoneOut[4])

// ^from ...
xyzREAL TEllipsoid::BLH_To_UTM_NzOwn(double Brad,double Lrad,double Hm, char ZoneIn[4])
{
//converts lat/long to UTM coords.  Equations from USGS Bulletin 1532
//East Longitudes are positive, West longitudes are negative.
//North latitudes are positive, South latitudes are negative
//Lat and Long are in decimal degrees
//Written by Chuck Gantz- chuck.gantz@globalstar.com
//. char szZoneOut[4]="";
    xyzREAL utm= {0};
            utm.z= Hm;
    double  Lat = RadToDeg(Brad);
    double  Long= RadToDeg(Lrad);
    double  UTMEasting ;
    double  UTMNorthing;
	double  LongOrigin;
	double  eccPrimeSquared;
	double  N, T, C, A, M;
	double a         = ELL_a;   //ellipsoid[ReferenceEllipsoid].EquatorialRadius;
	double eccSquared= ELL_e1E2;//ellipsoid[ReferenceEllipsoid].eccentricitySquared;
	double k0 = KOEF_UTM;

//Make sure the longitude is between -180.00 .. 179.9
	double LongTemp = (Long+180)-int((Long+180)/360)*360-180; // -180.00 .. 179.9;

	double LatRad  = DegToRad(Lat     );
	double LongRad = DegToRad(LongTemp);
	double LongOriginRad;
    char*  ZoneLetter;
	long   ZoneNumber;
//         В данном случае = исходному из ZoneIn
           ZoneNumber= strtoul(ZoneIn, &ZoneLetter, 10);
//.
//. long   Nz=    ZoneNumber;  if(1>Nz)||(Nz>60)) { MBeep();MBeep(); Nz= 1;}
//.        if(Nz>30) Nz-= 30;
//.        else      Nz+= 30;
//.        tiLZonaCentr = DegToRad * ( 6.0*Nz - 3.0 );
//.

/* +
	ZoneNumber = int((LongTemp + 180)/6) + 1;
	if( Lat      >= 56.0 && Lat      < 64.0 &&
        LongTemp >=  3.0 && LongTemp < 12.0 ) ZoneNumber = 32;
    if( Lat      >= 72.0 && Lat      < 84.0 )//  Special zones for Svalbard
	{
        if(      LongTemp >= 0.0  && LongTemp <  9.0 ) ZoneNumber = 31;
        else if( LongTemp >= 9.0  && LongTemp < 21.0 ) ZoneNumber = 33;
        else if( LongTemp >= 21.0 && LongTemp < 33.0 ) ZoneNumber = 35;
        else if( LongTemp >= 33.0 && LongTemp < 42.0 ) ZoneNumber = 37;
	}
+ */

	LongOrigin = (ZoneNumber - 1)*6 - 180 + 3;  //+3 puts origin in middle of zone
	LongOriginRad = DegToRad(LongOrigin);
/* +
//  compute the UTM Zone from the latitude and longitude
	sprintf(szZoneOut, "%ld%c", ZoneNumber, UTMLetterDesignator(Lat));
+ */

	eccPrimeSquared = (eccSquared)/(1-eccSquared);

	N = a/sqrt(1-eccSquared*sin(LatRad)*sin(LatRad));
	T = tan(LatRad)*tan(LatRad);
	C = eccPrimeSquared*cos(LatRad)*cos(LatRad);
	A = cos(LatRad)*(LongRad-LongOriginRad);

	M = a*((1- eccSquared/4	- 3*eccSquared*eccSquared/64  -  5*eccSquared*eccSquared*eccSquared/256)*LatRad
	     - (3 *eccSquared/8	+ 3*eccSquared*eccSquared/32  + 45*eccSquared*eccSquared*eccSquared/1024)*sin(2*LatRad)
         + (15*eccSquared*eccSquared/256 + 45*eccSquared*eccSquared*eccSquared/1024)*sin(4*LatRad)
         - (35*eccSquared*eccSquared*eccSquared/3072)*sin(6*LatRad));

	UTMEasting = (double)(k0*N*(A+(1-T+C)*A*A*A/6
	             + (5-18*T+T*T+72*C-58*eccPrimeSquared)*A*A*A*A*A/120) + 500000.0);

	UTMNorthing = (double)(k0*(M+N*tan(LatRad)*(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24
				 + (61-58*T+T*T+600*C-330*eccPrimeSquared)*A*A*A*A*A*A/720)));

    if(Lat < 0)  UTMNorthing += 10000000.0; //10000000 meter offset for southern hemisphere
    utm.x= UTMEasting ;
    utm.y= UTMNorthing;
//. if( ZoneOut ) strcpy(ZoneOut, szZoneOut);
    return( utm );
}//xyzREAL TEllipsoid::BLH_To_UTM_NzOwn(double B,double L,double H, char ZoneIn[4])

char  * TEllipsoid::ZonaUTM_BL(double B,double L)
{   long  Wz= 6;// для UTM
    char  sz = UTMLetterDesignator(RadToDeg(B));//  B - градусы |внутренняя функция|
    int j=0;
    while(fabs(L) > rwPi)// -180..+180
    {  j++;
       if(L<0) L += rwPiM2;
       else    L -= rwPiM2;
       if(j==2) break;
    };
    long   Lt = (long)RadToDeg(L);
	long   nz = ((Lt+180)/Wz + 1);
    sprintf(ZonaUTM,"%ld%c",nz,sz);
    return  ZonaUTM;
};

//   x= Nz*1000000+500000+Dx, где Nz=(ZonaUTM)
//                           East     North: y>=0, South: y<0
char  * TEllipsoid::ZonaUTMZ(double x,double y)
{
/***
    long  Nz= long(floor(x/1000000));
          x-= double( Nz )*1000000;
    sprintf(ZonaUTM,"%ld%c",Nz, (y<0)?'M':'N' );
    if(y<0) y+= 10000000.0;
    xyzREAL pw= UTM_To_BLH(   x,   y, 0.0, ZonaUTM);
    return      ZonaUTM_BL(pw.x,pw.y);
***/
    xyzREAL blh= UTMZ_To_BLH(x, y, 0, ZonaUTM);// return xyzREAL(B,L,H) in radian, m
    return                            ZonaUTM;
};

/// #define Kravchenko
double TEllipsoid::Calc_XB(double B)// [In: Radian] Возврат длины дуги меридиана от экватора до (B)  в метрах
{  double X=0.0;
#ifdef  Kravchenko
   double es= asin(ELL_e1);
//        LPW("@ asin(ELL_e1)=%s", rwStrRadian(es)); -> 4°41'34.0939037"
   double tes= tan(es);
   double k  = tes/tan(es/2.0);
// double p  = sqrt(1.0+tes*tes/(2.0*k));  LPW("@ p =%15.13f",p); ==
   double p  = sqrt(k/2.0);            //  LPW("@ p`=%15.13f",p); == ^
   double u  = Calc_Bu(B);
          X  = ELL_b*((1.0+k)*u-2.0*p*atan(p*tan(u)));
#else
 #ifdef  Haimov
//      Хаимов, стр.77
   double CELL= BELL/(1.0-EM1);
   double e2     =        EM2;
   double e4= e2*e2;
   double e6= e4*e2;
   double e8= e6*e2;
   double kb0= 1.0 - 3.0*e2/4.0 + 45.0*e4/64.0 - 175.0*e6/256.0 + 11025.0*e8/16384.0;
   double kb1= kb0 - 1.0;
   double kb2=                    15.0*e4/32.0 - 175.0*e6/384.0 +  3675.0*e8/ 8192.0;
   double kb3=                                 -  35.0*e6/ 96.0 +   735.0*e8/ 2048.0;
   double kb4=                                                      315.0*e8/ 1024.0;
// F( B ):
   double cosB_1 = cos(B);
   double cosB_2 = cosB_1*cosB_1;
   double cosB_3 = cosB_2*cosB_1;
   double cosB_5 = cosB_3*cosB_2;
   double cosB_7 = cosB_5*cosB_2;
   X=    kb0*   B
     + ( kb1*cosB_1
       + kb2*cosB_3
       + kb3*cosB_5
       + kb4*cosB_7) * sin(B) * CELL;
 #else
// F( B ):
   double cosB_1 = cos(B);
   double sinB_1 = sin(B);
   double sinB_2 = sinB_1*sinB_1;
   double sinB_4 = sinB_2*sinB_2;
 //X= ka[0]*B-sinB_1*cosB_1*( (ka[2]-ka[4]+ka[6])+(2.0*ka[4]-wk6)*sinB_2 + wk6*sinB_4);
   X= Ka0  *B-sinB_1*cosB_1*(  wk246             +           wk46*sinB_2 + wk6*sinB_4);
 #endif
#endif
   return X;
}//double TEllipsoid::Calc_M_ofB(double B)

double TEllipsoid::Calc_BX(double M)// [In: Meter ] Возврат широты, соотв дуге меридиана от экватора в радианах
{  double Bx= 0;
#ifdef  Kravchenko
   double es= asin(ELL_e1);// LPW("@ asin(ELL_e1)=%s", rwStrRadian(es));
   double tes= tan(es);
   double k  = tes/tan(es/2.0);
// double p  = sqrt(1.0+tes*tes/(2.0*k));  LPW("@ p =%15.13f",p); ==
   double p  = sqrt(k/2.0);            //  LPW("@ p`=%15.13f",p); == ^
// double u  = Calc_Bu(B);
//        X  = ELL_b*((1.0+k)*u-2.0*p*atan(p*tan(u)));
//        u  = (X/ELL_b + 2.0*p*atan(p*tan(u))/(1.0+k);
   double Uc = (M/ ELL_b);
   double Bu = Uc/(1.0+k);
   double Bw;
   int i;
   for(i=0; i<255; i++)
   {      Bw=   Bu;
          Bu = (Uc + 2.0*p*atan(p*tan(Bu)))/(1.0+k);
   //...  if(fabs(Bu-Bw)<=0.000000000001)     break;//  65 ИТЕРАЦИЙ (  0 mm)
          if(fabs(Bu-Bw)<=0.00000000001)     break; //  59 ИТЕРАЦИЙ (  0 mm)
   //     if(fabs(Bu-Bw)<=0.00000000005)     break; //  55 ИТЕРАЦИЙ (- 1 mm)
   //...  if(fabs(Bu-Bw)<=0.0000000001)     break;  //  53 ИТЕРАЦИЙ (- 1 mm)
   //...  if(fabs(Bu-Bw)<=0.000000001)     break;   //  48 ИТЕРАЦИЙ (-10 mm)
   }
//.LPW("@ i=%ld",i);
   Bx=  Calc_Bc(Bu);
#else
// Морозов, стр.29~ + вывод формул
   double bt    = M / Ka0;
   double cosB_1= cos(bt);
   double sinB_1= sin(bt);
   double sinB_2= sinB_1*sinB_1;
   double sinB_4= sinB_2*sinB_2;
// Bx = bt + sinB_1*cosB_1*( (2.0*pq[1]+4.0*pq[3]+6.0*pq[5])-(8.0*pq[3]+wq6)*sinB_2 + wq6*sinB_4);
   Bx = bt + sinB_1*cosB_1*(  wq246                         -           wq46*sinB_2 + wq6*sinB_4);
#endif
   return(Bx);
}//double TEllipsoid::Calc_B_ofM(double M)

// .x= EastCoord->(Yг) не ссодержит: Nz и + 500000.0
xyREAL TEllipsoid::LzOwn_GsK_To_BLH(double Lz, double dYg,double Xg)// return xyzREAL(B,L,H) in radian
{  xyREAL bl;
// Морозов В.П. стр.228 (!ok) 09.05.02
   double Yg = dYg;
   double Bx = Calc_BX(Xg);
   double sinB_1 = sin(Bx);
   double cosB_1 = cos(Bx);
   double tanB_1 = tan(Bx);
   double cosB_2 = cosB_1*cosB_1;
   double sinB_2 = sinB_1*sinB_1;
   double N_1 = AELL/sqrt(1-EM1*sinB_2);
   double n_2 = EM2*cosB_2;//(Вспомогательная функция эсцентриситета)**
   double n_4 = n_2*n_2;
   double n_6 = n_4*n_2;
   double V_2 = 1.0 + n_2;
// double N_1 = N;
   double N_2 = N_1*N_1;
   double N_4 = N_2*N_2;
   double N_6 = N_4*N_2;
   double tanB_2 = tanB_1*tanB_1;
   double tanB_4 = tanB_2*tanB_2;
   double tanB_6 = tanB_4*tanB_2;

   double ab[9]={0};
   ab[2]= - V_2*(tanB_1);      ab[2] /=  2.0 * N_2;
   ab[1]= + 1.0/(N_1*cosB_1);//ab[1] /=  1.0;
   ab[4]= -ab[2]*(   5.0+   3.0*tanB_2+ n_2-9.0*n_2*tanB_2-4.0*n_4);
   ab[3]= -ab[1]*(   1.0+   2.0*tanB_2+ n_2);
   ab[6]= +ab[2]*(  61.0+  90.0*tanB_2+  45.0*tanB_4+46.0*n_2-252.0*n_2*tanB_2-90.0*n_2*tanB_4);
   ab[5]= +ab[1]*(   5.0+  28.0*tanB_2+  24.0*tanB_4+ 6.0*n_2+  8.0*n_2*tanB_2);
   ab[8]= -ab[2]*(1385.0+3633.0*tanB_2+4095.0*tanB_4+1575.0*tanB_6);
   ab[7]= -ab[1]*(  61.0+ 662.0*tanB_2+1320.0*tanB_4+ 720.0*tanB_6);
   ab[4]/=    12.0 * N_2;
   ab[3]/=     6.0 * N_2;
   ab[6]/=   360.0 * N_4;
   ab[5]/=   120.0 * N_4;
   ab[8]/= 20160.0 * N_6;
   ab[7]/=  5040.0 * N_6;

   double y_1 = Yg;
   double y_2 = y_1*y_1;
   double y_3 = y_2*y_1;
   double y_4 = y_3*y_1;
   double y_5 = y_4*y_1;
   double y_6 = y_5*y_1;
   double y_7 = y_6*y_1;
   double y_8 = y_7*y_1;

   bl.x= Bx + ab[2]*y_2 + ab[4]*y_4 + ab[6]*y_6 + ab[8]*y_8;// B=
   bl.y= Lz + ab[1]*y_1 + ab[3]*y_3 + ab[5]*y_5 + ab[7]*y_7;// L=
   return bl;
}//xyREAL TEllipsoid::LzOwn_GsK_To_BLH(

// .x= EastCoord->(Yг) не ссодержит: Nz и +500000.0
xyREAL TEllipsoid::BLH_To_GsK_LzOwn(double Lz, double  B,double L)// (B,L,H) in radian, return(Yg,Xg,H )
{  xyREAL yx;
// void TGaussKruger::GeodGaus_LzOwn(double Lz,double B,double L, double &x,double &y)
// Морозов В.П., стр.222-225 (!ok)  14.04.02
   double sinB_1 = sin(B);
   double cosB_1 = cos(B);
   double tanB_1 = tan(B);
   double cosB_2 = cosB_1*cosB_1;
   double sinB_2 = sinB_1*sinB_1;
   double N   = AELL/sqrt(1-EM1*sinB_2);
   double n_2 = EM2*cosB_2;//(Вспомогательная функция эсцентриситета)**
   double n_4 = n_2*n_2;

   double sinBxN = sinB_1*N;

   double tanB_2 = tanB_1*tanB_1;
   double tanB_4 = tanB_2*tanB_2;
   double tanB_6 = tanB_4*tanB_2;
   double cosB_3 = cosB_2*cosB_1;
   double cosB_5 = cosB_3*cosB_2;
   double cosB_7 = cosB_5*cosB_2;

   double ab[9]={0};
   ab[2] = sinBxN*cosB_1;
   ab[1] =      N*cosB_1;
   ab[4] = sinBxN*cosB_3*(   5.0-       tanB_2+               9.0*n_2+4.0*n_4);
   ab[3] =      N*cosB_3*(   1.0-       tanB_2+                   n_2        );
   ab[6] = sinBxN*cosB_5*(  61.0-  58.0*tanB_2+      tanB_4+270.0*n_2-330.0*n_2*tanB_2);
   ab[5] =      N*cosB_5*(   5.0-  18.0*tanB_2+      tanB_4+ 14.0*n_2- 58.0*n_2*tanB_2);
   ab[8] = sinBxN*cosB_7*(1385.0-3111.0*tanB_2+543.0*tanB_4-tanB_6);
   ab[7] =      N*cosB_7*(  61.0- 479.0*tanB_2+179.0*tanB_4-tanB_6);
   ab[2]/=     2.0;
///ab[1]/=     1.0;
   ab[4]/=    24.0;
   ab[3]/=     6.0;
   ab[6]/=   720.0;
   ab[5]/=   120.0;
   ab[8]/= 40320.0;
   ab[7]/=  5040.0;

   double dL_1 = (L-Lz);
   double dL_2 = dL_1*dL_1;
   double dL_3 = dL_2*dL_1;
   double dL_4 = dL_3*dL_1;
   double dL_5 = dL_4*dL_1;
   double dL_6 = dL_5*dL_1;
   double dL_7 = dL_6*dL_1;
   double dL_8 = dL_7*dL_1;

   double Xg = Calc_XB(B) + ab[2]*dL_2 + ab[4]*dL_4 + ab[6]*dL_6 + ab[8]*dL_8;
   double Yg =              ab[1]*dL_1 + ab[3]*dL_3 + ab[5]*dL_5 + ab[7]*dL_7;

   yx.x= Yg;
   yx.y= Xg;
   return yx;
}//xyREAL TEllipsoid::BLH_To_GsK_LzOwn(


//                             East      North     Геодезическая высота(относ. эллипсоида)->ТРАНСЛЯЦИЯ
xyzREAL TEllipsoid::GsK_To_BLH(double Yg,double Xg,double H, long Wz) // return xyzREAL(B,L,H) in radian, m
{  xyzREAL blh= {0};
           blh.z= H;
   long   Nz= long( floor(Yg / 1000000));
   double  y= fmod(Yg, 1000000) - 500000;
   double Lz= LZona_GsK(Nz,Wz);
   xyREAL pt= LzOwn_GsK_To_BLH(Lz, y, Xg);
   blh.x= pt.x;
   blh.y= pt.y;
   return  blh;
}//xyzREAL TEllipsoid::GsK_To_BLH(double x,double y,double h, long Wz) // return xyzREAL(B,L,H) in radian, m

// return( x: East , y: North, H: Геодезическая высота(относ. эллипсоида)->ТРАНСЛЯЦИЯ)
xyzREAL TEllipsoid::BLH_To_GsK(double B,double L,double H, long Wz, long Nz)// (B,L,H) in radian, return(.x=Yg,.y=Xg,H )
{  xyzREAL yxh={0,0,H};
   long    NZgk;
   double  Lz;
   if(!((Wz==6)||(Wz==3))) Wz= 6;
   if(  (Nz< 0)||(Nz>60) ) Nz= 0;
   if(!Nz) { NZgk= NZona_GsK(L,Wz); Lz = LZona_GsK(NZgk, Wz);}
   else    { NZgk= Nz;              Lz = LZona_GsK(NZgk, Wz);}
   xyREAL  pt = BLH_To_GsK_LzOwn(Lz, B, L);//  ->  dL= (L - Lz)
   yxh.y = pt.y;
   yxh.x = pt.x + double(NZgk)*1000000 + 500000;
   return  yxh;
}//xyzREAL TEllipsoid::BLH_To_GsK(double B,double L,double H, long Wz) // (B,L,H) in radian, return( x,y,h )

/*
//LatLong- UTM conversion.cpp
//Lat Long - UTM, UTM - Lat Long conversions
//. ++++
//.const double PI = double(3.14159265358979323846);
//.const double FOURTHPI= PI    /   4;
//.const double DegToRad = PI    / 180;
//.const double RadToDeg = 180.0 /  PI;
//. ++++
//.
//.#define       RWpi     double(3.14159265358979323846)
//.const  double rwCRadian= 648000.0/RWpi;
//.const  double rwCSecond= RWpi/648000.0;
//.inline double rwSecond(double radian) { return( radian*rwCRadian );};
//.inline double rwRadian(double second) { return( second*rwCSecond );};
//.
//________________________________________________________________________________________.
//.class Ellipsoid
struct   Ellipsoid
{
//
// public:
//	  Ellipsoid(){};
//	  Ellipsoid(int Id, char* name, double radius, double ecc)
//	  {  id                 = Id; 
//      ellipsoidName      = name; 
//      EquatorialRadius   = radius;
//      eccentricitySquared= ecc;
//	  };
//
	  int    id;
	  char*  ellipsoidName;
	  double EquatorialRadius; 
	  double eccentricitySquared;  
};
//________________________________________________________________________________________.   

//static Ellipsoid ellipsoid[] = 
         Ellipsoid ellipsoid[] = 
{//  id, Ellipsoid name, Equatorial Radius, square of eccentricity	
   { -1, "Placeholder"           ,       0,           0},
			{  1, "Airy"                  , 6377563, 0.00667054 },
			{  2, "Australian National"   , 6378160, 0.006694542},
			{  3, "Bessel 1841"           , 6377397, 0.006674372},
			{  4, "Bessel 1841 (Nambia)"  , 6377484, 0.006674372},
			{  5, "Clarke 1866"           , 6378206, 0.006768658},
			{  6, "Clarke 1880"           , 6378249, 0.006803511},
			{  7, "Everest"               , 6377276, 0.006637847},
			{  8, "Fischer 1960 (Mercury)", 6378166, 0.006693422},
			{  9, "Fischer 1968"          , 6378150, 0.006693422},
			{ 10, "GRS 1967"              , 6378160, 0.006694605},
			{ 11, "GRS 1980"              , 6378137, 0.00669438 },
			{ 12, "Helmert 1906"          , 6378200, 0.006693422},
			{ 13, "Hough"                 , 6378270, 0.00672267 },
			{ 14, "International"         , 6378388, 0.00672267 },
			{ 15, "Krassovsky"            , 6378245, 0.006693422},
			{ 16, "Modified Airy"         , 6377340, 0.00667054 },
			{ 17, "Modified Everest"      , 6377304, 0.006637847},
			{ 18, "Modified Fischer 1960" , 6378155, 0.006693422},
			{ 19, "South American 1969"   , 6378160, 0.006694542},
			{ 20, "WGS 60"                , 6378165, 0.006693422},
			{ 21, "WGS 66"                , 6378145, 0.006694542},
			{ 22, "WGS-72"                , 6378135, 0.006694318},
			{ 23, "WGS-84"                , 6378137, 0.00669438 }
};
*/
/* Reference ellipsoids derived from Peter H. Dana's website- 
http://www.utexas.edu/depts/grg/gcraft/notes/datum/elist.html
Department of Geography, University of Texas at Austin
Internet: pdana@mail.utexas.edu
3/22/95

Source
Defense Mapping Agency. 1987b. DMA Technical Report: Supplement to Department of Defense World Geodetic System
1984 Technical Report. Part I and II. Washington, DC: Defense Mapping Agency
*/

//.
//.void LLtoUTM(int   ReferenceEllipsoid, 
//.             const double Lat, const double Long, 
//.                   double &UTMNorthing, 
//.                   double &UTMEasting, 
//.                   char *  UTMZone)
//.
// return .x,.y,.z= 0

/* ++++
xyzREAL LLtoUTM(const   double Latr      //  B - radian
               ,const   double Longr     //  L - radian
               ,char   * UTMZone         
               ,int ReferenceEllipsoid)  // =23 WGS-84
{  xyzREAL utm={0};
   double  UTMEasting ;
   double  UTMNorthing;
   double  Lat = Latr * RadToDeg;
   double  Long= Longr* RadToDeg;
//converts lat/long to UTM coords.  Equations from USGS Bulletin 1532 
//East Longitudes are positive, West longitudes are negative. 
//North latitudes are positive, South latitudes are negative
//Lat and Long are in decimal degrees
//Written by Chuck Gantz- chuck.gantz@globalstar.com
	double a          = ellipsoid[ReferenceEllipsoid].EquatorialRadius;
	double eccSquared = ellipsoid[ReferenceEllipsoid].eccentricitySquared;
	double k0 = 0.9996;

	double LongOrigin;
	double eccPrimeSquared;
	double N, T, C, A, M;
	
//Make sure the longitude is between -180.00 .. 179.9
	double LongTemp = (Long+180)-int((Long+180)/360)*360-180; // -180.00 .. 179.9;

	double LatRad  = Lat*DegToRad;
	double LongRad = LongTemp*DegToRad;
	double LongOriginRad;
	int    ZoneNumber;

	ZoneNumber = int((LongTemp + 180)/6) + 1;

	if( Lat >= 56.0 && Lat < 64.0 && LongTemp >= 3.0 && LongTemp < 12.0 )
		ZoneNumber = 32;

  // Special zones for Svalbard
	if( Lat >= 72.0 && Lat < 84.0 ) 
	{
	  if(      LongTemp >= 0.0  && LongTemp <  9.0 ) ZoneNumber = 31;
	  else if( LongTemp >= 9.0  && LongTemp < 21.0 ) ZoneNumber = 33;
	  else if( LongTemp >= 21.0 && LongTemp < 33.0 ) ZoneNumber = 35;
	  else if( LongTemp >= 33.0 && LongTemp < 42.0 ) ZoneNumber = 37;
	 }
	LongOrigin = (ZoneNumber - 1)*6 - 180 + 3;  //+3 puts origin in middle of zone
	LongOriginRad = LongOrigin * DegToRad;

	//compute the UTM Zone from the latitude and longitude
	sprintf(UTMZone, "%ld%c", ZoneNumber, UTMLetterDesignator(Lat));

	eccPrimeSquared = (eccSquared)/(1-eccSquared);

	N = a/sqrt(1-eccSquared*sin(LatRad)*sin(LatRad));
	T = tan(LatRad)*tan(LatRad);
	C = eccPrimeSquared*cos(LatRad)*cos(LatRad);
	A = cos(LatRad)*(LongRad-LongOriginRad);

	M = a*((1- eccSquared/4		- 3*eccSquared*eccSquared/64  -  5*eccSquared*eccSquared*eccSquared/256)*LatRad 
				- (3*eccSquared/8	+ 3*eccSquared*eccSquared/32  + 45*eccSquared*eccSquared*eccSquared/1024)*sin(2*LatRad)
									+ (15 *eccSquared*eccSquared/256 + 45*eccSquared*eccSquared*eccSquared/1024)*sin(4*LatRad) 
									- (35 *eccSquared*eccSquared*eccSquared/3072)*sin(6*LatRad));
	
	UTMEasting = (double)(k0*N*(A+(1-T+C)*A*A*A/6
					+ (5-18*T+T*T+72*C-58*eccPrimeSquared)*A*A*A*A*A/120)
					+ 500000.0);

	UTMNorthing = (double)(k0*(M+N*tan(LatRad)*(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24
				 + (61-58*T+T*T+600*C-330*eccPrimeSquared)*A*A*A*A*A*A/720)));

   if(Lat < 0) UTMNorthing += 10000000.0; //10000000 meter offset for southern hemisphere
   utm.x= UTMEasting ;
   utm.y= UTMNorthing;
   return( utm );
}

//.
//.void UTMtoLL(int   ReferenceEllipsoid, 
//.             const double UTMNorthing, 
//.             const double UTMEasting, 
//.             const char*  UTMZone,
//.			          double & Lat,  double & Long )
//.
//. return  .x=B,.y=L,.z= 0
xyzREAL UTMtoLL(const   double UTMEasting // .x
               ,const   double UTMNorthing// .y
               ,const   char * UTMZone
               ,int ReferenceEllipsoid)   // =23 WGS-84      
{ xyzREAL  blh= {0};
  double   Lat,Long;
//converts UTM coords to lat/long.  Equations from USGS Bulletin 1532
//East Longitudes are positive, West longitudes are negative. 
//North latitudes are positive, South latitudes are negative
//Lat and Long are in decimal degrees. 
//Written by Chuck Gantz- chuck.gantz@globalstar.com
	double k0 = 0.9996;
	double a          = ellipsoid[ReferenceEllipsoid].EquatorialRadius;
	double eccSquared = ellipsoid[ReferenceEllipsoid].eccentricitySquared;
	double eccPrimeSquared;
	double e1 = (1-sqrt(1-eccSquared))/(1+sqrt(1-eccSquared));
	double N1, T1, C1, R1, D, M;
	double LongOrigin;
	double mu, phi1, phi1Rad;
	double x, y;
	int    ZoneNumber;
	char*  ZoneLetter;
	int    NorthernHemisphere; //1 for northern hemispher, 0 for southern

	x = UTMEasting - 500000.0; //remove 500,000 meter offset for longitude
	y = UTMNorthing;

	ZoneNumber = strtoul(UTMZone, &ZoneLetter, 10);
	if((*ZoneLetter - 'N') >= 0)
		NorthernHemisphere = 1;//point is in northern hemisphere
	else
	{  NorthernHemisphere = 0;//point is in southern hemisphere
		y -= 10000000.0;//remove 10,000,000 meter offset used for southern hemisphere
	}

	LongOrigin = (ZoneNumber - 1)*6 - 180 + 3;  //+3 puts origin in middle of zone

	eccPrimeSquared = (eccSquared)/(1-eccSquared);

	M = y / k0;
	mu = M/(a*(1 -                         eccSquared/4 
                -            3*eccSquared*eccSquared/64
                - 5*eccSquared*eccSquared*eccSquared/256));

	phi1Rad = mu +     (3*e1/2  -    27*e1*e1*e1/32)*sin(2*mu) 
				    + (21*e1*e1/16 - 55*e1*e1*e1*e1/32)*sin(4*mu)
				    +                 (151*e1*e1*e1/96)*sin(6*mu);
	phi1 = phi1Rad*RadToDeg;

	N1 = a/sqrt(1-eccSquared*sin(phi1Rad)*sin(phi1Rad));
	T1 = tan(phi1Rad)*tan(phi1Rad);
	C1 = eccPrimeSquared*cos(phi1Rad)*cos(phi1Rad);
	R1 = a*(1-eccSquared)/pow(1-eccSquared*sin(phi1Rad)*sin(phi1Rad), 1.5);
	D  = x/(N1*k0);

	Lat = phi1Rad - (N1*tan(phi1Rad)/R1)
                  *(D*D/2
                    -(5+3*T1+10*C1-4*C1*C1-9*eccPrimeSquared)*D*D*D*D/24
			           +(61+90*T1+298*C1+45*T1*T1-252*eccPrimeSquared-3*C1*C1)*D*D*D*D*D*D/720);
//	Lat  = Lat * RadToDeg;
// blh.x= Lat * DegToRad;
   blh.x= Lat;

	Long = (D - (1+2*T1+C1)*D*D*D/6 
             + (5-2*C1+28*T1-3*C1*C1+8*eccPrimeSquared+24*T1*T1)*D*D*D*D*D/120)
         /cos(phi1Rad);
//	Long = LongOrigin + Long * RadToDeg;
// blh.y= Long*DegToRad;
   blh.y= LongOrigin * DegToRad + Long;
   return( blh );
}
++++ */


double Dist2D(xyREAL p1, xyREAL p2)
{
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

double Dist3D(xyzREAL p1, xyzREAL p2)
{
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y) + (p1.z - p2.z)*(p1.z - p2.z));
}
