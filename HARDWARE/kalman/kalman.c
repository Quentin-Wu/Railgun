#include "kalman.h"

/*       
        Q:����������Q���󣬶�̬��Ӧ��죬�����ȶ��Ա仵
        R:����������R���󣬶�̬��Ӧ�����������ȶ��Ա��  
*/

#define KALMAN_Q 0.02

#define KALMAN_R 7.0000

/* �������˲����� */

double KalmanFilter(double ResrcData,double ProcessNiose_Q,double MeasureNoise_R)
{

    double R = MeasureNoise_R;
    double Q = ProcessNiose_Q;

    static double x_last;
    double x_mid = x_last;
    double x_now;

    static double p_last;
    double p_mid ;
    double p_now;

    double kg;

    x_mid=x_last;                       //x_last=x(k-1|k-1),x_mid=x(k|k-1)
    p_mid=p_last+Q;                     //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����

    /*
     *  �������˲������Ҫ��ʽ
     */
    kg=p_mid/(p_mid+R);                 //kgΪkalman filter,RΪ����
    x_now=x_mid+kg*(ResrcData-x_mid);   //���Ƴ������Ž�
    p_now=(1-kg)*p_mid;                 //���Ž��Ӧ��covariance
    p_last = p_now;                     //����covarianceֵ
    x_last = x_now;                     //����ϵͳ״ֵ̬

    return x_now;

}



