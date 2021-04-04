#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "..\avisynth_c.h"
#include "functions_c.h"

typedef struct Saturation_Percentiles {
    double perc;
    int sixtyFour;
} Saturation_Percentiles;

AVS_VideoFrame * AVSC_CC Saturation_Percentiles_get_frame (AVS_FilterInfo * p, int n)
{
  AVS_VideoFrame * src;
   Saturation_Percentiles* params = (Saturation_Percentiles*) p->user_data;

  src = avs_get_frame(p->child, n);

    int row_size, height, src_pitch,x, y,sxf;
    long pxls;
  const BYTE* rrcp;
   BYTE* srcp;
   double bOG,gOG,rOG,prc,d_pxls;

prc=params->perc;
sxf=params->sixtyFour;

  avs_make_writable(p->env, &src);

      srcp = avs_get_write_ptr(src);
        rrcp = avs_get_read_ptr(src);
      src_pitch = avs_get_pitch(src);
      row_size = avs_get_row_size(src);
      height = avs_get_height(src);



long k=0;
pxls=height*row_size;
d_pxls=(double)(pxls);

            double *sats = (double*)malloc( pxls* sizeof(double));

                        for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

                 double currBlue=(sxf==1)?(double)rrcp[x]+rrcp[x+1]*256:(double)rrcp[x];
                 double currGreen=(sxf==1)?(double)rrcp[x+2]+rrcp[x+3]*256:(double)rrcp[x+1];
                 double currRed=(sxf==1)?(double)rrcp[x+4]+rrcp[x+5]*256:(double)rrcp[x+2];


    bOG=(sxf==1)?currBlue*rcptHiBit:currBlue*rcptwoFiveFive;     // B
       gOG=(sxf==1)?currGreen*rcptHiBit:currGreen*rcptwoFiveFive;   //G
         rOG=(sxf==1)?currRed*rcptHiBit:currRed*rcptwoFiveFive;     // R


             double mx=MAX(rOG,MAX(gOG,bOG));
    double sat=(mx==0)?0:(mx-MIN(rOG,MIN(gOG,bOG)))/mx;

         sats[k]=sat;

        k++;
      }
            rrcp += src_pitch;
      }

 qsort(sats, pxls, sizeof(double), compare);
long refEl=ceil(prc*d_pxls)-1;
  double refSat=sats[refEl];

///////////////ACTUALLY DRAW PIXELS///////////////////////////////////////
      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {
                 double currBlue=(sxf==1)?(double)srcp[x]+srcp[x+1]*256:(double)srcp[x];
                 double currGreen=(sxf==1)?(double)srcp[x+2]+srcp[x+3]*256:(double)srcp[x+1];
                 double currRed=(sxf==1)?(double)srcp[x+4]+srcp[x+5]*256:(double)srcp[x+2];

          bOG=(sxf==1)?currBlue*rcptHiBit:currBlue*rcptwoFiveFive;     // B
       gOG=(sxf==1)?currGreen*rcptHiBit:currGreen*rcptwoFiveFive;   //G
         rOG=(sxf==1)?currRed*rcptHiBit:currRed*rcptwoFiveFive;     // R


             double mx=MAX(rOG,MAX(gOG,bOG));
    double sat=(mx==0)?0:(mx-MIN(rOG,MIN(gOG,bOG)))/mx;

if(sat>refSat){
srcp[x] =0; //blue : blue
srcp[x+1] =(sxf==1)?0:0; // blue : green
srcp[x+2] =(sxf==1)? 0:0; // green: red
srcp[x+3] =(sxf==1)? 0:srcp[x+3]; //green : self
srcp[x+4] =(sxf==1)? 0:srcp[x+4]; //red : self
srcp[x+5] =(sxf==1)? 0:srcp[x+5]; //red : self
}

x=(sxf==1)?x+7:x+3;

      }
            srcp += src_pitch;
      }

      free(sats);

  return src;
}

AVS_Value AVSC_CC create_Saturation_Percentiles (AVS_ScriptEnvironment * env,AVS_Value args, void * dg)
{
  AVS_Value v;
  AVS_FilterInfo * fi;
  AVS_Clip * new_clip = avs_new_c_filter(env, &fi, avs_array_elt(args, 0), 1);
  Saturation_Percentiles *params = (Saturation_Percentiles*)malloc(sizeof(Saturation_Percentiles));

if (!params)
      return avs_void;

        params->perc= avs_defined(avs_array_elt(args, 1))?avs_as_float(avs_array_elt(args, 1)):1;

  if (!((avs_is_rgb32(&fi->vi))||(avs_is_rgb64(&fi->vi)))) {
    return avs_new_value_error ("Input video must be in RGB32 OR RGB64 format!");
  } else {

     if(avs_defined(avs_array_elt(args, 2))){
        params->sixtyFour =avs_as_bool(avs_array_elt(args, 2));
     }else{
       params->sixtyFour = (avs_is_rgb64(&fi->vi))?true:false;
     }
         fi->user_data = (void*) params;
    fi->get_frame = Saturation_Percentiles_get_frame;
    v = avs_new_value_clip(new_clip);
  }

  avs_release_clip(new_clip);
  return v;
}

const char * AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment * env)
{
   avs_add_function(env, "Saturation_Percentiles", "c[perc]f[sixtyFour]b", create_Saturation_Percentiles, 0);
   return "Saturation_Percentiles C plugin";
}
