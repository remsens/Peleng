#ifndef __Ellipsoid_H
#define __Ellipsoid_H

#ifndef        ELL_KRAS42
   #define     ELL_SPHERE    long(0)
   #define     ELL_WGS84     long(1)
   #define     ELL_PZ90      long(2)
   #define     ELL_KRAS42    long(3)
   #define     ELL_KRAS95    long(4)
   #define     ELL_BESSEL    long(5)
#endif

#define KOEF_UTM        0.9996
#define KOEF_UTM_BACK   1.00040016006402561  /*  1./0.9996  */

#ifndef      RWpi
//#define     vmPi      double(3.14159265358979323846 )
  #define     RWpi      double(3.14159265358979323846 )
  #define     RWpiL           (3.14159265358979323846L)
  #define     RWpi2    (RWpi/2.0)
#endif

////  for ATan2 & ATan2Pi
#ifndef      rwPi
  #define     rwPi      RWpi
  #define     rwPiD2   (RWpi/2.0)
  #define     rwPiM2   (RWpi*2.0)
#endif


#ifndef    _CASE
   #define  CASE          case
   #define _CASE    break;case
   #define _case    break;case
   #define _DEFAULT break;default
   #define _default break;default
#endif

#ifndef    SwapValue
   #define SwapValue(a,b)  {(a)^=(b);(b)^=(a);(a)^=(b);}
// + #define SwapValue(a,b)   (a)^=    (b)^=    (a)^=(b)
#endif

#ifndef _XYZ_REAL_
#define _XYZ_REAL_
   struct  xyREAL { double   x,y; };  typedef   xyREAL*   LPxyREAL;
   struct  xyzREAL{ double x,y,z; };  typedef  xyzREAL*  LPxyzREAL;
#endif

#ifndef _LONGREAL_
#define _LONGREAL_
   typedef  long double     LDouble;
   typedef  long double *  PLDouble;
   struct    xyLDouble{ LDouble      x,y; };
   struct   xyzLDouble{ LDouble    x,y,z; };
   struct  txyzLDouble{ LDouble t, x,y,z; };
#endif


// �����(����)-06-278-04 ... (��-95).pdf

// CK-95 >> WGS84
// DX    =  +24.653 �  DY = - 129.136 � DZ = - 83.057 �
// RX    = - 0.06696"  RY = +0.00391"   RZ= - 0.12902"
// Scale = - 0.175 * 10^-6.
//                  WE= 7.292115 * 10-5;   // ���/���:  ������� �������� �������� ����� WGS-84
#define We_Earth double(7.2921151467e-5)   /* �.�.���������, ���� "������" ������ ������ �����*/
#define fM_Earth double(398600.43996e+9)   /* m^3/s^2) �.�.������ -> �.�.������]              */

// fabs(Angle) < 2*Pi
#define Range2Pi(Angle) (((Angle)<0) ?(-fmod(fabs(Angle),rwPiM2)) :(fmod(Angle,rwPiM2)))
// 0 <= (Angle) < 2*Pi
#define Range2Pi_Plus(Angle) ((Range2Pi(Angle)<0) ?(Range2Pi(Angle)+rwPiM2) :Range2Pi(Angle))
//  0  <= (Angle) <  +Pi  |  -Pi  <= (Angle) < 0    //    ������ ���������
//  arctan(y,x):             -Pi  <  (Angle) <=  +Pi
#define RangePi_Equal(Angle) ((Range2Pi_Plus(Angle)>=rwPi) ?(Range2Pi_Plus(Angle)-rwPiM2) :Range2Pi_Plus(Angle))
///____________________________________________________________________________.
///
extern  const  double TO_DEGREE;  //  = (180.0/RWpi);
extern  const  double TO_RADIAN;  //  = (RWpi/180.0);
extern  const  double FROM_SECOND;//  = (RWpi/648000.0);
extern  const  double FROM_RADIAN;//  = (648000.0/RWpi);

inline double RadToDeg(double radians) { return( radians * TO_DEGREE   ); };
inline double DegToRad(double degrees) { return( degrees * TO_RADIAN   ); };
inline double SecToRad(double seconds) { return( seconds * FROM_SECOND ); };
inline double RadToSec(double radians) { return( radians * FROM_RADIAN ); };

// inline    double Sqrt (double q) {return ((q<E_DOUBLE)?0:sqrt(q));};
inline       double Sqrt (     double q) {return ((q<=0)?0:sqrt (q));};
inline  long double SqrtL(long double q) {return ((q<=0)?0:sqrtl(q));};

// log   calculates the  natural logarithm of q.
inline       double Log  (     double q) {return ((q<=0)?0:log  (q));};
// log10 calculates the base ten logarithm of q.
inline       double Log10(     double q) {return ((q<=0)?0:log10(q));};

#define          E_DOUBLE  double(1.0e-24) // double(1.0e-15)
inline  double ATan2(double Dy,double Dx)  // return [-Pi .. +Pi ]
{
  if( (fabs(Dy)< E_DOUBLE)&&
      (fabs(Dx)< E_DOUBLE) ) return(0);
  if(  fabs(Dy)< E_DOUBLE  ) return((Dx>0)?(0)     :( rwPi  ));
  if(  fabs(Dx)< E_DOUBLE  ) return((Dy>0)?(rwPiD2):(-rwPiD2));
  return ::atan2(Dy,Dx);
};
#define           EL_DOUBLE  (1.0e-32L)
inline  long double ATan2L(long double Dy, long double Dx)  // return [-Pi .. +Pi ]
{
  if( (fabsl(Dy)< EL_DOUBLE)&&
      (fabsl(Dx)< EL_DOUBLE) ) return(0);
  if(  fabsl(Dy)< EL_DOUBLE  ) return((Dx>0)?(0)     :( rwPi  ));
  if(  fabsl(Dx)< EL_DOUBLE  ) return((Dy>0)?(rwPiD2):(-rwPiD2));
  return ::atan2l(Dy,Dx);
};

inline  double ATan2Pi(double Dy,double Dx)// return [0 .. 2.0*Pi]
{
  if( (fabs(Dy)< E_DOUBLE)&&
      (fabs(Dx)< E_DOUBLE) ) return        (0);
  if(  fabs(Dy)< E_DOUBLE  ) return((Dx>0)?(0)     :( rwPi       ));
  if(  fabs(Dx)< E_DOUBLE  ) return((Dy>0)?(rwPiD2):( rwPi+rwPiD2));
  double at= ::atan2(Dy, Dx);
  if   ( at < 0) at+= rwPiM2;
  return at;
};

double   Dist2D(xyREAL  p1,xyREAL  p2);
double   Dist3D(xyzREAL p1,xyzREAL p2);
// return 2 ������� .x & .y
xyREAL   rwSolveQ_2 (double   a,double   b,double   c, long* aOK=NULL);// a*x*x+b*x+c= 0 //
xyREAL     SolveQ_2L(LDouble aq,LDouble bq,LDouble cq, long* aOK=NULL);// a*x*x+b*x+c= 0 //


struct TValueEllipsoid
{//  int    VELL_i;      //   ������������� �������
	  char   VELL_Id [16];
	  double VELL_a;      //   EquatorialRadius=  a
	  double VELL_p;      //   �������� ������ = (a-b)/a = 1-sqrt(1-e^2)
	  char   ELL_Name[24];
};
//________________________________________________________________________________________.
//
extern   TValueEllipsoid VEllipsoid[];
extern   long            VEllipsoid_N;

#define  rwPi        RWpi
#define  SPHERE_ELL  1.0   /* double(0.99925) */
extern   double      ANGLE_TOPO;// == (Pi/2)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.
   double Calc_To (double  Ro);// (       sec), ������ ������ ������
   double Calc_Mo (double  Ro);// (m     /sec), Mo ������� �������� �������� ��������
   double Calc_Wo (double  Ro);// (������/sec), Wo ������� �������  �������� ��������
// ������� ��������=
   double Calc_Mt (double  Ro  // Tv � �������� �� ������� [Tv= Ti-T�]
                  ,double  Tv);// Ro= ������� ������� ������
   double Calc_We (          );// (������/sec), We ������� �������� �������� �����
///��������� �������� �������� �������� �� ������ � �� �������� �� �����
   double Calc_OE (double  Re
                  ,double  Ho);// { return (Re+Ho) / Re;  };
///��������� �������� �������� �������� �������� �� ����� � �������� �� ������
   double Calc_EO (double  Re
                  ,double  Ho);// { return Re / (Re+Ho);  };
// ��������������� ������(�) ���������� �� ������, ��������������� Xs, Ys, Zs
   double Calc_Rgc(double  Xc // ��� ������ �������� �������
                  ,double  Yc // , �������� �� ������� �� ����������
                  ,double  Zc);// {return sqrt(Xc*Xc+Yc*Yc+Zc*Zc);};
// ������������� ����������� �����������: �������...
   double Calcf_Io(double Io,double Bf, double *L_o,double *U_o,double *A_o);;// return(Lo)
   double Calcf_Ao(double Ao,double Bf, double *I_o,double *L_o,double *U_o);;// return(Lo)
   double Calcf_Uo(double Uo,double Bf, double *A_o,double *I_o,double *L_o);;// return(Lo)
//________________________________________________________________________________________.
//
long    NZona_GsK (double L, long Wz= 6);// L(Radian), lz={ 6 | 3 }
double  LZona_GsK (long  Nz, long Wz= 6);// return L(Radian) -- ������� ������� ���������

//      return:    Radian`s  ...  ����������� �������� ����
double  LZona_UTM (long   Nz_UTM);//             1..60
double  LZona_UTM (char * sz_UTM);// sz_UTM[4]= "1..60'C'"

long    NZona_UTM (double Lrad  );// L(Radian), Lz= 6: ��������� ��, �� �� �����.���������
long    NZona_UTM (double Lrad, double Brad);// ...........................C����.���������

long    IsUTMZ(double X_East);    // return  NZona_UTM[1..60] if UTMZ-Format, else <=0

//  Latitude is inside the UTM limits of ==84N to >=80S
char   UTMLetterDesignator(double Lat);//   +/-B - �������
xyREAL UTMLetter_Lat1Lat2 (char  Cutm);//  return  .x= Lat1(min), y=Lat2(max) � ��������
//                                     if  !(.x || .y) ��������� ������
    xyREAL   UTM_To_UTMZ(double XEast,    double YNorth,               char szZonaUTM_In [4]);
    xyzREAL  UTM_To_UTMZ(double XEast,    double YNorth,     double H, char szZonaUTM_In [4]);

//  TEllipsoid::
//  // if( !szZonaUTM_Out[0] ) ������ ...
//  //......TEllipsoid::............................................>> NNc: c>=N: North, c<=M: South
//  xyREAL  UTMZ_To_UTM (double XEastPlus,double YNorthMinus,          char szZonaUTM_Out[4]);
//  xyzREAL UTMZ_To_UTM (double XEastPlus,double YNorthMinus,double H, char szZonaUTM_Out[4]);
//
//______________________________________________________________________________+
//
// ��������� �������������:
// ���������������(XYZ)  <<==>>  ������������� ������(BLH) <<==>> UTM, Gauss-Krueger, ...
class TEllipsoid     // ������� � ����� ...
{ public:// iELL=[1..N] ������ ����������: [1],2,3, ...
         // iELL<=0:    Sphere(Rs = DeltaOrSphere), if(DeltaOrSphere<=0) DeltaOrSphere=1;
   void  Init(long   iELL,double DeltaOrRSphere);
   TEllipsoid( )                                 { Init(ELL_WGS84,       0.0);};
   TEllipsoid(long   iELL,double DeltaOrRSphere) { Init(iELL, DeltaOrRSphere);};
   TEllipsoid(double a, double b)                { Init_Ellipsoid(a, b);      };
  ~TEllipsoid( ) { };
   long   ELL_i;   // =1: WGS-84,                      =2: ��-90             =3: �...1942 �.
   double ELL_a;   //  a= 6378137      b= a*(1-p)      a= 6378136            a= 6378245
   double ELL_b;   //  b= 6356752.314  a-b=21384.7...  b= 6356751.362        b= 6356863.001877
   double ELL_c;   //  c= a^2/b = b/(1-e^2) = a/sqrt(1-e^2) = �������� ������ ��������
   double ELL_p;   //  p= 1.0/298.257223563            p= 1.0/298.257839303  p= 1.0/298.3
//        ELL_p    �������� ������ p= (a-b)/a = 1-b/a : [0 .. <1],  p==0 ��� �����
// double ELL_beta;^   == a/(a-b)= 1/p  [[ �� ��������� ��� ����� ]]
   double ELL_ba;  //  == b/a = (1-p)
//        ELL_ef   �������� �������������� = ���������� �� ������ ������� �� ������(1|2)
   double ELL_ef;  //  sqrt(a^2-b^2)       = sqrt(ELL_a*ELL_a-ELL_b*ELL_b);
   double ELL_pf;  //  (b^2/a)= a*(1-e^2) ��������� ��������
   double ELL_ra;  //  (a+ef)= a*(1+e) ������  ��������� (�� ������)
   double ELL_rp;  //  (a-ef)= a*(1-e) ������ ���������� (�� ������)
   double ELL_e1;  //  ef/a=(ELL_ef/ELL_a) = ( e )            ������  ��������������
   double ELL_e2;  //  ef/b=(ELL_ef/ELL_b) = ( e')            ������  ��������������
   double ELL_e1E2;// [ EM1 ] = 1-b^2/a^2  = ( e )^2  ������� ������� ���������������
   double ELL_e2E2;// [ EM2 ] = a^2/b^2-1  = ( e')^2  ������� ������� ���������������
// +
// double ELL_e0;  // [ EM0 ] 1/(1-p)= beta/(beta-1) = a/b
   double ELL_ab;  // [ EM0 ] 1/(1-p)= beta/(beta-1) = a/b
   double ELL_c1;  // [ EC1 ] b*e2E2       = (ELL_b*ELL_e2E2)  =  b * e'^2
   double ELL_c2;  // [ EC2 ] a*e1E2       = (ELL_a*ELL_e1E2)  =  a * e ^2
// �������������:
   double AELL;// =     ELL_a;       // +
   double BETA;// = 1.0/ELL_p;       // +
   double BELL;// =     ELL_b;       // +
   double EM0 ;// =     ELL_ab;      // +
   double EM1 ;// =     ELL_e1E2;    // +
   double EM2 ;// =     ELL_e2E2;    // +
   double EC0 ;// =     ELL_a*ELL_ab;// +
   double EC1 ;// =     ELL_c1;      // +
   double EC2 ;// =     ELL_c2;      // +
   double Ka0 ;
   double wk246,wk6,wk46;
   double wq246,wq6,wq46;
//^
   char  ZonaUTM[4];
// Rp- ������ ���������
   double Calc_Rp (double  Bc){return        ELL_a*cos( Calc_Bu(Bc) );   };

// Bu- �����������     ������[ �� ������ B ]
   double Calc_Bu (double  Bc){if(ELL_i<=0)  return(Bc);
                               return        atan( (ELL_b/ELL_a)*tan(Bc) );};
// B�- ������������� ������( Bu- �����������   ������ )
   double Calc_Bc (double  Bu){if(ELL_i<=0)  return(Bu);
                               return        atan( (ELL_a/ELL_b)*tan(Bu) );};
// Bf- ��������������� ������[ �� ������ B ]  tg(Bf)= (b/a)tg(Bu)= (1-e^2)*tg(B))
   double Calc_Bf (double  Bc){if(ELL_i<=0)  return(Bc);
                               return        atan( (1.0-ELL_e1E2)*tan(Bc));};
// Bgd- �������������   ������  tg(B)= tg(Bf)/(1-e^2);
// Bf - ��������������� ������[ �� ������ B ]  tg(Bf)= (b/a)tg(Bu)= (1-e^2)*tg(B))
   double Calc_Bgd(double  Bf){if(ELL_i<=0)  return(Bf);
                               return        atan( tan(Bf)/(1.0-ELL_e1E2));};
// nl- ��������������� ������� ��������������� [ �� ������ B ]
   double Calc_nl (double  Bc){if(ELL_i<=0)  return(0.0);
                               return        ELL_e2*cos(Bc); };  // e'*cos(B)
// W - �������� �������������� ������� [ �� ������ B ]
// W = sqrt(1-e^2)*V = (b/a)*V
   double Calc_W  (double  Bc){if(ELL_i<=0)  return(1.0);     // W= V*sqrt(1-e*e)
                               double sinB = sin(Bc);         // ELL_e1E2=   e*e
                               return sqrt(1.0-ELL_e1E2*sinB*sinB);   };
// V - �������� �������������� ������� [ �� ������ B ]
// V = sqrt(1+e'^2)*W = (a/b)*W
   double Calc_V  (double  Bc){if(ELL_i<=0)  return(1.0);
                               double cosB = cos(Bc);         // ELL_e2E2=  e'*e'
                               return sqrt(1.0 + ELL_e2E2*cosB*cosB); };
// ������ �������� ������� ��������� [ �� ������ B ] = c/V
   double Calc_Nc (double  Bc){if(ELL_i<=0)  return(ELL_a);
                               double cosB = cos(Bc);
                               double  ue  = (1.0 + ELL_e2E2*cosB*cosB);
                               return (ELL_c / sqrt (ue) );           };
// ������ �������� ������� ��������� [ �� ������ B ] = a/W
   double Calc_N  (double  Bc){if(ELL_i<=0)  return(ELL_a);
                               double sinB = sin(Bc);
                               double  ue  = (1.0 - ELL_e1E2*sinB*sinB);
                               return (ELL_a / sqrt (ue) );           };
// ������ �������� ���������         [ �� ������ B ]
   double Calc_M  (double  Bc){if(ELL_i<=0)  return(ELL_a);
                               double sinB = sin(Bc);
                               double  ue  = (1.0 - ELL_e1E2*sinB*sinB);
                               return (ELL_a*(1.0 - ELL_e1E2) / (ue*sqrt(ue)) );
                              };
// ������� ������ �������� ���������� �� ������ Bc = sqrt(M*N) = a*sqrt(1-e^2) / (1-e^2*sin(B)^2)
// ������� ������ ���. ����� � ������ Bc+-1.5� ��� +-(150-200)��
   double Calc_RMN(double  Bc){if(ELL_i<=0)  return(ELL_a);
                               double sinB = sin(Bc);
                               double  ue  = (1.0 - ELL_e1E2*sinB*sinB);
                               return (ELL_a * sqrt(1.0-ELL_e1E2)/(ue));
                              };
// ��������������� ������(�) ���������� �� ������ Bc
   double Calc_Rgc(double  Bc){if(ELL_i<=0)  return (  ELL_a  );
                               double ba  =  ( ELL_b / ELL_a  );
                               double Bu  =  atan( ba*tan(Bc) );
                               double r   =  ELL_a *  cos(Bu)  ;
                               double Bf  =  atan( ba*tan(Bu) );
                                      Bf  =  fabs(Bf);
                               if    (Bf >= (89.99*(RWpi/180.0))) return(ELL_b);
                               return r/cos(Bf);
                              };
// �����.������, �������� ����� ������� �� p1 � p2 |p1->p2| � ��������.�.�.(X,Y,Z)
// return(X,Y,Z) ��������� ����� �� p1 �� ����������� ����������, ������� �� ������ p1->p2
// Dgc= ���������� ��(p1) �� ����������� ����������
   xyzREAL VectorToEllipsoid(xyzREAL p1,xyzREAL p2, double * ADgc= NULL);

// ���������� ������ Io
// return(X,Y) X=ELL_a, Y=ELL_b`(��� ������� ���������� ���������� ������)
// BMax= ������������ ������, ����������� �������������� ������,
//       �� ������������ � ������ ����
   xyREAL Calc_Cut(double Io, double * ABMax= NULL);

// �����.����� � ������������ � ��������.�.�. Ph(X,Y,Z)
// return(X,Y,Z) ����� �� ����������� ����������, ������� �� ������ ��(Ph) � ������ �.�.
// Dgc= ���������� ��(Ph) �� ����������� ���������� �� ����������� � ������ ����
   xyzREAL Calc_Dgc(xyzREAL Ph, double * ADgc= NULL);
    xyREAL Calc_Dgc( xyREAL Ph, double * ADgc= NULL);// �� �� ��� �������

// �����.����� � ������������ � ��������.�.�. Ph(X,Y,Z) -> ��������.������ � XYZ
// return  (X,Y,Z) ����� ����������� ������� �� (Ph) � ����������� ����������
// Hgc= ���������� ��(Ph) �� ����������� ����������  �� ������� � �����������
   xyzREAL Calc_Hgd(xyzREAL Ph, double * AHgd= NULL);
    xyREAL Calc_Hgd( xyREAL Ph, double * AHgd= NULL);// �� �� ��� �������

// ������  � �������� ������� ���������, ��������� � �������(Ta= �������� ��������)
   double Calc_Rpf(double  Ta){if(ELL_i<=0)  return (  ELL_a  );
                               return ( ELL_pf /(1.0+ELL_e1*cos(Ta)));
                              };
//.�������� �������� � �������� ������� ���������, ��������� � �������(Rt= ������)
//.double Calc_Ta (double  Rt){if(ELL_i<=0)  return ( 0 );
//.                            return (acos((ELL_pf-Rt)/(ELL_e1*Rt)));
//.                           };
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
   double        AMeridian (          double  B,double  L, long Wz=6);// ��� 6|3^��������� ����
   double  LzOwn_AMeridian (double Lz,double  B,double  L);

   double  Calc_PofBL(double Bc, double L2minL1)// [In: Meter ] ������� ����� ���� ��������� �� Radian(B) � Radian(L2-L1)
   {
   // double r= Calc_N(Bc)*cos(Bc);
      return    Calc_N(Bc)*cos(Bc) * L2minL1;
   };
   double  Calc_LofBP(double Bc, double PLength)// [In: Radian] ������� (L2-L1) �� ����� ����(PLength) � ��������� Radian(B)
   {  return PLength/(Calc_N(Bc)*cos(Bc));
   }
   double  Calc_XB(double Bx);// [In: Radian] ������� ����� ���� ��������� �� �������� �� (Bx)  � ������
   double  Calc_BX(double Xb);// [In: Meter ] ������� ������, ����� ���� ��������� �� �������� � ��������

//                                             H - ���������������
   xyzREAL BLH_To_XYZ(double B,double L,double H);// (B,L,H) in radian, return( X,Y,Z )
//                                                        H - ���������������
   xyzREAL XYZ_To_BLH(double X,double Y,double Z);// return xyzREAL(B,L,H) in radian, m
//___________________________________________________________________________________________.
//___________________________________________________________________________________________.
//
   char  * ZonaUTM_BL(double B, double L);

// xEast = Nz*1000000+500000+x, ��� Nz=NZona_UTM
// yNorth>=0: North, yNorth<0: South ��� ����������� 10000000.0
   char  * ZonaUTMZ  (double xEast,double yNorth);
   //..............................................................>> NNc: c>=N: North, c<=M: South
   xyREAL  UTMZ_To_UTM (double XEastPlus,double YNorthMinus,          char szZonaUTM_Out[4]);
   xyzREAL UTMZ_To_UTM (double XEastPlus,double YNorthMinus,double H, char szZonaUTM_Out[4]);
//___________________________________________________________________________________________.
//
//                         ������������� ������(�����. ����������)->����������
   xyzREAL UTM_To_BLH      (double xEast,double yNorth,double H, char ZoneIn [4]);// return xyzREAL(B,L,H) in radian, m
//                                                      In parameter
   xyzREAL BLH_To_UTM_NzOwn(double B    ,double L     ,double H, char ZoneIn [4]);
// B,L: Radian`s  ...                                   return("1..60C..X")           Target 1..60 UTM
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^
   xyzREAL BLH_To_UTM(double B,double L,double H, char ZoneOut[4]=NULL);// (B,L,H) in radian, return(x, y, H)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^
// ��� � � ������ GsK:
//                                                 � ���� NzUTM    Target +1..60
   xyzREAL BLH_To_UTM (double B,double L,double H, char ZoneOut[4], long  Nz_UTM);// (B,L,H) in radian, return(x, y, H)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^
// ������ GsK:
// .����� ���� UTM ���������� � ���������� (xEast) � ����: +Nz*1000000+500000+x
// yNorth>=0: North, yNorth<0: South ��� ����������� 10000000.0          Target 1..60
   xyzREAL BLH_To_UTMZ(double B,double L,double H, char ZoneOut[4]=NULL, long  Nz_UTM= 0);// (B,L,H) in radian, return(x, y, H)

// .����� ���� UTM ���������� � ���������� (xEast) � ����: +Nz*1000000+500000+x
// yNorth>=0: North, yNorth<0: South ��� ����������� 10000000.0
   xyzREAL UTMZ_To_BLH(double xEast,double yNorth,double H, char ZoneOut[4]=NULL);// return xyzREAL(B,L,H) in radian, m

// .x= EastCoord �� ���������: Nz � + 500000.0  � �������
// .y= (+/-) ��� ���������� +10000000 ��� .y<0
   xyREAL BLH_To_UTMZ_NzOwn(long   NzUTM// ����� ���� [1..60] UTM, ��� ������� ��������
                           ,double B    // Radian`s: ������
                           ,double L);  // Radian`s: ������
   xyREAL NzOwn_UTMZ_To_BLH(long   NzUTM// ����� ���� [1..60] UTM, ��� ������� ��������
                           ,double xEast
                           ,double yNorth);// return xyREAL(B,L) in radian, m
//________________________________________________________________________________________.
//________________________________________________________________________________________.
//
// .x= EastCoord->(Y�)  ���������: Nz � + 500000.0  � �������  ->    B,L: ������
//                    East      North      ������������� ������(�����. ����������)->����������
   xyzREAL GsK_To_BLH(double Yg,double Xg, double H, long Wz=6);// return xyzREAL(B,L,H) in radian

// return( x: East , y: North, H: ������������� ������(�����. ����������)->����������)
   xyzREAL BLH_To_GsK(double  B,double  L,double  H, long Wz=6, long Nz= 0);// (B,L,H) in radian, return(.x=Yg,.y=Xg,H )
                      //                                       ^Target 1..60
// .x= EastCoord->(Y�) �� ���������: Nz � + 500000.0  � �������,     B,L: ������
//                          double Lz  Radian`s: ������ ��������
   xyREAL  LzOwn_GsK_To_BLH(double Lz, double dYg,double Xg);// return xyREAL(B,L) in radian
   xyREAL  BLH_To_GsK_LzOwn(double Lz, double   B,double  L);// (B,L) in radian, return(dY�,X�)
//___________________________________________________________________________________________.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.

// UTM                                                h - ������������
   xyzREAL UTM_To_XYZ  (double    x,double  y,double  h, char ZoneIn [4]=NULL)// return(X,Y,Z)
   {  xyzREAL blh= UTM_To_BLH(    x,        y,        h,      ZoneIn );
      return       BLH_To_XYZ(blh.x,    blh.y,    blh.z);
   };                                                                         //            h - ������������
   xyzREAL XYZ_To_UTM  (double    X,double  Y,double  Z, char ZoneOut[4]=NULL)// return(x,y,h)
   {  xyzREAL blh= XYZ_To_BLH(    X,        Y,        Z);
      return       BLH_To_UTM(blh.x,    blh.y,    blh.z,      ZoneOut);
   };

// .x= EastCoord->(Y�) ���������: Nz*1000000.0 +  500000.0, B,L: ������
// .y= �������� �������� (+/-) ��� ����������� +10000000.0  ���  .y < 0
//  � ������� ...
   xyzREAL UTMZ_To_XYZ  (double    x,double  y,double  h)
   {  xyzREAL blh= UTMZ_To_BLH(    x,        y,        h);
      return        BLH_To_XYZ(blh.x,    blh.y,    blh.z);
   };                                                                         //            h - ������������
   xyzREAL XYZ_To_UTMZ  (double    X,double  Y,double  Z, char ZoneOut[4]=NULL)// return(x,y,h)
   {  xyzREAL blh= XYZ_To_BLH (    X,        Y,        Z);
      return       BLH_To_UTMZ(blh.x,    blh.y,    blh.z,      ZoneOut);
   };

// Gauss-Krueger   //   East        North     ������������� ������(�����. ����������)
   xyzREAL GsK_To_XYZ  (double    x,double  y,double  h, long Wz=6)           // return(X,Y,Z)
   {  xyzREAL blh= GsK_To_BLH(    x,        y,        h,      Wz  );
      return       BLH_To_XYZ(blh.x,    blh.y,    blh.z);
   };                                                                         //            h - ������������
   xyzREAL XYZ_To_GsK  (double    X,double  Y,double  Z, long Wz=6)           // return(x,y,h)
   {  xyzREAL blh= XYZ_To_BLH(    X,        Y,        Z);
      return       BLH_To_GsK(blh.x,    blh.y,    blh.z,      Wz  );
   };
// private:
void Init_Ellipsoid(double a, double b, long iELL= 0);
};// class TEllipsoid {^ ...}

#endif

