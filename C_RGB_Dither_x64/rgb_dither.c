#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "..\avisynth_c.h"
#include "functions_c.h"

typedef struct RGB_Dither {
    double sdv;
    double dark;
    int sixtyFour;
} RGB_Dither;

AVS_VideoFrame * AVSC_CC RGB_Dither_get_frame (AVS_FilterInfo * p, int n)
{
  AVS_VideoFrame * src;
   RGB_Dither* params = (RGB_Dither*) p->user_data;

  src = avs_get_frame(p->child, n);

    int row_size, height, src_pitch,x, y,sxf;
   BYTE* srcp;

double sddev=params->sdv;
double drk=params->dark;
sxf=params->sixtyFour;

  avs_make_writable(p->env, &src);

      srcp = avs_get_write_ptr(src);
      src_pitch = avs_get_pitch(src);
      row_size = avs_get_row_size(src);
      height = avs_get_height(src);

///////////////ACTUALLY DRAW PIXELS///////////////////////////////////////
      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {


                 double currBlue=(sxf==1)?(double)srcp[x]+srcp[x+1]*256:(double)srcp[x];
                 double currGreen=(sxf==1)?(double)srcp[x+2]+srcp[x+3]*256:(double)srcp[x+1];
                 double currRed=(sxf==1)?(double)srcp[x+4]+srcp[x+5]*256:(double)srcp[x+2];


    double bOG=(sxf==1)?currBlue*rcptHiBit:currBlue*rcptwoFiveFive;     // B
     double gOG=(sxf==1)?currGreen*rcptHiBit:currGreen*rcptwoFiveFive;   //G
      double rOG=(sxf==1)?currRed*rcptHiBit:currRed*rcptwoFiveFive;  // R

double R=rOG;
double G=gOG;
double B=bOG;

double mx_d=MAX(rOG,MAX(gOG,bOG));
uint64_t mx=MAX(currRed,MAX(currGreen,currBlue));
uint64_t mn=MIN(currRed,MIN(currGreen,currBlue));
uint64_t rgb_sum=currRed+currGreen+currBlue;
uint64_t rgb_prod=currRed*currGreen*currBlue;

uint64_t x1 = 0, w1 = 0, s1 =((n+1)*(x+1)*(height+1))*(mx+1);
uint64_t x2 = 0, w2 = 0, s2 =((p->vi.num_frames+1)*(y+1)*(row_size+1))*(mn+1);

uint64_t r1=msws53(x1, w1, s1, x2, w2, s2);

uint64_t f_nm=p->vi.fps_numerator;
f_nm=(f_nm>1)?f_nm:1;

uint64_t f_dnm=p->vi.fps_denominator;
f_dnm=(f_dnm>1)?f_dnm:1;

s1 =((f_nm)*(x+1)*(row_size+1))*(rgb_prod+1)*(n+1);
s2=((f_dnm)*(y+1)*(height+1))*(rgb_sum+1)*(n+1);

uint64_t r2=msws53(x1, w1, s1, x2, w2, s2);

int xp;
double r12 =(double)( (frexp (r1*r2 , &xp)-0.5)*2);

 r12=logisticMap(r12);

double dth=grey_dither(mx_d,r12,sddev,sxf);

R=(mx_d==0)?dth:dth*(R/mx_d);
G=(mx_d==0)?dth:dth*(G/mx_d);
B=(mx_d==0)?dth:dth*(B/mx_d);

R=lerp(R,lerp(R,rOG,mx_d),drk);
G=lerp(G,lerp(G,gOG,mx_d),drk);
B=lerp(B,lerp(B,bOG,mx_d),drk);

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


AVS_Value AVSC_CC create_RGB_Dither (AVS_ScriptEnvironment * env,AVS_Value args, void * dg)
{
  AVS_Value v;
  AVS_FilterInfo * fi;
  AVS_Clip * new_clip = avs_new_c_filter(env, &fi, avs_array_elt(args, 0), 1);
  RGB_Dither *params = (RGB_Dither*)malloc(sizeof(RGB_Dither));

if (!params)
      return avs_void;

        params->sdv= avs_defined(avs_array_elt(args, 1))?avs_as_float(avs_array_elt(args, 1)):1.8;
        params->dark= avs_defined(avs_array_elt(args, 2))?avs_as_float(avs_array_elt(args, 2)):0.5;

  if (!((avs_is_rgb32(&fi->vi))||(avs_is_rgb64(&fi->vi)))) {
    return avs_new_value_error ("Input video must be in RGB32 OR RGB64 format!");
  } else if(params->sdv<0){
       return avs_new_value_error ("sdv must be >0!");
  } else if(params->dark<0 || params->dark>1){
       return avs_new_value_error ("dark must be between 0 and 1!");
  }else {

     if(avs_defined(avs_array_elt(args, 3))){
        params->sixtyFour =avs_as_bool(avs_array_elt(args, 3));
     }else{
       params->sixtyFour = (avs_is_rgb64(&fi->vi))?true:false;
     }
         fi->user_data = (void*) params;
    fi->get_frame = RGB_Dither_get_frame;
    v = avs_new_value_clip(new_clip);
  }
  avs_release_clip(new_clip);
  return v;
}

const char * AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment * env)
{
   avs_add_function(env, "RGB_Dither", "c[sdv]f[dark]f[sixtyFour]b", create_RGB_Dither, 0);
   return "RGB_Dither C plugin";
}
