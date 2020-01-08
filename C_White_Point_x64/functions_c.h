#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define PI  3.14159265358979323846
#define lerp(a,b,t) ((1 - (t)) * (a) + (t) * (b) )
#define  mod(a,N) ((a) - (N)*floor((a)/(N)))
#define third 1.0/3.0
#define rcptwoFiveFive 1.0/255.0

void rgb2hsv (double rgb[3],double hsv[3])
{

double r=rgb[0];
double g=rgb[1];
double b=rgb[2];


    double max = MAX(r,MAX(g, b));
    double min = MIN(r,MIN(g, b));

    double diff=max-min;

    hsv[2] = max;

    if (max == 0.0f) {
        hsv[1] = 0;
        hsv[0] = 0;
    }

   else  if (diff == 0.0f) {
        hsv[1] = 0;
        hsv[0] = 0;
    }

    else {
        hsv[1] = diff / max;

        if (max == r) {
            hsv[0] = (g-b)/diff;
        }
        else if (max == g) {
            hsv[0] = 2+(b-r)/diff;
        }
        else {
            hsv[0] = 4+(r-g)/diff;
    }


    if(hsv[0]!=0){
    hsv[0]/=6;
hsv[0]=hsv[0]-(double)floor(hsv[0]);
    }

    hsv[0]=(hsv[0] < 0)?hsv[0]+1:hsv[0];

    }

}

void rgb2hsv_white (double rgb[3],double hsv[3])
{

double r=rgb[0];
double g=rgb[1];
double b=rgb[2];


    double max = MAX(r,MAX(g, b));
    double min = MIN(r,MIN(g, b));

    double diff=max-min;

    hsv[2] = max;

    if (max == 0.0f) {
        hsv[1] = 0;
        hsv[0] = 1;
    }

   else  if (diff == 0.0f) {
        hsv[1] = 0;
        hsv[0] = 1;
    }

    else {
        hsv[1] = diff / max;

        if (max == r) {
            hsv[0] = (g-b)/diff;
        }
        else if (max == g) {
            hsv[0] = 2+(b-r)/diff;
        }
        else {
            hsv[0] = 4+(r-g)/diff;
    }


    if(hsv[0]!=0){
    hsv[0]/=6;
hsv[0]=hsv[0]-(double)floor(hsv[0]);
    }

    hsv[0]=(hsv[0] < 0)?hsv[0]+1:hsv[0];

    }

}



void hsv2rgb(double hsv[3], double rgb[3])
{
double h=hsv[0];
double s=hsv[1];
double v=hsv[2];


   int i = floor(h * 6);
   double f = h * 6 - i;
   double p = v * (1 - s);
    double q = v * (1 - f * s);
   double t = v * (1 - (1 - f) * s);
    switch (i % 6) {
        case 0: rgb[0] = v, rgb[1] = t, rgb[2] = p; break;
        case 1: rgb[0] = q, rgb[1] = v, rgb[2] = p; break;
        case 2: rgb[0] = p, rgb[1] = v, rgb[2] = t; break;
        case 3: rgb[0] = p, rgb[1] = q, rgb[2] = v; break;
        case 4: rgb[0] = t, rgb[1] = p, rgb[2] = v; break;
        case 5: rgb[0] = v, rgb[1] = p, rgb[2] = q; break;
    }


}

void hsv_white2rgb(double hsv[3], double rgb[3])
{
double h=hsv[0];
double s=hsv[1];
double v=hsv[2];

if(h==1 && (s==0)){
    rgb[0]=v;
    rgb[1]=v;
    rgb[2]=v;

}else{
   int i = floor(h * 6);
   double f = h * 6 - i;
   double p = v * (1 - s);
    double q = v * (1 - f * s);
   double t = v * (1 - (1 - f) * s);
    switch (i % 6) {
        case 0: rgb[0] = v, rgb[1] = t, rgb[2] = p; break;
        case 1: rgb[0] = q, rgb[1] = v, rgb[2] = p; break;
        case 2: rgb[0] = p, rgb[1] = v, rgb[2] = t; break;
        case 3: rgb[0] = p, rgb[1] = q, rgb[2] = v; break;
        case 4: rgb[0] = t, rgb[1] = p, rgb[2] = v; break;
        case 5: rgb[0] = v, rgb[1] = p, rgb[2] = q; break;
    }
}

}

void hsv2hmv( double hsv[3],double hmv[3])
{

hmv[0]=hsv[0];
hmv[1]=(hsv[2]*(-hsv[1]+2))/2;
hmv[2]=hsv[2];

}

void hmv2hsv( double hmv[3],double hsv[3])
{

hsv[0]=hmv[0];
hsv[1]=((-2*hmv[1])/hmv[2])+2;
hsv[2]=hmv[2];


}


void mchg2hsv( double hmv[3], double m[1],double hsv[3])
{

//double mOG=hmv[1];

hsv[0]=hmv[0];


double a =2*m[0]-hmv[2];
double b=hmv[2];
double zero_t=(a-b==0)?1:a/(a-b);
double one_t=(a-b==0)?1:(a-1)/(a-b);

if (a<0){
    a=0;
    b=lerp(hmv[2],m[0],zero_t);

}else if (a>1){
    a=1;
    b=lerp(hmv[2],m[0],one_t);
}

hsv[2]=MAX(a,b);
hsv[1]=((-2*m[0])/hsv[2])+2;



}

void hwb2hsv( double hwb[3],double hsv[3])
{
double h=hwb[0];
double w=hwb[1];
double b=hwb[2];

if(w+b>1){
w=w/(w+b);
b=b/(w+b);
}

hsv[0]=h;
hsv[1]=1-(w/(1-b));
hsv[2]=1-b;

}

void hsv2hwb( double hsv[3],double hwb[3])
{

hwb[0]=hsv[0];
hwb[1]=(1-hsv[1])*hsv[2];
hwb[2]=1-hsv[2];


}


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


double CR[3][3];
zeroMatrix(3,3,CR);

CR[0][0]=BradTo[0]/BradFrom[0];
CR[1][1]=BradTo[1]/BradFrom[1];
CR[2][2]=BradTo[2]/BradFrom[2];

double convBrad2[3][3];
double convBrad[3][3];
zeroMatrix(3,3,convBrad);
zeroMatrix(3,3,convBrad2);

mul(3,3,3,BradfordInv,CR,convBrad2);
mul(3,3,3,convBrad2,Bradford,convBrad);

double XYZ[3]={0.95047,1,1.08883};

mul(3,3,1,convBrad,XYZ,outp);

}

void sRGB2Linear(double rgb[3],double outp[3]){

	for (int p=0;p<3;p++){
        outp[p]=(rgb[p] > 0.0404482362771082 )?pow(fabs((rgb[p]+0.055)/1.055),2.4):rgb[p]/12.92;
}

}

void Linear2sRGB(double rgb[3],double outp[3]){


for (int p=0; p<3; p++){

  outp[p]=(rgb[p]> 0.00313066844250063)?1.055 * pow(rgb[p],1/2.4) - 0.055:12.92 *rgb[p];
}

}

void rgb2xyY(double rgb[3],double outp[3]){

   double rgbNew[3]={0,0,0};

	double XYZ[3]={0,0,0};

	for (int p=0;p<3;p++){
        rgbNew[p]=(rgb[p] > 0.0404482362771082 )?pow(fabs((rgb[p]+0.055)/1.055),2.4):rgb[p]/12.92;
}



double v1[3]={0.4124564,0.3575761, 0.1804375};
double v2[3]={0.2126729,0.7151522,0.072175};
double v3[3]={0.0193339,0.119192,0.9503041};

XYZ[0]=v1[0]*rgbNew[0]+v1[1]*rgbNew[1]+v1[2]*rgbNew[2];
XYZ[1]=v2[0]*rgbNew[0]+v2[1]*rgbNew[1]+v2[2]*rgbNew[2];
XYZ[2]=v3[0]*rgbNew[0]+v3[1]*rgbNew[1]+v3[2]*rgbNew[2];



	double XYZtot=XYZ[0]+XYZ[1]+XYZ[2];

	double x=XYZ[0]/XYZtot;
	double y=XYZ[1]/XYZtot;

	outp[0]=x;
	outp[1]=y;
	outp[2]=XYZ[1];

}

//Source: https://stackoverflow.com/a/45263428; http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html

void LinRGB2xyY(double rgbNew[3],double outp[3]){

	double XYZ[3]={0,0,0};

double v1[3]={0.4124564,0.3575761, 0.1804375};
double v2[3]={0.2126729,0.7151522,0.072175};
double v3[3]={0.0193339,0.119192,0.9503041};

XYZ[0]=v1[0]*rgbNew[0]+v1[1]*rgbNew[1]+v1[2]*rgbNew[2];
XYZ[1]=v2[0]*rgbNew[0]+v2[1]*rgbNew[1]+v2[2]*rgbNew[2];
XYZ[2]=v3[0]*rgbNew[0]+v3[1]*rgbNew[1]+v3[2]*rgbNew[2];



	double XYZtot=XYZ[0]+XYZ[1]+XYZ[2];

	double x=XYZ[0]/XYZtot;
	double y=XYZ[1]/XYZtot;

	outp[0]=x;
	outp[1]=y;
	outp[2]=XYZ[1];

}

//Source: https://stackoverflow.com/a/45263428; http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html

void xyY2XYZ(double xyY[3],double outp[3]){
	double X=(xyY[2]/xyY[1])*xyY[0];
	double Z=(xyY[2]/xyY[1])*(1-xyY[0]-xyY[1]);

	outp[0]=X;
	outp[1]=xyY[2];
	outp[2]=Z;
}


void RGB2rgb(double RGB[3],double rgb[3]){
	double RGBtot=RGB[0]+RGB[1]+RGB[2];

	double r=RGB[0]/RGBtot;
	double g=RGB[1]/RGBtot;

    rgb[0]=r;
	rgb[1]=g;
	rgb[2]=1-r-g;
}

void rgb2RGB_White(double rgb[3],double RGB[3]){

	double mx_prp=MAX( rgb[0],MAX( rgb[1], rgb[2]));
	double rcp_mx_prp=(mx_prp==0)?0:pow(mx_prp,-1);

    RGB[0]=rgb[0]*rcp_mx_prp;
    RGB[1]=rgb[1]*rcp_mx_prp;
    RGB[2]=rgb[2]*rcp_mx_prp;

double mx_RGB=MAX( RGB[0],MAX( RGB[1], RGB[2]));
double rcp_mx_RGB=(mx_RGB==0)?0:pow(mx_RGB,-1);

    RGB[0]=RGB[0]*rcp_mx_RGB;
    RGB[1]=RGB[1]*rcp_mx_RGB;
    RGB[2]=RGB[2]*rcp_mx_RGB;

if(MAX( RGB[0],MAX( RGB[1], RGB[2]))==0){
    RGB[0]=1;
    RGB[1]=1;
    RGB[2]=1;
}

}

void rgb2RGB_val(double rgb[3],double val[1],double RGB[3]){

	double mx_prp=MAX( rgb[0],MAX( rgb[1], rgb[2]));
	double rcp_mx_prp=(mx_prp==0)?0:pow(mx_prp,-1);

    RGB[0]=rgb[0]*rcp_mx_prp;
    RGB[1]=rgb[1]*rcp_mx_prp;
    RGB[2]=rgb[2]*rcp_mx_prp;

double mx_RGB=MAX( RGB[0],MAX( RGB[1], RGB[2]));
double rcp_mx_RGB=(mx_RGB==0)?0:pow(mx_RGB,-1);

    RGB[0]=(RGB[0]*rcp_mx_RGB)*val[0];
    RGB[1]=(RGB[1]*rcp_mx_RGB)*val[0];
    RGB[2]=(RGB[2]*rcp_mx_RGB)*val[0];

if(MAX( RGB[0],MAX( RGB[1], RGB[2]))==0){
    RGB[0]=1;
    RGB[1]=1;
    RGB[2]=1;
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


void XYZ2xyY_Grey(double XYZ[3],double outp[3]){
	double XYZtot=XYZ[0]+XYZ[1]+XYZ[2];

	double x=XYZ[0]/XYZtot;
	double y=XYZ[1]/XYZtot;
	outp[0]=x;
	outp[1]=y;
	outp[2]=XYZ[1];
}


void xyY2rgb(double xyY[3],double RGB[3]){
double X;
double Z;

double x;
double y;
double Y;



    double *pxyY = &xyY[0];

    xyY[0]=*(pxyY);
xyY[1]=*(pxyY+1);
xyY[2]=*(pxyY+2);

 Y=*(pxyY+2);
  y=*(pxyY+1);
 x=*(pxyY);
  X=(Y/y)*x;
 Z=(Y/y)*(1.0-x-y);



    double *pRGB = &RGB[0];
*(pRGB)=0; *(pRGB+1)=0; *(pRGB+2)=0;


*(pRGB)=3.2404542*X-1.5371385*Y-0.4985314*Z;
*(pRGB+1)=-0.969266*X+1.8760108*Y+0.041556*Z;
*(pRGB+2)=0.0556434*X-0.2040259*Y+1.0572252*Z;

for (int i=0;i<3;i++){

   *(pRGB+i)=(*(pRGB+i)> 0.00313066844250063)?1.055 * pow(*(pRGB+i),1/2.4) - 0.055:12.92 **(pRGB+i);
}


}


//Source: http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html

void xyY2LinRGB(double xyY[3],double RGB[3]){
double X;
double Z;

double x;
double y;
double Y;



    double *pxyY = &xyY[0];

    xyY[0]=*(pxyY);
xyY[1]=*(pxyY+1);
xyY[2]=*(pxyY+2);

 Y=*(pxyY+2);
  y=*(pxyY+1);
 x=*(pxyY);
  X=(Y/y)*x;
 Z=(Y/y)*(1.0-x-y);



    double *pRGB = &RGB[0];
*(pRGB)=0; *(pRGB+1)=0; *(pRGB+2)=0;


*(pRGB)=3.2404542*X-1.5371385*Y-0.4985314*Z;
*(pRGB+1)=-0.969266*X+1.8760108*Y+0.041556*Z;
*(pRGB+2)=0.0556434*X-0.2040259*Y+1.0572252*Z;

}


//Source: http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html


void rgb2Other_XYZ (double rgb[3], double rgbTo[3], double XYZog[3],double XYZnew[3]){

	  double rgbLin[3];
	  double rgbNew[3];

	for (int p=0;p<3;p++){
        rgbLin[p]=(rgb[p] > 0.0404482362771082 )?pow(fabs((rgb[p]+0.055)/1.055),2.4):rgb[p]/12.92;
        rgbNew[p]=(rgbTo[p] > 0.0404482362771082 )?pow(fabs((rgbTo[p]+0.055)/1.055),2.4):rgbTo[p]/12.92;
}



double v1[3]={0.4124564,0.3575761, 0.1804375};
double v2[3]={0.2126729,0.7151522,0.072175};
double v3[3]={0.0193339,0.119192,0.9503041};

XYZog[0]=v1[0]*rgbLin[0]+v1[1]*rgbLin[1]+v1[2]*rgbLin[2];
XYZog[1]=v2[0]*rgbLin[0]+v2[1]*rgbLin[1]+v2[2]*rgbLin[2];
XYZog[2]=v3[0]*rgbLin[0]+v3[1]*rgbLin[1]+v3[2]*rgbLin[2];

XYZnew[0]=v1[0]*rgbNew[0]+v1[1]*rgbNew[1]+v1[2]*rgbNew[2];
XYZnew[1]=v2[0]*rgbNew[0]+v2[1]*rgbNew[1]+v2[2]*rgbNew[2];
XYZnew[2]=v3[0]*rgbNew[0]+v3[1]*rgbNew[1]+v3[2]*rgbNew[2];

		/*	return XYZ2xyY_Grey(
                         WPconv2Grey(XYZog,XYZnew)
                         ).xy;*/ //HLSL next steps

}


void LinRGB2Other_XYZ (double rgb[3], double rgbTo[3], double XYZog[3],double XYZnew[3]){

double v1[3]={0.4124564,0.3575761, 0.1804375};
double v2[3]={0.2126729,0.7151522,0.072175};
double v3[3]={0.0193339,0.119192,0.9503041};

XYZog[0]=v1[0]*rgb[0]+v1[1]*rgb[1]+v1[2]*rgb[2];
XYZog[1]=v2[0]*rgb[0]+v2[1]*rgb[1]+v2[2]*rgb[2];
XYZog[2]=v3[0]*rgb[0]+v3[1]*rgb[1]+v3[2]*rgb[2];

XYZnew[0]=v1[0]*rgbTo[0]+v1[1]*rgbTo[1]+v1[2]*rgbTo[2];
XYZnew[1]=v2[0]*rgbTo[0]+v2[1]*rgbTo[1]+v2[2]*rgbTo[2];
XYZnew[2]=v3[0]*rgbTo[0]+v3[1]*rgbTo[1]+v3[2]*rgbTo[2];
		/*	return XYZ2xyY_Grey(
                         WPconv2Grey(XYZog,XYZnew)
                         ).xy;*/ //HLSL next steps

}

void rgb2Grey_XYZ (double rgb[3], double XYZog[3],double XYZnew[3]){

	  double rgbLin[3];

	for (int p=0;p<3;p++){
        rgbLin[p]=(rgb[p] > 0.0404482362771082 )?pow(fabs((rgb[p]+0.055)/1.055),2.4):rgb[p]/12.92;
}


double rgbNewTot=rgbLin[0]+rgbLin[1]+rgbLin[2];
double rgbNewAvg=rgbNewTot/3;

double rgbNew[3]={rgbNewAvg,rgbNewAvg,rgbNewAvg};

double v1[3]={0.4124564,0.3575761, 0.1804375};
double v2[3]={0.2126729,0.7151522,0.072175};
double v3[3]={0.0193339,0.119192,0.9503041};

XYZog[0]=v1[0]*rgbLin[0]+v1[1]*rgbLin[1]+v1[2]*rgbLin[2];
XYZog[1]=v2[0]*rgbLin[0]+v2[1]*rgbLin[1]+v2[2]*rgbLin[2];
XYZog[2]=v3[0]*rgbLin[0]+v3[1]*rgbLin[1]+v3[2]*rgbLin[2];

XYZnew[0]=v1[0]*rgbNew[0]+v1[1]*rgbNew[1]+v1[2]*rgbNew[2];
XYZnew[1]=v2[0]*rgbNew[0]+v2[1]*rgbNew[1]+v2[2]*rgbNew[2];
XYZnew[2]=v3[0]*rgbNew[0]+v3[1]*rgbNew[1]+v3[2]*rgbNew[2];

		/*	return XYZ2xyY_Grey(
                         WPconv2Grey(XYZog,XYZnew)
                         ).xy;*/ //HLSL next steps

}

void LinRGB2Grey_XYZ (double rgb[3], double XYZog[3],double XYZnew[3]){

double rgbNewTot=rgb[0]+rgb[1]+rgb[2];
double rgbNewAvg=rgbNewTot/3;

double rgbNew[3]={rgbNewAvg,rgbNewAvg,rgbNewAvg};

double v1[3]={0.4124564,0.3575761, 0.1804375};
double v2[3]={0.2126729,0.7151522,0.072175};
double v3[3]={0.0193339,0.119192,0.9503041};

XYZog[0]=v1[0]*rgb[0]+v1[1]*rgb[1]+v1[2]*rgb[2];
XYZog[1]=v2[0]*rgb[0]+v2[1]*rgb[1]+v2[2]*rgb[2];
XYZog[2]=v3[0]*rgb[0]+v3[1]*rgb[1]+v3[2]*rgb[2];

XYZnew[0]=v1[0]*rgbNew[0]+v1[1]*rgbNew[1]+v1[2]*rgbNew[2];
XYZnew[1]=v2[0]*rgbNew[0]+v2[1]*rgbNew[1]+v2[2]*rgbNew[2];
XYZnew[2]=v3[0]*rgbNew[0]+v3[1]*rgbNew[1]+v3[2]*rgbNew[2];

		/*	return XYZ2xyY_Grey(
                         WPconv2Grey(XYZog,XYZnew)
                         ).xy;*/ //HLSL next steps

}

void xy2XYZ(double xyCoord[2],double outp[3]){
        outp[0]=(1*pow(xyCoord[1],-1))*xyCoord[0];
        outp[1]=1;
        outp[2]= (1*pow(xyCoord[1],-1))*(1-xyCoord[0]-xyCoord[1]);
}


void XYZ2Lab(double XYZ[3], double WP_XYZ[3], double Lab[3]){


double XYZadj[3];

 XYZadj[0]  = XYZ[0] / WP_XYZ[0];
 XYZadj[1] = XYZ[1]/WP_XYZ[1];
 XYZadj[2] = XYZ[2] /WP_XYZ[2];

for (int p=0; p<3; p++){
        XYZadj[p]=(XYZadj[p] > 0.008856)?pow(XYZadj[p],1/3):7.787*XYZadj[p] + 16/116;
}

Lab[0] = ( 116 * XYZadj[1] ) - 16;
Lab[1] = 500 * ( XYZadj[0] - XYZadj[1] );
Lab[2] = 200 * ( XYZadj[1] - XYZadj[2] );

// {0 to 100,-1 to 1 , -1 to 1}

}

void Lab2Lch(double Lab[3],double Lch[3]){

double L=Lab[0]/100;
double a=2*(Lab[1]+1); //Rescale for [0-1]
double b=2*(Lab[2]+1); //Rescale for [0-1]

Lch[0]=L;

Lch[1]=sqrt(a*a+b*b)/sqrt(2);

Lch[2]=atan2(b,a)/(0.5*PI);

}
//Source: http://wiki.nuaj.net/index.php/Color_Transforms#XYZ_.E2.86.92_L.2Aa.2Ab.2A


void hsv2hsl(double hsv[3],double hsl[3]){

	hsl[0]=hsv[0];

    hsl[2] = (2 - hsv[1]) * hsv[2] / 2;

    if (hsl[2]  != 0) {
        if ( hsl[2]  == 1) {
            hsl[1] = 0;
        } else if ( hsl[2]  < 0.5) {
            hsl[1] = hsv[1] * hsv[1] / (hsl[2] * 2);
        } else {
            hsl[1] = hsv[1] * hsv[2] / (2 - hsl[2] * 2);
        }
    }

}


void RGB2HSI(double RGB[3],double HSI[3]){

double RGB_tot=RGB[0]+RGB[1]+RGB[2];
double rgb[3];
if(RGB_tot==0){
rgb[0]=third;
rgb[1]=third;
rgb[2]=third;
}else{
rgb[0]=RGB[0]/RGB_tot;
rgb[1]=RGB[1]/RGB_tot;
rgb[2]=RGB[2]/RGB_tot;

}

if(RGB[0]==RGB[1] && (RGB[1]==RGB[2])){

    HSI[0]=0;
    HSI[1]=0;

}else{

double w=0.5*(2*RGB[0]-RGB[1]-RGB[2])/sqrt((  RGB[0]-RGB[1]  )*(  RGB[0]-RGB[1]  )  + (  RGB[0]-RGB[2]  )*(  RGB[1]-RGB[2]  )     );
w=MIN(MAX(w,-1),1);
HSI[0]=acos(w);
HSI[0]=(RGB[2]>RGB[1])?2*PI-HSI[0]:HSI[0];

if(rgb[0]<=rgb[1] && (rgb[0]<=rgb[2]) ){
    HSI[1]=1-3*rgb[0];
}

if(rgb[1]<=rgb[0] && (rgb[1]<=rgb[2]) ){
    HSI[1]=1-3*rgb[1];
}

if(rgb[2]<=rgb[0] && (rgb[2]<=rgb[1]) ){
    HSI[1]=1-3*rgb[2];
}




}
}


//Source: http://fourier.eng.hmc.edu/e161/lectures/ColorProcessing/node2.html

void RGB2TintSL(double rgb[3],double SL[2]){



    	double r_=rgb[0]-(1/3);
	double g_=rgb[1]-(1/3);

	 SL[0]=sqrt((9/5)*(r_*r_+g_*g_))/(sqrt(2.4));
SL[1]=rgb[0]*0.299+rgb[1]*0.587+rgb[2]*0.114;
}


/* HLSL NESTED FUNCTIONS
float3 WPChangeRGB(float3 color, float3 from, float3 to){

return xyY2rgb(
               XYZ2xyY(
                       WPconv(
                              xyY2XYZ(
                                      rgb2xyY(
                                              color.rgb
                                              )
                                      ),from,to)
                       )
               );

}


float3 WPChangexyY(float3 color, float3 from, float3 to){

return XYZ2xyY(
               WPconv(
                      xyY2XYZ(
                              color.xyz
                              ),from,to)
               );

}

float3 WPChangeXYZ(float3 color, float3 from, float3 to){

return WPconv(
              xyY2XYZ(
                      rgb2xyY(
                              color.xyz
                              )
                      ),from,to);

}


*/
