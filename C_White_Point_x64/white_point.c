#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../avisynth_c.h"
#include "functions_c.h"

typedef struct WhitePoint {
    int post;
        double x;
        double y;
} WhitePoint;


AVS_VideoFrame* AVSC_CC WhitePoint_get_frame(AVS_FilterInfo* fi, int n)
{
 WhitePoint* params = (WhitePoint*) fi->user_data;

   AVS_VideoFrame* src = avs_get_frame(fi->child, n);

   int row_size, height, src_pitch,x, y, p,pst;
   BYTE* srcp;
   double CIEx,CIEy,rOG,bOG,gOG,cust_x,cust_y,lrp;

pst=params->post;
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

if(pst==1){
double counterAll=0;

double sumR_gc=0;
double sumG_gc=0;
double sumB_gc=0;


      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

//double x_shift=(double)x/(double)row_size;
                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcptwoFiveFive;     // B
       gOG=currGreen*rcptwoFiveFive;   //G
         rOG=currRed*rcptwoFiveFive;     // R

double curr_rgb_dst[3]={rOG,gOG,bOG};

sumR_gc+=curr_rgb_dst[0];
sumG_gc+= curr_rgb_dst[1];
sumB_gc+=curr_rgb_dst[2];
        counterAll+=1;


x+=3;

}



      }



      double rcp_counterAll=(counterAll==0)?1:pow(counterAll,-1);

    double avg_rgb_gc[3];

avg_rgb_gc[0]=sumR_gc*rcp_counterAll;
avg_rgb_gc[1]=sumG_gc*rcp_counterAll;
avg_rgb_gc[2]=sumB_gc*rcp_counterAll;


double avg_rgb_hsv[3];
rgb2hsv(avg_rgb_gc ,avg_rgb_hsv);


 double initSat_avg=avg_rgb_hsv[1];

 avg_rgb_hsv[1]=MAX(0.5*(avg_rgb_hsv[1]*avg_rgb_hsv[1] +(1-avg_rgb_hsv[2])),0);
double mx_diff=MAX(initSat_avg,1-initSat_avg);

lrp=(mx_diff==0)?1:fabs(initSat_avg-avg_rgb_hsv[1])/mx_diff;
}

double rgbxyY_lst[3];
double rgbXYZ_lst[3];
double rgbxyY_lst_bk[3];
double rgb_lst_bk[3];
double WPConvXYZ4[3];
double cust_xy_lst[2]={cust_x,cust_y};
double cust_XYZ_lst[3];

  double WPchgRGB_lst[3];
      for (y=0; y<height; y++) {

      for (x=0; x<row_size; x++) {

                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcptwoFiveFive;     // B
       gOG=currGreen*rcptwoFiveFive;   //G
         rOG=currRed*rcptwoFiveFive;     // R

double curr_rgb_dst_lst[3]={rOG,gOG,bOG};

if(pst==1){
double curr_rgb_dst_lst_hsv[3];
double curr_rgb_dst_lin_lst[3];
rgb2hsv(curr_rgb_dst_lst,curr_rgb_dst_lst_hsv);

double initSat_avg=curr_rgb_dst_lst_hsv[1];
    curr_rgb_dst_lst_hsv[1]=MAX(0.5*(curr_rgb_dst_lst_hsv[1]*curr_rgb_dst_lst_hsv[1] +(1-curr_rgb_dst_lst_hsv[2])),0);

double lrpr=1-third*((1-curr_rgb_dst_lst_hsv[1])   +lrp+(1-initSat_avg))*(1-initSat_avg);
curr_rgb_dst_lst_hsv[1]=lerp(curr_rgb_dst_lst_hsv[1],initSat_avg,lrpr);

hsv2rgb(curr_rgb_dst_lst_hsv,WPchgRGB_lst);

if (cust_x!=CIEx || (cust_y!=CIEy)){

        if(rOG==0 && (gOG==0) && (bOG==0)){
    WPchgRGB_lst[0]=0;
    WPchgRGB_lst[1]=0;
    WPchgRGB_lst[2]=0;

}else{

rgb2xyY(WPchgRGB_lst,rgbxyY_lst);
xyY2XYZ(rgbxyY_lst,rgbXYZ_lst);

xy2XYZ(cust_xy_lst,cust_XYZ_lst);

WPconv(rgbXYZ_lst,D65XYZ,cust_XYZ_lst,WPConvXYZ4);
XYZ2xyY(WPConvXYZ4,rgbxyY_lst_bk);
xyY2rgb(rgbxyY_lst_bk,rgb_lst_bk);


WPchgRGB_lst[0]=rgb_lst_bk[0];
WPchgRGB_lst[1]=rgb_lst_bk[1];
WPchgRGB_lst[2]=rgb_lst_bk[2];
}


}
      }else{

if (cust_x!=CIEx || (cust_y!=CIEy)){
        if(rOG==0 && (gOG==0) && (bOG==0)){
    WPchgRGB_lst[0]=0;
    WPchgRGB_lst[1]=0;
    WPchgRGB_lst[2]=0;

}else{

rgb2xyY(curr_rgb_dst_lst,rgbxyY_lst);
xyY2XYZ(rgbxyY_lst,rgbXYZ_lst);

xy2XYZ(cust_xy_lst,cust_XYZ_lst);

WPconv(rgbXYZ_lst,D65XYZ,cust_XYZ_lst,WPConvXYZ4);
XYZ2xyY(WPConvXYZ4,rgbxyY_lst_bk);
xyY2rgb(rgbxyY_lst_bk,rgb_lst_bk);


WPchgRGB_lst[0]=rgb_lst_bk[0];
WPchgRGB_lst[1]=rgb_lst_bk[1];
WPchgRGB_lst[2]=rgb_lst_bk[2];
}
}else{
WPchgRGB_lst[0]=rOG;
WPchgRGB_lst[1]=gOG;
WPchgRGB_lst[2]=bOG;

}
      }


             srcp[x] = MAX(MIN(round(WPchgRGB_lst[2]*255),255),0);
             srcp[x+1] =MAX(MIN(round(WPchgRGB_lst[1]*255),255),0);
        srcp[x+2] = MAX(MIN(round(WPchgRGB_lst[0]*255),255),0);



x+=3;
}

      srcp += src_pitch;


      }

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

          params->post = avs_defined(avs_array_elt(args, 1))?avs_as_bool(avs_array_elt(args, 1)):true;
          params->x = avs_defined(avs_array_elt(args, 2))?avs_as_float(avs_array_elt(args, 2)):0.312727;
          params->y = avs_defined(avs_array_elt(args, 3))?avs_as_float(avs_array_elt(args, 3)):0.329023;


   fi->user_data = (void*) params;
   fi->get_frame = WhitePoint_get_frame;


   v = avs_new_value_clip(new_clip);

   avs_release_clip(new_clip);
  // free(params);
   return v;
}

const char* AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment* env)
{
   avs_add_function(env, "WhitePoint", "c[post]b[x]f[y]f[blend]b", create_WhitePoint, 0);
   return "WhitePoint sample C plugin";
}
