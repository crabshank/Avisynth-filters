#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
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


void zeroMatrix( int rows_a, int cols_a, void* ma){

      double (*a)[cols_a] = (double (*)[cols_a]) ma;

	int i, j;

	// Initializing elements of matrix mult to 0.
	for(i = 0; i < rows_a; ++i)
	{
		for(j = 0; j < cols_a; ++j)
		{
			a[i][j] = 0;
		}
	}

}

void zeroArray( int cols_a, double a[]){

	int i;

	// Initializing elements of matrix mult to 0.
	for(i = 0; i < cols_a; ++i)
	{
			a[i] = 0.0;
	}
}

  // A is m x p and B is p x n, so C is m x n
void mul( int rows_a, int cols_a, int cols_b,void* ma, void* mb, void* mres)
{



int i,j,k;

      double (*a)[cols_a] = (double (*)[cols_a]) ma;
      double (*b)[cols_b] = (double (*)[cols_b]) mb;
      double (*res)[cols_b] = (double (*)[cols_b]) mres;
  zeroMatrix( rows_a,cols_b,res);
      double *pta = &a[0][0];
      double *ptb = &b[0][0];
      double *ptr = &res[0][0];
  //  double*ptr, *ptr1, *ptr2;



  	// Multiplying matrix firstMatrix and secondMatrix and storing in array mult.
	for(i = 0; i < rows_a; i++)
	{
		for(j = 0; j < cols_b; j++)
		{
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

void rgb2xyY(double rgb[3],double outp[3]){


      double  n1=(rgb[0] > 0.0404482362771082 )?fastPrecisePow(fabs((rgb[0]+0.055)/1.055),2.4):rgb[0]/12.92;
     double   n2=(rgb[1] > 0.0404482362771082 )?fastPrecisePow(fabs((rgb[1]+0.055)/1.055),2.4):rgb[1]/12.92;
      double  n3=(rgb[2] > 0.0404482362771082 )?fastPrecisePow(fabs((rgb[2]+0.055)/1.055),2.4):rgb[2]/12.92;



double X=0.4124564*n1+0.3575761*n2+0.1804375*n3;
double Y=0.2126729*n1+0.7151522*n2+0.072175*n3;
double Z=0.0193339*n1+0.119192*n2+0.9503041*n3;

	double XYZtot=X+Y+Z;

	outp[0]=X/XYZtot;
	outp[1]=Y/XYZtot;
	outp[2]=Y;

}

//Source: https://stackoverflow.com/a/45263428; http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html


void xyY2XYZ(double xyY[3],double outp[3]){
	double X=(xyY[2]/xyY[1])*xyY[0];
	double Z=(xyY[2]/xyY[1])*(1-xyY[0]-xyY[1]);

	outp[0]=X;
	outp[1]=xyY[2];
	outp[2]=Z;
}

void XYZ2xyY(double XYZ[3],double outp[3]){
	double XYZtot=XYZ[0]+XYZ[1]+XYZ[2];

	double x=XYZ[0]/XYZtot;
	double y=XYZ[1]/XYZtot;

    outp[0]=x;
	outp[1]=y;
	outp[2]=XYZ[1];
}


void xyY2rgb(double xyY[3],double RGB[3]){

 double Y=xyY[2];
double  y=xyY[1];
double x=xyY[0];
 double X=(Y/y)*x;
 double Z=(Y/y)*(1.0-x-y);

double r=3.2404542*X-1.5371385*Y-0.4985314*Z;
double g=-0.969266*X+1.8760108*Y+0.041556*Z;
double b=0.0556434*X-0.2040259*Y+1.0572252*Z;


   r=(r> 0.00313066844250063)?1.055 * fastPrecisePow(r,1.0/2.4) - 0.055:12.92 *r;
   g=(g> 0.00313066844250063)?1.055 * fastPrecisePow(g,1.0/2.4) - 0.055:12.92 *g;
   b=(b> 0.00313066844250063)?1.055 * fastPrecisePow(b,1.0/2.4) - 0.055:12.92 *b;

RGB[0]=r;
RGB[1]=g;
RGB[2]=b;
}

//Source: http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html

void XYZ2rgb(double XYZ[3],double RGB[3]){

double r=3.2404542*XYZ[0]-1.5371385*XYZ[1]-0.4985314*XYZ[2];
double g=-0.969266*XYZ[0]+1.8760108*XYZ[1]+0.041556*XYZ[2];
double b=0.0556434*XYZ[0]-0.2040259*XYZ[1]+1.0572252*XYZ[2];


   r=(r> 0.00313066844250063)?1.055 * fastPrecisePow(r,1.0/2.4) - 0.055:12.92 *r;
   g=(g> 0.00313066844250063)?1.055 * fastPrecisePow(g,1.0/2.4) - 0.055:12.92 *g;
   b=(b> 0.00313066844250063)?1.055 * fastPrecisePow(b,1.0/2.4) - 0.055:12.92 *b;

RGB[0]=r;
RGB[1]=g;
RGB[2]=b;
}

void rgb2XYZ(double rgb[3],double outp[3]){


      double  n1=(rgb[0] > 0.0404482362771082 )?fastPrecisePow(fabs((rgb[0]+0.055)/1.055),2.4):rgb[0]/12.92;
     double   n2=(rgb[1] > 0.0404482362771082 )?fastPrecisePow(fabs((rgb[1]+0.055)/1.055),2.4):rgb[1]/12.92;
      double  n3=(rgb[2] > 0.0404482362771082 )?fastPrecisePow(fabs((rgb[2]+0.055)/1.055),2.4):rgb[2]/12.92;


outp[0] =0.4124564*n1+0.3575761*n2+0.1804375*n3;
outp[1]=0.2126729*n1+0.7151522*n2+0.072175*n3;
outp[2]=0.0193339*n1+0.119192*n2+0.9503041*n3;

}

//Source: https://stackoverflow.com/a/45263428; http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.htm

void xy2XYZ(double xyCoord[2],double outp[3]){
        outp[0]=(1.0/xyCoord[1])*xyCoord[0];
        outp[1]=1;
        outp[2]=(1.0/xyCoord[1])*(1-xyCoord[0]-xyCoord[1]);
}
