#include <stdint.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define lerp(a,b,t) ((1 - (t)) * (a) + (t) * (b) )
#define rcptwoFiveFive 1.0/255.0
#define rcptHiBit 1.0/65535.0
#define root_twelve 3.464101615137755
#define two53 9007199254740992.0

uint64_t sconst[] = {
0xcaf32e182df87139,
0x2f768dac7bd5142f,
0xf5cb7923ed83419b,
0xbd8a2796d321864b,
0xe8cfb5a916bc4a93,
0xe4f5acd7c748df31,
0xe1fa45cbd78e9ba5,
0xbfa95c43b8642c7d,
0x371b825a76b93e5d,
0xd81ea45f719b48c3,
0x4f1e86c36123b57d,
0x9b876df18d45c6ef,
0x178fa56d4bca6217,
0xc3718fd4873a64b5,
0x1af8d6e51ad649b7,
0x28b71edacfd5a671,
0xfae7cd4154c689d1,
0xbdf6248a6bc872a5,
0xa137f8d63457e921,
0x125e7f3a69ecb5a3,
0xb12e3857687cd5a9,
0xb53427cd4295ebd7,
0xf765bead7241aecb,
0x819a34fc27e4c153,
0xca652d1f4f65b183,
0xca6d73b9c6b15e29,
0xfa29b7134fbd8713,
0xf5ba18e9f8712dc5,
0x4921df75f2e4ba3d,
0x2c5f4e8b274ed1a9
};


inline static uint32_t mswsi(uint64_t xi, uint64_t wi, uint64_t si) {

   xi *= xi; xi += (wi += si);
   return xi = (xi>>32) | (xi<<32);

}

inline static uint64_t init_rand_digits(uint64_t n) {

   uint64_t c, i, j, k, m, r, t, u, v, xi, wi, si;

   /* initialize state for local msws rng */

   r = n / 100000000;
   t = n % 100000000;
   si = sconst[r%30];
   r /= 30;
   xi = wi = t*si + r*si*100000000;

   /* get odd random number for low order digit */

   u = (mswsi(xi, wi, si) % 8) * 2 + 1; v = (1<<u);

   /* get rest of digits */

   for (m=60,c=0;m>0;) {
      j = mswsi(xi, wi, si);                /* get 8 digit 32-bit random word */
      for (i=0;i<32;i+=4) {
         k = (j>>i) & 0xf;        /* get a digit */
         if (k!=0 && (c & (1<<k)) == 0) { /* not 0 and not previous */
            c |= (1<<k);
            u |= (k<<m);          /* add digit to output */
            m -= 4;
	    if (m==24 || m==28) c = (1<<k) | v;
            if (m==0) break;
         }
      }
   }

   return u;

}

inline static uint64_t msws53(uint64_t x1, uint64_t w1, uint64_t s1, uint64_t x2, uint64_t w2, uint64_t s2) {
uint64_t xx;
x1 *= x1; xx = x1 += (w1 += s1); x1 = (x1 >> 32) | (x1 << 32);
x2 *= x2; x2 += (w2 += s2); x2 = (x2 >> 32) | (x2 << 32);
return (xx ^ x2)>>11;
}

//Source: Middle Square Weyl Sequence RNG - Bernard Widynski1

double grey_dither(double colour, double rand,double sdv, int sxf){

double sAB=sdv*root_twelve*0.5;
double randm=sAB*(2*rand-1)*-1;
double out=(sdv!=0)?colour+(randm*( (sxf==1)?rcptHiBit:rcptwoFiveFive) ):colour;

return out;

}

double logisticMap(double rand){
double rMap =3.98;
double randOld=rand;

for (int i=0;i<20;i++){
randOld=rMap*randOld*(1-randOld);
}
randOld=12000*randOld+0.597*randOld;

double fractpart, intpart;
fractpart = modf(randOld, &intpart);

return fractpart;
}
