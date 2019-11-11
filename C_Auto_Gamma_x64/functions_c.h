#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define lerp(a, b, t) (a + t*(b - a))

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


void f_gammaLow(double rgb[3], double gamma,double gamma_hi, double dist){
    double outcol[3];
    for (int i=0; i<3; i++){
    double col=rgb[i];
gamma_hi=(log10(pow(col,gamma+1)-pow(col,gamma))-log10(col))/log10(col);
outcol[i]=(pow(col,gamma)-pow(col,1+gamma)+pow(col,1+gamma_hi));
    }

double hsv[3];
rgb2hsv(outcol,hsv);
double hwb[3]={hsv[0],(1-hsv[1])*hsv[2],1-hsv[2]};

 dist=sqrt(pow(hwb[1],2)+pow(hwb[2],2));


}

