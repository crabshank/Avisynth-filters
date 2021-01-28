#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "..\avisynth_c.h"
#include "functions_c.h"

typedef struct Linear_Gamma {
      int linear;
      int sixtyFour;
} Linear_Gamma;

AVS_VideoFrame * AVSC_CC Linear_Gamma_get_frame (AVS_FilterInfo * p, int n)
{
  AVS_VideoFrame * src;
   Linear_Gamma* params = (Linear_Gamma*) p->user_data;

  src = avs_get_frame(p->child, n);

    int row_size, height, src_pitch,x, y,lnr,sxf;
   BYTE* srcp;
   double bOG,gOG,rOG,R,G,B;


lnr=params->linear;
sxf=params->sixtyFour;

  avs_make_writable(p->env, &src);

      srcp = avs_get_write_ptr(src);
      src_pitch = avs_get_pitch(src);
      row_size = avs_get_row_size(src);
      height = avs_get_height(src);

///////////////ACTUALLY DRAW PIXELS///////////////////////////////////////
      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

//double x_shift=(double)x/(double)row_size;

                 double currBlue=(sxf==1)?(double)srcp[x]+srcp[x+1]*256:(double)srcp[x];
                 double currGreen=(sxf==1)?(double)srcp[x+2]+srcp[x+3]*256:(double)srcp[x+1];
                 double currRed=(sxf==1)?(double)srcp[x+4]+srcp[x+5]*256:(double)srcp[x+2];


    bOG=(sxf==1)?currBlue*rcptHiBit:currBlue*rcptwoFiveFive;     // B
       gOG=(sxf==1)?currGreen*rcptHiBit:currGreen*rcptwoFiveFive;   //G
         rOG=(sxf==1)?currRed*rcptHiBit:currRed*rcptwoFiveFive;     // R


if (lnr==1){
    R=(rOG > 0.0404482362771082 )?fastPrecisePow(fabs((rOG+0.055)*rcpOFiveFive),2.4):rOG*rcpTwelveNineTwo;
    G=(gOG > 0.0404482362771082 )?fastPrecisePow(fabs((gOG+0.055)*rcpOFiveFive),2.4):gOG*rcpTwelveNineTwo;
    B=(bOG > 0.0404482362771082 )?fastPrecisePow(fabs((bOG+0.055)*rcpOFiveFive),2.4):bOG*rcpTwelveNineTwo;
}else if(lnr==8){
    R=fastPrecisePow(rOG,invTwoSix);
    G=fastPrecisePow(gOG,invTwoSix);
    B=fastPrecisePow(bOG,invTwoSix);
}else if (lnr==5){
    R=fastPrecisePow(rOG,2.2);
    G=fastPrecisePow(gOG,2.2);
    B=fastPrecisePow(bOG,2.2);
}else if(lnr==3){
    R=(rOG < recBetaLin )?rcpFourFive*rOG:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-rOG)),rcpTxFourFive);
    G=(gOG < recBetaLin )?rcpFourFive*gOG:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-gOG)),rcpTxFourFive);
    B=(bOG < recBetaLin )?rcpFourFive*bOG:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-bOG)),rcpTxFourFive);
}else if(lnr==2){
    R=(rOG> 0.00313066844250063)?1.055 * fastPrecisePow(rOG,rcpTwoFour) - 0.055:12.92 *rOG;
    G=(gOG> 0.00313066844250063)?1.055 * fastPrecisePow(gOG,rcpTwoFour) - 0.055:12.92 *gOG;
    B=(bOG> 0.00313066844250063)?1.055 * fastPrecisePow(bOG,rcpTwoFour) - 0.055:12.92 *bOG;
}else if(lnr==7){
    R=fastPrecisePow(rOG,2.6);
    G=fastPrecisePow(gOG,2.6);
    B=fastPrecisePow(bOG,2.6);
}else if(lnr==6){
    R=fastPrecisePow(rOG,invTwoTwo);
    G=fastPrecisePow(gOG,invTwoTwo);
    B=fastPrecisePow(bOG,invTwoTwo);
}else if(lnr==4){
    R=(rOG< recBeta)?4.5*rOG:recAlpha*fastPrecisePow(rOG,0.45)-(recAlpha-1);
    G=(gOG< recBeta)?4.5*gOG:recAlpha*fastPrecisePow(gOG,0.45)-(recAlpha-1);
    B=(bOG< recBeta)?4.5*bOG:recAlpha*fastPrecisePow(bOG,0.45)-(recAlpha-1);
}else if (lnr==9){
    R=(rOG>0.5)?rcpTwelve*(fastPrecisePow(euler_e,(rOG-HLG_c)*rcp_HLG_a)+HLG_b):rOG*rOG*third;
    G=(gOG>0.5)?rcpTwelve*(fastPrecisePow(euler_e,(gOG-HLG_c)*rcp_HLG_a)+HLG_b):gOG*gOG*third;
    B=(bOG>0.5)?rcpTwelve*(fastPrecisePow(euler_e,(bOG-HLG_c)*rcp_HLG_a)+HLG_b):bOG*bOG*third;
}else if (lnr==10){
    R=(rOG > rcpTwelve)?HLG_a*log(12.0*rOG-HLG_b)+HLG_c:root_three*fastPrecisePow(rOG,0.5);
    G=(gOG > rcpTwelve)?HLG_a*log(12.0*gOG-HLG_b)+HLG_c:root_three*fastPrecisePow(gOG,0.5);
    B=(bOG > rcpTwelve)?HLG_a*log(12.0*bOG-HLG_b)+HLG_c:root_three*fastPrecisePow(bOG,0.5);
}

int wp_b=MAX(MIN(round(B*255),255),0);
int wp_g=MAX(MIN(round(G*255),255),0);
int wp_r=MAX(MIN(round(R*255),255),0);

srcp[x] =wp_b; //blue : blue
srcp[x+1] =(sxf==1)?wp_b:wp_g; // blue : green
srcp[x+2] =(sxf==1)? wp_g:wp_r; // green: red
srcp[x+3] =(sxf==1)? wp_g:srcp[x+3]; //green : self
srcp[x+4] =(sxf==1)? wp_r:srcp[x+4]; //red : self
srcp[x+5] =(sxf==1)? wp_r:srcp[x+5]; //red : self

x=(sxf==1)?x+7:x+3;

      }
            srcp += src_pitch;
      }

  return src;
}

AVS_Value AVSC_CC create_Linear_Gamma (AVS_ScriptEnvironment * env,AVS_Value args, void * dg)
{
  AVS_Value v;
  AVS_FilterInfo * fi;
  AVS_Clip * new_clip = avs_new_c_filter(env, &fi, avs_array_elt(args, 0), 1);
  Linear_Gamma *params = (Linear_Gamma*)malloc(sizeof(Linear_Gamma));

if (!params)
      return avs_void;


        params->linear= avs_defined(avs_array_elt(args, 1))?avs_as_int(avs_array_elt(args, 1)):1;

 if ((params->linear<1)||(params->linear>10)){
            return avs_new_value_error ("Allowed linear values are between 1 and 10!");
          }else{
  if (!((avs_is_rgb32(&fi->vi))||(avs_is_rgb64(&fi->vi)))) {
    return avs_new_value_error ("Input video must be in RGB32 OR RGB64 format!");
  } else {

     if(avs_defined(avs_array_elt(args, 2))){
        params->sixtyFour =avs_as_bool(avs_array_elt(args, 2));
     }else{
       params->sixtyFour = (avs_is_rgb64(&fi->vi))?true:false;
     }
         fi->user_data = (void*) params;
    fi->get_frame = Linear_Gamma_get_frame;
    v = avs_new_value_clip(new_clip);
  }
          }
  avs_release_clip(new_clip);
  return v;
}

const char * AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment * env)
{
   avs_add_function(env, "Linear_Gamma", "c[linear]i[sixtyFour]b", create_Linear_Gamma, 0);
   return "Linear_Gamma C plugin";
}
