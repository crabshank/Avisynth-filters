#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define lerp(a,b,t) ((1 - (t)) * (a) + (t) * (b) )
#define rcptwoFiveFive 1.0/255.0

inline double fastPrecisePow(double a, double b) {
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

  return r * u.d;
}
//Source: https://martin.ankerl.com/2012/01/25/optimized-approximative-fastPrecisePow-in-c-and-cpp/


void f_gammaLow(double rgb[3], double gamma,double gamma_hi, double dist){
    double outcol[3];

    for (int i=0; i<3; i++){
    double col=rgb[i];
    double ln=(col-(1-col)*fastPrecisePow(col,gamma));
gamma_hi=(ln<=0 || (col==0))?gamma:log(ln)/log(col);
outcol[i]=1-fabs((fastPrecisePow(-col,gamma+1)+fastPrecisePow(col,gamma_hi+1)+fastPrecisePow(col,gamma))-rgb[i]);

    }

    	double shift = 1 - MIN(outcol[0], MIN(outcol[1], outcol[2])) - MAX(outcol[0], MAX(outcol[1], outcol[2]));

		outcol[0] = shift + outcol[0];
		outcol[1] = shift + outcol[1];
		outcol[2] = shift + outcol[2];

		double tot=outcol[0]+outcol[1]+outcol[2];
		double totOG=rgb[0]+rgb[1]+rgb[2];
 dist=tot-totOG;

       double  mxOG=MAX(rgb[0],MAX(rgb[1],rgb[2]));
       double  mx=MAX(outcol[0],MAX(outcol[1],outcol[2]));

       if(mx>mxOG){
            double diff=mx-mxOG;
 dist=(dist<0)?dist-diff:dist+diff;
       }

}
