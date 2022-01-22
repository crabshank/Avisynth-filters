#define rcptwoFiveFive 1.0/255.0
#define rcptHiBit 1.0/65535.0
#define rcpTwoFour 1.0/2.4
#define rcpOFiveFive 1.0/1.055
#define rcpTwelveNineTwo 1.0/12.92
#define recAlpha 1.09929682680944
#define rcpRecAlpha 1.0/1.09929682680944
#define recBeta 0.018053968510807
#define recBetaLin 0.004011993002402
#define rcpFourFive 1.0/4.5
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
#define third 1.0/3.0

const double D65XYZ[3]={0.95047,1,1.08883};
const double D65_x= 0.312727;
const double D65_y= 0.329023;

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

// A is m x p and B is p x n, so C is m x n
void mul( int rows_a, int cols_a, int cols_b,void* ma, void* mb, void* mres)
{

int i,j,k;

      double (*a)[cols_a] = (double (*)[cols_a]) ma;
      double (*b)[cols_b] = (double (*)[cols_b]) mb;
      double (*res)[cols_b] = (double (*)[cols_b]) mres;

      double *pta = &a[0][0];
      double *ptb = &b[0][0];
      double *ptr = &res[0][0];
  //  double*ptr, *ptr1, *ptr2;



  	// Multiplying matrix firstMatrix and secondMatrix and storing in array mult.
	for(i = 0; i < rows_a; i++)
	{
		for(j = 0; j < cols_b; j++)
		{
		    *(ptr+(i*cols_b+j))=0;
			for(k=0; k<cols_a; k++)
			{
				*(ptr+(i*cols_b+j)) += *(pta+(i*cols_a+k))**(ptb+(k*cols_b+j));

			}

		}

	}

}

void Linearise(double rgbGam[3], double rgbLin[3],int mode, int aprxPw){
if(aprxPw==1){
    if ((mode==0)||(mode==6)){ //sRGB transfer
          rgbLin[0]=(rgbGam[0] > 0.0404482362771082 )?fastPrecisePow(fabs((rgbGam[0]+0.055)*rcpOFiveFive),2.4):rgbGam[0]*rcpTwelveNineTwo;
          rgbLin[1]=(rgbGam[1] > 0.0404482362771082 )?fastPrecisePow(fabs((rgbGam[1]+0.055)*rcpOFiveFive),2.4):rgbGam[1]*rcpTwelveNineTwo;
          rgbLin[2]=(rgbGam[2] > 0.0404482362771082 )?fastPrecisePow(fabs((rgbGam[2]+0.055)*rcpOFiveFive),2.4):rgbGam[2]*rcpTwelveNineTwo;
    }else if ((mode==5)||(mode==10)){ //DCI-P3
          rgbLin[0]=fastPrecisePow(rgbGam[0],2.6);
          rgbLin[1]=fastPrecisePow(rgbGam[1],2.6);
          rgbLin[2]=fastPrecisePow(rgbGam[2],2.6);
    }else if (mode==7){ //Original NTSC - Source: 47 CFR, Section 73.682 - TV transmission standards
          rgbLin[0]=fastPrecisePow(rgbGam[0],2.2);
          rgbLin[1]=fastPrecisePow(rgbGam[1],2.2);
          rgbLin[2]=fastPrecisePow(rgbGam[2],2.2);
    }else if(mode==11){ //Rec. 2100 HLG
          rgbLin[0]=(rgbLin[0]>0.5)?rcpTwelve*(fastPrecisePow(euler_e,(rgbLin[0]-HLG_c)*rcp_HLG_a)+HLG_b):rgbLin[0]*rgbLin[0]*third;
          rgbLin[1]=(rgbLin[1]>0.5)?rcpTwelve*(fastPrecisePow(euler_e,(rgbLin[1]-HLG_c)*rcp_HLG_a)+HLG_b):rgbLin[1]*rgbLin[1]*third;
          rgbLin[2]=(rgbLin[2]>0.5)?rcpTwelve*(fastPrecisePow(euler_e,(rgbLin[2]-HLG_c)*rcp_HLG_a)+HLG_b):rgbLin[2]*rgbLin[2]*third;
    }else{ //Rec transfer
          rgbLin[0]=(rgbGam[0] < recBetaLin )?rcpFourFive*rgbGam[0]:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-rgbGam[0])),rcpTxFourFive);
          rgbLin[1]=(rgbGam[1] < recBetaLin )?rcpFourFive*rgbGam[1]:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-rgbGam[1])),rcpTxFourFive);
          rgbLin[2]=(rgbGam[1] < recBetaLin )?rcpFourFive*rgbGam[2]:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-rgbGam[2])),rcpTxFourFive);
    }
}else{
	    if ((mode==0)||(mode==6)){ //sRGB transfer
          rgbLin[0]=(rgbGam[0] > 0.0404482362771082 )?pow(fabs((rgbGam[0]+0.055)*rcpOFiveFive),2.4):rgbGam[0]*rcpTwelveNineTwo;
          rgbLin[1]=(rgbGam[1] > 0.0404482362771082 )?pow(fabs((rgbGam[1]+0.055)*rcpOFiveFive),2.4):rgbGam[1]*rcpTwelveNineTwo;
          rgbLin[2]=(rgbGam[2] > 0.0404482362771082 )?pow(fabs((rgbGam[2]+0.055)*rcpOFiveFive),2.4):rgbGam[2]*rcpTwelveNineTwo;
    }else if ((mode==5)||(mode==10)){ //DCI-P3
          rgbLin[0]=pow(rgbGam[0],2.6);
          rgbLin[1]=pow(rgbGam[1],2.6);
          rgbLin[2]=pow(rgbGam[2],2.6);
    }else if (mode==7){ //Original NTSC - Source: 47 CFR, Section 73.682 - TV transmission standards
          rgbLin[0]=pow(rgbGam[0],2.2);
          rgbLin[1]=pow(rgbGam[1],2.2);
          rgbLin[2]=pow(rgbGam[2],2.2);
    }else if(mode==11){ //Rec. 2100 HLG
          rgbLin[0]=(rgbLin[0]>0.5)?rcpTwelve*(pow(euler_e,(rgbLin[0]-HLG_c)*rcp_HLG_a)+HLG_b):rgbLin[0]*rgbLin[0]*third;
          rgbLin[1]=(rgbLin[1]>0.5)?rcpTwelve*(pow(euler_e,(rgbLin[1]-HLG_c)*rcp_HLG_a)+HLG_b):rgbLin[1]*rgbLin[1]*third;
          rgbLin[2]=(rgbLin[2]>0.5)?rcpTwelve*(pow(euler_e,(rgbLin[2]-HLG_c)*rcp_HLG_a)+HLG_b):rgbLin[2]*rgbLin[2]*third;
    }else{ //Rec transfer
          rgbLin[0]=(rgbGam[0] < recBetaLin )?rcpFourFive*rgbGam[0]:pow(-1*(rcpRecAlpha*(1-recAlpha-rgbGam[0])),rcpTxFourFive);
          rgbLin[1]=(rgbGam[1] < recBetaLin )?rcpFourFive*rgbGam[1]:pow(-1*(rcpRecAlpha*(1-recAlpha-rgbGam[1])),rcpTxFourFive);
          rgbLin[2]=(rgbGam[1] < recBetaLin )?rcpFourFive*rgbGam[2]:pow(-1*(rcpRecAlpha*(1-recAlpha-rgbGam[2])),rcpTxFourFive);
    }
}
}

void Apply_gamma(double rgbLin[3], double rgbGam[3], int mode, int aprxPw){

   double r=rgbLin[0];
   double g=rgbLin[1];
   double b=rgbLin[2];
if(aprxPw==1){
    if ((mode==0)||(mode==6)){ //sRGB transfer
        rgbGam[0]=(r> 0.00313066844250063)?1.055 * fastPrecisePow(r,rcpTwoFour) - 0.055:12.92 *r;
        rgbGam[1]=(g> 0.00313066844250063)?1.055 * fastPrecisePow(g,rcpTwoFour) - 0.055:12.92 *g;
        rgbGam[2]=(b> 0.00313066844250063)?1.055 * fastPrecisePow(b,rcpTwoFour) - 0.055:12.92 *b;
    }else if ((mode==5)||(mode==10)){ //DCI-P3
        rgbGam[0]=fastPrecisePow(r,invTwoSix);
        rgbGam[1]=fastPrecisePow(g,invTwoSix);
        rgbGam[2]=fastPrecisePow(b,invTwoSix);
    }else if (mode==7){ //Original NTSC
        rgbGam[0]=fastPrecisePow(r,invTwoTwo);
        rgbGam[1]=fastPrecisePow(g,invTwoTwo);
        rgbGam[2]=fastPrecisePow(b,invTwoTwo);
    }else if (mode==11){ //Rec. 2100 HLG
        rgbGam[0]=(rgbGam[0] > rcpTwelve)?HLG_a*log(12.0*rgbGam[0]-HLG_b)+HLG_c:root_three*fastPrecisePow(rgbGam[0],0.5);
        rgbGam[1]=(rgbGam[1] > rcpTwelve)?HLG_a*log(12.0*rgbGam[1]-HLG_b)+HLG_c:root_three*fastPrecisePow(rgbGam[1],0.5);
        rgbGam[2]=(rgbGam[2] > rcpTwelve)?HLG_a*log(12.0*rgbGam[2]-HLG_b)+HLG_c:root_three*fastPrecisePow(rgbGam[2],0.5);
    }else{ //Rec transfer
        rgbGam[0]=(r< recBeta)?4.5*r:recAlpha*fastPrecisePow(r,0.45)-(recAlpha-1);
        rgbGam[1]=(g< recBeta)?4.5*g:recAlpha*fastPrecisePow(g,0.45)-(recAlpha-1);
        rgbGam[2]=(b< recBeta)?4.5*b:recAlpha*fastPrecisePow(b,0.45)-(recAlpha-1);
    }
}else{
	    if ((mode==0)||(mode==6)){ //sRGB transfer
        rgbGam[0]=(r> 0.00313066844250063)?1.055 * pow(r,rcpTwoFour) - 0.055:12.92 *r;
        rgbGam[1]=(g> 0.00313066844250063)?1.055 * pow(g,rcpTwoFour) - 0.055:12.92 *g;
        rgbGam[2]=(b> 0.00313066844250063)?1.055 * pow(b,rcpTwoFour) - 0.055:12.92 *b;
    }else if ((mode==5)||(mode==10)){ //DCI-P3
        rgbGam[0]=pow(r,invTwoSix);
        rgbGam[1]=pow(g,invTwoSix);
        rgbGam[2]=pow(b,invTwoSix);
    }else if (mode==7){ //Original NTSC
        rgbGam[0]=pow(r,invTwoTwo);
        rgbGam[1]=pow(g,invTwoTwo);
        rgbGam[2]=pow(b,invTwoTwo);
    }else if (mode==11){ //Rec. 2100 HLG
        rgbGam[0]=(rgbGam[0] > rcpTwelve)?HLG_a*log(12.0*rgbGam[0]-HLG_b)+HLG_c:root_three*pow(rgbGam[0],0.5);
        rgbGam[1]=(rgbGam[1] > rcpTwelve)?HLG_a*log(12.0*rgbGam[1]-HLG_b)+HLG_c:root_three*pow(rgbGam[1],0.5);
        rgbGam[2]=(rgbGam[2] > rcpTwelve)?HLG_a*log(12.0*rgbGam[2]-HLG_b)+HLG_c:root_three*pow(rgbGam[2],0.5);
    }else{ //Rec transfer
        rgbGam[0]=(r< recBeta)?4.5*r:recAlpha*pow(r,0.45)-(recAlpha-1);
        rgbGam[1]=(g< recBeta)?4.5*g:recAlpha*pow(g,0.45)-(recAlpha-1);
        rgbGam[2]=(b< recBeta)?4.5*b:recAlpha*pow(b,0.45)-(recAlpha-1);
    }
}
}

void WPconv_func(double XYZ[3],double from[3], double to[3],double outp[3])
{

 double Bradford[3][3]={
{0.8951,0.2664,-0.1614},
{-0.7502,1.7135,0.0367},
{0.0389,-0.0685,1.0296}};

 double BradfordInv[3][3]={
{0.9869929,-0.1470543,0.1599627},
{0.4323053,0.5183603,0.0492912},
{-0.0085287,0.0400428,0.9684867}};


double BradFrom[3];

mul(3,3,1,Bradford,from,BradFrom);

double BradTo[3];

mul(3,3,1,Bradford,to,BradTo);


double CR[3][3]={{0,0,0},{0,0,0},{0,0,0}};

CR[0][0]=BradTo[0]/BradFrom[0];
CR[1][1]=BradTo[1]/BradFrom[1];
CR[2][2]=BradTo[2]/BradFrom[2];

double convBrad2[3][3]={{0,0,0},{0,0,0},{0,0,0}};
double convBrad[3][3]={{0,0,0},{0,0,0},{0,0,0}};

mul(3,3,3,BradfordInv,CR,convBrad2);
mul(3,3,3,convBrad2,Bradford,convBrad);


mul(3,3,1,convBrad,XYZ,outp);

//mul(1,3,3,XYZ,Bradford,outp); //WORKS (double3 X 3x3 )!!!!

//mul(3,3,1,Bradford,XYZ,outp); //WORKS (3x3 X double3)!!!

}

void WPconv(double XYZ[3],double from[3], double to[3],double outp[3])
{
	WPconv_func(XYZ, from, to, outp);
}

void WPconv2Grey(double from[3], double to[3],double outp[3])
{
	WPconv_func(D65XYZ, from, to, outp); //D65
}

void LinRGB2XYZ(double rgbLin[3], double XYZ_out[3], int mode)
{
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

XYZ_out[0]=v1[0]*rgbLin[0]+v1[1]*rgbLin[1]+v1[2]*rgbLin[2];
XYZ_out[1]=v2[0]*rgbLin[0]+v2[1]*rgbLin[1]+v2[2]*rgbLin[2];
XYZ_out[2]=v3[0]*rgbLin[0]+v3[1]*rgbLin[1]+v3[2]*rgbLin[2];

}

inline double LinRGB2Y(double rgbLin[3], int mode)
{
double v2[3];

if (mode==1){ //Rec 601 NTSC
    v2[0]=0.2124132;
    v2[1]=0.7010437;
    v2[2]=0.0865432;
}else if (mode==2){ //Rec 601 PAL
    v2[0]=0.2220379;
    v2[1]=0.7066384;
    v2[2]=0.0713236;
}else if(mode==5){ //DCI-P3
    v2[0]=0.209491677912731;
    v2[1]=0.721595254161044;
    v2[2]=0.068913067926226;
}else if(mode==6){
    v2[0]=0.2290036;
    v2[1]=0.691726725;
    v2[2]=0.079269675;
}else if ((mode==4)||(mode==11)){ //Rec 2020/2100
    v2[0]=0.26272171736164;
    v2[1]=0.677989275502262;
    v2[2]=0.059289007136098;
}else if (mode==7){ //Original NTSC
    v2[0]=0.2989666;
    v2[1]=0.5864212;
    v2[2]=0.1146122;
}else if (mode==8){ //Rec 601 D93
    v2[0]=0.1767506;
    v2[1]=0.7072321;
    v2[2]=0.1160173;
}else if (mode==9){ //Rec 709 D93;
    v2[0]=0.1799632;
    v2[1]=0.7231169;
    v2[2]=0.0969199;
}else if (mode==10){ //DCI-P3 D60/ACES
    v2[0]=0.23762331020788;
    v2[1]=0.689170669198985;
    v2[2]=0.073206020593136;
}else{ //sRGB - Rec 709
    v2[0]=0.2126729;
    v2[1]=0.7151522;
    v2[2]=0.072175;
}

return v2[0]*rgbLin[0]+v2[1]*rgbLin[1]+v2[2]*rgbLin[2];

}

void XYZ2LinRGB(double XYZ[3],double rgb_out[3], int mode)
{
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

    rgb_out[0]=v1[0]*XYZ[0]+v1[1]*XYZ[1]+v1[2]*XYZ[2];
    rgb_out[1]=v2[0]*XYZ[0]+v2[1]*XYZ[1]+v2[2]*XYZ[2];
    rgb_out[2]=v3[0]*XYZ[0]+v3[1]*XYZ[1]+v3[2]*XYZ[2];

}

void LinRGB2XYZ_grey(double lin_rgb[3], double XYZ_out[3],int mode)
{
	double avg=(lin_rgb[0]+lin_rgb[1]+lin_rgb[2])/3.0;
	double rgb_avg[3]={avg,avg,avg};
	LinRGB2XYZ(rgb_avg, XYZ_out, mode);
}

void XYZ2xyY(double XYZ[3], double xyY_out[3])
{
	double tot=XYZ[0]+XYZ[1]+XYZ[2];
	//Avoid putting double3(0,0,0) as XYZ!

	xyY_out[0]=XYZ[0]/tot;
	xyY_out[1]=XYZ[1]/tot;
	xyY_out[2]=XYZ[1];
}

void xyY2XYZ(double xyY[3], double XYZ_out[3])
{
       XYZ_out[0]=(1.0/xyY[1])*xyY[0]*xyY[2];
	   XYZ_out[1]=xyY[2];
	   XYZ_out[2]=(1.0/xyY[1])*(1-xyY[0]-xyY[1])*(xyY[2]);
}

void LinRGB2xyY(double rgb_lin[3], double xyY_out[3],int mode)
{
	double rgb_lin_adj[3]={rgb_lin[0],rgb_lin[1],rgb_lin[2]};

	int rgb_is_blk=(rgb_lin[0]==0 && rgb_lin[1]==0 && rgb_lin[2]==0)?1:0;

	if(rgb_is_blk==1){
		rgb_lin_adj[0]=1;
		rgb_lin_adj[1]=1;
		rgb_lin_adj[2]=1;
	}
	double XYZ[3];
	LinRGB2XYZ(rgb_lin_adj, XYZ, mode);
	XYZ2xyY(XYZ, xyY_out);
	xyY_out[2]=(rgb_is_blk==1)?0:xyY_out[2];
}

void rgb2XYZ(double rgb[3], double XYZ_out[3], int mode, int aprxPw)
{
	double rgbLin[3];
	Linearise(rgb, rgbLin, mode, aprxPw);
	LinRGB2XYZ(rgbLin, XYZ_out, mode);
}

void rgb2xyY(double rgb[3], double xyY_out[3], int mode, int aprxPw)
{
	double rgb_lin[3];
	Linearise(rgb, rgb_lin, mode, aprxPw);
	LinRGB2xyY(rgb_lin, xyY_out, mode);
}

void xyY2LinRGB(double xyY[3], double lin_rgb_out[3], int mode)
{
	double XYZ[3];
	xyY2XYZ(xyY, XYZ);
	XYZ2LinRGB(XYZ, lin_rgb_out,mode);
}

void xyY2rgb(double xyY[3], double rgb_out[3], int mode, int aprxPw)
{
	double lin_rgb[3];
	xyY2LinRGB(xyY, lin_rgb, mode);
	Apply_gamma(lin_rgb, rgb_out, mode, aprxPw);
}

void XYZ2rgb(double XYZ[3], double rgb_out[3], int mode, int aprxPw)
{
	double rgb_lin[3];
	XYZ2LinRGB(XYZ, rgb_lin, mode);
	Apply_gamma(rgb_lin, rgb_out, mode, aprxPw);
}

void rgb2XYZ_grey(double rgb[3], double XYZ_out[3], int mode, int aprxPw)
{
	double lin_rgb[3];
	Linearise(rgb, lin_rgb, mode, aprxPw);
	LinRGB2XYZ_grey(lin_rgb, XYZ_out, mode);
}

inline double rgb2Y(double rgb[3], int mode, int aprxPw)
{
	double lin_rgb[3];
	Linearise(rgb, lin_rgb, mode, aprxPw);
	return LinRGB2Y(lin_rgb, mode);
}

void xy2XYZ(double xyCoord[2], double XYZ_out[3])
{
	XYZ_out[0]=(1/xyCoord[1])*xyCoord[0];
	XYZ_out[1]=1;
	XYZ_out[2]=(1/xyCoord[1])*(1-xyCoord[0]-xyCoord[1]);
}

void get_xy(double rgb[3],double xy_out[2], int mode, int linr, int aprxPw)
{
	double XYZ1[3];
	double XYZ2[3];

	if(linr==0){
		rgb2XYZ(rgb,XYZ1,mode,aprxPw);
		rgb2XYZ_grey(rgb,XYZ2,mode,aprxPw);
	}else{
		LinRGB2XYZ(rgb,XYZ1,mode);
		LinRGB2XYZ_grey(rgb,XYZ2,mode);
	}

	double XYZ3[3];
	WPconv2Grey(XYZ1,XYZ2,XYZ3);

	double xyY[3];
	XYZ2xyY(XYZ3,xyY);

	xy_out[0]=xyY[0];
	xy_out[1]=xyY[1];
}

//Source: https://stackoverflow.com/a/45263428; http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.htm; https://en.wikipedia.org/wiki/Rec._2020#Transfer_characteristics
