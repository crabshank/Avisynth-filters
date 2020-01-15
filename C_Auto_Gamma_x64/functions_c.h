#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define lerp(a,b,t) ((1 - (t)) * (a) + (t) * (b) )

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

void f_gammaLow(double rgb[3], double gamma,double gamma_hi, double dist){
    double outcol[3];
    for (int i=0; i<3; i++){
    double col=rgb[i];
    double ln=(col-(1-col)*pow(col,gamma));
gamma_hi=(ln<=0 || (col==0))?gamma:log(ln)/log(col);
outcol[i]=pow(-col,gamma+1)+pow(col,gamma_hi+1)+pow(col,gamma);
    }

 dist=(outcol[0]-rgb[0])+(rgb[1]-outcol[1])+((outcol[2]-rgb[2]));


}
