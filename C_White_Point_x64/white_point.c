#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../avisynth_c.h"
#include "functions_c.h"

typedef struct WhitePoint {
    int debug;
        double start;
        double x;
        double y;
} WhitePoint;


AVS_VideoFrame* AVSC_CC WhitePoint_get_frame(AVS_FilterInfo* fi, int n)
{
 WhitePoint* params = (WhitePoint*) fi->user_data;

   AVS_VideoFrame* src = avs_get_frame(fi->child, n);

   int row_size, height, src_pitch,x, y, p,dbg;
   BYTE* srcp;
   double CIEx,CIEy,rOG,bOG,gOG,strt,cust_x,cust_y;

dbg=params->debug;
strt=params->start;
cust_x=params->x;
cust_y=params->y;

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

double r_avg_sum= 0;
double g_avg_sum= 0;
double b_avg_sum= 0;

double grey_metric_avg_sum=0;

double counter=0;

double rcp_twoFiveFive=pow(255,-1);


///////////////ACTUALLY DRAW PIXELS///////////////////////////////////////

      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {



//double x_shift=(double)x/(double)row_size;
                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcp_twoFiveFive;     // B
       gOG=currGreen*rcp_twoFiveFive;   //G
         rOG=currRed*rcp_twoFiveFive;     // R

double curr_rgb_dst[3]={rOG,gOG,bOG};
double curr_rgb_dst_lin[3];
sRGB2Linear(curr_rgb_dst,curr_rgb_dst_lin);

double rgb_hsi_dst[3];
RGB2HSI(curr_rgb_dst,rgb_hsi_dst);
double rgb_hsv_dst[3];
rgb2hsv(curr_rgb_dst,rgb_hsv_dst);
double rgb_hsv_dst_hwb[3];
hsv2hwb(rgb_hsv_dst,rgb_hsv_dst_hwb);
double rgb_hsv_dst_hsl[3];
hsv2hsl(rgb_hsv_dst,rgb_hsv_dst_hsl);
double curr_rgbLin_dst_xyY[3];
rgb2xyY(curr_rgb_dst,curr_rgbLin_dst_xyY);
/*
double curr_rgbLin_dst_XYZ[3];
xyY2XYZ(curr_rgbLin_dst_xyY,curr_rgbLin_dst_XYZ);

double curr_rgbLin_dst_Lab[3];
xyY2XYZ(curr_rgbLin_dst_xyY,curr_rgbLin_dst_Lab);
double curr_rgbLin_dst_Lch[3];
xyY2XYZ(curr_rgbLin_dst_xyY,curr_rgbLin_dst_Lch);
*/

double satSc=(1-rgb_hsv_dst[1])+1;
//double hSc=(1-curr_rgbLin_dst_Lch[2])+1;
//double sat_lSc=(1-rgb_hsv_dst_hsl[1])+1;

//double cSc=(1-curr_rgbLin_dst_Lch[1])+1;

double wSc=(rgb_hsv_dst_hwb[1])+1;
double bSc=(rgb_hsv_dst_hwb[2])+1;

double sat_hsiSc=(1-rgb_hsi_dst[1])+1;

double YSc=(curr_rgbLin_dst_xyY[2])+1;

double ChromaSc=(rgb_hsv_dst[1]*rgb_hsv_dst[2])+1;

double Chroma_hsl_Sc=(1-rgb_hsv_dst_hsl[1]*rgb_hsv_dst_hsl[2])+1;

double Chroma_hsi_Sc=(1-rgb_hsi_dst[1]*rgb_hsi_dst[2])+1;

double grey_metric_dst=(satSc*sat_hsiSc*wSc*bSc*ChromaSc*Chroma_hsl_Sc*Chroma_hsi_Sc-1)/(2*2*2*2*2*2*2-1);
grey_metric_dst*=YSc*0.5;



rgb_hsv_dst[1]=0;
//MAX(rgb_hsv_dst[1]-dst*grey_metric_dst,0);

double curr_rgb_desat[3];
hsv2rgb(rgb_hsv_dst,curr_rgb_desat);
double curr_rgb_desat_lin[3];
sRGB2Linear(curr_rgb_desat,curr_rgb_desat_lin);

if(grey_metric_dst<=strt && (grey_metric_dst<1)){

r_avg_sum+=curr_rgb_dst_lin[0];
g_avg_sum+=curr_rgb_dst_lin[1];
b_avg_sum+=curr_rgb_dst_lin[2];
grey_metric_avg_sum+=grey_metric_dst;
r_desat_sum+=curr_rgb_desat_lin[0];
g_desat_sum+=curr_rgb_desat_lin[1];
b_desat_sum+=curr_rgb_desat_lin[2];

counter=counter+1;
}

x+=3;

}

 // srcp += src_pitch;

      }

double rcp_counter=(counter==0)?1:pow(counter,-1);

double avg_rgb[3];
 avg_rgb[0]=r_avg_sum*rcp_counter;
 avg_rgb[1]=g_avg_sum*rcp_counter;
 avg_rgb[2]=b_avg_sum*rcp_counter;

double desat_avg_rgb[3];

 desat_avg_rgb[0]=r_desat_sum*rcp_counter;
 desat_avg_rgb[1]=r_desat_sum*rcp_counter;
 desat_avg_rgb[2]=r_desat_sum*rcp_counter;

 double avg_gm=(rcp_counter==0)?1:grey_metric_avg_sum*rcp_counter;

 avg_gm=MAX(0,MIN(avg_gm,1));


double desat_avg_rgb2[3];
 desat_avg_rgb2[0]=0.5*((avg_rgb[0]-desat_avg_rgb[0])-1)*(1-avg_gm);
 desat_avg_rgb2[1]=0.5*((avg_rgb[1]-desat_avg_rgb[1])-1)*(1-avg_gm);
 desat_avg_rgb2[2]=0.5*( (avg_rgb[2]-desat_avg_rgb[2])-1)*(1- avg_gm);
/*
double shift = 1 - MIN(desat_avg_rgb2[0], MIN(desat_avg_rgb2[1], desat_avg_rgb2[2])) - MAX(desat_avg_rgb2[0], MAX(desat_avg_rgb2[1], desat_avg_rgb2[2]));
//Source: https://github.com/vn971/linux-color-inversion/blob/master/shift.glsl

desat_avg_rgb2[0]=1-(desat_avg_rgb2[0]+shift)*avg_gm;
desat_avg_rgb2[1]=1-(desat_avg_rgb2[1]+shift)*avg_gm;
desat_avg_rgb2[2]=1-(desat_avg_rgb2[2]+shift)*avg_gm;
*/
double desat_avg_rgb_xyY[3];
LinRGB2xyY(desat_avg_rgb2,desat_avg_rgb_xyY);
double desat_avg_rgb_xy[2]={desat_avg_rgb_xyY[0],desat_avg_rgb_xyY[1]};
double desat_avg_rgb_XYZ[3];
xy2XYZ(desat_avg_rgb_xy,desat_avg_rgb_XYZ);

double XYZ_convert[3];

WPconv2Grey (D65XYZ,desat_avg_rgb_XYZ,XYZ_convert);

      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

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
/*
        double curr_rgbLin_fnl[3];
sRGB2Linear(px_rgb,curr_rgbLin_fnl);
*/
double rgb_hsi_fnl[3];
RGB2HSI(px_rgb,rgb_hsi_fnl);
double rgb_hsv_fnl[3];
rgb2hsv(px_rgb,rgb_hsv_fnl);
double rgb_hsv_fnl_hwb[3];
hsv2hwb(rgb_hsv_fnl,rgb_hsv_fnl_hwb);
double rgb_hsv_fnl_hsl[3];
hsv2hsl(rgb_hsv_fnl,rgb_hsv_fnl_hsl);
double curr_rgbLin_fnl_xyY[3];
rgb2xyY(px_rgb,curr_rgbLin_fnl_xyY);
/*
double curr_rgbLin_fnl_XYZ[3];
xyY2XYZ(curr_rgbLin_fnl_xyY,curr_rgbLin_fnl_XYZ);
double curr_rgbLin_fnl_Lab[3];
xyY2XYZ(curr_rgbLin_fnl_xyY,curr_rgbLin_fnl_Lab);
double curr_rgbLin_fnl_Lch[3];
xyY2XYZ(curr_rgbLin_fnl_xyY,curr_rgbLin_fnl_Lch);
*/

double satSc_fnl=(1-rgb_hsv_fnl[1])+1;
//double hSc_fnl=(1-curr_rgbLin_fnl_Lch[2])+1;
//double sat_lSc_fnl=(1-rgb_hsv_fnl_hsl[1])+1;

//double cSc_fnl=(1-curr_rgbLin_fnl_Lch[1])+1;

double wSc_fnl=(rgb_hsv_fnl_hwb[1])+1;
double bSc_fnl=(rgb_hsv_fnl_hwb[2])+1;

double sat_hsiSc_fnl=(1-rgb_hsi_fnl[1])+1;

double YSc_fnl=(curr_rgbLin_fnl_xyY[2])+1;

double ChromaSc_fnl=(rgb_hsv_fnl[1]*rgb_hsv_fnl[2])+1;

double Chroma_hsl_Sc_fnl=(1-rgb_hsv_fnl_hsl[1]*rgb_hsv_fnl_hsl[2])+1;

double Chroma_hsi_Sc_fnl=(1-rgb_hsi_fnl[1]*rgb_hsi_fnl[2])+1;

double grey_metric_fnl=(satSc_fnl*sat_hsiSc_fnl*wSc_fnl*bSc_fnl*ChromaSc_fnl*Chroma_hsl_Sc_fnl*Chroma_hsi_Sc_fnl-1)/(2*2*2*2*2*2*2-1);

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

if (cust_x!=CIEx || (cust_y!=CIEy)){

double WPConvXYZ2[3];
double cust_xy[2]={cust_x,cust_y};
double cust_XYZ[3];
xy2XYZ(cust_xy,cust_XYZ);

WPconv(WPConvXYZ,D65XYZ,cust_XYZ,WPConvXYZ2);
WPConvXYZ[0]=WPConvXYZ2[0];
WPConvXYZ[1]=WPConvXYZ2[1];
WPConvXYZ[2]=WPConvXYZ2[2];

}
XYZ2xyY(WPConvXYZ,WPConvXYZ_xyY);

xyY2rgb(WPConvXYZ_xyY,WPchgRGB);

if(rOG==0 && (gOG==0) && (bOG==0)){
    WPchgRGB[0]=0;
    WPchgRGB[1]=0;
    WPchgRGB[2]=0;
}

//if (x_shft<0.9){
                srcp[x] = MAX(MIN(round(WPchgRGB[2]*255),255),0);
             srcp[x+1] =MAX(MIN(round(WPchgRGB[1]*255),255),0);
        srcp[x+2] = MAX(MIN(round(WPchgRGB[0]*255),255),0);

//}

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

          params->debug = avs_defined(avs_array_elt(args, 1))?avs_as_bool(avs_array_elt(args, 1)):false;
          params->start = avs_defined(avs_array_elt(args, 2))?avs_as_float(avs_array_elt(args, 2)):1;
          params->x = avs_defined(avs_array_elt(args, 3))?avs_as_float(avs_array_elt(args, 2)):0.312727;
          params->y = avs_defined(avs_array_elt(args, 4))?avs_as_float(avs_array_elt(args, 2)):0.329023;



   fi->user_data = (void*) params;
   fi->get_frame = WhitePoint_get_frame;


   v = avs_new_value_clip(new_clip);

   avs_release_clip(new_clip);
  // free(params);
   return v;
}

const char* AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment* env)
{
   avs_add_function(env, "WhitePoint", "c[debug]b[start]f[x]f[y]f", create_WhitePoint, 0);
   return "WhitePoint sample C plugin";
}
