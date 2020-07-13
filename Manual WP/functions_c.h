#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
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

double D65XYZ[3]={0.95047,1,1.08883};

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




void WPconv(double XYZ[3],double from[3], double to[3],double outp[3]){

 //double *ptp = &outp[0];

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

void rgb2XYZ (double rgb[3], double XYZog[3],double XYZnew[3], int mode, int grey){

	  double rgbLin[3];

if ((mode==0)||(mode==6)){ //sRGB transfer
      rgbLin[0]=(rgb[0] > 0.0404482362771082 )?fastPrecisePow(fabs((rgb[0]+0.055)*rcpOFiveFive),2.4):rgb[0]*rcpTwelveNineTwo;
      rgbLin[1]=(rgb[1] > 0.0404482362771082 )?fastPrecisePow(fabs((rgb[1]+0.055)*rcpOFiveFive),2.4):rgb[1]*rcpTwelveNineTwo;
      rgbLin[2]=(rgb[2] > 0.0404482362771082 )?fastPrecisePow(fabs((rgb[2]+0.055)*rcpOFiveFive),2.4):rgb[2]*rcpTwelveNineTwo;
}else if (mode==5){ //DCI-P3
      rgbLin[0]=fastPrecisePow(rgb[0],2.6);
      rgbLin[1]=fastPrecisePow(rgb[1],2.6);
      rgbLin[2]=fastPrecisePow(rgb[2],2.6);
}else if (mode==7){ //Original NTSC - Source: 47 CFR, Section 73.682 - TV transmission standards
      rgbLin[0]=fastPrecisePow(rgb[0],2.2);
      rgbLin[1]=fastPrecisePow(rgb[1],2.2);
      rgbLin[2]=fastPrecisePow(rgb[2],2.2);
}else{ //Rec transfer
      rgbLin[0]=(rgb[0] < recBetaLin )?rcpFourFive*rgb[0]:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-rgb[0])),rcpTxFourFive);
      rgbLin[1]=(rgb[1] < recBetaLin )?rcpFourFive*rgb[1]:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-rgb[1])),rcpTxFourFive);
      rgbLin[2]=(rgb[1] < recBetaLin )?rcpFourFive*rgb[2]:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-rgb[2])),rcpTxFourFive);
}

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
}else if (mode==4){ //Rec 2020
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



void WPconv2Grey(double from[3], double to[3],double outp[3]){

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

mul(3,3,1,convBrad,D65XYZ,outp);

}

void XYZ2xyY(double XYZ[3],double outp[3]){
	double XYZtot=XYZ[0]+XYZ[1]+XYZ[2];

	double x=XYZ[0]/XYZtot;
	double y=XYZ[1]/XYZtot;

    outp[0]=x;
	outp[1]=y;
	outp[2]=XYZ[1];
}


void XYZ2rgb(double XYZ[3],double RGB[3], int mode){

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
}else if (mode==4){ //Rec 2020
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

double r=v1[0]*XYZ[0]+v1[1]*XYZ[1]+v1[2]*XYZ[2];
double g=v2[0]*XYZ[0]+v2[1]*XYZ[1]+v2[2]*XYZ[2];
double b=v3[0]*XYZ[0]+v3[1]*XYZ[1]+v3[2]*XYZ[2];

if ((mode==0)||(mode==6)){ //sRGB transfer
    RGB[0]=(r> 0.00313066844250063)?1.055 * fastPrecisePow(r,rcpTwoFour) - 0.055:12.92 *r;
    RGB[1]=(g> 0.00313066844250063)?1.055 * fastPrecisePow(g,rcpTwoFour) - 0.055:12.92 *g;
    RGB[2]=(b> 0.00313066844250063)?1.055 * fastPrecisePow(b,rcpTwoFour) - 0.055:12.92 *b;
}else if (mode==5){ //DCI-P3
    RGB[0]=fastPrecisePow(r,invTwoSix);
    RGB[1]=fastPrecisePow(g,invTwoSix);
    RGB[2]=fastPrecisePow(b,invTwoSix);
}else if (mode==7){ //Original NTSC
    RGB[0]=fastPrecisePow(r,invTwoTwo);
    RGB[1]=fastPrecisePow(g,invTwoTwo);
    RGB[2]=fastPrecisePow(b,invTwoTwo);
}else{ //Rec transfer
    RGB[0]=(r< recBeta)?4.5*r:recAlpha*fastPrecisePow(r,0.45)-(recAlpha-1);
    RGB[1]=(g< recBeta)?4.5*g:recAlpha*fastPrecisePow(g,0.45)-(recAlpha-1);
    RGB[2]=(b< recBeta)?4.5*b:recAlpha*fastPrecisePow(b,0.45)-(recAlpha-1);
}

}


//Source: https://stackoverflow.com/a/45263428; http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.htm; https://en.wikipedia.org/wiki/Rec._2020#Transfer_characteristics

void xy2XYZ(double xyCoord[2],double outp[3]){
        outp[0]=(1.0/xyCoord[1])*xyCoord[0];
        outp[1]=1;
        outp[2]=(1.0/xyCoord[1])*(1-xyCoord[0]-xyCoord[1]);
}
