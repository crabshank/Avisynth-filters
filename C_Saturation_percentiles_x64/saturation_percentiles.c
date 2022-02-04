#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "..\avisynth_c.h"
#include "functions_c.h"

typedef struct Saturation_Percentiles {
    int below;
    int out;
    int sixtyFour;
    int pxels;
    double *sats;
    double *satsOG;
    int refEl_lo;
    int refEl_hi;
    double refEl_lerp;
    int refEl;
    int gOut;
} Saturation_Percentiles;

AVS_VideoFrame * AVSC_CC Saturation_Percentiles_get_frame (AVS_FilterInfo * p, int n)
{
  AVS_VideoFrame * src;
   Saturation_Percentiles* params = (Saturation_Percentiles*) p->user_data;

  src = avs_get_frame(p->child, n);

    int row_size, height, src_pitch,x, y,sxf,blw,gOt;
    long pxls;
  const BYTE* rrcp;
   BYTE* srcp;
   double bOG,gOG,rOG,ot;

sxf=params->sixtyFour;
blw=params->below;
ot=params->out;
gOt=params->gOut;


  avs_make_writable(p->env, &src);

      srcp = avs_get_write_ptr(src);
        rrcp = avs_get_read_ptr(src);
      src_pitch = avs_get_pitch(src);
      row_size = avs_get_row_size(src);
      height = avs_get_height(src);

long k=0;
pxls=params->pxels;
double* sats=params->sats;
double* satsOG=params->satsOG;
                        for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

                 double currBlue=(sxf==1)?(double)rrcp[x]+rrcp[x+1]*256:(double)rrcp[x];
                 double currGreen=(sxf==1)?(double)rrcp[x+2]+rrcp[x+3]*256:(double)rrcp[x+1];
                 double currRed=(sxf==1)?(double)rrcp[x+4]+rrcp[x+5]*256:(double)rrcp[x+2];


    bOG=(sxf==1)?currBlue*rcptHiBit:currBlue*rcptwoFiveFive;     // B
       gOG=(sxf==1)?currGreen*rcptHiBit:currGreen*rcptwoFiveFive;   //G
         rOG=(sxf==1)?currRed*rcptHiBit:currRed*rcptwoFiveFive;     // R


             double mx=MAX(rOG,MAX(gOG,bOG));
             double mn=MIN(rOG,MIN(gOG,bOG));
             double chr=mx-mn;
             double sat;
             double mtr=chr;

                //out: 0->sat, 1->chr, 2->mcs

             if(ot!=1){ // sat || mcs
                sat=(mx==0)?0:(chr)/mx;

                if(ot==2){ //mcs
                    mtr=MIN(sat,chr);
                }else{ //sat
                    mtr=sat;
                }

             }


         sats[k]=mtr;
         satsOG[k]=mtr;

        k++;
        x=(sxf==1)?x+7:x+3;
      }
            rrcp += src_pitch;
      }

 qsort(sats, pxls, sizeof(double), compare);
double refSat=(params->refEl!=-1)?sats[params->refEl]:lerp(sats[params->refEl_lo],sats[params->refEl_hi],params->refEl_lerp);

k=0;

///////////////ACTUALLY DRAW PIXELS///////////////////////////////////////
      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

if(blw==1){
if(satsOG[k]>refSat){
srcp[x] =gOt; //blue : blue
srcp[x+1] =(sxf==1)?gOt:gOt; // blue : green
srcp[x+2] =(sxf==1)? gOt:gOt; // green: red
srcp[x+3] =(sxf==1)? gOt:srcp[x+3]; //green : self
srcp[x+4] =(sxf==1)? gOt:srcp[x+4]; //red : self
srcp[x+5] =(sxf==1)? gOt:srcp[x+5]; //red : self
}
}else{
if(satsOG[k]<refSat){
srcp[x] =gOt; //blue : blue
srcp[x+1] =(sxf==1)?gOt:gOt; // blue : green
srcp[x+2] =(sxf==1)? gOt:gOt; // green: red
srcp[x+3] =(sxf==1)? gOt:srcp[x+3]; //green : self
srcp[x+4] =(sxf==1)? gOt:srcp[x+4]; //red : self
srcp[x+5] =(sxf==1)? gOt:srcp[x+5]; //red : self
}
}

k++;
x=(sxf==1)?x+7:x+3;

      }
            srcp += src_pitch;
      }



  return src;
}

void AVSC_CC free_Saturation_Percentiles(AVS_FilterInfo* fi)
{
    Saturation_Percentiles* params = (Saturation_Percentiles*) fi->user_data;
    free(params);
}

AVS_Value AVSC_CC create_Saturation_Percentiles (AVS_ScriptEnvironment * env,AVS_Value args, void * dg)
{
  AVS_Value v;
  AVS_FilterInfo * fi;
  AVS_Clip * new_clip = avs_new_c_filter(env, &fi, avs_array_elt(args, 0), 1);
  Saturation_Percentiles *params = (Saturation_Percentiles*)malloc(sizeof(Saturation_Percentiles));

if (!params)
      return avs_void;

       double perc= avs_defined(avs_array_elt(args, 1))?avs_as_float(avs_array_elt(args, 1)):1;
        params->below= avs_defined(avs_array_elt(args, 2))?avs_as_bool(avs_array_elt(args, 2)):true;
        params->out= avs_defined(avs_array_elt(args, 3))?avs_as_int(avs_array_elt(args, 3)):0;
        int col= avs_defined(avs_array_elt(args, 4))?avs_as_int(avs_array_elt(args, 4)):0;

        double gOtD;

        switch(col){
            case 1:
             gOtD=0.5;
            break;

            case 2:
              gOtD=1;
            break;

            default:
              gOtD=0;
            break;
        }

  if (!((avs_is_rgb32(&fi->vi))||(avs_is_rgb64(&fi->vi)))) {
    return avs_new_value_error ("Input video must be in RGB32 OR RGB64 format!");
  }else if (params->out<0 || params->out>2) {
    return avs_new_value_error ("\"out\" must be between 0 and 2!");
  }else if (col<0 || col>2) {
    return avs_new_value_error ("\"col\" must be between 0 and 2!");
  } else {

     if(avs_defined(avs_array_elt(args, 5))){
        params->sixtyFour =avs_as_bool(avs_array_elt(args, 5));
     }else{
       params->sixtyFour = (avs_is_rgb64(&fi->vi))?true:false;
     }
     params->gOut=(params->sixtyFour==1)?round(gOtD*65535):round(gOtD*255);
     params->pxels=fi->vi.height*fi->vi.width;
     double d_pxels=(double)params->pxels;
    if(params->pxels==1){
        params->refEl=1;
    }else{
        double refEl_d=((d_pxels+1)*perc)-1;
        params->refEl_lo=floor(refEl_d);
        params->refEl_hi=ceil(refEl_d);
        params->refEl_lerp=refEl_d-floor(refEl_d);
        params->refEl=(params->refEl_lo==params->refEl_hi)?params->refEl_lo:-1;
    }

            params->sats = (double*)malloc( params->pxels* sizeof(double));
            params->satsOG = (double*)malloc( params->pxels* sizeof(double));

         fi->user_data = (void*) params;
    fi->get_frame = Saturation_Percentiles_get_frame;
    v = avs_new_value_clip(new_clip);
    fi->free_filter = free_Saturation_Percentiles;
  }
  avs_release_clip(new_clip);
  return v;
}

const char * AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment * env)
{
   avs_add_function(env, "Saturation_Percentiles", "c[perc]f[below]b[out]i[col]i[sixtyFour]b", create_Saturation_Percentiles, 0);
   return "Saturation_Percentiles C plugin";
}
