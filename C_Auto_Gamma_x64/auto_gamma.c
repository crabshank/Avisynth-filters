#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <stdio.h>
#include "..\avisynth_c.h"
#include "functions_c.h"

typedef struct Auto_Gamma {
    double* use_R;
    double* use_G;
    double* use_B;
    double a;
    double l;
    double h;
    int crush;
    int limitedRange;
    int mode;
    int linear;
    int sixtyFour;
    int pxels;
} Auto_Gamma;

/*
//DEBUG FILE STUFF

char* d_file_path="C:\\Program Files (x86)\\~AvsPmod\\plugins\\C_Auto_Gamma_x64\\debug.txt";

void makeFile(char* f_name){

    FILE *fptr;

    fptr = fopen(f_name,"w");

    if(fptr == NULL){
        exit(1);
    }

    fclose(fptr);
}

void appendFormattedToFile(char* f_name, const char * format, ...)
{
    FILE * fptr;

    fptr = fopen (f_name,"a");

    va_list args;
    va_start (args, format);
    vfprintf (fptr, format, args);
    va_end (args);

    fclose (fptr);

   return 0;
}*/

AVS_VideoFrame * AVSC_CC Auto_Gamma_get_frame (AVS_FilterInfo * p, int n)
{
    AVS_VideoFrame * src;
    Auto_Gamma* params = (Auto_Gamma*) p->user_data;
    src = avs_get_frame(p->child, n);

    int row_size, height, src_pitch,x, y,lmr,crs,lnr,mde,sxf;
   BYTE* srcp;
   const BYTE* rrcp;
   double a,lw,hi,runTot_r,runTot_g,runTot_b,bOG,gOG,rOG,gamma_high,gamma_low,R,G,B,counter;

a=params->a;
lw=params->l;
hi=params->h;
crs=params->crush;
lmr=params->limitedRange;
mde=params->mode;
lnr=params->linear;
sxf=params->sixtyFour;

  avs_make_writable(p->env, &src);

      srcp = avs_get_write_ptr(src);
      rrcp = avs_get_read_ptr(src);
      src_pitch = avs_get_pitch(src);
      row_size = avs_get_row_size(src);
      height = avs_get_height(src);

runTot_r=0;
runTot_g=0;
runTot_b=0;
counter=0;
int p_ix=0;

      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

                 double currBlue=(sxf==1)?(double)rrcp[x]+rrcp[x+1]*256:(double)rrcp[x];
                 double currGreen=(sxf==1)?(double)rrcp[x+2]+rrcp[x+3]*256:(double)rrcp[x+1];
                 double currRed=(sxf==1)?(double)rrcp[x+4]+rrcp[x+5]*256:(double)rrcp[x+2];


    bOG=(sxf==1)?currBlue*rcptHiBit:currBlue*rcptwoFiveFive;     // B
       gOG=(sxf==1)?currGreen*rcptHiBit:currGreen*rcptwoFiveFive;   //G
         rOG=(sxf==1)?currRed*rcptHiBit:currRed*rcptwoFiveFive;     // R

if (lnr==1){
    rOG=(rOG > 0.0404482362771082 )?fastPrecisePow(fabs((rOG+0.055)*rcpOFiveFive),2.4):rOG*rcpTwelveNineTwo;
    gOG=(gOG > 0.0404482362771082 )?fastPrecisePow(fabs((gOG+0.055)*rcpOFiveFive),2.4):gOG*rcpTwelveNineTwo;
    bOG=(bOG > 0.0404482362771082 )?fastPrecisePow(fabs((bOG+0.055)*rcpOFiveFive),2.4):bOG*rcpTwelveNineTwo;
}else if(lnr==8){
    rOG=fastPrecisePow(rOG,invTwoSix);
    gOG=fastPrecisePow(gOG,invTwoSix);
    bOG=fastPrecisePow(bOG,invTwoSix);
}else if (lnr==5){
    rOG=fastPrecisePow(rOG,2.2);
    gOG=fastPrecisePow(gOG,2.2);
    bOG=fastPrecisePow(bOG,2.2);
}else if (lnr==12){
    rOG=fastPrecisePow(rOG,rcpTwoFour);
    gOG=fastPrecisePow(gOG,rcpTwoFour);
    bOG=fastPrecisePow(bOG,rcpTwoFour);
}else if (lnr==11){
    rOG=fastPrecisePow(rOG,2.4);
    gOG=fastPrecisePow(gOG,2.4);
    bOG=fastPrecisePow(bOG,2.4);
}else if(lnr==3){
    rOG=(rOG < recBetaLin )?rcpFourFive*rOG:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-rOG)),rcpTxFourFive);
    gOG=(gOG < recBetaLin )?rcpFourFive*gOG:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-gOG)),rcpTxFourFive);
    bOG=(bOG < recBetaLin )?rcpFourFive*bOG:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-bOG)),rcpTxFourFive);
}else if(lnr==2){
    rOG=(rOG> 0.00313066844250063)?1.055 * fastPrecisePow(rOG,rcpTwoFour) - 0.055:12.92 *rOG;
    gOG=(gOG> 0.00313066844250063)?1.055 * fastPrecisePow(gOG,rcpTwoFour) - 0.055:12.92 *gOG;
    bOG=(bOG> 0.00313066844250063)?1.055 * fastPrecisePow(bOG,rcpTwoFour) - 0.055:12.92 *bOG;
}else if(lnr==7){
    rOG=fastPrecisePow(rOG,2.6);
    gOG=fastPrecisePow(gOG,2.6);
    bOG=fastPrecisePow(bOG,2.6);
}else if(lnr==6){
    rOG=fastPrecisePow(rOG,invTwoTwo);
    gOG=fastPrecisePow(gOG,invTwoTwo);
    B=fastPrecisePow(bOG,invTwoTwo);
}else if(lnr==4){
    rOG=(rOG< recBeta)?4.5*rOG:recAlpha*fastPrecisePow(rOG,0.45)-(recAlpha-1);
    gOG=(gOG< recBeta)?4.5*gOG:recAlpha*fastPrecisePow(gOG,0.45)-(recAlpha-1);
    bOG=(bOG< recBeta)?4.5*bOG:recAlpha*fastPrecisePow(bOG,0.45)-(recAlpha-1);
}else if (lnr==9){
    rOG=(rOG>0.5)?rcpTwelve*(fastPrecisePow(euler_e,(rOG-HLG_c)*rcp_HLG_a)+HLG_b):rOG*rOG*third;
    gOG=(gOG>0.5)?rcpTwelve*(fastPrecisePow(euler_e,(gOG-HLG_c)*rcp_HLG_a)+HLG_b):gOG*gOG*third;
    bOG=(bOG>0.5)?rcpTwelve*(fastPrecisePow(euler_e,(bOG-HLG_c)*rcp_HLG_a)+HLG_b):bOG*bOG*third;
}else if (lnr==10){
    rOG=(rOG > rcpTwelve)?HLG_a*log(12.0*rOG-HLG_b)+HLG_c:root_three*fastPrecisePow(rOG,0.5);
    gOG=(gOG > rcpTwelve)?HLG_a*log(12.0*gOG-HLG_b)+HLG_c:root_three*fastPrecisePow(gOG,0.5);
    bOG=(bOG > rcpTwelve)?HLG_a*log(12.0*bOG-HLG_b)+HLG_c:root_three*fastPrecisePow(bOG,0.5);
}

rOG=lerp(lw,hi,rOG);
gOG=lerp(lw,hi,gOG);
bOG=lerp(lw,hi,bOG);

  if(mde==1){
        runTot_r+=rOG*0.2124132;
        runTot_g+=gOG*0.7010437;
        runTot_b+=bOG*0.0865432;
    }else if(mde==2){
        runTot_r+=rOG*0.2220379;
        runTot_g+=gOG*0.7066384;
        runTot_b+=bOG*0.0713236;
    }else if(mde==5){ //DCI-P3
        runTot_r+=rOG*0.209491677912731;
        runTot_g+=gOG*0.721595254161044;
        runTot_b+=bOG*0.068913067926226;
    }else if(mde==6){
    runTot_r+=rOG*0.2290036;
    runTot_g+=gOG*0.691726725;
    runTot_b+=bOG*0.079269675;
    }else if ((mde==4)||(mde==11)){
        runTot_r+=rOG*0.26272171736164;
        runTot_g+=gOG*0.677989275502262;
        runTot_b+=bOG*0.059289007136098;
    }else if (mde==7){ //Original NTSC
        runTot_r+=rOG*0.2896886;
        runTot_g+=gOG*0.6056356;
        runTot_b+=bOG*0.1046758;
    }else if (mde==8){ //Rec 601 D93
        runTot_r+=rOG*0.1767506;
        runTot_g+=gOG*0.7072321;
        runTot_b+=bOG*0.1160173;
    }else if (mde==9){ //Rec 709 D93
        runTot_r+=rOG*0.1799632;
        runTot_g+=gOG*0.7231169;
        runTot_b+=bOG*0.0969199;
    }else if (mde==10){ //DCI-P3 D60/ACES
        runTot_r+=rOG*0.23762331020788;
        runTot_g+=gOG*0.689170669198985;
        runTot_b+=bOG*0.073206020593136;
    }else if(mde==12){ //Original NTSC D65
        runTot_r+=rOG*0.2896886;
        runTot_g+=gOG*0.6056356;
        runTot_b+=bOG*0.1046758;
    }else{
        runTot_r+=rOG*0.2126729;
        runTot_g+=gOG*0.7151522;
        runTot_b+=bOG*0.072175;
    }


params->use_R[p_ix]=rOG;
params->use_G[p_ix]=gOG;
params->use_B[p_ix]=bOG;

counter+=1;

        x=(sxf==1)?x+7:x+3;
        p_ix++;
      }
      rrcp+=src_pitch;
      }
p_ix=0;


double mxMean[3]={runTot_r/counter,runTot_g/counter,runTot_b/counter};

  double avg=(mxMean[0]+mxMean[1]+mxMean[2])*third;
  gamma_high=a;

  if(avg==1 || a==0){
    gamma_low=1;
  }else{
    double lt=(avg-(avg*fastPrecisePow(avg,a)))/(1-avg);

    gamma_low=(lt<=0)?1:log(lt)/log(avg);
  }

//appendFormattedToFile(d_file_path, "%f & %f\n",gamma_low, gamma_high);

///////////////ACTUALLY DRAW PIXELS///////////////////////////////////////
      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

double og_R=params->use_R[p_ix];
double og_G=params->use_G[p_ix];
double og_B=params->use_B[p_ix];

double og_RGB[3]={og_R,og_G,og_B};

double og_Y=LinRGB2Y(og_RGB, mde);

double nw_R=(gamma_high==1 && gamma_low==1)?og_R:lerp(fastPrecisePow(og_R,gamma_low),fastPrecisePow(og_R,gamma_high),og_R);
double nw_G=(gamma_high==1 && gamma_low==1)?og_G:lerp(fastPrecisePow(og_G,gamma_low),fastPrecisePow(og_G,gamma_high),og_G);
double nw_B=(gamma_high==1 && gamma_low==1)?og_B:lerp(fastPrecisePow(og_B,gamma_low),fastPrecisePow(og_B,gamma_high),og_B);

double nw_RGB[3]={nw_R,nw_G,nw_B};

double nw_Y=LinRGB2Y(nw_RGB, mde);

R=nw_RGB[0];
G=nw_RGB[1];
B=nw_RGB[2];

if (crs==0 && nw_Y<og_Y){
        R=og_RGB[0];
        G=og_RGB[1];
        B=og_RGB[2];
}else if (crs==1 && og_Y<nw_Y){
        R=og_RGB[0];
        G=og_RGB[1];
        B=og_RGB[2];
}

if(lmr==1){
    R=((R*255*(235-16)*rcptwoFiveFive)+16)*rcptwoFiveFive;
    G=((G*255*(235-16)*rcptwoFiveFive)+16)*rcptwoFiveFive;
    B=((B*255*(235-16)*rcptwoFiveFive)+16)*rcptwoFiveFive;
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
p_ix++;

      }
            srcp += src_pitch;
      }

  return src;
}

void AVSC_CC free_Auto_Gamma(AVS_FilterInfo* fi)
{
   Auto_Gamma* params = (Auto_Gamma*) fi->user_data;
   free(params);
}

AVS_Value AVSC_CC create_Auto_Gamma (AVS_ScriptEnvironment * env,AVS_Value args, void * dg)
{
  AVS_Value v;
  AVS_FilterInfo * fi;
  AVS_Clip * new_clip = avs_new_c_filter(env, &fi, avs_array_elt(args, 0), 1);
  Auto_Gamma *params = (Auto_Gamma*)malloc(sizeof(Auto_Gamma));

if (!params)
      return avs_void;
        params->a = avs_defined(avs_array_elt(args, 1))?avs_as_float(avs_array_elt(args, 1)):0.25;
        params->l = avs_defined(avs_array_elt(args, 2))?avs_as_float(avs_array_elt(args, 2)):0;
        params->h = avs_defined(avs_array_elt(args, 3))?avs_as_float(avs_array_elt(args, 3)):1;
        params->crush = avs_defined(avs_array_elt(args, 4))?avs_as_int(avs_array_elt(args, 4)):0;
        params->limitedRange= avs_defined(avs_array_elt(args, 5))?avs_as_bool(avs_array_elt(args, 5)):false;
        params->mode = avs_defined(avs_array_elt(args, 6))?avs_as_int(avs_array_elt(args, 6)):0;
        params->linear= avs_defined(avs_array_elt(args, 7))?avs_as_int(avs_array_elt(args, 7)):0;

          if ((params->l>1 || params->l<0)||(params->h>1 || params->h<0)){
            return avs_new_value_error ("h and l must be between 0 and 1!");
          }if ((params->mode<0)||(params->mode>12)){
            return avs_new_value_error ("Allowed modes are between 0 and 12!");
          } else if ((params->linear<0)||(params->linear>12)){
            return avs_new_value_error ("Allowed linear values are between 0 and 12!");
          }else{
  if (!((avs_is_rgb32(&fi->vi))||(avs_is_rgb64(&fi->vi)))) {
    return avs_new_value_error ("Input video must be in RGB32 OR RGB64 format!");
  } else {

     if(avs_defined(avs_array_elt(args, 8))){
        params->sixtyFour =avs_as_bool(avs_array_elt(args, 8));
     }else{
       params->sixtyFour = (avs_is_rgb64(&fi->vi))?true:false;
     }
      params->pxels=fi->vi.height*fi->vi.width;

    params->use_R = (double*)malloc( params->pxels* sizeof(double));
    params->use_G= (double*)malloc( params->pxels* sizeof(double));
    params->use_B= (double*)malloc( params->pxels* sizeof(double));

    //DEBUG FILE
    //makeFile(d_file_path);

    fi->user_data = (void*) params;
    fi->get_frame = Auto_Gamma_get_frame;
    v = avs_new_value_clip(new_clip);
      fi->free_filter = free_Auto_Gamma;
  }
          }
  avs_release_clip(new_clip);
  return v;
}


const char * AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment * env)
{
   avs_add_function(env, "Auto_Gamma", "c[a]f[l]f[h]f[crush]i[limitedRange]b[mode]i[linear]i[sixtyFour]b", create_Auto_Gamma, 0);
   return "Auto_Gamma C plugin";
}
