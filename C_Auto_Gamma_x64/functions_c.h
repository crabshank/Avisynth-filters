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

void xyY2XYZ(double xyY[3], double XYZ[3]){
    XYZ[0]= (1.0/xyY[1])*xyY[0]*xyY[2];
    XYZ[1]=xyY[2];
    XYZ[2]=(1.0/xyY[1])*(1-xyY[0]-xyY[1])*(xyY[2]);
}

void LinRGB2XYZ (double rgb[3], double XYZog[3],double XYZnew[3], int mode, int grey){

	  double rgbLin[3];

      rgbLin[0]=rgb[0];
      rgbLin[1]=rgb[1];
      rgbLin[2]=rgb[2];

double v1[3];
double v2[3];
double v3[3];

if (mode==1){ //Rec 601 NTSC
    v1[0]=0.3935891;
    v1[1]=0.3652497;
    v1[2]=0.1916313;
    v2[0]=0.2124132;
    v2[1]=0.7010437;
    v2[2]=0.0865432;
    v3[0]=0.0187423;
    v3[1]=0.1119313;
    v3[2]=0.9581563;
}else if (mode==2){ //Rec 601 PAL
    v1[0]=0.430619;
    v1[1]=0.3415419;
    v1[2]=0.1783091;
    v2[0]=0.2220379;
    v2[1]=0.7066384;
    v2[2]=0.0713236;
    v3[0]=0.0201853;
    v3[1]=0.1295504;
    v3[2]=0.9390944;
}else if(mode==5){ //DCI-P3
    v1[0]=0.445169815564552;
    v1[1]=0.277134409206778;
    v1[2]=0.172282669815565;
    v2[0]=0.209491677912731;
    v2[1]=0.721595254161044;
    v2[2]=0.068913067926226;
    v3[0]=0;
    v3[1]=0.047060560053981;
    v3[2]=0.907355394361973;
}else if(mode==6){
    v1[0]=0.48663265;
    v1[1]=0.2656631625;
    v1[2]=0.1981741875;
    v2[0]=0.2290036;
    v2[1]=0.691726725;
    v2[2]=0.079269675;
    v3[0]=0;
    v3[1]=0.0451126125;
    v3[2]=1.0437173875;
}else if ((mode==4)||(mode==11)){ //Rec 2020/2100
    v1[0]=0.637010191411101;
    v1[1]=0.144615027396969;
    v1[2]=0.16884478119193;
    v2[0]=0.26272171736164;
    v2[1]=0.677989275502262;
    v2[2]=0.059289007136098;
    v3[0]=0;
    v3[1]=0.028072328847647;
    v3[2]=1.06075767115235;
}else if (mode==7){ //Original NTSC
    v1[0]=0.6069928;
    v1[1]=0.1734485;
    v1[2]=0.2005713;
    v2[0]=0.2989666;
    v2[1]=0.5864212;
    v2[2]=0.1146122;
    v3[0]=0;
    v3[1]=0.0660756;
    v3[2]=1.1174687;
}else if (mode==8){ //Rec 601 D93
    v1[0]=0.3275085;
    v1[1]=0.3684739;
    v1[2]=0.2568954;
    v2[0]=0.1767506;
    v2[1]=0.7072321;
    v2[2]=0.1160173;
    v3[0]=0.0155956;
    v3[1]=0.1129194;
    v3[2]=1.2844772;
}else if (mode==9){ //Rec 709 D93
    v1[0]=0.3490195;
    v1[1]=0.3615584;
    v1[2]=0.2422998;
    v2[0]=0.1799632;
    v2[1]=0.7231169;
    v2[2]=0.0969199;
    v3[0]=0.0163603;
    v3[1]=0.1205195;
    v3[2]=1.2761125;
}else if (mode==10){ //DCI-P3 D60/ACES
    v1[0]=0.504949534191744;
    v1[1]=0.264681488895262;
    v1[2]=0.18301505148284;
    v2[0]=0.23762331020788;
    v2[1]=0.689170669198985;
    v2[2]=0.073206020593136;
    v3[0]=0;
    v3[1]=0.04494591320863;
    v3[2]=0.963879271142956;
}else{ //sRGB - Rec 709
    v1[0]=0.4124564;
    v1[1]=0.3575761;
    v1[2]=0.1804375;
    v2[0]=0.2126729;
    v2[1]=0.7151522;
    v2[2]=0.072175;
    v3[0]=0.0193339;
    v3[1]=0.119192;
    v3[2]=0.9503041;
}


XYZog[0]=v1[0]*rgbLin[0]+v1[1]*rgbLin[1]+v1[2]*rgbLin[2];
XYZog[1]=v2[0]*rgbLin[0]+v2[1]*rgbLin[1]+v2[2]*rgbLin[2];
XYZog[2]=v3[0]*rgbLin[0]+v3[1]*rgbLin[1]+v3[2]*rgbLin[2];

    if (grey==1){
    double rgbNewTot=rgbLin[0]+rgbLin[1]+rgbLin[2];
    double rgbNewAvg=rgbNewTot/3;
    double rgbNew[3]={rgbNewAvg,rgbNewAvg,rgbNewAvg};

    XYZnew[0]=v1[0]*rgbNew[0]+v1[1]*rgbNew[1]+v1[2]*rgbNew[2];
    XYZnew[1]=v2[0]*rgbNew[0]+v2[1]*rgbNew[1]+v2[2]*rgbNew[2];
    XYZnew[2]=v3[0]*rgbNew[0]+v3[1]*rgbNew[1]+v3[2]*rgbNew[2];
    }

}

void XYZ2xyY(double XYZ[3],double outp[3]){
	double XYZtot=XYZ[0]+XYZ[1]+XYZ[2];

	double x=XYZ[0]/XYZtot;
	double y=XYZ[1]/XYZtot;

    outp[0]=x;
	outp[1]=y;
	outp[2]=XYZ[1];
}

void XYZ2LinRGB(double XYZ[3],double RGB[3], int mode){

double v1[3];
double v2[3];
double v3[3];

if (mode==1){ //Rec 601 NTSC
    v1[0]=3.505396;
    v1[1]=-1.7394894;
    v1[2]=-0.543964;
    v2[0]=-1.0690722;
    v2[1]=1.9778245;
    v2[2]=0.0351722;
    v3[0]=0.05632;
    v3[1]=-0.1970226;
    v3[2]=1.0502026;
}else if (mode==2){ //Rec 601 PAL
    v1[0]=3.0628971;
    v1[1]=-1.3931791;
    v1[2]=-0.4757517;
    v2[0]=-0.969266;
    v2[1]=1.8760108;
    v2[2]=0.041556;
    v3[0]=0.0678775;
    v3[1]=-0.2288548;
    v3[2]=1.069349;
}else if(mode==5){ //DCI-P3
    v1[0]=2.72539403049173;
    v1[1]=-1.01800300622718;
    v1[2]=-0.440163195190036;
    v2[0]=-0.795168025808764;
    v2[1]=1.68973205484362;
    v2[2]=0.022647190608478;
    v3[0]=0.0412418913957;
    v3[1]=-0.087639019215862;
    v3[2]=1.10092937864632;
}else if(mode==6){
    v1[0]=2.49318075532897;
    v1[1]=-0.93126552549714;
    v1[2]=-0.402659723758882;
    v2[0]=-0.829503115821079;
    v2[1]=1.76269412111979;
    v2[2]=0.02362508874174;
    v3[0]=0.035853625780072;
    v3[1]=-0.076188954782652;
    v3[2]=0.957092621518022;
}else if ((mode==4)||(mode==11)){ //Rec 2020/2100
    v1[0]=1.71651066976197;
    v1[1]=-0.355641669986716;
    v1[2]=-0.253345541821907;
    v2[0]=-0.666693001182624;
    v2[1]=1.61650220834691;
    v2[2]=0.015768750389995;
    v3[0]=0.017643638767459;
    v3[1]=-0.042779781669045;
    v3[2]=0.942305072720018;
}else if (mode==7){ //Original NTSC
    v1[0]=1.9096754;
    v1[1]=-0.5323648;
    v1[2]=-0.2881607;
    v2[0]=-0.9849649;
    v2[1]=1.9997772;
    v2[2]=-0.0283168;
    v3[0]=0.0582407;
    v3[1]=-0.1182463;
    v3[2]=0.896554;
}else if (mode==8){ //Rec 601 D93
    v1[0]=4.2126707;
    v1[1]=-2.0904617;
    v1[2]=-0.6537183;
    v2[0]=-1.0597177;
    v2[1]=1.9605182;
    v2[2]=0.0348645;
    v3[0]=0.0420119;
    v3[1]=-0.1469691;
    v3[2]=0.7833991;
}else if (mode==9){ //Rec 709 D93
    v1[0]=3.8294307;
    v1[1]=-1.8165248;
    v1[2]=-0.5891432;
    v2[0]=-0.9585901;
    v2[1]=1.8553477;
    v2[2]=0.0410983;
    v3[0]=0.0414369;
    v3[1]=-0.1519354;
    v3[2]=0.7873016;
}else if (mode==10){ //DCI-P3 D60/ACES
    v1[0]=2.40274141422225;
    v1[1]=-0.897484163940685;
    v1[2]=-0.388053369996071;
    v2[0]=-0.832579648740884;
    v2[1]=1.76923175357438;
    v2[2]=0.023712711514772;
    v3[0]=0.038823381466857;
    v3[1]=-0.082499685617071;
    v3[2]=1.03636859971248;
}else{ //sRGB - Rec 709
    v1[0]=3.2404542;
    v1[1]=-1.5371385;
    v1[2]=-0.4985314;
    v2[0]=-0.969266;
    v2[1]=1.8760108;
    v2[2]=0.041556;
    v3[0]=0.0556434;
    v3[1]=-0.2040259;
    v3[2]=1.0572252;
}

    double rgbLin[3];
    rgbLin[0]=v1[0]*XYZ[0]+v1[1]*XYZ[1]+v1[2]*XYZ[2];
    rgbLin[1]=v2[0]*XYZ[0]+v2[1]*XYZ[1]+v2[2]*XYZ[2];
    rgbLin[2]=v3[0]*XYZ[0]+v3[1]*XYZ[1]+v3[2]*XYZ[2];

        RGB[0]=rgbLin[0];
        RGB[1]=rgbLin[1];
        RGB[2]=rgbLin[2];


}

//Source: https://stackoverflow.com/a/45263428; http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.htm; https://en.wikipedia.org/wiki/Rec._2020#Transfer_characteristics

void f_gammaLow(double rgb[3], double gamma,double gamma_hi, double dist){
    double outcol[3];
    double avg=(rgb[0]+rgb[1]+rgb[2])*third;

    for (int i=0; i<3; i++){
        double col=rgb[i];
        double ln=(col-(1-col)*fastPrecisePow(col,gamma));
        gamma_hi=(ln<=0 || (col==0))?gamma:log(ln)/log(col);
        outcol[i]=1-fabs((fastPrecisePow(-col,gamma+1)+fastPrecisePow(col,gamma_hi+1)+fastPrecisePow(col,gamma))-col);
    }

 dist=(avg-outcol[0])+(avg-outcol[1])+(outcol[2]-avg);
}
