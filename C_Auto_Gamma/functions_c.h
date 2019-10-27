#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define lerp(a, b, t) (a + t*(b - a))

void f_gammaLow(double col, double gamma,double gamma_hi, double outcol){
gamma_hi=(log10(pow(col,gamma+1)-pow(col,gamma))-log10(col))/log10(col);
outcol=(pow(col,gamma)-pow(col,1+gamma)+pow(col,1+gamma_hi));
outcol=(outcol==0)?0:1-col/outcol;
}
