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

void XYZ2rgb(double XYZ[3],double RGB[3]){

double r=3.2404542*XYZ[0]-1.5371385*XYZ[1]-0.4985314*XYZ[2];
double g=-0.969266*XYZ[0]+1.8760108*XYZ[1]+0.041556*XYZ[2];
double b=0.0556434*XYZ[0]-0.2040259*XYZ[1]+1.0572252*XYZ[2];


   RGB[0]=(r> 0.00313066844250063)?1.055 * fastPrecisePow(r,rcpTwoFour) - 0.055:12.92 *r;
   RGB[1]=(g> 0.00313066844250063)?1.055 * fastPrecisePow(g,rcpTwoFour) - 0.055:12.92 *g;
   RGB[2]=(b> 0.00313066844250063)?1.055 * fastPrecisePow(b,rcpTwoFour) - 0.055:12.92 *b;

}

void rgb2XYZ(double rgb[3],double outp[3]){


      double  n1=(rgb[0] > 0.0404482362771082 )?fastPrecisePow(fabs((rgb[0]+0.055)*rcpOFiveFive),2.4):rgb[0]*rcpTwelveNineTwo;
     double   n2=(rgb[1] > 0.0404482362771082 )?fastPrecisePow(fabs((rgb[1]+0.055)*rcpOFiveFive),2.4):rgb[1]*rcpTwelveNineTwo;
      double  n3=(rgb[2] > 0.0404482362771082 )?fastPrecisePow(fabs((rgb[2]+0.055)*rcpOFiveFive),2.4):rgb[2]*rcpTwelveNineTwo;


outp[0] =0.4124564*n1+0.3575761*n2+0.1804375*n3;
outp[1]=0.2126729*n1+0.7151522*n2+0.072175*n3;
outp[2]=0.0193339*n1+0.119192*n2+0.9503041*n3;

}

void XYZ2rgb_2020(double XYZ[3],double RGB[3]){


double r=1.71651066976197*XYZ[0]-0.355641669986716*XYZ[1]-0.253345541821907*XYZ[2];
double g=-0.666693001182624*XYZ[0]+1.61650220834691*XYZ[1]+0.015768750389995*XYZ[2];
double b=0.017643638767459*XYZ[0]-0.042779781669045*XYZ[1]+0.942305072720018*XYZ[2];

  RGB[0]=(r< recBeta)?4.5*r:recAlpha*fastPrecisePow(r,0.45)-(recAlpha-1);
  RGB[1]=(g< recBeta)?4.5*g:recAlpha*fastPrecisePow(g,0.45)-(recAlpha-1);
  RGB[2]=(b< recBeta)?4.5*b:recAlpha*fastPrecisePow(b,0.45)-(recAlpha-1);


}

void rgb2XYZ_2020(double rgb[3],double outp[3]){

double  n1=(rgb[0] < recBetaLin )?rcpFourFive*rgb[0]:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-rgb[0])),rcpTxFourFive);
double  n2=(rgb[1] < recBetaLin )?rcpFourFive*rgb[1]:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-rgb[1])),rcpTxFourFive);
double  n3=(rgb[1] < recBetaLin )?rcpFourFive*rgb[2]:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-rgb[2])),rcpTxFourFive);

outp[0] =0.637010191411101*n1+0.144615027396969*n2+0.168844781191930*n3;
outp[1]=0.26272171736164*n1+0.677989275502262*n2+0.059289007136098*n3;
outp[2]=0.028072328847647*n2+1.06075767115235*n3;

}

//Source: https://stackoverflow.com/a/45263428; http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.htm; https://en.wikipedia.org/wiki/Rec._2020#Transfer_characteristics

void xy2XYZ(double xyCoord[2],double outp[3]){
        outp[0]=(1.0/xyCoord[1])*xyCoord[0];
        outp[1]=1;
        outp[2]=(1.0/xyCoord[1])*(1-xyCoord[0]-xyCoord[1]);
}
