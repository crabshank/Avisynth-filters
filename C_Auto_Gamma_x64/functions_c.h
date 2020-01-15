#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define lerp(a,b,t) ((1 - (t)) * (a) + (t) * (b) )

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
