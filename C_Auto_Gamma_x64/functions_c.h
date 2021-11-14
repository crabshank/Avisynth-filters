#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define lerp(a,b,t) ((1 - (t)) * (a) + (t) * (b) )
#define rcptwoFiveFive 1.0/255.0
#define rcpTwoFour 1.0/2.4
#define rcptHiBit 1.0/65535.0
#define third 1.0/3.0
#define rcpOFiveFive 1.0/1.055
#define rcpTwelveNineTwo 1.0/12.92
#define rcpFourFive 1.0/4.5
#define recAlpha 1.09929682680944
#define rcpRecAlpha 1.0/1.09929682680944
#define recBeta 0.018053968510807
#define recBetaLin 0.004011993002402
#define rcpTxFourFive 10.0/4.5
#define invTwoTwo 5.0/11.0
#define invTwoSix 5.0/13.0
#define root_three 1.732050807568877
#define rcpTwelve 1.0/12.0
#define HLG_a 0.17883277
#define rcp_HLG_a 1.0/0.17883277
#define HLG_b 0.28466892
#define HLG_c 0.55991073
#define euler_e 2.718281828459045

inline double fastPrecisePow(double a, double b) {
	//added support for negative exponents
	int neg_b=0;

	if(b==0){
		return 1.0;
	}else if(b<0){
		neg_b=1;
		b=fabs(b);
	}
  // calculate approximation with fraction of the exponent
  int e = (int) b;
  union {
    double d;
    int x[2];
  } u = { a };
  u.x[1] = (int)((b - e) * (u.x[1] - 1072632447) + 1072632447);
  u.x[0] = 0;

  // exponentiation by squaring with the exponent's integer part
  // double r = u.d makes everything much slower, not sure why
  double r = 1.0;
  while (e) {
    if (e & 1) {
      r *= a;
    }
    a *= a;
    e >>= 1;
  }

	double res= r * u.d;

  if(neg_b==0){
	  return res;
  }else{
	   return 1.0/res;
  }

}
//Source: https://martin.ankerl.com/2012/01/25/optimized-approximative-fastPrecisePow-in-c-and-cpp/



void f_gammaLow(double rgb[3], double gamma,double gamma_hi, double dist){
    double outcol[3];
double avg=(rgb[0]+rgb[1]+rgb[2])*third;
    for (int i=0; i<3; i++){
    double col=rgb[i];
    double ln=(col-(1-col)*fastPrecisePow(col,gamma));
gamma_hi=(ln<=0 || (col==0))?gamma:log(ln)/log(col);
outcol[i]=1-fabs((fastPrecisePow(-col,gamma+1)+fastPrecisePow(col,gamma_hi+1)+fastPrecisePow(col,gamma))-rgb[i]);


    }


 dist=(avg-outcol[0])+(avg-outcol[1])+(outcol[2]-avg);
}
