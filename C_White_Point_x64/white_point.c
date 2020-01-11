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
        int blend;
        double desat;
} WhitePoint;


AVS_VideoFrame* AVSC_CC WhitePoint_get_frame(AVS_FilterInfo* fi, int n)
{
 WhitePoint* params = (WhitePoint*) fi->user_data;

   AVS_VideoFrame* src = avs_get_frame(fi->child, n);

   int row_size, height, src_pitch,x, y, p,dbg,blnd;
   BYTE* srcp;
   double CIEx,CIEy,rOG,bOG,gOG,strt,cust_x,cust_y,dst;

dbg=params->debug;
strt=params->start;
cust_x=params->x;
cust_y=params->y;
blnd=params->blend;
dst=params->desat;

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


double counterAll=0;

double sumR=0;
double sumG=0;
double sumB=0;

double sumR_=0;
double sumG_=0;
double sumB_=0;

///////////////ACTUALLY DRAW PIXELS///////////////////////////////////////

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

double curr_rgb_dst_lin[3];
sRGB2Linear(curr_rgb_dst,curr_rgb_dst_lin);

double curr_rgb_dst_lin_prp[3];

RGB2rgb(curr_rgb_dst_lin,curr_rgb_dst_lin_prp);

double Sc=1-(MAX(curr_rgb_dst_lin_prp[0],MAX(curr_rgb_dst_lin_prp[1],curr_rgb_dst_lin_prp[2]))-MIN(curr_rgb_dst_lin_prp[0],MIN(curr_rgb_dst_lin_prp[1],curr_rgb_dst_lin_prp[2])));
double mx=MAX(curr_rgb_dst_lin[0],MAX(curr_rgb_dst_lin[1],curr_rgb_dst_lin[2]));
  Sc=MIN(MAX(third*(2*Sc+mx),0),1);
double mtrc=(strt==0)?1:Sc/strt;
if (Sc<=strt){

    sumR+=curr_rgb_dst_lin[0]*(mtrc);
    sumG+=curr_rgb_dst_lin[1]*(mtrc);
    sumB+=curr_rgb_dst_lin[2]*(mtrc);


sumR_+=curr_rgb_dst_lin[0];
sumG_+= curr_rgb_dst_lin[1];
sumB_+=curr_rgb_dst_lin[2];
        counterAll+=1;

      }


x+=3;

}



      }


      double rcp_counterAll=(counterAll==0)?1:pow(counterAll,-1);


    double avg_rgb[3];
    double avg_rgb_nw[3];
    double avg_rgb_nw_prp[3];
    double avg_rgb_nw_wht[3];
    double avg_rgb_prp[3];
    double avg_rgb_wht[3];

avg_rgb[0]=sumR*rcp_counterAll;
avg_rgb[1]=sumG*rcp_counterAll;
avg_rgb[2]=sumB*rcp_counterAll;

avg_rgb_nw[0]=sumR_*rcp_counterAll;
avg_rgb_nw[1]=sumG_*rcp_counterAll;
avg_rgb_nw[2]=sumB_*rcp_counterAll;

RGB2rgb(avg_rgb_nw,avg_rgb_nw_prp);
rgb2RGB_White(avg_rgb_nw_prp,avg_rgb_nw_wht);

RGB2rgb(avg_rgb,avg_rgb_prp);
rgb2RGB_White(avg_rgb_prp,avg_rgb_wht);

double avg_rgb_hsv[3];
rgb2hsv(avg_rgb,avg_rgb_hsv);

double h_a=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (60*pow(360,-1))  ), 1) - 0.5);
double h_b=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (120*pow(360,-1))  ), 1) - 0.5);
double h_c=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (180*pow(360,-1))  ), 1) - 0.5);
double h_d=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (240*pow(360,-1))  ), 1) - 0.5);
double h_e=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (300*pow(360,-1))  ), 1) - 0.5);
double h_f=MIN(1-avg_rgb_hsv[0],avg_rgb_hsv[0]);


double hd_max=MAX(h_a,MAX(h_b,MAX(h_c,MAX(h_d,MAX(h_e,h_f)))));

if (hd_max==h_a){
    avg_rgb_hsv[0]=60*pow(360,-1);
}else if (hd_max==h_b){
    avg_rgb_hsv[0]=120*pow(360,-1);
}else if (hd_max==h_c){
    avg_rgb_hsv[0]=180*pow(360,-1);
}else if (hd_max==h_d){
    avg_rgb_hsv[0]=240*pow(360,-1);
}else if (hd_max==h_e){
    avg_rgb_hsv[0]=300*pow(360,-1);
}else{
    avg_rgb_hsv[0]=0;
}

double avg_rgb_bk[3];
double avg_rgb_prp_bk[3];
double avg_rgb_wht_bk[3];
double avg_rgb_prp_bk2[3];
double avg_rgb_prp_bk3[3];
double avg_rgb_wht_bk2[3];
double avg_rgb_wht_bk3[3];

hsv2rgb(avg_rgb_hsv,avg_rgb_bk );

RGB2rgb(avg_rgb_bk,avg_rgb_prp_bk);
rgb2RGB_White(avg_rgb_prp_bk,avg_rgb_wht_bk);

avg_rgb_prp_bk2[0]=(avg_rgb_prp_bk[0]>pow(3,-1))?1-2*avg_rgb_prp_bk[0]:0.5*(avg_rgb_prp_bk[0]-pow(3,-1));
avg_rgb_prp_bk2[1]=(avg_rgb_prp_bk[1]>pow(3,-1))?1-2*avg_rgb_prp_bk[1]:0.5*(avg_rgb_prp_bk[1]-pow(3,-1));
avg_rgb_prp_bk2[2]=(avg_rgb_prp_bk[2]>pow(3,-1))?1-2*avg_rgb_prp_bk[2]:0.5*(avg_rgb_prp_bk[2]-pow(3,-1));

avg_rgb_wht_bk3[0]=1-avg_rgb_prp_bk2[0];
avg_rgb_wht_bk3[1]=1-avg_rgb_prp_bk2[1];
avg_rgb_wht_bk3[2]=1-avg_rgb_prp_bk2[2];
RGB2rgb(avg_rgb_wht_bk3,avg_rgb_prp_bk3);

rgb2RGB_White(avg_rgb_prp_bk3 ,avg_rgb_wht_bk2);


double XYZ_convert_OG[3];
double XYZ_convert_OG2[3];
double XYZ_convert[3];
double XYZ_convert2[3];
double XYZ_convert4[3];
double XYZ_convert3[3];
double white[3]={1,1,1};

LinRGB2Other_XYZ (avg_rgb_wht_bk2,white,XYZ_convert_OG,XYZ_convert2);

WPconv2Grey (XYZ_convert_OG,XYZ_convert2 ,XYZ_convert);

LinRGB2Other_XYZ (white,avg_rgb_wht_bk2,XYZ_convert_OG2,XYZ_convert4);

WPconv2Grey (XYZ_convert_OG2,XYZ_convert4 ,XYZ_convert3);
/////////////////////////////////////////////////////////////////////////

double sumR_wp=0;
double sumG_wp=0;
double sumB_wp=0;

      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {


//double x_shift=(double)x/(double)row_size;
//double y_shift=(double)y/(double)height;
                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcptwoFiveFive;     // B
       gOG=currGreen*rcptwoFiveFive;   //G
         rOG=currRed*rcptwoFiveFive;     // R

double curr_rgb_dst_fnl[3]={rOG,gOG,bOG};

double curr_rgb_dst_lin_fnl[3];
double curr_rgb_dst_lin_hsv_fnl[3];
sRGB2Linear(curr_rgb_dst_fnl,curr_rgb_dst_lin_fnl);
            rgb2hsv(curr_rgb_dst_lin_fnl,curr_rgb_dst_lin_hsv_fnl);


    double WPchgRGB[3];
    double WPchgRGB2[3];

        double WPchgRGB_hsv[3];
    double WPchgRGB2_hsv[3];

    double rgbxyY[3];
double rgbXYZ[3];
double WPConvXYZ[3];
double WPConvXYZ_xyY[3];
double WPConvXYZ2[3];
double WPConvXYZ_xyY2[3];

LinRGB2xyY(curr_rgb_dst_lin_fnl,rgbxyY);
xyY2XYZ(rgbxyY,rgbXYZ);


WPconv(rgbXYZ,D65XYZ,XYZ_convert,WPConvXYZ);
WPconv(rgbXYZ,D65XYZ,XYZ_convert3,WPConvXYZ2);

XYZ2xyY(WPConvXYZ,WPConvXYZ_xyY);

xyY2LinRGB(WPConvXYZ_xyY,WPchgRGB);
XYZ2xyY(WPConvXYZ2,WPConvXYZ_xyY2);

xyY2LinRGB(WPConvXYZ_xyY2,WPchgRGB2);

rgb2hsv(WPchgRGB,WPchgRGB_hsv);
rgb2hsv(WPchgRGB2,WPchgRGB2_hsv);
double min_sat=MIN(curr_rgb_dst_lin_hsv_fnl[1],MIN(WPchgRGB2_hsv[1],WPchgRGB_hsv[1]));


if (WPchgRGB2_hsv[1]==min_sat){

    WPchgRGB[0]=WPchgRGB2[0];
    WPchgRGB[1]=WPchgRGB2[1];
    WPchgRGB[2]=WPchgRGB2[2];

} else if (curr_rgb_dst_lin_hsv_fnl[1]==min_sat){

    WPchgRGB[0]=curr_rgb_dst_lin_fnl[0];
    WPchgRGB[1]=curr_rgb_dst_lin_fnl[1];
    WPchgRGB[2]=curr_rgb_dst_lin_fnl[2];

}


if(rOG==0 && (gOG==0) && (bOG==0)){
    WPchgRGB[0]=0;
    WPchgRGB[1]=0;
    WPchgRGB[2]=0;

}


 sumR_wp+=WPchgRGB[0];
 sumG_wp+=WPchgRGB[1];
 sumB_wp+=WPchgRGB[2];

x+=3;
}

      }




double avg_rgb_wp[3];
double avg_rgb_wp_prp[3];
double avg_rgb_wp_wht[3];

avg_rgb_wp[0]=sumR_wp*rcp_counterAll;
avg_rgb_wp[1]=sumG_wp*rcp_counterAll;
avg_rgb_wp[2]=sumB_wp*rcp_counterAll;

RGB2rgb(avg_rgb_wp,avg_rgb_wp_prp);
rgb2RGB_White(avg_rgb_wp_prp,avg_rgb_wp_wht);



double XYZ_convert_OG3[3];
double XYZ_convert5[3];
double XYZ_convert6[3];


LinRGB2Other_XYZ (avg_rgb_nw_wht,avg_rgb_wp_wht,XYZ_convert_OG3,XYZ_convert5);

WPconv2Grey (XYZ_convert_OG3,XYZ_convert5 ,XYZ_convert6);

      for (y=0; y<height; y++) {

      for (x=0; x<row_size; x++) {


//double x_shift=(double)x/(double)row_size;
//double y_shift=(double)y/(double)height;
                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcptwoFiveFive;     // B
       gOG=currGreen*rcptwoFiveFive;   //G
         rOG=currRed*rcptwoFiveFive;     // R

double curr_rgb_dst_lst[3]={rOG,gOG,bOG};
double curr_rgb_dst_hsv[3];
rgb2hsv(curr_rgb_dst_lst,curr_rgb_dst_hsv);
double curr_rgb_dst_lst_YUV[3];

double curr_rgb_dst_lin_lst[3];
double curr_rgb_dst_lin_lst_YUV[3];
sRGB2Linear(curr_rgb_dst_lst,curr_rgb_dst_lin_lst);
rgb2yuvWP(curr_rgb_dst_lst,curr_rgb_dst_lst_YUV);
double curr_rgb_dst_lin_prp_lst[3];
RGB2rgb(curr_rgb_dst_lin_lst,curr_rgb_dst_lin_prp_lst);

double curr_rgb_dst_lin_wht_lst[3];

rgb2RGB_White(curr_rgb_dst_lin_prp_lst,curr_rgb_dst_lin_wht_lst);


double Sc_lst=1-(MAX(curr_rgb_dst_lin_prp_lst[0],MAX(curr_rgb_dst_lin_prp_lst[1],curr_rgb_dst_lin_prp_lst[2]))-MIN(curr_rgb_dst_lin_prp_lst[0],MIN(curr_rgb_dst_lin_prp_lst[1],curr_rgb_dst_lin_prp_lst[2])));

double mx_lst=MAX(curr_rgb_dst_lin_lst[0],MAX(curr_rgb_dst_lin_lst[1],curr_rgb_dst_lin_lst[2]));
  Sc_lst=MIN(MAX(third* (2*Sc_lst+mx_lst),0),1);

if(dbg==1){

    if (Sc_lst>strt){

        srcp[x] = 127;
                srcp[x+1] =255;
        srcp[x+2] = 0;
    }
}else{

    double WPchgRGB_lst[3];
    double WPchgRGB_lst_bk[3];
    double WPchgRGB_lst_gc[3];

    double rgbxyY_lst[3];
double rgbXYZ_lst[3];
double WPConvXYZ_lst[3];
double WPConvXYZ_xyY_lst[3];

LinRGB2xyY(curr_rgb_dst_lin_lst,rgbxyY_lst);
xyY2XYZ(rgbxyY_lst,rgbXYZ_lst);

WPconv(rgbXYZ_lst,D65XYZ,XYZ_convert6,WPConvXYZ_lst);

if (cust_x!=CIEx || (cust_y!=CIEy)){

double WPConvXYZ4[3];
double cust_xy_lst[2]={cust_x,cust_y};
double cust_XYZ_lst[3];
xy2XYZ(cust_xy_lst,cust_XYZ_lst);

WPconv(WPConvXYZ_lst,D65XYZ,cust_XYZ_lst,WPConvXYZ4);
WPConvXYZ_lst[0]=WPConvXYZ4[0];
WPConvXYZ_lst[1]=WPConvXYZ4[1];
WPConvXYZ_lst[2]=WPConvXYZ4[2];

}

XYZ2xyY(WPConvXYZ_lst,WPConvXYZ_xyY_lst);
xyY2rgb(WPConvXYZ_xyY_lst,WPchgRGB_lst);
if (blnd==1){


double WPchgRGB_lst_YUV[3];
double WPchgRGB_lst_YUV_bk[3];
double WPchgRGB_lst_hsv[3];
rgb2yuvWP(WPchgRGB_lst,WPchgRGB_lst_YUV);

WPchgRGB_lst_YUV[1]=curr_rgb_dst_lst_YUV[1];
WPchgRGB_lst_YUV[2]=curr_rgb_dst_lst_YUV[2];
yuv2rgbWP(WPchgRGB_lst_YUV,WPchgRGB_lst_YUV_bk);
rgb2hsv(WPchgRGB_lst_YUV_bk,WPchgRGB_lst_hsv);
     double lrp_blnd=1-(0.5 - fabs(mod(fabs(WPchgRGB_lst_hsv[0] - curr_rgb_dst_hsv[0])  , 1) - 0.5))*2;
//double lrp_blnd=sqrt((WPchgRGB_lst[0]-curr_rgb_dst_lst[0]) * (WPchgRGB_lst[0]-curr_rgb_dst_lst[0]) + (WPchgRGB_lst[1]-curr_rgb_dst_lst[1])*(WPchgRGB_lst[1]-curr_rgb_dst_lst[1])+(WPchgRGB_lst[2]-curr_rgb_dst_lst[2])*(WPchgRGB_lst[2]-curr_rgb_dst_lst[2]) )/sqrt(3);

WPchgRGB_lst_hsv[1]=WPchgRGB_lst_hsv[1]-(1-curr_rgb_dst_hsv[1])*dst*lrp_blnd;
hsv2rgb(WPchgRGB_lst_hsv,WPchgRGB_lst);
}


if(rOG==0 && (gOG==0) && (bOG==0)){
    WPchgRGB_lst[0]=0;
    WPchgRGB_lst[1]=0;
    WPchgRGB_lst[2]=0;

}



                srcp[x] = MAX(MIN(round(WPchgRGB_lst[2]*255),255),0);
             srcp[x+1] =MAX(MIN(round(WPchgRGB_lst[1]*255),255),0);
        srcp[x+2] = MAX(MIN(round(WPchgRGB_lst[0]*255),255),0);

}


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

          params->debug = avs_defined(avs_array_elt(args, 1))?avs_as_bool(avs_array_elt(args, 1)):false;
          params->start = avs_defined(avs_array_elt(args, 2))?avs_as_float(avs_array_elt(args, 2)):1;
          params->x = avs_defined(avs_array_elt(args, 3))?avs_as_float(avs_array_elt(args, 3)):0.312727;
          params->y = avs_defined(avs_array_elt(args, 4))?avs_as_float(avs_array_elt(args, 4)):0.329023;
          params->blend = avs_defined(avs_array_elt(args, 5))?avs_as_bool(avs_array_elt(args, 5)):true;
        params->desat = avs_defined(avs_array_elt(args, 6))?avs_as_float(avs_array_elt(args, 6)):0;


   fi->user_data = (void*) params;
   fi->get_frame = WhitePoint_get_frame;


   v = avs_new_value_clip(new_clip);

   avs_release_clip(new_clip);
  // free(params);
   return v;
}

const char* AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment* env)
{
   avs_add_function(env, "WhitePoint", "c[debug]b[start]f[x]f[y]f[blend]b[desat]f", create_WhitePoint, 0);
   return "WhitePoint sample C plugin";
}
