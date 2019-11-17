#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../avisynth_c.h"
#include "functions_c.h"

typedef struct WhitePoint {
      double desat;
    int debug;
        double start;
} WhitePoint;


AVS_VideoFrame* AVSC_CC WhitePoint_get_frame(AVS_FilterInfo* fi, int n)
{
 WhitePoint* params = (WhitePoint*) fi->user_data;

   AVS_VideoFrame* src = avs_get_frame(fi->child, n);



   int row_size, height, src_pitch,x, y, p,dbg,itr,lmr,bestEl;
   BYTE* srcp;
   double CIEx,CIEy,rOG,bOG,gOG,strt,HWblack,grey_metric,mn,mx,sat,cumulSSD,dblItr,dblP,bestSSD,man_x,man_y,a_OG,b_OG,tol,a,b,c,f_a,f_c,dst;


dst=params->desat;
dbg=params->debug;
strt=params->start;



   CIEx= 0.312727;
      CIEy= 0.329023;
      double D65XYZ[3]={0.95047,1,1.08883};
            int planes[] ={AVS_CS_BGR32};
src = avs_get_frame(fi->child, n);
   avs_make_writable(fi->env, &src);

p=0;
      srcp = avs_get_write_ptr_p(src, planes[p]);
      src_pitch = avs_get_pitch_p(src, planes[p]);
      row_size = avs_get_row_size_p(src, planes[p]);
      height = avs_get_height_p(src, planes[p]);



double r_desat_sum= 0;
double g_desat_sum= 0;
double b_desat_sum= 0;

double r_tint_sum= 0;
double g_tint_sum= 0;
double b_tint_sum= 0;



double r_avg_sum= 0;
double g_avg_sum= 0;
double b_avg_sum= 0;

double counter=0;


///////////////ACTUALLY DRAW PIXELS///////////////////////////////////////

      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {



double x_shift=(double)x/(double)row_size;
                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue/255.0;     // B
       gOG=currGreen/255.0;   //G
         rOG=currRed/255.0;  // R

///////////TO CHANGE WHITE POINT////////////////////////

double curr_rgb_dst[3]={rOG,gOG,bOG};

double curr_rgbLin_dst[3];
sRGB2Linear(curr_rgb_dst,curr_rgbLin_dst);
double rgb_hsi_dst[3];
RGB2HSI(curr_rgbLin_dst,rgb_hsi_dst);
double rgb_hsv_dst[3];
rgb2hsv(curr_rgbLin_dst,rgb_hsv_dst);
double rgb_hsv_dst_hwb[3];
hsv2hwb(rgb_hsv_dst,rgb_hsv_dst_hwb);
double rgb_hsv_dst_hsl[3];
hsv2hsl(rgb_hsv_dst,rgb_hsv_dst_hsl);
double curr_rgbLin_dst_xyY[3];
rgb2xyY(curr_rgbLin_dst,curr_rgbLin_dst_xyY);
double curr_rgbLin_dst_XYZ[3];
xyY2XYZ(curr_rgbLin_dst_xyY,curr_rgbLin_dst_XYZ);
double curr_rgbLin_dst_Lab[3];
xyY2XYZ(curr_rgbLin_dst_xyY,curr_rgbLin_dst_Lab);
double curr_rgbLin_dst_Lch[3];
xyY2XYZ(curr_rgbLin_dst_xyY,curr_rgbLin_dst_Lch);


double satSc=(1-rgb_hsv_dst[1])+1;
double hSc=(1-curr_rgbLin_dst_Lch[2])+1;
double sat_lSc=(1-rgb_hsv_dst_hsl[1])+1;

double cSc=(1-curr_rgbLin_dst_Lch[1])+1;

double wSc=(rgb_hsv_dst_hwb[1])+1;
double bSc=(rgb_hsv_dst_hwb[2])+1;

double sat_hsiSc=(1-rgb_hsi_dst[1])+1;

double YSc=(curr_rgbLin_dst_xyY[2])+1;

float ChromaSc=(rgb_hsv_dst[1]*rgb_hsv_dst[2])+1;

double grey_metric_dst=grey_metric=(satSc*sat_hsiSc*wSc*bSc*ChromaSc-1)/(2*2*2*2*2-1);
 grey_metric_dst*=YSc*0.5;


//float lerper=grey_metric;

rgb_hsv_dst[1]=MAX(MIN(rgb_hsv_dst[1]-lerp(0,dst,grey_metric_dst),1),0);

double curr_rgb_desat[3];
hsv2rgb(rgb_hsv_dst,curr_rgb_desat);

//float4 c1Lin=c1;

double curr_rgb_desat_gc[3];
Linear2sRGB(curr_rgb_desat,curr_rgb_desat_gc);


double curr_tint_rgb[3];
curr_tint_rgb[0]=1-fabs(curr_rgb_desat[0]-curr_rgbLin_dst[0]);
curr_tint_rgb[1]=1-fabs(curr_rgb_desat[1]-curr_rgbLin_dst[1]);
curr_tint_rgb[2]=1-fabs(curr_rgb_desat[2]-curr_rgbLin_dst[2]);


//double mxTint=MAX(curr_tint_rgb[0],MAX(curr_tint_rgb[1],curr_tint_rgb[2]));



if(grey_metric_dst<=strt ){
 r_desat_sum+=curr_rgb_desat[0];
 g_desat_sum+=curr_rgb_desat[1];
 b_desat_sum+=curr_rgb_desat[2];

  r_tint_sum+=curr_tint_rgb[0];
 g_tint_sum+=curr_tint_rgb[1];
 b_tint_sum+=curr_tint_rgb[2];

  r_avg_sum+=curr_rgbLin_dst[0];
 g_avg_sum+=curr_rgbLin_dst[1];
 b_avg_sum+=curr_rgbLin_dst[2];
counter+=1;
}

x+=3;

}

 // srcp += src_pitch;
////////////////////////////////////////////////////

      }

       //END OF IMAGE DRAWING LOOP

/////////////////DRAW PIXELS END/////////////////////////////////




double avg_rgb[3]={r_avg_sum/counter,g_avg_sum/counter,b_avg_sum,counter};
double avg_rgb_gc[3];
Linear2sRGB(avg_rgb,avg_rgb_gc);

double XYZ_greyed_avg_rgb[3];
double avg_rgb_xyY[3];
rgb2xyY(avg_rgb,avg_rgb_xyY);
double avg_rgb_xy[2]={avg_rgb_xyY[0],avg_rgb_xyY[1]};
double avg_rgb_XYZ[3];
xy2XYZ(avg_rgb_xy,avg_rgb_XYZ);


double desat_avg_rgb[3]={r_desat_sum/counter,g_desat_sum/counter,b_desat_sum/counter};
double desat_avg_rgb_bk[3];
Linear2sRGB(desat_avg_rgb,desat_avg_rgb_bk);

double desat_avg_rgb_xyY[3];
rgb2xyY(desat_avg_rgb_bk,desat_avg_rgb_xyY);
double desat_avg_rgb_xy[2]={desat_avg_rgb_xyY[0],desat_avg_rgb_xyY[1]};
double desat_avg_rgb_XYZ[3];
xy2XYZ(desat_avg_rgb_xy,desat_avg_rgb_XYZ);

double tint_avg_rgb[3]={r_tint_sum/counter,g_tint_sum/counter,b_tint_sum/counter};

double tint_avg_rgb_bk[3];
Linear2sRGB(tint_avg_rgb,tint_avg_rgb_bk);

double tint_avg_rgb_xyY[3];
rgb2xyY(tint_avg_rgb,tint_avg_rgb_xyY);
double tint_avg_rgb_xy[2]={tint_avg_rgb_xyY[0],tint_avg_rgb_xyY[1]};
double tint_avg_rgb_XYZ[3];
xy2XYZ(tint_avg_rgb_xy,tint_avg_rgb_XYZ);

double XYZ_convert[3];

WPconv2Grey (tint_avg_rgb_XYZ,D65XYZ,XYZ_convert);



      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {



double x_shift=(double)x/(double)row_size;
                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];


bOG=currBlue/255.0;     // B
       gOG=currGreen/255.0;   //G
         rOG=currRed/255.0;  // R


double x_shft=(double)x/(double)row_size;
///////////TO CHANGE WHITE POINT////////////////////////

double px_rgb[3]={rOG,gOG,bOG};

if(dbg==1){

        double curr_rgbLin_fnl[3];
sRGB2Linear(px_rgb,curr_rgbLin_fnl);
double rgb_hsi_fnl[3];
RGB2HSI(curr_rgbLin_fnl,rgb_hsi_fnl);
double rgb_hsv_fnl[3];
rgb2hsv(curr_rgbLin_fnl,rgb_hsv_fnl);
double rgb_hsv_fnl_hwb[3];
hsv2hwb(rgb_hsv_fnl,rgb_hsv_fnl_hwb);
double rgb_hsv_fnl_hsl[3];
hsv2hsl(rgb_hsv_fnl,rgb_hsv_fnl_hsl);
double curr_rgbLin_fnl_xyY[3];
rgb2xyY(curr_rgbLin_fnl,curr_rgbLin_fnl_xyY);
double curr_rgbLin_fnl_XYZ[3];
xyY2XYZ(curr_rgbLin_fnl_xyY,curr_rgbLin_fnl_XYZ);
double curr_rgbLin_fnl_Lab[3];
xyY2XYZ(curr_rgbLin_fnl_xyY,curr_rgbLin_fnl_Lab);
double curr_rgbLin_fnl_Lch[3];
xyY2XYZ(curr_rgbLin_fnl_xyY,curr_rgbLin_fnl_Lch);


double satSc_fnl=(1-rgb_hsv_fnl[1])+1;
double hSc_fnl=(1-curr_rgbLin_fnl_Lch[2])+1;
double sat_lSc_fnl=(1-rgb_hsv_fnl_hsl[1])+1;

double cSc_fnl=(1-curr_rgbLin_fnl_Lch[1])+1;

double wSc_fnl=(rgb_hsv_fnl_hwb[1])+1;
double bSc_fnl=(rgb_hsv_fnl_hwb[2])+1;

double sat_hsiSc_fnl=(1-rgb_hsi_fnl[1])+1;

double YSc_fnl=(curr_rgbLin_fnl_xyY[2])+1;

double grey_metric_fnl=(satSc_fnl*sat_hsiSc_fnl*wSc_fnl*bSc_fnl-1)/(2*2*2*2-1);
 grey_metric_fnl*=YSc_fnl*0.5;

    if (grey_metric_fnl>strt){

        srcp[x] = 127;
                srcp[x+1] =255;
        srcp[x+2] = 0;
    }
}else{

    double rgbxyY[3];
double rgbXYZ[3];
double WPConvXYZ[3];
double WPConvXYZ_xyY[3];
double WPchgRGB[3];
rgb2xyY(px_rgb,rgbxyY);
xyY2XYZ(rgbxyY,rgbXYZ);

WPconv(rgbXYZ,D65XYZ,XYZ_convert,WPConvXYZ);

XYZ2xyY(WPConvXYZ,WPConvXYZ_xyY);

xyY2rgb(WPConvXYZ_xyY,WPchgRGB);

if (x_shft>0.5){
                srcp[x] = MAX(MIN(round(WPchgRGB[2]*255),255),0);
             srcp[x+1] =MAX(MIN(round(WPchgRGB[1]*255),255),0);
        srcp[x+2] = MAX(MIN(round(WPchgRGB[0]*255),255),0);


}


}
////////////////////////////////////////////////////

x+=3;
      }
            srcp += src_pitch;
      } //END OF IMAGE DRAWING LOOP

/////////////////DRAW PIXELS END/////////////////////////////////



   return src;
}


AVS_Value AVSC_CC create_WhitePoint(AVS_ScriptEnvironment* env, AVS_Value args, void* user_data)
{
   AVS_Value v;
   AVS_FilterInfo* fi;


   AVS_Clip* new_clip = avs_new_c_filter(env, &fi, avs_array_elt(args, 0), 1);

WhitePoint *params = (WhitePoint*)malloc(sizeof(WhitePoint));

if (!params)
      return avs_void;



   if (!avs_is_rgb(&fi->vi)) {
      return avs_new_value_error("Input video must be in RGB format!");
   }



        params->desat = avs_defined(avs_array_elt(args, 1))?avs_as_float(avs_array_elt(args, 1)):0;
        params->start = avs_defined(avs_array_elt(args, 3))?avs_as_float(avs_array_elt(args, 3)):1;

       params->debug = avs_defined(avs_array_elt(args, 2))?avs_as_bool(avs_array_elt(args, 2)):false;


   fi->user_data = (void*) params;
   fi->get_frame = WhitePoint_get_frame;


   v = avs_new_value_clip(new_clip);

   avs_release_clip(new_clip);
  // free(params);
   return v;
}

const char* AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment* env)
{
   avs_add_function(env, "WhitePoint", "c[desat]f[debug]b[start]f", create_WhitePoint, 0);
   return "WhitePoint sample C plugin";
}
