#ifndef SAVITSKIGOLAY1DALG
#define SAVITSKIGOLAY1DALG

#include "BaseNoiseAlg.h"
#include <QMessageBox>
#include <QProgressDialog>
#include <QApplication>
#include <array>
#include "../Library/Types.h"
#include "../Library/Utils/Compare.h"


template <typename T>
class SavitskiGolay1DAlg : public BaseNoiseAlg<T>
{
public:
    SavitskiGolay1DAlg(HyperCube* cube, Attributes* attr)
        :BaseNoiseAlg<T>(cube, attr)
    {
        InitialiseCoeff();
    }
    virtual ~SavitskiGolay1DAlg()
    {

    }

public:
    virtual bool Execute()
    {

        if (BaseNoiseAlg<T>::m_attributes->GetApplyToAllCube())
        {
            return ToCube();
        } else
        {
            ToSpectrWindow();
            return false;
        }
    }

private:
    void InitialiseCoeff()
    {
        coeff_quadr_5_left_pixels_0_row = {0.8857, 0.2571, -0.0857, -0.1429, 0.0857};
        coeff_quadr_5_left_pixels_1_row = {0.2571, 0.3714, 0.3429, 0.1714, -0.1429};
        coeff_quadr_5_right_pixels_3_row = {-0.1429, 0.1714, 0.3429, 0.3714, 0.2571};
        coeff_quadr_5_right_pixels_4_row = {0.0857, -0.1429, -0.0857, 0.2571, 0.8857};

        coeff_cubic_5_left_pixels_0_row = {0.9857,    0.0571,   -0.0857,    0.0571,   -0.0143};
        coeff_cubic_5_left_pixels_1_row = {0.0571,    0.7714,    0.3429,   -0.2286,    0.0571};
        coeff_cubic_5_right_pixels_3_row = {0.0571,   -0.2286,    0.3429,    0.7714,    0.0571};
        coeff_cubic_5_right_pixels_4_row = {-0.0143,    0.0571,   -0.0857,    0.0571,    0.9857};

        coeff_quadr_7_left_pixels_0_row = {0.7619,    0.3571,    0.0714,   -0.0952,   -0.1429,   -0.0714,    0.1190};
        coeff_quadr_7_left_pixels_1_row = {0.3571,    0.2857,    0.2143,    0.1429,    0.0714,   -0.0000,   -0.0714};
        coeff_quadr_7_left_pixels_2_row = {0.0714,    0.2143,    0.2857,    0.2857,    0.2143,    0.0714,   -0.1429};
        coeff_quadr_7_right_pixels_4_row = {-0.1429,    0.0714,    0.2143,    0.2857,    0.2857,    0.2143,    0.0714};
        coeff_quadr_7_right_pixels_5_row = {-0.0714,   -0.0000,    0.0714,    0.1429,    0.2143,    0.2857,    0.3571};
        coeff_quadr_7_right_pixels_6_row = {0.1190,   -0.0714,   -0.1429,   -0.0952,    0.0714,    0.3571,    0.7619};

        coeff_cubic_7_left_pixels_0_row = {0.9286,    0.1905,   -0.0952,   -0.0952,    0.0238,    0.0952,   -0.0476};
        coeff_cubic_7_left_pixels_1_row = {0.1905,    0.4524,    0.3810,    0.1429,   -0.0952,   -0.1667,    0.0952};
        coeff_cubic_7_left_pixels_2_row = {-0.0952,    0.3810,    0.4524,    0.2857,    0.0476,   -0.0952,   0.0238};
        coeff_cubic_7_right_pixels_4_row = {0.0238,   -0.0952,    0.0476,    0.2857 ,   0.4524,   0.3810,   -0.0952};
        coeff_cubic_7_right_pixels_5_row = {0.0952,   -0.1667,   -0.0952,    0.1429 ,   0.3810 ,   0.4524,    0.1905};
        coeff_cubic_7_right_pixels_6_row = { -0.0476,    0.0952,    0.0238 ,  -0.0952 ,  -0.0952 ,   0.1905,    0.9286};


        coeff_quadr_9_left_pixels_0_row = {0.6606,    0.3818,    0.1636,    0.0061,   -0.0909,   -0.1273,   -0.1030,   -0.0182,    0.1273};
        coeff_quadr_9_left_pixels_1_row = {0.3818,    0.2788,    0.1909,    0.1182,    0.0606,    0.0182,   -0.0091,   -0.0212,   -0.0182};
        coeff_quadr_9_left_pixels_2_row = {0.1636,    0.1909,    0.2009,    0.1935,    0.1688,    0.1268,    0.0675,   -0.0091,   -0.1030};
        coeff_quadr_9_left_pixels_3_row = {0.0061,    0.1182,    0.1935,    0.2320,    0.2338,    0.1987,    0.1268,    0.0182,   -0.1273};
        coeff_quadr_9_right_pixels_5_row = {-0.1273,    0.0182,    0.1268,    0.1987,    0.2338,    0.2320,    0.1935,    0.1182,    0.0061};
        coeff_quadr_9_right_pixels_6_row = {-0.1030 ,  -0.0091,    0.0675,    0.1268,    0.1688,    0.1935,    0.2009,    0.1909,    0.1636};
        coeff_quadr_9_right_pixels_7_row = {-0.0182,   -0.0212,   -0.0091,    0.0182,    0.0606,    0.1182,    0.1909,    0.2788,    0.3818};
        coeff_quadr_9_right_pixels_8_row = {0.1273,   -0.0182,   -0.1030,   -0.1273,   -0.0909,    0.0061,    0.1636,    0.3818,    0.6606};

        coeff_cubic_9_left_pixels_0_row = {0.8586,    0.2828,   -0.0202,   -0.1212,   -0.0909,         0 ,   0.0808,    0.0808,   -0.0707};
        coeff_cubic_9_left_pixels_1_row = {0.2828,    0.3283,    0.2828,   0.1818 ,   0.0606,   -0.0455,   -0.1010,   -0.0707,    0.0808};
        coeff_cubic_9_left_pixels_2_row = {-0.0202,    0.2828,    0.3716,    0.3117 ,   0.1688,    0.0087,   -0.1032,   -0.1010,    0.0808};
        coeff_cubic_9_left_pixels_3_row = {-0.1212,    0.1818,    0.3117 ,   0.3139 ,   0.2338 ,   0.1169 ,   0.0087,   -0.0455,   -0.0000};
        coeff_cubic_9_right_pixels_5_row = {-0.0000,   -0.0455,    0.0087,    0.1169,    0.2338 ,   0.3139,    0.3117,    0.1818,   -0.1212};
        coeff_cubic_9_right_pixels_6_row = {0.0808,   -0.1010,   -0.1032,    0.0087,    0.1688,    0.3117,    0.3716,    0.2828,   -0.0202};
        coeff_cubic_9_right_pixels_7_row = {0.0808,   -0.0707,   -0.1010,   -0.0455,    0.0606 ,   0.1818,    0.2828,    0.3283,    0.2828};
        coeff_cubic_9_right_pixels_8_row = {-0.0707,    0.0808,    0.0808,         0 ,  -0.0909 ,  -0.1212,   -0.0202,    0.2828,    0.8586};
    // 4-5 степень
        coeff_quartic_7_left_pixels_0_row = {0.9870,    0.0541,   -0.0758,    0.0216,    0.0433,   -0.0411,    0.0108};
        coeff_quartic_7_left_pixels_1_row = {0.0541,    0.7706,    0.3355,   -0.1299,   -0.1407,    0.1515,   -0.0411};
        coeff_quartic_7_left_pixels_2_row = {-0.0758,    0.3355,    0.4589,    0.3247,    0.0541,   -0.1407,    0.0433};
        coeff_quartic_7_right_pixels_4_row = {0.0433,   -0.1407,    0.0541,    0.3247,    0.4589,    0.3355,   -0.0758};
        coeff_quartic_7_right_pixels_5_row = {-0.0411,    0.1515,   -0.1407,   -0.1299,    0.3355,    0.7706,    0.0541};
        coeff_quartic_7_right_pixels_6_row = {0.0108,   -0.0411,    0.0433,    0.0216,   -0.0758,    0.0541,    0.9870};

        coeff_quintic_7_left_pixels_0_row = {0.9989,    0.0065,   -0.0162,    0.0216,   -0.0162,    0.0065,   -0.0011};
        coeff_quintic_7_left_pixels_1_row = {0.0065,    0.9610,    0.0974,   -0.1299,    0.0974,   -0.0390,    0.0065};
        coeff_quintic_7_left_pixels_2_row = {-0.0162,    0.0974,    0.7565,    0.3247,   -0.2435,    0.0974,   -0.0162};
        coeff_quintic_7_right_pixels_4_row = {-0.0162,    0.0974,   -0.2435,    0.3247,    0.7565,    0.0974,   -0.0162};
        coeff_quintic_7_right_pixels_5_row = {0.0065,   -0.0390,   0.0974,   -0.1299,    0.0974,    0.9610,    0.0065};
        coeff_quintic_7_right_pixels_6_row = {-0.0011,    0.0065,   -0.0162,    0.0216,   -0.0162,    0.0065,    0.9989};


        coeff_quartic_9_left_pixels_0_row = {0.9565,    0.1360,   -0.0971,   -0.0583,    0.0350,    0.0629,    0.0039,   -0.0660,    0.0272};
        coeff_quartic_9_left_pixels_1_row = {0.1360,    0.5486,    0.3982,    0.0874,  -0.1282,   -0.1399,    0.0144,    0.1496,   -0.0660};
        coeff_quartic_9_left_pixels_2_row = {-0.0971,    0.3982,    0.4320,    0.2622,    0.0699,   -0.0408,   -0.0427,    0.0144,    0.0039};
        coeff_quartic_9_left_pixels_3_row = {-0.0583,    0.0874,    0.2622,    0.3543,    0.3147,    0.1573,   -0.0408,   -0.1399,    0.0629};
        coeff_quartic_9_right_pixels_5_row = {0.0629,   -0.1399,   -0.0408,    0.1573,    0.3147,    0.3543,    0.2622,    0.0874,   -0.0583};
        coeff_quartic_9_right_pixels_6_row = {0.0039,    0.0144,   -0.0427,   -0.0408,    0.0699,    0.2622,    0.4320,    0.3982,   -0.0971};
        coeff_quartic_9_right_pixels_7_row = {-0.0660,    0.1496,    0.0144,   -0.1399,   -0.1282,    0.0874,    0.3982,    0.5486,    0.1360};
        coeff_quartic_9_right_pixels_8_row = {0.0272,   -0.0660,    0.0039,    0.0629,    0.0350,   -0.0583,   -0.0971,    0.1360,    0.9565};

        coeff_quintic_9_left_pixels_0_row = {0.9907,    0.0420,   -0.0629,    0.0186,    0.0350,   -0.0140,   -0.0303,    0.0280,   -0.0070};
        coeff_quintic_9_left_pixels_1_row = {0.0420,    0.8071,    0.3042,   -0.1241,   -0.1282,    0.0717,    0.1084,  -0.1090,    0.0280};
        coeff_quintic_9_left_pixels_2_row = {-0.0629,    0.3042,    0.4662,    0.3392,    0.0699,   -0.1177,   -0.0769,    0.1084,   -0.0303};
        coeff_quintic_9_left_pixels_3_row = {0.0186,   -0.1241,    0.3392,    0.5274,    0.3147,   -0.0157,   -0.1177,    0.0717,   -0.0140};
        coeff_quintic_9_right_pixels_5_row = {-0.0140,    0.0717,   -0.1177,   -0.0157,    0.3147,    0.5274,    0.3392,   -0.1241,    0.0186};
        coeff_quintic_9_right_pixels_6_row = {-0.0303,    0.1084,   -0.0769,   -0.1177,    0.0699,   0.3392,    0.4662,    0.3042,   -0.0629};
        coeff_quintic_9_right_pixels_7_row = {0.0280,   -0.1090,    0.1084,    0.0717,   -0.1282,   -0.1241,    0.3042,    0.8071,    0.0420};
        coeff_quintic_9_right_pixels_8_row = {-0.0070,    0.0280,   -0.0303,   -0.0140,    0.0350,    0.0186,   -0.0629,    0.0420,    0.9907};

        // коэффициенты для полинома x^2 и x^3, окно - 5 пикселей. Последнее значение - значение нормировки
        coeff_quadr_cubic_5 = {-3, 12, 17, 12, -3, 35};
        // коэффициенты для полинома x^2 и x^3, окно - 7 пикселей. Последнее значение - значение нормировки
        coeff_quadr_cubic_7 = {-2, 3, 6, 7, 6, 3, -2, 21};
        // коэффициенты для полинома x^2 и x^3, окно - 9 пикселей. Последнее значение - значение нормировки
        coeff_quadr_cubic_9 = {-21, 14, 39, 54, 59, 54, 39, 14, -21, 231};
        // коэффициенты для полинома x^4 и x^5, окно - 7 пикселей. Последнее значение - значение нормировки
        coeff_quartic_quintic_7 = {5, -30, 75, 131, 75, -30, 5, 231};
        // коэффициенты для полинома x^4 и x^5, окно - 9 пикселей. Последнее значение - значение нормировки
        coeff_quartic_quintic_9 = {15, -55, 30, 135, 179, 135, 30, -55, 15, 429};
    }

private:
    std::array <double, 5> coeff_quadr_5_left_pixels_0_row;
    std::array <double, 5> coeff_quadr_5_left_pixels_1_row;// = {0.2571, 0.3714, 0.3429, 0.1714, -0.1429};
    std::array <double, 5> coeff_quadr_5_right_pixels_3_row;// = {-0.1429, 0.1714, 0.3429, 0.3714, 0.2571};
    std::array <double, 5> coeff_quadr_5_right_pixels_4_row;// = {0.0857, -0.1429, -0.0857, 0.2571, 0.8857};

    std::array <double, 5> coeff_cubic_5_left_pixels_0_row;// = {0.9857,    0.0571,   -0.0857,    0.0571,   -0.0143};
    std::array <double, 5> coeff_cubic_5_left_pixels_1_row;// = {0.0571,    0.7714,    0.3429,   -0.2286,    0.0571};
    std::array <double, 5> coeff_cubic_5_right_pixels_3_row;// = {0.0571,   -0.2286,    0.3429,    0.7714,    0.0571};
    std::array <double, 5> coeff_cubic_5_right_pixels_4_row;// = {-0.0143,    0.0571,   -0.0857,    0.0571,    0.9857};

    std::array <double, 7> coeff_quadr_7_left_pixels_0_row;// = {0.7619,    0.3571,    0.0714,   -0.0952,   -0.1429,   -0.0714,    0.1190};
    std::array <double, 7> coeff_quadr_7_left_pixels_1_row;// = {0.3571,    0.2857,    0.2143,    0.1429,    0.0714,   -0.0000,   -0.0714};
    std::array <double, 7> coeff_quadr_7_left_pixels_2_row;// = {0.0714,    0.2143,    0.2857,    0.2857,    0.2143,    0.0714,   -0.1429};
    std::array <double, 7> coeff_quadr_7_right_pixels_4_row;// = {-0.1429,    0.0714,    0.2143,    0.2857,    0.2857,    0.2143,    0.0714};
    std::array <double, 7> coeff_quadr_7_right_pixels_5_row;// = {-0.0714,   -0.0000,    0.0714,    0.1429,    0.2143,    0.2857,    0.3571};
    std::array <double, 7> coeff_quadr_7_right_pixels_6_row;// = {0.1190,   -0.0714,   -0.1429,   -0.0952,    0.0714,    0.3571,    0.7619};

    std::array <double, 7> coeff_cubic_7_left_pixels_0_row;// = {0.9286,    0.1905,   -0.0952,   -0.0952,    0.0238,    0.0952,   -0.0476};
    std::array <double, 7> coeff_cubic_7_left_pixels_1_row;// = {0.1905,    0.4524,    0.3810,    0.1429,   -0.0952,   -0.1667,    0.0952};
    std::array <double, 7> coeff_cubic_7_left_pixels_2_row;// = {-0.0952,    0.3810,    0.4524,    0.2857,    0.0476,   -0.0952,   0.0238};
    std::array <double, 7> coeff_cubic_7_right_pixels_4_row;// = {0.0238,   -0.0952,    0.0476,    0.2857 ,   0.4524,   0.3810,   -0.0952};
    std::array <double, 7> coeff_cubic_7_right_pixels_5_row;// = {0.0952,   -0.1667,   -0.0952,    0.1429 ,   0.3810 ,   0.4524,    0.1905};
    std::array <double, 7> coeff_cubic_7_right_pixels_6_row;// = { -0.0476,    0.0952,    0.0238 ,  -0.0952 ,  -0.0952 ,   0.1905,    0.9286};

    std::array <double, 9> coeff_quadr_9_left_pixels_0_row;// = {0.6606,    0.3818,    0.1636,    0.0061,   -0.0909,   -0.1273,   -0.1030,   -0.0182,    0.1273};
    std::array <double, 9> coeff_quadr_9_left_pixels_1_row;// = {0.3818,    0.2788,    0.1909,    0.1182,    0.0606,    0.0182,   -0.0091,   -0.0212,   -0.0182};
    std::array <double, 9> coeff_quadr_9_left_pixels_2_row;// = {0.1636,    0.1909,    0.2009,    0.1935,    0.1688,    0.1268,    0.0675,   -0.0091,   -0.1030};
    std::array <double, 9> coeff_quadr_9_left_pixels_3_row;// = {0.0061,    0.1182,    0.1935,    0.2320,    0.2338,    0.1987,    0.1268,    0.0182,   -0.1273};
    std::array <double, 9> coeff_quadr_9_right_pixels_5_row;// = {-0.1273,    0.0182,    0.1268,    0.1987,    0.2338,    0.2320,    0.1935,    0.1182,    0.0061};
    std::array <double, 9> coeff_quadr_9_right_pixels_6_row;// = {-0.1030 ,  -0.0091,    0.0675,    0.1268,    0.1688,    0.1935,    0.2009,    0.1909,    0.1636};
    std::array <double, 9> coeff_quadr_9_right_pixels_7_row;// = {-0.0182,   -0.0212,   -0.0091,    0.0182,    0.0606,    0.1182,    0.1909,    0.2788,    0.3818};
    std::array <double, 9> coeff_quadr_9_right_pixels_8_row;// = {0.1273,   -0.0182,   -0.1030,   -0.1273,   -0.0909,    0.0061,    0.1636,    0.3818,    0.6606};

    std::array <double, 9> coeff_cubic_9_left_pixels_0_row;// = {0.8586,    0.2828,   -0.0202,   -0.1212,   -0.0909,         0 ,   0.0808,    0.0808,   -0.0707};
    std::array <double, 9> coeff_cubic_9_left_pixels_1_row;// = {0.2828,    0.3283,    0.2828,   0.1818 ,   0.0606,   -0.0455,   -0.1010,   -0.0707,    0.0808};
    std::array <double, 9> coeff_cubic_9_left_pixels_2_row;// = {-0.0202,    0.2828,    0.3716,    0.3117 ,   0.1688,    0.0087,   -0.1032,   -0.1010,    0.0808};
    std::array <double, 9> coeff_cubic_9_left_pixels_3_row;// = {-0.1212,    0.1818,    0.3117 ,   0.3139 ,   0.2338 ,   0.1169 ,   0.0087,   -0.0455,   -0.0000};
    std::array <double, 9> coeff_cubic_9_right_pixels_5_row;// = {-0.0000,   -0.0455,    0.0087,    0.1169,    0.2338 ,   0.3139,    0.3117,    0.1818,   -0.1212};
    std::array <double, 9> coeff_cubic_9_right_pixels_6_row;// = {0.0808,   -0.1010,   -0.1032,    0.0087,    0.1688,    0.3117,    0.3716,    0.2828,   -0.0202};
    std::array <double, 9> coeff_cubic_9_right_pixels_7_row;// = {0.0808,   -0.0707,   -0.1010,   -0.0455,    0.0606 ,   0.1818,    0.2828,    0.3283,    0.2828};
    std::array <double, 9> coeff_cubic_9_right_pixels_8_row;// = {-0.0707,    0.0808,    0.0808,         0 ,  -0.0909 ,  -0.1212,   -0.0202,    0.2828,    0.8586};
// 4-5 степень
    std::array <double, 7> coeff_quartic_7_left_pixels_0_row;// = {0.9870,    0.0541,   -0.0758,    0.0216,    0.0433,   -0.0411,    0.0108};
    std::array <double, 7> coeff_quartic_7_left_pixels_1_row;// = {0.0541,    0.7706,    0.3355,   -0.1299,   -0.1407,    0.1515,   -0.0411};
    std::array <double, 7> coeff_quartic_7_left_pixels_2_row;// = {-0.0758,    0.3355,    0.4589,    0.3247,    0.0541,   -0.1407,    0.0433};
    std::array <double, 7> coeff_quartic_7_right_pixels_4_row;// = {0.0433,   -0.1407,    0.0541,    0.3247,    0.4589,    0.3355,   -0.0758};
    std::array <double, 7> coeff_quartic_7_right_pixels_5_row;// = {-0.0411,    0.1515,   -0.1407,   -0.1299,    0.3355,    0.7706,    0.0541};
    std::array <double, 7> coeff_quartic_7_right_pixels_6_row;// = {0.0108,   -0.0411,    0.0433,    0.0216,   -0.0758,    0.0541,    0.9870};

    std::array <double, 7> coeff_quintic_7_left_pixels_0_row;// = {0.9989,    0.0065,   -0.0162,    0.0216,   -0.0162,    0.0065,   -0.0011};
    std::array <double, 7> coeff_quintic_7_left_pixels_1_row;// = {0.0065,    0.9610,    0.0974,   -0.1299,    0.0974,   -0.0390,    0.0065};
    std::array <double, 7> coeff_quintic_7_left_pixels_2_row;// = {-0.0162,    0.0974,    0.7565,    0.3247,   -0.2435,    0.0974,   -0.0162};
    std::array <double, 7> coeff_quintic_7_right_pixels_4_row;// = {-0.0162,    0.0974,   -0.2435,    0.3247,    0.7565,    0.0974,   -0.0162};
    std::array <double, 7> coeff_quintic_7_right_pixels_5_row;// = {0.0065,   -0.0390,   0.0974,   -0.1299,    0.0974,    0.9610,    0.0065};
    std::array <double, 7> coeff_quintic_7_right_pixels_6_row;// = {-0.0011,    0.0065,   -0.0162,    0.0216,   -0.0162,    0.0065,    0.9989};


    std::array <double, 9> coeff_quartic_9_left_pixels_0_row;// = {0.9565,    0.1360,   -0.0971,   -0.0583,    0.0350,    0.0629,    0.0039,   -0.0660,    0.0272};
    std::array <double, 9> coeff_quartic_9_left_pixels_1_row;// = {0.1360,    0.5486,    0.3982,    0.0874,  -0.1282,   -0.1399,    0.0144,    0.1496,   -0.0660};
    std::array <double, 9> coeff_quartic_9_left_pixels_2_row;// = {-0.0971,    0.3982,    0.4320,    0.2622,    0.0699,   -0.0408,   -0.0427,    0.0144,    0.0039};
    std::array <double, 9> coeff_quartic_9_left_pixels_3_row;// = {-0.0583,    0.0874,    0.2622,    0.3543,    0.3147,    0.1573,   -0.0408,   -0.1399,    0.0629};
    std::array <double, 9> coeff_quartic_9_right_pixels_5_row;// = {0.0629,   -0.1399,   -0.0408,    0.1573,    0.3147,    0.3543,    0.2622,    0.0874,   -0.0583};
    std::array <double, 9> coeff_quartic_9_right_pixels_6_row;// = {0.0039,    0.0144,   -0.0427,   -0.0408,    0.0699,    0.2622,    0.4320,    0.3982,   -0.0971};
    std::array <double, 9> coeff_quartic_9_right_pixels_7_row;// = {-0.0660,    0.1496,    0.0144,   -0.1399,   -0.1282,    0.0874,    0.3982,    0.5486,    0.1360};
    std::array <double, 9> coeff_quartic_9_right_pixels_8_row;// = {0.0272,   -0.0660,    0.0039,    0.0629,    0.0350,   -0.0583,   -0.0971,    0.1360,    0.9565};

    std::array <double, 9> coeff_quintic_9_left_pixels_0_row;// = {0.9907,    0.0420,   -0.0629,    0.0186,    0.0350,   -0.0140,   -0.0303,    0.0280,   -0.0070};
    std::array <double, 9> coeff_quintic_9_left_pixels_1_row;// = {0.0420,    0.8071,    0.3042,   -0.1241,   -0.1282,    0.0717,    0.1084,  -0.1090,    0.0280};
    std::array <double, 9> coeff_quintic_9_left_pixels_2_row;// = {-0.0629,    0.3042,    0.4662,    0.3392,    0.0699,   -0.1177,   -0.0769,    0.1084,   -0.0303};
    std::array <double, 9> coeff_quintic_9_left_pixels_3_row;// = {0.0186,   -0.1241,    0.3392,    0.5274,    0.3147,   -0.0157,   -0.1177,    0.0717,   -0.0140};
    std::array <double, 9> coeff_quintic_9_right_pixels_5_row;// = {-0.0140,    0.0717,   -0.1177,   -0.0157,    0.3147,    0.5274,    0.3392,   -0.1241,    0.0186};
    std::array <double, 9> coeff_quintic_9_right_pixels_6_row;// = {-0.0303,    0.1084,   -0.0769,   -0.1177,    0.0699,   0.3392,    0.4662,    0.3042,   -0.0629};
    std::array <double, 9> coeff_quintic_9_right_pixels_7_row;// = {0.0280,   -0.1090,    0.1084,    0.0717,   -0.1282,   -0.1241,    0.3042,    0.8071,    0.0420};
    std::array <double, 9> coeff_quintic_9_right_pixels_8_row;// = {-0.0070,    0.0280,   -0.0303,   -0.0140,    0.0350,    0.0186,   -0.0629,    0.0420,    0.9907};

    // коэффициенты для полинома x^2 и x^3, окно - 5 пикселей. Последнее значение - значение нормировки
    std::array <u::int32, 6> coeff_quadr_cubic_5;// = {-3, 12, 17, 12, -3, 35};
    // коэффициенты для полинома x^2 и x^3, окно - 7 пикселей. Последнее значение - значение нормировки
    std::array <u::int32, 8> coeff_quadr_cubic_7;// = {-2, 3, 6, 7, 6, 3, -2, 21};
    // коэффициенты для полинома x^2 и x^3, окно - 9 пикселей. Последнее значение - значение нормировки
    std::array <u::int32, 10> coeff_quadr_cubic_9;// = {-21, 14, 39, 54, 59, 54, 39, 14, -21, 231};
    // коэффициенты для полинома x^4 и x^5, окно - 7 пикселей. Последнее значение - значение нормировки
    std::array <u::int32, 8> coeff_quartic_quintic_7;// = {5, -30, 75, 131, 75, -30, 5, 231};
    // коэффициенты для полинома x^4 и x^5, окно - 9 пикселей. Последнее значение - значение нормировки
    std::array <u::int32, 10> coeff_quartic_quintic_9;// = {15, -55, 30, 135, 179, 135, 30, -55, 15, 429};

private:

    void ChooseCoef(u::int32* coeff, u::uint32& normalization)
    {
        u::int8 maskPixels = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();
        switch (maskPixels)
        {
            case 5:
            {
             memcpy(coeff, coeff_quadr_cubic_5.data(),  maskPixels*sizeof(u::int32));
             normalization = coeff_quadr_cubic_5[maskPixels]; break;
            }

            case 7:
            {
                switch(BaseNoiseAlg<T>::m_attributes->GetDegreePolinom())
                {
                    case 2:
                    case 3: { memcpy(coeff, coeff_quadr_cubic_7.data(),  maskPixels*sizeof(u::int32)); normalization = coeff_quadr_cubic_7[maskPixels]; break; }
                    case 4:
                    case 5:{ memcpy(coeff, coeff_quartic_quintic_7.data(),  maskPixels*sizeof(u::int32)); normalization = coeff_quartic_quintic_7[maskPixels]; break; }
                    default: break;
                }
                break;
            }
            case 9:
            {
                switch(BaseNoiseAlg<T>::m_attributes->GetDegreePolinom())
                {
                    case 2:
                    case 3: { memcpy(coeff, coeff_quadr_cubic_9.data(),  maskPixels*sizeof(u::int32)); normalization = coeff_quadr_cubic_9[maskPixels]; break; }
                    case 4:
                    case 5:{ memcpy(coeff, coeff_quartic_quintic_9.data(),  maskPixels*sizeof(u::int32)); normalization = coeff_quartic_quintic_9[maskPixels]; break; }
                    default:  break;
                }
                break;
            }
        }
    }

    template <typename M>
    void ApplyFilterToSpectr(M* spectr, u::uint32 sizeSpectr, u::int32* coeff, u::uint32 normalization)
    {
        u::int8 maskPixels = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();

        for (u::uint32 i = maskPixels/2; i < sizeSpectr - maskPixels/2; i++)
        {
            M temp = 0;
            for (u::uint8 j = 0; j < maskPixels; j++)
            {
                temp+= spectr[i + j-maskPixels/2]*coeff[j];
            }
            spectr[i] = temp/normalization;
        }

    }

    template <typename M>
    void CalculateEdges(M* spectr, u::uint32 sizeSpectr, bool bSideRight)
    {
        u::int8 maskPixels = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();
        u::int8 degreePolinom = BaseNoiseAlg<T>::m_attributes->GetDegreePolinom();
        switch(maskPixels)
        {
            case 5:
            {
                switch(degreePolinom)
                {
                    case 2:
                    {
                        double tempRow0 = 0;
                        double tempRow1 = 0;
                        if (!bSideRight)
                        {
                            for (int i = 0; i < 5; i++)
                            {
                                tempRow0 += spectr[i]*coeff_quadr_5_left_pixels_0_row[i];
                                tempRow1 += spectr[i]*coeff_quadr_5_left_pixels_1_row[i];
                            }
                            spectr[0] = tempRow0;
                            spectr[1] = tempRow1;

                        } else
                        {
                            for (int i = 0; i < 5; i++)
                            {
                                tempRow0 += spectr[sizeSpectr - maskPixels + i]*coeff_quadr_5_right_pixels_3_row[i];
                                tempRow1 += spectr[sizeSpectr - maskPixels + i]*coeff_quadr_5_right_pixels_4_row[i];
                            }
                            spectr[sizeSpectr - 2] = tempRow0;
                            spectr[sizeSpectr - 1] = tempRow1;
                        }
                        return;
                    }
                    case 3:
                    {
                        double tempRow0 = 0;
                        double tempRow1 = 0;
                        if (!bSideRight)
                        {
                            for (int i = 0; i < 5; i++)
                            {
                                tempRow0 += spectr[i]*coeff_cubic_5_left_pixels_0_row[i];
                                tempRow1 += spectr[i]*coeff_cubic_5_left_pixels_1_row[i];
                            }
                            spectr[0] = tempRow0;
                            spectr[1] = tempRow1;

                        } else
                        {
                            for (int i = 0; i < 5; i++)
                            {
                                tempRow0 += spectr[sizeSpectr - maskPixels + i]*coeff_cubic_5_right_pixels_3_row[i];
                                tempRow1 += spectr[sizeSpectr - maskPixels + i]*coeff_cubic_5_right_pixels_4_row[i];
                            }
                            spectr[sizeSpectr - 2] = tempRow0;
                            spectr[sizeSpectr - 1] = tempRow1;
                        }
                            return;
                      }
                      default: return;
                }
            }
            case 7:
            {
                switch(degreePolinom)
                {
                    case 2:
                    {
                        double tempRow0 = 0;
                        double tempRow1 = 0;
                        double tempRow2 = 0;
                        if (!bSideRight)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                tempRow0 += spectr[i]*coeff_quadr_7_left_pixels_0_row[i];
                                tempRow1 += spectr[i]*coeff_quadr_7_left_pixels_1_row[i];
                                tempRow1 += spectr[i]*coeff_quadr_7_left_pixels_2_row[i];
                            }
                            spectr[0] = tempRow0;
                            spectr[1] = tempRow1;
                            spectr[2] = tempRow2;

                        } else
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                tempRow0 += spectr[sizeSpectr - maskPixels + i]*coeff_cubic_7_right_pixels_4_row[i];
                                tempRow1 += spectr[sizeSpectr - maskPixels + i]*coeff_cubic_7_right_pixels_5_row[i];
                                tempRow2 += spectr[sizeSpectr - maskPixels + i]*coeff_cubic_7_right_pixels_6_row[i];
                            }
                            spectr[sizeSpectr - 3] = tempRow0;
                            spectr[sizeSpectr - 2] = tempRow1;
                            spectr[sizeSpectr - 1] = tempRow2;
                        }
                        return;
                    }
                    case 3:
                    {
                        double tempRow0 = 0;
                        double tempRow1 = 0;
                        double tempRow2 = 0;
                        if (!bSideRight)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                tempRow0 += spectr[i]*coeff_cubic_7_left_pixels_0_row[i];
                                tempRow1 += spectr[i]*coeff_cubic_7_left_pixels_1_row[i];
                                tempRow1 += spectr[i]*coeff_cubic_7_left_pixels_2_row[i];
                            }
                            spectr[0] = tempRow0;
                            spectr[1] = tempRow1;
                            spectr[2] = tempRow2;

                        } else
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                tempRow0 += spectr[sizeSpectr - maskPixels + i]*coeff_cubic_7_right_pixels_4_row[i];
                                tempRow1 += spectr[sizeSpectr - maskPixels + i]*coeff_cubic_7_right_pixels_5_row[i];
                                tempRow2 += spectr[sizeSpectr - maskPixels + i]*coeff_cubic_7_right_pixels_6_row[i];
                            }
                            spectr[sizeSpectr - 3] = tempRow0;
                            spectr[sizeSpectr - 2] = tempRow1;
                            spectr[sizeSpectr - 1] = tempRow2;
                        }
                        return;
                    }
                    case 4:
                    {
                        double tempRow0 = 0;
                        double tempRow1 = 0;
                        double tempRow2 = 0;
                        if (!bSideRight)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                tempRow0 += spectr[i]*coeff_quartic_7_left_pixels_0_row[i];
                                tempRow1 += spectr[i]*coeff_quartic_7_left_pixels_1_row[i];
                                tempRow1 += spectr[i]*coeff_quartic_7_left_pixels_2_row[i];
                            }
                            spectr[0] = tempRow0;
                            spectr[1] = tempRow1;
                            spectr[2] = tempRow2;

                        } else
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                tempRow0 += spectr[sizeSpectr - maskPixels + i]*coeff_quartic_7_right_pixels_4_row[i];
                                tempRow1 += spectr[sizeSpectr - maskPixels + i]*coeff_quartic_7_right_pixels_5_row[i];
                                tempRow2 += spectr[sizeSpectr - maskPixels + i]*coeff_quartic_7_right_pixels_6_row[i];
                            }
                            spectr[sizeSpectr - 3] = tempRow0;
                            spectr[sizeSpectr - 2] = tempRow1;
                            spectr[sizeSpectr - 1] = tempRow2;
                        }
                        return;
                    }
                    case 5:
                    {
                        double tempRow0 = 0;
                        double tempRow1 = 0;
                        double tempRow2 = 0;
                        if (!bSideRight)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                tempRow0 += spectr[i]*coeff_quintic_7_left_pixels_0_row[i];
                                tempRow1 += spectr[i]*coeff_quintic_7_left_pixels_1_row[i];
                                tempRow1 += spectr[i]*coeff_quintic_7_left_pixels_2_row[i];
                            }
                            spectr[0] = tempRow0;
                            spectr[1] = tempRow1;
                            spectr[2] = tempRow2;

                        } else
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                tempRow0 += spectr[sizeSpectr - maskPixels + i]*coeff_quintic_7_right_pixels_4_row[i];
                                tempRow1 += spectr[sizeSpectr - maskPixels + i]*coeff_quintic_7_right_pixels_5_row[i];
                                tempRow2 += spectr[sizeSpectr - maskPixels + i]*coeff_quintic_7_right_pixels_6_row[i];
                            }
                            spectr[sizeSpectr - 3] = tempRow0;
                            spectr[sizeSpectr - 2] = tempRow1;
                            spectr[sizeSpectr - 1] = tempRow2;
                        }
                        return;
                    }
                    default: return;
                    }
                return;
            }
            case 9:
            {
                switch(degreePolinom)
                {
                case 2:
                {
                    double tempRow0 = 0;
                    double tempRow1 = 0;
                    double tempRow2 = 0;
                    double tempRow3 = 0;
                    if (!bSideRight)
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            tempRow0 += spectr[i]*coeff_quadr_9_left_pixels_0_row[i];
                            tempRow1 += spectr[i]*coeff_quadr_9_left_pixels_1_row[i];
                            tempRow1 += spectr[i]*coeff_quadr_9_left_pixels_2_row[i];
                            tempRow2 += spectr[i]*coeff_quadr_9_left_pixels_3_row[i];
                        }
                        spectr[0] = tempRow0;
                        spectr[1] = tempRow1;
                        spectr[2] = tempRow2;
                        spectr[3] = tempRow3;

                    } else
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            tempRow0 += spectr[sizeSpectr - maskPixels + i]*coeff_cubic_9_right_pixels_5_row[i];
                            tempRow1 += spectr[sizeSpectr - maskPixels + i]*coeff_cubic_9_right_pixels_6_row[i];
                            tempRow2 += spectr[sizeSpectr - maskPixels + i]*coeff_cubic_9_right_pixels_7_row[i];
                            tempRow3 += spectr[sizeSpectr - maskPixels + i]*coeff_cubic_9_right_pixels_8_row[i];
                        }
                        spectr[sizeSpectr - 4] = tempRow0;
                        spectr[sizeSpectr - 3] = tempRow1;
                        spectr[sizeSpectr - 2] = tempRow2;
                        spectr[sizeSpectr - 1] = tempRow3;
                    }
                    return;
                }
                case 3:
                {
                    double tempRow0 = 0;
                    double tempRow1 = 0;
                    double tempRow2 = 0;
                    double tempRow3 = 0;
                    if (!bSideRight)
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            tempRow0 += spectr[i]*coeff_cubic_9_left_pixels_0_row[i];
                            tempRow1 += spectr[i]*coeff_cubic_9_left_pixels_1_row[i];
                            tempRow1 += spectr[i]*coeff_cubic_9_left_pixels_2_row[i];
                            tempRow2 += spectr[i]*coeff_cubic_9_left_pixels_3_row[i];
                        }
                        spectr[0] = tempRow0;
                        spectr[1] = tempRow1;
                        spectr[2] = tempRow2;
                        spectr[3] = tempRow3;

                    } else
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            tempRow0 += spectr[sizeSpectr - maskPixels + i]*coeff_cubic_9_right_pixels_5_row[i];
                            tempRow1 += spectr[sizeSpectr - maskPixels + i]*coeff_cubic_9_right_pixels_6_row[i];
                            tempRow2 += spectr[sizeSpectr - maskPixels + i]*coeff_cubic_9_right_pixels_7_row[i];
                            tempRow2 += spectr[sizeSpectr - maskPixels + i]*coeff_cubic_9_right_pixels_8_row[i];
                        }
                        spectr[sizeSpectr - 4] = tempRow0;
                        spectr[sizeSpectr - 3] = tempRow1;
                        spectr[sizeSpectr - 2] = tempRow2;
                        spectr[sizeSpectr - 1] = tempRow3;
                    }
                    return;
                }
                case 4:
                {
                    double tempRow0 = 0;
                    double tempRow1 = 0;
                    double tempRow2 = 0;
                    double tempRow3 = 0;
                    if (!bSideRight)
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            tempRow0 += spectr[i]*coeff_quartic_9_left_pixels_0_row[i];
                            tempRow1 += spectr[i]*coeff_quartic_9_left_pixels_1_row[i];
                            tempRow1 += spectr[i]*coeff_quartic_9_left_pixels_2_row[i];
                            tempRow2 += spectr[i]*coeff_quartic_9_left_pixels_3_row[i];
                        }
                        spectr[0] = tempRow0;
                        spectr[1] = tempRow1;
                        spectr[2] = tempRow2;
                        spectr[3] = tempRow3;

                    } else
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            tempRow0 += spectr[sizeSpectr - maskPixels + i]*coeff_quartic_9_right_pixels_5_row[i];
                            tempRow1 += spectr[sizeSpectr - maskPixels + i]*coeff_quartic_9_right_pixels_6_row[i];
                            tempRow2 += spectr[sizeSpectr - maskPixels + i]*coeff_quartic_9_right_pixels_7_row[i];
                            tempRow2 += spectr[sizeSpectr - maskPixels + i]*coeff_quartic_9_right_pixels_8_row[i];
                        }
                        spectr[sizeSpectr - 4] = tempRow0;
                        spectr[sizeSpectr - 3] = tempRow1;
                        spectr[sizeSpectr - 2] = tempRow2;
                        spectr[sizeSpectr - 1] = tempRow3;
                    }
                    return;
                }
                case 5:
                {
                    double tempRow0 = 0;
                    double tempRow1 = 0;
                    double tempRow2 = 0;
                    double tempRow3 = 0;
                    if (!bSideRight)
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            tempRow0 += spectr[i]*coeff_quintic_9_left_pixels_0_row[i];
                            tempRow1 += spectr[i]*coeff_quintic_9_left_pixels_1_row[i];
                            tempRow1 += spectr[i]*coeff_quintic_9_left_pixels_2_row[i];
                            tempRow2 += spectr[i]*coeff_quintic_9_left_pixels_3_row[i];
                        }
                        spectr[0] = tempRow0;
                        spectr[1] = tempRow1;
                        spectr[2] = tempRow2;
                        spectr[3] = tempRow3;

                    } else
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            tempRow0 += spectr[sizeSpectr - maskPixels + i]*coeff_quintic_9_right_pixels_5_row[i];
                            tempRow1 += spectr[sizeSpectr - maskPixels + i]*coeff_quintic_9_right_pixels_6_row[i];
                            tempRow2 += spectr[sizeSpectr - maskPixels + i]*coeff_quintic_9_right_pixels_7_row[i];
                            tempRow2 += spectr[sizeSpectr - maskPixels + i]*coeff_quintic_9_right_pixels_8_row[i];
                        }
                        spectr[sizeSpectr - 4] = tempRow0;
                        spectr[sizeSpectr - 3] = tempRow1;
                        spectr[sizeSpectr - 2] = tempRow2;
                        spectr[sizeSpectr - 1] = tempRow3;
                    }
                    return;
                }
                default: return;
                }
                return;
            }
        }
    }



    template <typename M>
    void CalculateEdgesCube(M** dataCube, u::uint32 sizeSpectr, u::uint32 row, u::uint32 col, bool bSideRight)
    {
        u::int8 maskPixels = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();
        u::int8 degreePolinom = BaseNoiseAlg<T>::m_attributes->GetDegreePolinom();
        switch(maskPixels)
        {
            case 5:
            {
                switch(degreePolinom)
                {
                    case 2:
                    {
                        double tempRow0 = 0;
                        double tempRow1 = 0;
                        if (!bSideRight)
                        {
                            for (int i = 0; i < 5; i++)
                            {
                                tempRow0 += dataCube[i][row*sizeSpectr+col]*coeff_quadr_5_left_pixels_0_row[i];
                                tempRow1 += dataCube[i][row*sizeSpectr+col]*coeff_quadr_5_left_pixels_1_row[i];
                            }
                            dataCube[0][row*sizeSpectr+col] = tempRow0;
                            dataCube[1][row*sizeSpectr+col] = tempRow1;

                        } else
                        {
                            for (int i = 0; i < 5; i++)
                            {
                                tempRow0 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_quadr_5_right_pixels_3_row[i];
                                tempRow1 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_quadr_5_right_pixels_4_row[i];
                            }
                            dataCube[sizeSpectr - 2][row*sizeSpectr+col] = tempRow0;
                            dataCube[sizeSpectr - 1][row*sizeSpectr+col] = tempRow1;
                        }
                        return;
                    }
                    case 3:
                    {
                        double tempRow0 = 0;
                        double tempRow1 = 0;
                        if (!bSideRight)
                        {
                            for (int i = 0; i < 5; i++)
                            {
                                tempRow0 += dataCube[i][row*sizeSpectr+col]*coeff_cubic_5_left_pixels_0_row[i];
                                tempRow1 += dataCube[i][row*sizeSpectr+col]*coeff_cubic_5_left_pixels_1_row[i];
                            }
                            dataCube[0][row*sizeSpectr+col] = tempRow0;
                            dataCube[1][row*sizeSpectr+col] = tempRow1;

                        } else
                        {
                            for (int i = 0; i < 5; i++)
                            {
                                tempRow0 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_cubic_5_right_pixels_3_row[i];
                                tempRow1 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_cubic_5_right_pixels_4_row[i];
                            }
                            dataCube[sizeSpectr - 2][row*sizeSpectr+col] = tempRow0;
                            dataCube[sizeSpectr - 1][row*sizeSpectr+col] = tempRow1;
                        }
                            return;
                      }
                      default: return;
                }
            }
            case 7:
            {
                switch(degreePolinom)
                {
                    case 2:
                    {
                        double tempRow0 = 0;
                        double tempRow1 = 0;
                        double tempRow2 = 0;
                        if (!bSideRight)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                tempRow0 += dataCube[i][row*sizeSpectr+col]*coeff_quadr_7_left_pixels_0_row[i];
                                tempRow1 += dataCube[i][row*sizeSpectr+col]*coeff_quadr_7_left_pixels_1_row[i];
                                tempRow1 += dataCube[i][row*sizeSpectr+col]*coeff_quadr_7_left_pixels_2_row[i];
                            }
                            dataCube[0][row*sizeSpectr+col] = tempRow0;
                            dataCube[1][row*sizeSpectr+col] = tempRow1;
                            dataCube[2][row*sizeSpectr+col] = tempRow2;

                        } else
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                tempRow0 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_cubic_7_right_pixels_4_row[i];
                                tempRow1 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_cubic_7_right_pixels_5_row[i];
                                tempRow2 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_cubic_7_right_pixels_6_row[i];
                            }
                            dataCube[sizeSpectr - 3][row*sizeSpectr+col] = tempRow0;
                            dataCube[sizeSpectr - 2][row*sizeSpectr+col] = tempRow1;
                            dataCube[sizeSpectr - 1][row*sizeSpectr+col] = tempRow2;
                        }
                        return;
                    }
                    case 3:
                    {
                        double tempRow0 = 0;
                        double tempRow1 = 0;
                        double tempRow2 = 0;
                        if (!bSideRight)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                tempRow0 += dataCube[i][row*sizeSpectr+col]*coeff_cubic_7_left_pixels_0_row[i];
                                tempRow1 += dataCube[i][row*sizeSpectr+col]*coeff_cubic_7_left_pixels_1_row[i];
                                tempRow1 += dataCube[i][row*sizeSpectr+col]*coeff_cubic_7_left_pixels_2_row[i];
                            }
                            dataCube[0][row*sizeSpectr+col] = tempRow0;
                            dataCube[1][row*sizeSpectr+col] = tempRow1;
                            dataCube[2][row*sizeSpectr+col] = tempRow2;

                        } else
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                tempRow0 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_cubic_7_right_pixels_4_row[i];
                                tempRow1 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_cubic_7_right_pixels_5_row[i];
                                tempRow2 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_cubic_7_right_pixels_6_row[i];
                            }
                            dataCube[sizeSpectr - 3][row*sizeSpectr+col] = tempRow0;
                            dataCube[sizeSpectr - 2][row*sizeSpectr+col] = tempRow1;
                            dataCube[sizeSpectr - 1][row*sizeSpectr+col] = tempRow2;
                        }
                        return;
                    }
                    case 4:
                    {
                        double tempRow0 = 0;
                        double tempRow1 = 0;
                        double tempRow2 = 0;
                        if (!bSideRight)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                tempRow0 += dataCube[i][row*sizeSpectr+col]*coeff_quartic_7_left_pixels_0_row[i];
                                tempRow1 += dataCube[i][row*sizeSpectr+col]*coeff_quartic_7_left_pixels_1_row[i];
                                tempRow1 += dataCube[i][row*sizeSpectr+col]*coeff_quartic_7_left_pixels_2_row[i];
                            }
                            dataCube[0][row*sizeSpectr+col] = tempRow0;
                            dataCube[1][row*sizeSpectr+col] = tempRow1;
                            dataCube[2][row*sizeSpectr+col] = tempRow2;

                        } else
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                tempRow0 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_quartic_7_right_pixels_4_row[i];
                                tempRow1 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_quartic_7_right_pixels_5_row[i];
                                tempRow2 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_quartic_7_right_pixels_6_row[i];
                            }
                            dataCube[sizeSpectr - 3][row*sizeSpectr+col] = tempRow0;
                            dataCube[sizeSpectr - 2][row*sizeSpectr+col] = tempRow1;
                            dataCube[sizeSpectr - 1][row*sizeSpectr+col] = tempRow2;
                        }
                        return;
                    }
                    case 5:
                    {
                        double tempRow0 = 0;
                        double tempRow1 = 0;
                        double tempRow2 = 0;
                        if (!bSideRight)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                tempRow0 += dataCube[i][row*sizeSpectr+col]*coeff_quintic_7_left_pixels_0_row[i];
                                tempRow1 += dataCube[i][row*sizeSpectr+col]*coeff_quintic_7_left_pixels_1_row[i];
                                tempRow1 += dataCube[i][row*sizeSpectr+col]*coeff_quintic_7_left_pixels_2_row[i];
                            }
                            dataCube[0][row*sizeSpectr+col] = tempRow0;
                            dataCube[1][row*sizeSpectr+col] = tempRow1;
                            dataCube[2][row*sizeSpectr+col] = tempRow2;

                        } else
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                tempRow0 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_quintic_7_right_pixels_4_row[i];
                                tempRow1 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_quintic_7_right_pixels_5_row[i];
                                tempRow2 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_quintic_7_right_pixels_6_row[i];
                            }
                            dataCube[sizeSpectr - 3][row*sizeSpectr+col] = tempRow0;
                            dataCube[sizeSpectr - 2][row*sizeSpectr+col] = tempRow1;
                            dataCube[sizeSpectr - 1][row*sizeSpectr+col] = tempRow2;
                        }
                        return;
                    }
                    default: return;
                    }
                return;
            }
            case 9:
            {
                switch(degreePolinom)
                {
                case 2:
                {
                    double tempRow0 = 0;
                    double tempRow1 = 0;
                    double tempRow2 = 0;
                    double tempRow3 = 0;
                    if (!bSideRight)
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            tempRow0 += dataCube[i][row*sizeSpectr+col]*coeff_quadr_9_left_pixels_0_row[i];
                            tempRow1 += dataCube[i][row*sizeSpectr+col]*coeff_quadr_9_left_pixels_1_row[i];
                            tempRow1 += dataCube[i][row*sizeSpectr+col]*coeff_quadr_9_left_pixels_2_row[i];
                            tempRow2 += dataCube[i][row*sizeSpectr+col]*coeff_quadr_9_left_pixels_3_row[i];
                        }
                        dataCube[0][row*sizeSpectr+col] = tempRow0;
                        dataCube[1][row*sizeSpectr+col] = tempRow1;
                        dataCube[2][row*sizeSpectr+col] = tempRow2;
                        dataCube[3][row*sizeSpectr+col] = tempRow3;

                    } else
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            tempRow0 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_cubic_9_right_pixels_5_row[i];
                            tempRow1 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_cubic_9_right_pixels_6_row[i];
                            tempRow2 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_cubic_9_right_pixels_7_row[i];
                            tempRow3 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_cubic_9_right_pixels_8_row[i];
                        }
                        dataCube[sizeSpectr - 4][row*sizeSpectr+col] = tempRow0;
                        dataCube[sizeSpectr - 3][row*sizeSpectr+col] = tempRow1;
                        dataCube[sizeSpectr - 2][row*sizeSpectr+col] = tempRow2;
                        dataCube[sizeSpectr - 1][row*sizeSpectr+col] = tempRow3;
                    }
                    return;
                }
                case 3:
                {
                    double tempRow0 = 0;
                    double tempRow1 = 0;
                    double tempRow2 = 0;
                    double tempRow3 = 0;
                    if (!bSideRight)
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            tempRow0 += dataCube[i][row*sizeSpectr+col]*coeff_cubic_9_left_pixels_0_row[i];
                            tempRow1 += dataCube[i][row*sizeSpectr+col]*coeff_cubic_9_left_pixels_1_row[i];
                            tempRow1 += dataCube[i][row*sizeSpectr+col]*coeff_cubic_9_left_pixels_2_row[i];
                            tempRow2 += dataCube[i][row*sizeSpectr+col]*coeff_cubic_9_left_pixels_3_row[i];
                        }
                        dataCube[0][row*sizeSpectr+col] = tempRow0;
                        dataCube[1][row*sizeSpectr+col] = tempRow1;
                        dataCube[2][row*sizeSpectr+col] = tempRow2;
                        dataCube[3][row*sizeSpectr+col] = tempRow3;

                    } else
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            tempRow0 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_cubic_9_right_pixels_5_row[i];
                            tempRow1 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_cubic_9_right_pixels_6_row[i];
                            tempRow2 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_cubic_9_right_pixels_7_row[i];
                            tempRow2 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_cubic_9_right_pixels_8_row[i];
                        }
                        dataCube[sizeSpectr - 4][row*sizeSpectr+col] = tempRow0;
                        dataCube[sizeSpectr - 3][row*sizeSpectr+col] = tempRow1;
                        dataCube[sizeSpectr - 2][row*sizeSpectr+col] = tempRow2;
                        dataCube[sizeSpectr - 1][row*sizeSpectr+col] = tempRow3;
                    }
                    return;
                }
                case 4:
                {
                    double tempRow0 = 0;
                    double tempRow1 = 0;
                    double tempRow2 = 0;
                    double tempRow3 = 0;
                    if (!bSideRight)
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            tempRow0 += dataCube[i][row*sizeSpectr+col]*coeff_quartic_9_left_pixels_0_row[i];
                            tempRow1 += dataCube[i][row*sizeSpectr+col]*coeff_quartic_9_left_pixels_1_row[i];
                            tempRow1 += dataCube[i][row*sizeSpectr+col]*coeff_quartic_9_left_pixels_2_row[i];
                            tempRow2 += dataCube[i][row*sizeSpectr+col]*coeff_quartic_9_left_pixels_3_row[i];
                        }
                        dataCube[0][row*sizeSpectr+col] = tempRow0;
                        dataCube[1][row*sizeSpectr+col] = tempRow1;
                        dataCube[2][row*sizeSpectr+col] = tempRow2;
                        dataCube[3][row*sizeSpectr+col] = tempRow3;

                    } else
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            tempRow0 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_quartic_9_right_pixels_5_row[i];
                            tempRow1 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_quartic_9_right_pixels_6_row[i];
                            tempRow2 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_quartic_9_right_pixels_7_row[i];
                            tempRow2 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_quartic_9_right_pixels_8_row[i];
                        }
                        dataCube[sizeSpectr - 4][row*sizeSpectr+col] = tempRow0;
                        dataCube[sizeSpectr - 3][row*sizeSpectr+col] = tempRow1;
                        dataCube[sizeSpectr - 2][row*sizeSpectr+col] = tempRow2;
                        dataCube[sizeSpectr - 1][row*sizeSpectr+col] = tempRow3;
                    }
                    return;
                }
                case 5:
                {
                    double tempRow0 = 0;
                    double tempRow1 = 0;
                    double tempRow2 = 0;
                    double tempRow3 = 0;
                    if (!bSideRight)
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            tempRow0 += dataCube[i][row*sizeSpectr+col]*coeff_quintic_9_left_pixels_0_row[i];
                            tempRow1 += dataCube[i][row*sizeSpectr+col]*coeff_quintic_9_left_pixels_1_row[i];
                            tempRow1 += dataCube[i][row*sizeSpectr+col]*coeff_quintic_9_left_pixels_2_row[i];
                            tempRow2 += dataCube[i][row*sizeSpectr+col]*coeff_quintic_9_left_pixels_3_row[i];
                        }
                        dataCube[0][row*sizeSpectr+col] = tempRow0;
                        dataCube[1][row*sizeSpectr+col] = tempRow1;
                        dataCube[2][row*sizeSpectr+col] = tempRow2;
                        dataCube[3][row*sizeSpectr+col] = tempRow3;

                    } else
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            tempRow0 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_quintic_9_right_pixels_5_row[i];
                            tempRow1 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_quintic_9_right_pixels_6_row[i];
                            tempRow2 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_quintic_9_right_pixels_7_row[i];
                            tempRow2 += dataCube[sizeSpectr - maskPixels + i][row*sizeSpectr+col]*coeff_quintic_9_right_pixels_8_row[i];
                        }
                        dataCube[sizeSpectr - 4][row*sizeSpectr+col] = tempRow0;
                        dataCube[sizeSpectr - 3][row*sizeSpectr+col] = tempRow1;
                        dataCube[sizeSpectr - 2][row*sizeSpectr+col] = tempRow2;
                        dataCube[sizeSpectr - 1][row*sizeSpectr+col] = tempRow3;
                    }
                    return;
                }
                default: return;
                }
                return;
            }
        }
    }

    bool ToCube()
    {
        QIcon icon(":/NoiseRemover/icons/NoiseRemover.png");
        QProgressDialog* progressBar  = new QProgressDialog();
        progressBar->setLabelText("        Устранение шумов у гиперспектральных данных         ");
        progressBar->setWindowIcon(icon);
        QString descr = QString("Фильтр Савитского-Голау. Маска: %1 пикселей. Степень полинома: %2").arg(QString::number(BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()), QString::number(BaseNoiseAlg<T>::m_attributes->GetDegreePolinom()));
        progressBar->setWindowTitle(descr);
        progressBar->setRange(0, 100);
        progressBar->setWindowModality(Qt::WindowModal);
        progressBar->setCancelButtonText("Отмена");
        progressBar->show();

        u::int8 maskPixels = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();
        u::int32* coeff = new u::int32[maskPixels];
        u::uint32 normalization = 0;
        ChooseCoef(coeff, normalization);
        u::uint32 size = BaseNoiseAlg<T>::m_cube->GetSizeSpectrum();
        u::uint32 linesCube = BaseNoiseAlg<T>::m_cube->GetLines();
        u::uint32 columnsCube = BaseNoiseAlg<T>::m_cube->GetColumns();
        int percent = 0;
        u::uint32 maxValueBar = linesCube*columnsCube;
        T** dataCube = (T**)BaseNoiseAlg<T>::m_cube->GetDataCube();
        progressBar->setValue(0);
        QApplication::processEvents();
        for (u::uint32 lines = 0; lines < linesCube; lines++)
        {
             for (u::uint32 col = 0; col <columnsCube; col++)
             {
                 if (progressBar->wasCanceled())
                 {
                     delete progressBar;
                     //From HDF
                     return false;
                 }
                 CalculateEdgesCube(dataCube, size, lines, col, false);
                 for (u::uint32 i = maskPixels/2; i < size - maskPixels/2; i++)
                 {
                     double temp = 0;
                     for (u::uint8 j = 0; j < maskPixels; j++)
                     {
                         temp+= dataCube[i + j-maskPixels/2][lines*columnsCube + col]*coeff[j];
                     }
                     T value = temp/normalization;
                     BaseNoiseAlg<T>::m_cube->SetDataBuffer(i, &value, sizeof(T), (lines*columnsCube+col)*sizeof(T));
                 }
                 CalculateEdgesCube(dataCube, size, lines, col, true);
                 double a = lines*columnsCube + col;
                 double b = a/maxValueBar*100;
                 percent = b*100/100;
                 progressBar->setValue(percent);
                 QApplication::processEvents();
             }
        }
        if (!progressBar->wasCanceled())
        {
            progressBar->setValue(100);
            QApplication::processEvents();
            progressBar->hide();
            delete progressBar;
        }
        return true;
    }

    void ToSpectrWindow()
    {
        QVector<double> XUnits = BaseNoiseAlg<T>::m_attributes->GetXUnits();
        QVector<double> spectrMas = BaseNoiseAlg<T>::m_attributes->GetYUnits();
        // определим массив коэффициентов
        u::int8 maskPixels = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();
        u::int32* coeff = new u::int32[maskPixels];
        u::uint32 normalization;
        ChooseCoef(coeff, normalization);
        // проверить, меняется ли data
        CalculateEdges<double>(spectrMas.data(), spectrMas.size(), false);
        ApplyFilterToSpectr<double>(spectrMas.data(), spectrMas.size(), coeff, normalization);
        CalculateEdges<double>(spectrMas.data(), spectrMas.size(), true);
        delete [] coeff;
        BaseNoiseAlg<T>::m_attributes->ClearUnitsLists();
        BaseNoiseAlg<T>::m_attributes->SetExternalSpectrFlag(true);
        //BaseNoiseAlg<T>::m_attributes->SetExternalSpectrFormat(0);
        QString descr = QString(" Савитский-Голау фильтр %1x%1").arg(QString::number(BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()));
        BaseNoiseAlg<T>::m_attributes->SetDescriptionItem("Устранение шумов:", descr);
        BaseNoiseAlg<T>::m_attributes->SetXUnit(XUnits);
        BaseNoiseAlg<T>::m_attributes->SetYUnit(spectrMas);
        BaseNoiseAlg<T>::m_attributes->GetAvailablePlugins().value("Spectr UI")->Execute(BaseNoiseAlg<T>::m_cube, BaseNoiseAlg<T>::m_attributes);
    }
};
#endif // SAVITSKIGOLAY1DALG

