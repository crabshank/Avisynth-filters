#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define lerp(a,b,t) ((1 - (t)) * (a) + (t) * (b) )
#define rcptwoFiveFive 1.0/255.0
#define rcptHiBit 1.0/65535.0


int compare (const void * a, const void * b)
{
  return ( *(double*)a - *(double*)b );
}
