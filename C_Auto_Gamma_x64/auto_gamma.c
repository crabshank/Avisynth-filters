#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "..\avisynth_c.h"
#include "functions_c.h"

typedef struct Auto_Gamma {
      double bracketA;
      double bracketB;
      int tolerance;
      int crush;
      int limitedRange;
      int linear;
} Auto_Gamma;

AVS_VideoFrame * AVSC_CC Auto_Gamma_get_frame (AVS_FilterInfo * p, int n)
{
  AVS_VideoFrame * src;
   Auto_Gamma* params = (Auto_Gamma*) p->user_data;

  src = avs_get_frame(p->child, n);

    int row_size, height, src_pitch,x, y,max_iters,tol,tolr,opt,lmr,crs,lnr;
   BYTE* srcp;
   double a,b,c,mx,runTot_r,runTot_g,runTot_b,bOG,gOG,rOG,f_c,gamma_high,gamma_high_tmp,gamma_low,f_a,R,G,B,counter;

a =   params->bracketA;
b=params->bracketB;
tol=params->tolerance;
crs=params->crush;
crs=round(crs);
lmr=params->limitedRange;
lnr=params->linear;

  avs_make_writable(p->env, &src);

      srcp = avs_get_write_ptr(src);
      src_pitch = avs_get_pitch(src);
      row_size = avs_get_row_size(src);
      height = avs_get_height(src);

runTot_r=0;
runTot_g=0;
runTot_b=0;
counter=0;


      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {



//double x_shift=(double)x/(double)row_size;
                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcptwoFiveFive;     // B
       gOG=currGreen*rcptwoFiveFive;   //G
         rOG=currRed*rcptwoFiveFive;     // R

         if (lnr==1){
            bOG=(bOG > 0.0404482362771082)?fastPrecisePow(fabs((bOG+0.055)/1.055),2.4):bOG/12.92;
            gOG=(gOG > 0.0404482362771082)?fastPrecisePow(fabs((gOG+0.055)/1.055),2.4):gOG/12.92;
            rOG=(rOG > 0.0404482362771082)?fastPrecisePow(fabs((rOG+0.055)/1.055),2.4):rOG/12.92;
         }else if(lnr==2){
            bOG=(bOG > 0.00313066844250063)?1.055*(fastPrecisePow(bOG,rcptwo_four)) - 0.055:bOG*12.92;
            gOG=(gOG > 0.00313066844250063)?1.055*(fastPrecisePow(gOG,rcptwo_four)) - 0.055:gOG*12.92;
            rOG=(rOG > 0.00313066844250063)?1.055*(fastPrecisePow(rOG,rcptwo_four)) - 0.055:rOG*12.92;
         }

runTot_r+=rOG*0.2126729;
runTot_g+=gOG*0.7151522;
runTot_b+=bOG*0.072175;
counter+=1;

        x=x+3;
      }
      }


//Bisection method solver/////////////////////////////////////////
p=1;
tolr=1.0/fastPrecisePow(10,(double)tol);
max_iters=ceil((log10(b-a)-log10(tolr))/log10(2));
opt=0;
double mxMean[3]={runTot_r/counter,runTot_g/counter,runTot_b/counter};
while(p<=max_iters){
    c=0.5*(a+b);
    f_gammaLow(mxMean, c,gamma_high,f_c);

    if(f_c==0||(0.5*(b-a)<tolr)){
    p=max_iters;
    gamma_low=c;
    opt=1;
    }

    p++;
    f_gammaLow(mxMean, a,gamma_high_tmp,f_a );
   // sgn_c=(f_c<0)?-1:1;
    //sgn_a=(f_a<0)?-1:1;
    if(f_a>f_c){
        a=c;
    }else{
    b=c;
    }

}
///////////////////////////////////////////////////////


///////////////ACTUALLY DRAW PIXELS///////////////////////////////////////
      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

double x_shift=(double)x/(double)row_size;

                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcptwoFiveFive;     // B
       gOG=currGreen*rcptwoFiveFive;   //G
         rOG=currRed*rcptwoFiveFive;     // R

           double bLin=(bOG > 0.0404482362771082)?fastPrecisePow(fabs((bOG+0.055)/1.055),2.4):bOG/12.92;
           double gLin=(gOG > 0.0404482362771082)?fastPrecisePow(fabs((gOG+0.055)/1.055),2.4):gOG/12.92;
           double rLin=(rOG > 0.0404482362771082)?fastPrecisePow(fabs((rOG+0.055)/1.055),2.4):rOG/12.92;

                  if (lnr==1){
                        bOG=bLin;
                        gOG=gLin;
                        rOG=rLin;
         }else if(lnr==2){
            bOG=(bOG > 0.00313066844250063)?1.055*(fastPrecisePow(bOG,rcptwo_four)) - 0.055:bOG*12.92;
            gOG=(gOG > 0.00313066844250063)?1.055*(fastPrecisePow(gOG,rcptwo_four)) - 0.055:gOG*12.92;
            rOG=(rOG > 0.00313066844250063)?1.055*(fastPrecisePow(rOG,rcptwo_four)) - 0.055:rOG*12.92;
         }
R=rOG;
G=gOG;
B=bOG;
double Yog=0.2126729*rLin+0.7151522*gLin+0.072175*bLin;

         if(opt==1){

R=lerp(fastPrecisePow(rOG,gamma_low),fastPrecisePow(rOG,gamma_high),rOG);
G=lerp(fastPrecisePow(gOG,gamma_low),fastPrecisePow(gOG,gamma_high),gOG);
B=lerp(fastPrecisePow(bOG,gamma_low),fastPrecisePow(bOG,gamma_high),bOG);

double Ynw=0.2126729*R+0.7151522*G+0.072175*B;
if (crs==0){
if (Ynw<Yog){
R=rOG;
G=gOG;
B=bOG;
}
}else if (crs==1){
if (Ynw>Yog){
R=rOG;
G=gOG;
B=bOG;
}
}

if(lmr==1){
    R=(( R*255*(235-16)*rcptwoFiveFive )+16)*rcptwoFiveFive;
     G=(( G*255*(235-16)*rcptwoFiveFive )+16)*rcptwoFiveFive;
      B=(( B*255*(235-16)*rcptwoFiveFive )+16)*rcptwoFiveFive;
}

         }

////////////////////////////////////////////////////
//if (x_shift>0.5){
                srcp[x] = MAX(MIN(round(B*255),255),0);
             srcp[x+1] =MAX(MIN(round(G*255),255),0);
        srcp[x+2] = MAX(MIN(round(R*255),255),0);
//}

             x=x+3;
      }
            srcp += src_pitch;
      }

  return src;
}

AVS_Value AVSC_CC create_Auto_Gamma (AVS_ScriptEnvironment * env,AVS_Value args, void * dg)
{
  AVS_Value v;
  AVS_FilterInfo * fi;
  AVS_Clip * new_clip = avs_new_c_filter(env, &fi, avs_array_elt(args, 0), 1);
  Auto_Gamma *params = (Auto_Gamma*)malloc(sizeof(Auto_Gamma));

if (!params)
      return avs_void;
        params->bracketA = avs_defined(avs_array_elt(args, 1))?avs_as_float(avs_array_elt(args, 1)):0;
        params->bracketB = avs_defined(avs_array_elt(args, 2))?avs_as_float(avs_array_elt(args, 2)):11;
        params->tolerance = avs_defined(avs_array_elt(args, 3))?avs_as_int(avs_array_elt(args, 3)):3    ;
        params->crush = avs_defined(avs_array_elt(args, 4))?avs_as_int(avs_array_elt(args, 4)):0;
        params->limitedRange= avs_defined(avs_array_elt(args, 5))?avs_as_bool(avs_array_elt(args, 5)):false;
        params->linear= avs_defined(avs_array_elt(args, 6))?avs_as_int(avs_array_elt(args, 6)):0;

  if (!avs_is_rgb32(&fi->vi)) {
    return avs_new_value_error ("Input video must be in RGB format!");
  } else {
         fi->user_data = (void*) params;
    fi->get_frame = Auto_Gamma_get_frame;
    v = avs_new_value_clip(new_clip);
  }
  avs_release_clip(new_clip);
  return v;
}



const char * AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment * env)
{
   avs_add_function(env, "Auto_Gamma", "c[a]f[b]f[tolerance]i[crush]i[limitedRange]b[linear]i", create_Auto_Gamma, 0);
   return "Auto_Gamma sample C plugin";
}
