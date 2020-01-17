#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define lerp(a,b,t) ((1 - (t)) * (a) + (t) * (b) )



void f_gammaLow(double rgb[3], double gamma,double gamma_hi, double dist){
    double outcol[3];

    for (int i=0; i<3; i++){
    double col=rgb[i];
    double ln=(col-(1-col)*pow(col,gamma));
gamma_hi=(ln<=0 || (col==0))?gamma:log(ln)/log(col);
outcol[i]=1-fabs((pow(-col,gamma+1)+pow(col,gamma_hi+1)+pow(col,gamma))-rgb[i]);


    }

    	double shift = 1 - MIN(outcol[0], MIN(outcol[1], outcol[2])) - MAX(outcol[0], MAX(outcol[1], outcol[2]));

		outcol[0] = shift + outcol[0];
		outcol[1] = shift + outcol[1];
		outcol[2] = shift + outcol[2];

 dist=(rgb[0]-outcol[0])+(rgb[1]-outcol[1])+((outcol[2]-rgb[2]));
}

