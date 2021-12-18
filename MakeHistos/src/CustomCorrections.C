#include <math.h>


namespace AdHocMassCor{

  float massCor_PF_all( float d0p, float d0m, int year){
    if (year == 2016){
if      (d0p >= -0.010 && d0p < -0.005 && d0m >= -0.002 && d0m < -0.001) return -0.87;
else if (d0p >= -0.010 && d0p < -0.005 && d0m >= 0.0    && d0m < 0.001)  return -0.87;
else if (d0p >= -0.010 && d0p < -0.005 && d0m >= 0.0    && d0m < 0.001)  return -0.95;

else if (d0p >= -0.005 && d0p < -0.003 && d0m >= -0.002 && d0m < -0.001) return -0.53;
else if (d0p >= -0.005 && d0p < -0.003 && d0m >= -0.001 && d0m < 0.0)    return -0.67;
else if (d0p >= -0.005 && d0p < -0.003 && d0m >= 0.0    && d0m < 0.001)  return -0.71;
else if (d0p >= -0.005 && d0p < -0.003 && d0m >= 0.001  && d0m < 0.002)  return -0.96;

else if (d0p >= -0.003 && d0p < -0.002 && d0m >= -0.003 && d0m < -0.002) return -0.33;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= -0.002 && d0m < -0.001) return -0.40;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= -0.001 && d0m < 0.0)    return -0.51;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= 0.0    && d0m < 0.001)  return -0.67;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= 0.001  && d0m < 0.002)  return -0.90;

else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.010 && d0m < -0.005) return  0.34;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.005 && d0m < -0.003) return  0.00;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.003 && d0m < -0.002) return -0.14;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.002 && d0m < -0.001) return -0.23;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.001 && d0m < 0.0)    return -0.33;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= 0.0    && d0m < 0.001)  return -0.58;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= 0.001  && d0m < 0.002)  return -0.77;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= 0.002  && d0m < 0.003)  return -0.79;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= 0.003  && d0m < 0.005)  return -1.14;

else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.010 && d0m < -0.005) return  0.34;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.005 && d0m < -0.003) return  0.16;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.003 && d0m < -0.002) return  0.04;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.002 && d0m < -0.001) return -0.12;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.001 && d0m < 0.0)    return -0.21;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.0    && d0m < 0.001)  return -0.32;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.001  && d0m < 0.002)  return -0.57;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.002  && d0m < 0.003)  return -0.72;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.003  && d0m < 0.005)  return -0.72;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.005  && d0m < 0.010)  return -1.02;

else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.010 && d0m < -0.005) return  0.46;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.005 && d0m < -0.003) return  0.25;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.003 && d0m < -0.002) return  0.25;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.002 && d0m < -0.001) return  0.07;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.001 && d0m < -0.0)   return -0.15;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.0    && d0m < 0.001)  return -0.22;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.001  && d0m < 0.002)  return -0.36;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.002  && d0m < 0.003)  return -0.52;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.003  && d0m < 0.005)  return -0.67;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.005  && d0m < 0.010)  return -0.94;

else if (d0p >= 0.001  && d0p < 0.002  && d0m >= -0.005 && d0m < -0.003) return  0.35;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= -0.003 && d0m < -0.002) return  0.57;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= -0.002 && d0m < -0.001) return  0.36;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= -0.001 && d0m < -0.0)   return  0.08;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.0    && d0m < 0.001)  return -0.11;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.001  && d0m < 0.002)  return -0.25;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.002  && d0m < 0.003)  return -0.42;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.003  && d0m < 0.005)  return -0.55;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.005  && d0m < 0.010)  return -0.87;

else if (d0p >= 0.002  && d0p < 0.003  && d0m >= -0.002 && d0m < -0.001) return  0.32;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= -0.001 && d0m < -0.0)   return  0.18;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= 0.0    && d0m < 0.001)  return  0.02;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= 0.001  && d0m < 0.002)  return -0.12;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= 0.002  && d0m < 0.003)  return -0.28;

else if (d0p >= 0.003  && d0p < 0.005  && d0m >= -0.002 && d0m < -0.001) return  0.43;
else if (d0p >= 0.003  && d0p < 0.005  && d0m >= -0.001 && d0m < -0.0)   return  0.32;
else if (d0p >= 0.003  && d0p < 0.005  && d0m >= 0.0    && d0m < 0.001)  return  0.16;
else if (d0p >= 0.003  && d0p < 0.005  && d0m >= 0.001  && d0m < 0.002)  return  0.06;

else if (d0p >= 0.005  && d0p < 0.010  && d0m >= -0.001 && d0m < -0.0)   return  0.46;
else if (d0p >= 0.005  && d0p < 0.010  && d0m >= 0.0    && d0m < 0.001)  return  0.36;
else if (d0p >= 0.005  && d0p < 0.010  && d0m >= 0.001  && d0m < 0.002)  return  0.03;

else return 0.0;
    } // close if 2016
    else if (year ==2017){
return 0.0;
    } // close if 2017

    else if (year == 2018){
return 0.0;
    } //close if 2018

    else return 0.0;
  } // End of massCor_all





  float massCor_PF_CC( float d0p, float d0m, int year){
    if (year == 2016){
if      (d0p >= -0.010 && d0p < -0.005 && d0m >= 0.0    && d0m < 0.001)  return -0.60;
else if (d0p >= -0.010 && d0p < -0.005 && d0m >= 0.0    && d0m < 0.001)  return -0.64;

else if (d0p >= -0.005 && d0p < -0.003 && d0m >= -0.002 && d0m < -0.001) return -0.48;
else if (d0p >= -0.005 && d0p < -0.003 && d0m >= -0.001 && d0m < 0.0)    return -0.50;
else if (d0p >= -0.005 && d0p < -0.003 && d0m >= 0.0    && d0m < 0.001)  return -0.54;

else if (d0p >= -0.003 && d0p < -0.002 && d0m >= -0.003 && d0m < -0.002) return -0.33;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= -0.002 && d0m < -0.001) return -0.33;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= -0.001 && d0m < 0.0)    return -0.40;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= 0.0    && d0m < 0.001)  return -0.55;

else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.005 && d0m < -0.003) return -0.15;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.003 && d0m < -0.002) return -0.18;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.002 && d0m < -0.001) return -0.21;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.001 && d0m < 0.0)    return -0.28;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= 0.0    && d0m < 0.001)  return -0.49;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= 0.001  && d0m < 0.002)  return -0.64;

else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.010 && d0m < -0.005) return  0.16;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.005 && d0m < -0.003) return  0.06;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.003 && d0m < -0.002) return -0.01;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.002 && d0m < -0.001) return -0.12;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.001 && d0m < 0.0)    return -0.20;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.0    && d0m < 0.001)  return -0.28;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.001  && d0m < 0.002)  return -0.48;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.002  && d0m < 0.003)  return -0.52;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.003  && d0m < 0.005)  return -0.57;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.005  && d0m < 0.010)  return -0.73;

else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.010 && d0m < -0.005) return  0.24;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.005 && d0m < -0.003) return  0.14;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.003 && d0m < -0.002) return  0.15;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.002 && d0m < -0.001) return  0.07;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.001 && d0m < -0.0)   return -0.14;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.0    && d0m < 0.001)  return -0.20;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.001  && d0m < 0.002)  return -0.30;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.002  && d0m < 0.003)  return -0.38;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.003  && d0m < 0.005)  return -0.50;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.005  && d0m < 0.010)  return -0.65;

else if (d0p >= 0.001  && d0p < 0.002  && d0m >= -0.002 && d0m < -0.001) return  0.39;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= -0.001 && d0m < -0.0)   return  0.07;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.0    && d0m < 0.001)  return -0.11;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.001  && d0m < 0.002)  return -0.23;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.002  && d0m < 0.003)  return -0.34;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.003  && d0m < 0.005)  return -0.34;

else if (d0p >= 0.002  && d0p < 0.003  && d0m >= -0.001 && d0m < -0.0)   return  0.09;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= 0.0    && d0m < 0.001)  return -0.02;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= 0.001  && d0m < 0.002)  return -0.15;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= 0.002  && d0m < 0.003)  return -0.32;

else if (d0p >= 0.003  && d0p < 0.005  && d0m >= -0.001 && d0m < -0.0)   return  0.20;
else if (d0p >= 0.003  && d0p < 0.005  && d0m >= 0.0    && d0m < 0.001)  return  0.05;
else if (d0p >= 0.003  && d0p < 0.005  && d0m >= 0.001  && d0m < 0.002)  return -0.05;

else if (d0p >= 0.005  && d0p < 0.010  && d0m >= -0.001 && d0m < -0.0)   return  0.24;
else if (d0p >= 0.005  && d0p < 0.010  && d0m >= 0.0    && d0m < 0.001)  return  0.17;

else return 0.0;
    } // close if 2016
    else if (year ==2017){
return 0.0;
    } // close if 2017

    else if (year == 2018){
return 0.0;
    } //close if 2018

    else return 0.0;
  } // End of massCor_CC





  float massCor_PF_FX( float d0p, float d0m, int year){
    if (year == 2016){
if      (d0p >= -0.010 && d0p < -0.005 && d0m >= 0.0    && d0m < 0.001)  return -1.20;
else if (d0p >= -0.010 && d0p < -0.005 && d0m >= 0.0    && d0m < 0.001)  return -1.42;

else if (d0p >= -0.005 && d0p < -0.003 && d0m >= -0.002 && d0m < -0.001) return -0.60;
else if (d0p >= -0.005 && d0p < -0.003 && d0m >= -0.001 && d0m < 0.0)    return -0.90;
else if (d0p >= -0.005 && d0p < -0.003 && d0m >= 0.0    && d0m < 0.001)  return -1.01;

else if (d0p >= -0.003 && d0p < -0.002 && d0m >= -0.003 && d0m < -0.002) return -0.41;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= -0.002 && d0m < -0.001) return -0.52;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= -0.001 && d0m < 0.0)    return -0.68;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= 0.0    && d0m < 0.001)  return -0.86;

else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.005 && d0m < -0.003) return  0.13;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.003 && d0m < -0.002) return -0.06;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.002 && d0m < -0.001) return -0.28;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.001 && d0m < 0.0)    return -0.45;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= 0.0    && d0m < 0.001)  return -0.76;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= 0.001  && d0m < 0.002)  return -0.95;

else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.010 && d0m < -0.005) return  0.57;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.005 && d0m < -0.003) return  0.31;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.003 && d0m < -0.002) return  0.12;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.002 && d0m < -0.001) return -0.10;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.001 && d0m < 0.0)    return -0.26;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.0    && d0m < 0.001)  return -0.42;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.001  && d0m < 0.002)  return -0.73;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.002  && d0m < 0.003)  return -0.97;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.003  && d0m < 0.005)  return -0.97;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.005  && d0m < 0.010)  return -1.44;

else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.010 && d0m < -0.005) return  0.77;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.005 && d0m < -0.003) return  0.43;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.003 && d0m < -0.002) return  0.40;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.002 && d0m < -0.001) return  0.07;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.001 && d0m < -0.0)   return -0.16;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.0    && d0m < 0.001)  return -0.27;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.001  && d0m < 0.002)  return -0.47;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.002  && d0m < 0.003)  return -0.73;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.003  && d0m < 0.005)  return -0.91;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.005  && d0m < 0.010)  return -1.30;

else if (d0p >= 0.001  && d0p < 0.002  && d0m >= -0.002 && d0m < -0.001) return  0.33;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= -0.001 && d0m < -0.0)   return  0.09;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.0    && d0m < 0.001)  return -0.10;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.001  && d0m < 0.002)  return -0.31;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.002  && d0m < 0.003)  return -0.56;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.003  && d0m < 0.005)  return -0.72;

else if (d0p >= 0.002  && d0p < 0.003  && d0m >= -0.001 && d0m < -0.0)   return  0.30;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= 0.0    && d0m < 0.001)  return  0.10;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= 0.001  && d0m < 0.002)  return -0.05;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= 0.002  && d0m < 0.003)  return -0.25;

else if (d0p >= 0.003  && d0p < 0.005  && d0m >= -0.001 && d0m < -0.0)   return  0.51;
else if (d0p >= 0.003  && d0p < 0.005  && d0m >= 0.0    && d0m < 0.001)  return  0.30;
else if (d0p >= 0.003  && d0p < 0.005  && d0m >= 0.001  && d0m < 0.002)  return  0.13;

else if (d0p >= 0.005  && d0p < 0.010  && d0m >= -0.001 && d0m < -0.0)   return  0.79;
else if (d0p >= 0.005  && d0p < 0.010  && d0m >= 0.0    && d0m < 0.001)  return  0.62;

else return 0.0;
    } // close if 2016
    else if (year ==2017){
return 0.0;
    } // close if 2017

    else if (year == 2018){
return 0.0;
    } //close if 2018

    else return 0.0;
  } // End of massCor_FX









  float massCor_KaMu_all( float d0p, float d0m, int year){
    if (year == 2016){
if      (d0p >= -0.010 && d0p < -0.005 && d0m >= -0.002 && d0m < -0.001) return -0.84;
else if (d0p >= -0.010 && d0p < -0.005 && d0m >= 0.0    && d0m < 0.001)  return -0.87;
else if (d0p >= -0.010 && d0p < -0.005 && d0m >= 0.0    && d0m < 0.001)  return -0.96;

else if (d0p >= -0.005 && d0p < -0.003 && d0m >= -0.002 && d0m < -0.001) return -0.53;
else if (d0p >= -0.005 && d0p < -0.003 && d0m >= -0.001 && d0m < 0.0)    return -0.66;
else if (d0p >= -0.005 && d0p < -0.003 && d0m >= 0.0    && d0m < 0.001)  return -0.71;
else if (d0p >= -0.005 && d0p < -0.003 && d0m >= 0.001  && d0m < 0.002)  return -0.98;

else if (d0p >= -0.003 && d0p < -0.002 && d0m >= -0.003 && d0m < -0.002) return -0.29;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= -0.002 && d0m < -0.001) return -0.39;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= -0.001 && d0m < 0.0)    return -0.51;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= 0.0    && d0m < 0.001)  return -0.68;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= 0.001  && d0m < 0.002)  return -0.84;

else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.010 && d0m < -0.005) return  0.30;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.005 && d0m < -0.003) return  0.04;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.003 && d0m < -0.002) return -0.14;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.002 && d0m < -0.001) return -0.24;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.001 && d0m < 0.0)    return -0.34;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= 0.0    && d0m < 0.001)  return -0.59;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= 0.001  && d0m < 0.002)  return -0.79;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= 0.002  && d0m < 0.003)  return -0.75;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= 0.003  && d0m < 0.005)  return -1.16;

else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.010 && d0m < -0.005) return  0.35;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.005 && d0m < -0.003) return  0.16;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.003 && d0m < -0.002) return  0.04;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.002 && d0m < -0.001) return -0.12;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.001 && d0m < 0.0)    return -0.22;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.0    && d0m < 0.001)  return -0.32;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.001  && d0m < 0.002)  return -0.58;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.002  && d0m < 0.003)  return -0.71;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.003  && d0m < 0.005)  return -0.71;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.005  && d0m < 0.010)  return -1.03;

else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.010 && d0m < -0.005) return  0.45;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.005 && d0m < -0.003) return  0.24;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.003 && d0m < -0.002) return  0.25;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.002 && d0m < -0.001) return  0.07;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.001 && d0m < -0.0)   return -0.15;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.0    && d0m < 0.001)  return -0.22;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.001  && d0m < 0.002)  return -0.36;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.002  && d0m < 0.003)  return -0.53;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.003  && d0m < 0.005)  return -0.67;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.005  && d0m < 0.010)  return -0.95;

else if (d0p >= 0.001  && d0p < 0.002  && d0m >= -0.005 && d0m < -0.003) return  0.44;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= -0.003 && d0m < -0.002) return  0.64;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= -0.002 && d0m < -0.001) return  0.37;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= -0.001 && d0m < -0.0)   return  0.07;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.0    && d0m < 0.001)  return -0.11;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.001  && d0m < 0.002)  return -0.25;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.002  && d0m < 0.003)  return -0.42;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.003  && d0m < 0.005)  return -0.57;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.005  && d0m < 0.010)  return -0.76;

else if (d0p >= 0.002  && d0p < 0.003  && d0m >= -0.002 && d0m < -0.001) return  0.34;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= -0.001 && d0m < -0.0)   return  0.20;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= 0.0    && d0m < 0.001)  return  0.02;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= 0.001  && d0m < 0.002)  return -0.12;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= 0.002  && d0m < 0.003)  return -0.29;

else if (d0p >= 0.003  && d0p < 0.005  && d0m >= -0.002 && d0m < -0.001) return  0.47;
else if (d0p >= 0.003  && d0p < 0.005  && d0m >= -0.001 && d0m < -0.0)   return  0.32;
else if (d0p >= 0.003  && d0p < 0.005  && d0m >= 0.0    && d0m < 0.001)  return  0.16;
else if (d0p >= 0.003  && d0p < 0.005  && d0m >= 0.001  && d0m < 0.002)  return  0.06;

else if (d0p >= 0.005  && d0p < 0.010  && d0m >= -0.001 && d0m < -0.0)   return  0.49;
else if (d0p >= 0.005  && d0p < 0.010  && d0m >= 0.0    && d0m < 0.001)  return  0.38;
else if (d0p >= 0.005  && d0p < 0.010  && d0m >= 0.001  && d0m < 0.002)  return  0.12;

else return 0.0;
    } // close if 2016
    else if (year ==2017){
return 0.0;
    } // close if 2017

    else if (year == 2018){
return 0.0;
    } //close if 2018

    else return 0.0;
  } // End of massCor_KaMu_all





  float massCor_KaMu_CC( float d0p, float d0m, int year){
    if (year == 2016){
if      (d0p >= -0.010 && d0p < -0.005 && d0m >= 0.0    && d0m < 0.001)  return -0.60;
else if (d0p >= -0.010 && d0p < -0.005 && d0m >= 0.0    && d0m < 0.001)  return -0.66;

else if (d0p >= -0.005 && d0p < -0.003 && d0m >= -0.002 && d0m < -0.001) return -0.49;
else if (d0p >= -0.005 && d0p < -0.003 && d0m >= -0.001 && d0m < 0.0)    return -0.50;
else if (d0p >= -0.005 && d0p < -0.003 && d0m >= 0.0    && d0m < 0.001)  return -0.54;

else if (d0p >= -0.003 && d0p < -0.002 && d0m >= -0.003 && d0m < -0.002) return -0.30;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= -0.002 && d0m < -0.001) return -0.33;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= -0.001 && d0m < 0.0)    return -0.40;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= 0.0    && d0m < 0.001)  return -0.56;

else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.005 && d0m < -0.003) return -0.16;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.003 && d0m < -0.002) return -0.20;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.002 && d0m < -0.001) return -0.23;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.001 && d0m < 0.0)    return -0.30;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= 0.0    && d0m < 0.001)  return -0.50;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= 0.001  && d0m < 0.002)  return -0.69;

else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.010 && d0m < -0.005) return  0.16;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.005 && d0m < -0.003) return  0.04;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.003 && d0m < -0.002) return -0.02;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.002 && d0m < -0.001) return -0.14;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.001 && d0m < 0.0)    return -0.21;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.0    && d0m < 0.001)  return -0.30;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.001  && d0m < 0.002)  return -0.47;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.002  && d0m < 0.003)  return -0.54;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.003  && d0m < 0.005)  return -0.57;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.005  && d0m < 0.010)  return -0.75;

else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.010 && d0m < -0.005) return  0.22;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.005 && d0m < -0.003) return  0.13;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.003 && d0m < -0.002) return  0.13;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.002 && d0m < -0.001) return  0.06;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.001 && d0m < -0.0)   return -0.15;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.0    && d0m < 0.001)  return -0.21;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.001  && d0m < 0.002)  return -0.32;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.002  && d0m < 0.003)  return -0.40;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.003  && d0m < 0.005)  return -0.50;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.005  && d0m < 0.010)  return -0.65;

else if (d0p >= 0.001  && d0p < 0.002  && d0m >= -0.002 && d0m < -0.001) return  0.38;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= -0.001 && d0m < -0.0)   return  0.05;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.0    && d0m < 0.001)  return -0.12;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.001  && d0m < 0.002)  return -0.23;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.002  && d0m < 0.003)  return -0.35;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.003  && d0m < 0.005)  return -0.35;

else if (d0p >= 0.002  && d0p < 0.003  && d0m >= -0.001 && d0m < -0.0)   return  0.09;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= 0.0    && d0m < 0.001)  return -0.04;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= 0.001  && d0m < 0.002)  return -0.17;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= 0.002  && d0m < 0.003)  return -0.29;

else if (d0p >= 0.003  && d0p < 0.005  && d0m >= -0.001 && d0m < -0.0)   return  0.20;
else if (d0p >= 0.003  && d0p < 0.005  && d0m >= 0.0    && d0m < 0.001)  return  0.04;
else if (d0p >= 0.003  && d0p < 0.005  && d0m >= 0.001  && d0m < 0.002)  return -0.03;

else if (d0p >= 0.005  && d0p < 0.010  && d0m >= -0.001 && d0m < -0.0)   return  0.24;
else if (d0p >= 0.005  && d0p < 0.010  && d0m >= 0.0    && d0m < 0.001)  return  0.17;

else return 0.0;
    } // close if 2016
    else if (year ==2017){
return 0.0;
    } // close if 2017

    else if (year == 2018){
return 0.0;
    } //close if 2018

    else return 0.0;
  } // End of massCor_KaMu_CC





  float massCor_KaMu_FX( float d0p, float d0m, int year){
    if (year == 2016){
if      (d0p >= -0.010 && d0p < -0.005 && d0m >= 0.0    && d0m < 0.001)  return -1.21;
else if (d0p >= -0.010 && d0p < -0.005 && d0m >= 0.0    && d0m < 0.001)  return -1.40;

else if (d0p >= -0.005 && d0p < -0.003 && d0m >= -0.002 && d0m < -0.001) return -0.60;
else if (d0p >= -0.005 && d0p < -0.003 && d0m >= -0.001 && d0m < 0.0)    return -0.89;
else if (d0p >= -0.005 && d0p < -0.003 && d0m >= 0.0    && d0m < 0.001)  return -1.01;

else if (d0p >= -0.003 && d0p < -0.002 && d0m >= -0.003 && d0m < -0.002) return -0.31;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= -0.002 && d0m < -0.001) return -0.49;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= -0.001 && d0m < 0.0)    return -0.66;
else if (d0p >= -0.003 && d0p < -0.002 && d0m >= 0.0    && d0m < 0.001)  return -0.88;

else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.005 && d0m < -0.003) return  0.20;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.003 && d0m < -0.002) return -0.05;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.002 && d0m < -0.001) return -0.27;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= -0.001 && d0m < 0.0)    return -0.43;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= 0.0    && d0m < 0.001)  return -0.76;
else if (d0p >= -0.002 && d0p < -0.001 && d0m >= 0.001  && d0m < 0.002)  return -0.95;

else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.010 && d0m < -0.005) return  0.63;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.005 && d0m < -0.003) return  0.31;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.003 && d0m < -0.002) return  0.13;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.002 && d0m < -0.001) return -0.08;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= -0.001 && d0m < 0.0)    return -0.24;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.0    && d0m < 0.001)  return -0.39;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.001  && d0m < 0.002)  return -0.75;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.002  && d0m < 0.003)  return -0.95;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.003  && d0m < 0.005)  return -0.96;
else if (d0p >= -0.001 && d0p < 0.0    && d0m >= 0.005  && d0m < 0.010)  return -1.44;

else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.010 && d0m < -0.005) return  0.77;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.005 && d0m < -0.003) return  0.44;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.003 && d0m < -0.002) return  0.43;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.002 && d0m < -0.001) return  0.08;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= -0.001 && d0m < -0.0)   return -0.15;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.0    && d0m < 0.001)  return -0.26;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.001  && d0m < 0.002)  return -0.45;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.002  && d0m < 0.003)  return -0.71;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.003  && d0m < 0.005)  return -0.89;
else if (d0p >= 0.0    && d0p < 0.001  && d0m >= 0.005  && d0m < 0.010)  return -1.34;

else if (d0p >= 0.001  && d0p < 0.002  && d0m >= -0.002 && d0m < -0.001) return  0.34;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= -0.001 && d0m < -0.0)   return  0.11;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.0    && d0m < 0.001)  return -0.08;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.001  && d0m < 0.002)  return -0.29;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.002  && d0m < 0.003)  return -0.55;
else if (d0p >= 0.001  && d0p < 0.002  && d0m >= 0.003  && d0m < 0.005)  return -0.77;

else if (d0p >= 0.002  && d0p < 0.003  && d0m >= -0.001 && d0m < -0.0)   return  0.35;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= 0.0    && d0m < 0.001)  return  0.13;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= 0.001  && d0m < 0.002)  return -0.02;
else if (d0p >= 0.002  && d0p < 0.003  && d0m >= 0.002  && d0m < 0.003)  return -0.28;

else if (d0p >= 0.003  && d0p < 0.005  && d0m >= -0.001 && d0m < -0.0)   return  0.55;
else if (d0p >= 0.003  && d0p < 0.005  && d0m >= 0.0    && d0m < 0.001)  return  0.31;
else if (d0p >= 0.003  && d0p < 0.005  && d0m >= 0.001  && d0m < 0.002)  return  0.14;

else if (d0p >= 0.005  && d0p < 0.010  && d0m >= -0.001 && d0m < -0.0)   return  0.86;
else if (d0p >= 0.005  && d0p < 0.010  && d0m >= 0.0    && d0m < 0.001)  return  0.67;

else return 0.0;
    } // close if 2016
    else if (year ==2017){
return 0.0;
    } // close if 2017

    else if (year == 2018){
return 0.0;
    } //close if 2018

    else return 0.0;
  } // End of massCor_KaMu_FX




} // end namespace AdHocMassCor






















namespace PtGeoCor_UF{
  float PtGeo_Wide_PF( float d0, float pt, float eta, int year) { // correction applied to pt_PF
    float pt_cor = 0.0; // additive to the pt
    if (year == 2016) {
      if      (abs(eta) < 0.9) pt_cor = 516.02/10000.0 * d0 * pt * pt;
      else if (abs(eta) < 1.7) pt_cor = 775.06/10000.0 * d0 * pt * pt;
      else if (abs(eta) < 2.4) pt_cor = 1143.5/10000.0 * d0 * pt * pt;
    }
    else if (year == 2017) {
    }
    else if (year == 2018) {
    }
    return pt_cor;
  } // end of PtGeo_PF( float d0, float pt, float eta, int year) 


  float PtGeo_Wide_Roch( float d0, float pt, float eta, int year) { // correction applied to Roch
    float pt_cor = 0.0; // additive to the pt
    if (year == 2016) {
      if      (abs(eta) < 0.9) pt_cor = 522.02/10000.0 * d0 * pt * pt;
      else if (abs(eta) < 1.7) pt_cor = 814.62/10000.0 * d0 * pt * pt;
      else if (abs(eta) < 2.4) pt_cor = 1079.4/10000.0 * d0 * pt * pt;
    }
    else if (year == 2017) {
    }
    else if (year == 2018) {
    }
    return pt_cor;
  } // end of float PtGeo_Roch( float d0, float pt, float eta, int year) {


  float PtGeo_Wide_KaMu( float d0, float pt, float eta, int year) { // correction applied to KaMu
    float pt_cor = 0.0; // additive to the pt
    if (year == 2016) {
      if      (abs(eta) < 0.9) pt_cor = 554.15/10000.0 * d0 * pt * pt;
      else if (abs(eta) < 1.7) pt_cor = 765.41/10000.0 * d0 * pt * pt;
      else if (abs(eta) < 2.4) pt_cor = 1163.0/10000.0 * d0 * pt * pt;
    }
    else if (year == 2017) {
    }
    else if (year == 2018) {
    }
    return pt_cor;
  } // end of float PtGeo_Wide_KaMu( float d0, float pt, float eta, int year) {


  float PtGeo_Narrow_PF( float d0, float pt, float eta, int year) { // correction applied to pt_PF
    float pt_cor = 0.0; // additive to the pt
    if (year == 2016) {
      if      (abs(eta) < 0.9) pt_cor = 635.31/10000.0 * d0 * pt * pt;
      else if (abs(eta) < 1.7) pt_cor = 879.31/10000.0 * d0 * pt * pt;
      else if (abs(eta) < 2.4) pt_cor = 1175.5/10000.0 * d0 * pt * pt;
    }
    else if (year == 2017) {
    }
    else if (year == 2018) {
    }
    return pt_cor;
  } // end of PtGeo_PF( float d0, float pt, float eta, int year) 


  float PtGeo_Narrow_Roch( float d0, float pt, float eta, int year) { // correction applied to Roch
    float pt_cor = 0.0; // additive to the pt
    if (year == 2016) {
      if      (abs(eta) < 0.9) pt_cor = 644.82/10000.0 * d0 * pt * pt;
      else if (abs(eta) < 1.7) pt_cor = 885.44/10000.0 * d0 * pt * pt;
      else if (abs(eta) < 2.4) pt_cor = 1102.0/10000.0 * d0 * pt * pt;
    }
    else if (year == 2017) {
    }
    else if (year == 2018) {
    }
    return pt_cor;
  } // end of float PtGeo_Roch( float d0, float pt, float eta, int year) {


  float PtGeo_Narrow_KaMu( float d0, float pt, float eta, int year) { // correction applied to KaMu
    float pt_cor = 0.0; // additive to the pt
    if (year == 2016) {
      if      (abs(eta) < 0.9) pt_cor = 650.08/10000.0 * d0 * pt * pt;
      else if (abs(eta) < 1.7) pt_cor = 901.70/10000.0 * d0 * pt * pt;
      else if (abs(eta) < 2.4) pt_cor = 1325.1/10000.0 * d0 * pt * pt;
    }
    else if (year == 2017) {
    }
    else if (year == 2018) {
    }
    return pt_cor;
  } // end of float PtGeo_Narrow_KaMu( float d0, float pt, float eta, int year) {


  float PtGeo_Piece30_KaMu(float d0, float pt, float eta, int year) { // correction applied to KaMu
    float pt_cor = 0.0; 
    if (year == 2016) {
      if      (abs(eta) < 0.9) {
	if      ( d0 < -0.003 ) pt_cor = (287.46/10000.0 * d0 - 0.8090/10000.0) * pt * pt;
	else if ( d0 <  0.003 ) pt_cor = (646.75/10000.0 * d0 - 0.1757/10000.0) * pt * pt;
	else			pt_cor = (260.19/10000.0 * d0 + 0.3990/10000.0) * pt * pt;
      }
      else if (abs(eta) < 1.7) {
	if      ( d0 < -0.003 ) pt_cor = (452.27/10000.0 * d0 - 1.3386/10000.0) * pt * pt;
        else if ( d0 <  0.003 ) pt_cor = (909.01/10000.0 * d0 - 0.3583/10000.0) * pt * pt;
        else                    pt_cor = (420.97/10000.0 * d0 + 0.7031/10000.0) * pt * pt;
      } 
      else if (abs(eta) < 2.4) {
	if      ( d0 < -0.003 ) pt_cor = (705.03/10000.0 * d0 - 2.5884/10000.0) * pt * pt;
        else if ( d0 <  0.003 ) pt_cor = (1337.2/10000.0 * d0 - 0.6652/10000.0) * pt * pt;
        else                    pt_cor = (956.72/10000.0 * d0 - 0.5516/10000.0) * pt * pt;
      }
    } // end of if (year == 2016)
    else if (year == 2017) {
    }
    else if (year == 2018) {
    }
    return pt_cor;
  } // end of float PtGeo_Piece30_KaMu(float d0, float pt, float eta, int year)


  float PtGeo_Piece25_KaMu(float d0, float pt, float eta, int year) { // correction applied to KaMu
    float pt_cor = 0.0; 
    if (year == 2016) {
      if      (abs(eta) < 0.9) {
	if      ( d0 < -0.0025 ) pt_cor = (286.13/10000.0 * d0 - 0.8152/10000.0) * pt * pt;
	else if ( d0 <  0.0025 ) pt_cor = (703.07/10000.0 * d0 - 0.1841/10000.0) * pt * pt;
	else			 pt_cor = (243.46/10000.0 * d0 + 0.4950/10000.0) * pt * pt;
      }
      else if (abs(eta) < 1.7) {
	if      ( d0 < -0.0025 ) pt_cor = (463.11/10000.0 * d0 - 1.2765/10000.0) * pt * pt;
        else if ( d0 <  0.0025 ) pt_cor = (960.24/10000.0 * d0 - 0.3664/10000.0) * pt * pt;
        else                     pt_cor = (451.44/10000.0 * d0 + 0.5138/10000.0) * pt * pt;
      } 
      else if (abs(eta) < 2.4) {
	if      ( d0 < -0.0025 ) pt_cor = (742.51/10000.0 * d0 - 2.3549/10000.0) * pt * pt;
        else if ( d0 <  0.0025 ) pt_cor = (1371.9/10000.0 * d0 - 0.6656/10000.0) * pt * pt;
        else                     pt_cor = (769.36/10000.0 * d0 + 0.4841/10000.0) * pt * pt;
      }
    } // end of if (year == 2016)
    else if (year == 2017) {
    }
    else if (year == 2018) {
    }
    return pt_cor;
  } // end of float PtGeo_Piece25_KaMu(float d0, float pt, float eta, int year)


  float PtGeo_Piece20_KaMu(float d0, float pt, float eta, int year) { // correction applied to KaMu
    float pt_cor = 0.0; 
    if (year == 2016) {
      if      (abs(eta) < 0.9) {
	if      ( d0 < -0.002 ) pt_cor = (268.98/10000.0 * d0 - 0.9040/10000.0) * pt * pt;
	else if ( d0 <  0.002 ) pt_cor = (751.98/10000.0 * d0 - 0.1836/10000.0) * pt * pt;
	else			pt_cor = (212.17/10000.0 * d0 + 0.6646/10000.0) * pt * pt;
      }
      else if (abs(eta) < 1.7) {
	if      ( d0 < -0.002 ) pt_cor = (431.12/10000.0 * d0 - 1.4477/10000.0) * pt * pt;
        else if ( d0 <  0.002 ) pt_cor = (1039.5/10000.0 * d0 - 0.3216/10000.0) * pt * pt;
        else                    pt_cor = (462.70/10000.0 * d0 + 0.4436/10000.0) * pt * pt;
      } 
      else if (abs(eta) < 2.4) {
	if      ( d0 < -0.002 ) pt_cor = (739.80/10000.0 * d0 - 2.3705/10000.0) * pt * pt;
        else if ( d0 <  0.002 ) pt_cor = (1319.6/10000.0 * d0 - 0.6655/10000.0) * pt * pt;
        else                    pt_cor = (730.74/10000.0 * d0 + 0.6876/10000.0) * pt * pt;
      }
    } // end of if (year == 2016)
    else if (year == 2017) {
    }
    else if (year == 2018) {
    }
    return pt_cor;
  } // end of float PtGeo_Piece20_KaMu(float d0, float pt, float eta, int year)


  float PtGeo_Alter30_KaMu(float d0, float pt, float eta, int year) { // correction applied to KaMu
    float pt_cor = 0.0; 
    if (year == 2016) {
      if      (abs(eta) < 0.9) {
	if      ( d0 < -0.003 ) pt_cor = (287.46/10000.0 * d0 - 0.6333/10000.0) * pt * pt;
	else if ( d0 <  0.003 ) pt_cor = (646.75/10000.0 * d0 ) * pt * pt;
	else			pt_cor = (260.19/10000.0 * d0 + 0.5747/10000.0) * pt * pt;
      }
      else if (abs(eta) < 1.7) {
	if      ( d0 < -0.003 ) pt_cor = (452.27/10000.0 * d0 - 0.9803/10000.0) * pt * pt;
        else if ( d0 <  0.003 ) pt_cor = (909.01/10000.0 * d0 ) * pt * pt;
        else                    pt_cor = (420.97/10000.0 * d0 + 1.0614/10000.0) * pt * pt;
      } 
      else if (abs(eta) < 2.4) {
	if      ( d0 < -0.003 ) pt_cor = (705.03/10000.0 * d0 - 1.9232/10000.0) * pt * pt;
        else if ( d0 <  0.003 ) pt_cor = (1337.2/10000.0 * d0 - 0.6652/10000.0) * pt * pt;
        else                    pt_cor = (956.72/10000.0 * d0 + 0.1136/10000.0) * pt * pt;
      }
    } // end of if (year == 2016)
    else if (year == 2017) {
    }
    else if (year == 2018) {
    }
    return pt_cor;
  } // end of float PtGeo_Piece30_KaMu(float d0, float pt, float eta, int year)




  float PtGeo_Alter25_KaMu(float d0, float pt, float eta, int year) { // correction applied to KaMu
    float pt_cor = 0.0; 
    if (year == 2016) {
      if      (abs(eta) < 0.9) {
	if      ( d0 < -0.0025 ) pt_cor = (286.13/10000.0 * d0 - 0.6311/10000.0) * pt * pt;
	else if ( d0 <  0.0025 ) pt_cor = (703.07/10000.0 * d0 ) * pt * pt;
	else		 	 pt_cor = (243.46/10000.0 * d0 + 0.6791/10000.0) * pt * pt;
      }
      else if (abs(eta) < 1.7) {
	if      ( d0 < -0.0025 ) pt_cor = (463.11/10000.0 * d0 - 0.9101/10000.0) * pt * pt;
        else if ( d0 <  0.0025 ) pt_cor = (960.24/10000.0 * d0 ) * pt * pt;
        else                     pt_cor = (451.44/10000.0 * d0 + 0.8802/10000.0) * pt * pt;
      } 
      else if (abs(eta) < 2.4) {
	if      ( d0 < -0.0025 ) pt_cor = (742.51/10000.0 * d0 - 1.6893/10000.0) * pt * pt;
        else if ( d0 <  0.0025 ) pt_cor = (1371.9/10000.0 * d0 ) * pt * pt;
        else                     pt_cor = (769.36/10000.0 * d0 + 1.1497/10000.0) * pt * pt;
      }
    } // end of if (year == 2016)
    else if (year == 2017) {
    }
    else if (year == 2018) {
    }
    return pt_cor;
  } // end of float PtGeo_Piece25_KaMu(float d0, float pt, float eta, int year)

  float PtGeo_Alter20_KaMu(float d0, float pt, float eta, int year) { // correction applied to KaMu
    float pt_cor = 0.0; 
    if (year == 2016) {
      if      (abs(eta) < 0.9) {
	if      ( d0 < -0.002 ) pt_cor = (268.98/10000.0 * d0 - 0.7204/10000.0) * pt * pt;
	else if ( d0 <  0.002 ) pt_cor = (751.98/10000.0 * d0 ) * pt * pt;
	else			pt_cor = (212.17/10000.0 * d0 + 0.8472/10000.0) * pt * pt;
      }
      else if (abs(eta) < 1.7) {
	if      ( d0 < -0.002 ) pt_cor = (431.12/10000.0 * d0 - 1.1261/10000.0) * pt * pt;
        else if ( d0 <  0.002 ) pt_cor = (1039.5/10000.0 * d0 ) * pt * pt;
        else                    pt_cor = (462.70/10000.0 * d0 + 0.7652/10000.0) * pt * pt;
      } 
      else if (abs(eta) < 2.4) {
	if      ( d0 < -0.002 ) pt_cor = (739.80/10000.0 * d0 - 1.7050/10000.0) * pt * pt;
        else if ( d0 <  0.002 ) pt_cor = (1319.6/10000.0 * d0 ) * pt * pt;
        else                    pt_cor = (730.74/10000.0 * d0 + 1.3531/10000.0) * pt * pt;
      }
    } // end of if (year == 2016)
    else if (year == 2017) {
    }
    else if (year == 2018) {
    }
    return pt_cor;
  } // end of float PtGeo_Piece20_KaMu(float d0, float pt, float eta, int year)



  float PtGeo_tanh_KaMu(float d0, float pt, float eta, int year) { // correction applied to KaMu
    float pt_cor = 0.0;
    if (year == 2016) {
      if      (abs(eta) < 0.9) pt_cor = ( 1.116 * tanh(679.40*d0) + 171.84 * d0 - 0.2034 ) * pt * pt / 10000.0;
      else if (abs(eta) < 1.7) pt_cor = ( 1.294 * tanh(646.23*d0) + 390.31 * d0 - 0.3462 ) * pt * pt / 10000.0;
      else 		       pt_cor = ( 0.851 * tanh(595.69*d0) + 925.38 * d0 - 0.7122 ) * pt * pt / 10000.0;
    }
    else if (year == 2017) {
    }
    else if (year == 2018) {  // for 2018 it is Roch, using it for now
      if      (abs(eta) < 0.9) pt_cor = ( 1.036 * tanh(660.86*d0) + 275.22 * d0 - 0.1305 ) * pt * pt / 10000.0;
      else if (abs(eta) < 1.7) pt_cor = ( 1.385 * tanh(520.61*d0) + 494.80 * d0 - 0.2535 ) * pt * pt / 10000.0;
      else                     pt_cor = ( 0.784 * tanh(772.87*d0) + 1122.9 * d0 - 0.4349 ) * pt * pt / 10000.0;
    }
    return pt_cor;
  } // end of float PtGeo_tanh_KaMu(float d0, float pt, float eta, int year)

  float PtGeo_tanhShift_KaMu(float d0, float pt, float eta, int year) { // correction applied to KaMu
    float pt_cor = 0.0;
    if (year == 2016) {
      if      (abs(eta) < 0.9) pt_cor = ( 1.116 * tanh(679.40*d0) + 171.84 * d0 ) * pt * pt / 10000.0;
      else if (abs(eta) < 1.7) pt_cor = ( 1.294 * tanh(646.23*d0) + 390.31 * d0 ) * pt * pt / 10000.0;
      else                     pt_cor = ( 0.851 * tanh(595.69*d0) + 925.38 * d0 ) * pt * pt / 10000.0;
    }
    else if (year == 2017) {
    }
    else if (year == 2018) {
      if      (abs(eta) < 0.9) pt_cor = ( 1.036 * tanh(660.86*d0) + 275.22 * d0 ) * pt * pt / 10000.0;
      else if (abs(eta) < 1.7) pt_cor = ( 1.385 * tanh(520.61*d0) + 494.80 * d0 ) * pt * pt / 10000.0;
      else                     pt_cor = ( 0.784 * tanh(772.87*d0) + 1122.9 * d0 ) * pt * pt / 10000.0;
    }
    return pt_cor;
  } // end of float PtGeo_tanhNoInt_KaMu(float d0, float pt, float eta, int year)


  float PtGeo_BS_Roch(float d0, float pt, float eta, int year) {
    float pt_cor = 0.0;
    if (year == 2016) {
      if      (abs(eta) < 0.9) pt_cor = 411.34 * d0 * pt * pt / 10000.0;
      else if (abs(eta) < 1.7) pt_cor = 673.40 * d0 * pt * pt / 10000.0;
      else		       pt_cor = 1099.0 * d0 * pt * pt / 10000.0;
    }
    else if (year == 2017) {
      if      (abs(eta) < 0.9) pt_cor = 582.32 * d0 * pt * pt / 10000.0;
      else if (abs(eta) < 1.7) pt_cor = 974.05 * d0 * pt * pt / 10000.0;
      else                     pt_cor = 1263.4 * d0 * pt * pt / 10000.0;
    }
    else if (year == 2018) {
      if      (abs(eta) < 0.9) pt_cor = 650.84 * d0 * pt * pt / 10000.0;
      else if (abs(eta) < 1.7) pt_cor = 988.37 * d0 * pt * pt / 10000.0;
      else                     pt_cor = 1484.6 * d0 * pt * pt / 10000.0;
    }
    return (pt - pt_cor);
  } // end of float PtGeo_BS_Roch(float d0, float pt, float eta, int year) 

} // end namespace PtGeoCor_UF
