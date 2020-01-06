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

/*
double r_avg_sum_gm= 0;
double g_avg_sum_gm= 0;
double b_avg_sum_gm= 0;
*/
double counterAll=0;

//double counter=0;
//double counterSc=0;

double sumR=0;
double sumG=0;
double sumB=0;

double sumR_=0;
double sumG_=0;
double sumB_=0;

/*
double sumMin=0;
double sumMax=0;*/
double sumChroma=0;

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

double curr_rgb_dst_lin_prp[3];
//double curr_rgb_dst_lin_wht[3];
//double curr_rgb_dst_lin_hsv[3];
RGB2rgb(curr_rgb_dst_lin,curr_rgb_dst_lin_prp);
/*
rgb2hsv(curr_rgb_dst_lin,curr_rgb_dst_lin_hsv);
double curr_rgb_dst_hsi[3];
RGB2HSI(curr_rgb_dst_lin,curr_rgb_dst_hsi);
*/
/*
rgb2RGB_White(curr_rgb_dst_lin_prp,curr_rgb_dst_lin_wht);
*/


/*




curr_rgb_dst_hwb[2]=1;

double curr_rgb_dst_hwb_hsv[3];
hwb2hsv(curr_rgb_dst_hwb,curr_rgb_dst_hwb_hsv);

double curr_rgb_dst_hwb_rgb[3];
hsv2rgb(curr_rgb_dst_hwb_hsv,curr_rgb_dst_hwb_rgb);
*/
double Sc=(MAX(curr_rgb_dst_lin_prp[0],MAX(curr_rgb_dst_lin_prp[1],curr_rgb_dst_lin_prp[2]))-MIN(curr_rgb_dst_lin_prp[0],MIN(curr_rgb_dst_lin_prp[1],curr_rgb_dst_lin_prp[2])));
//double satSc=curr_rgb_dst_hsv[1];


double rootSat=sqrt(  pow(curr_rgb_dst_lin[0]-curr_rgb_dst_lin[1],2) + pow(curr_rgb_dst_lin[0]-curr_rgb_dst_lin[2],2)  +pow(curr_rgb_dst_lin[1]-curr_rgb_dst_lin[2],2))/sqrt(3);
 // Sc=(1-curr_rgb_dst_hsv[1])*(curr_rgb_dst_hsi[1])*(1-0.5*(Sc+(1-curr_rgb_dst_hwb[1])))*2;
//double min=MIN(fabs(curr_rgb_dst_lin[0]-curr_rgb_dst_lin[1]),MIN(fabs(curr_rgb_dst_lin[0]-curr_rgb_dst_lin[2]), fabs(curr_rgb_dst_lin[1]-curr_rgb_dst_lin[2])  ));
 Sc=MIN(MAX((Sc*rootSat),0),1);
if (1-Sc<=strt){

    sumR+=curr_rgb_dst_lin[0]*Sc;
    sumG+=curr_rgb_dst_lin[1]*Sc;
    sumB+=curr_rgb_dst_lin[2]*Sc;

        counterAll+=1;

      }


x+=3;

}



      }


      double rcp_counterAll=(counterAll==0)?1:pow(counterAll,-1);


    double avg_rgb[3];

avg_rgb[0]=sumR*rcp_counterAll;
avg_rgb[1]=sumG*rcp_counterAll;
avg_rgb[2]=sumB*rcp_counterAll;
double avg_rgb_hsv[3];
rgb2hsv(avg_rgb,avg_rgb_hsv);


double h_a=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (60*pow(360,-1))  ), 1) - 0.5);
double h_b=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (120*pow(360,-1))  ), 1) - 0.5);
double h_c=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (180*pow(360,-1))  ), 1) - 0.5);
double h_d=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (240*pow(360,-1))  ), 1) - 0.5);
double h_e=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (300*pow(360,-1))  ), 1) - 0.5);
double h_f=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (1)  ), 1) - 0.5);


double hd_min=MAX(h_a,MAX(h_b,MAX(h_c,MAX(h_d,MAX(h_e,MAX(h_f,avg_rgb_hsv[0]))))));

if (hd_min==h_a){
    avg_rgb_hsv[0]=60*pow(360,-1);
}else if (hd_min==h_b){
    avg_rgb_hsv[0]=120*pow(360,-1);
}else if (hd_min==h_c){
    avg_rgb_hsv[0]=180*pow(360,-1);
}else if (hd_min==h_d){
    avg_rgb_hsv[0]=240*pow(360,-1);
}else if (hd_min==h_e){
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
double XYZ_convert2[3];
double XYZ_convert[3];


double white[3]={1,1,1};

LinRGB2Other_XYZ (white,avg_rgb_wht_bk2,XYZ_convert_OG,XYZ_convert2);
//LinRGB2Other_XYZ (white,avg_rgb_hmv_fix_inv_wht,XYZ_convert_OG,XYZ_convert2);



   //LinRGB2Other_XYZ (avg_rgb_hmv_fix_inv_wht2,white,XYZ_convert_OG_inv,XYZ_convert2_inv);


WPconv2Grey (XYZ_convert_OG,XYZ_convert2 ,XYZ_convert);


      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {


double x_shift=(double)x/(double)row_size;
double y_shift=(double)y/(double)height;
                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcp_twoFiveFive;     // B
       gOG=currGreen*rcp_twoFiveFive;   //G
         rOG=currRed*rcp_twoFiveFive;     // R

double curr_rgb_dst_fnl[3]={rOG,gOG,bOG};

double curr_rgb_dst_lin_fnl[3];
sRGB2Linear(curr_rgb_dst_fnl,curr_rgb_dst_lin_fnl);

double curr_rgb_dst_lin_prp_fnl[3];
RGB2rgb(curr_rgb_dst_lin_fnl,curr_rgb_dst_lin_prp_fnl);

double curr_rgb_dst_lin_wht_fnl[3];

rgb2RGB_White(curr_rgb_dst_lin_prp_fnl,curr_rgb_dst_lin_wht_fnl);


double Sc_fnl=(MAX(curr_rgb_dst_lin_prp_fnl[0],MAX(curr_rgb_dst_lin_prp_fnl[1],curr_rgb_dst_lin_prp_fnl[2]))-MIN(curr_rgb_dst_lin_prp_fnl[0],MIN(curr_rgb_dst_lin_prp_fnl[1],curr_rgb_dst_lin_prp_fnl[2])));

double rootSat_fnl=sqrt(  pow(curr_rgb_dst_lin_fnl[0]-curr_rgb_dst_lin_fnl[1],2) + pow(curr_rgb_dst_lin_fnl[0]-curr_rgb_dst_lin_fnl[2],2)  +pow(curr_rgb_dst_lin_fnl[1]-curr_rgb_dst_lin_fnl[2],2))/sqrt(3);

 Sc_fnl=MIN(MAX((Sc_fnl*rootSat_fnl),0),1);


if(dbg==1){

    if (1-Sc_fnl>strt){

        srcp[x] = 127;
                srcp[x+1] =255;
        srcp[x+2] = 0;
    }
}else{
    double WPchgRGB[3];

    double rgbxyY[3];
double rgbXYZ[3];
double WPConvXYZ[3];
double WPConvXYZ_xyY[3];

rgb2xyY(curr_rgb_dst_fnl,rgbxyY);
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



                srcp[x] = MAX(MIN(round(WPchgRGB[2]*255),255),0);
             srcp[x+1] =MAX(MIN(round(WPchgRGB[1]*255),255),0);
        srcp[x+2] = MAX(MIN(round(WPchgRGB[0]*255),255),0);



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
          params->x = avs_defined(avs_array_elt(args, 3))?avs_as_float(avs_array_elt(args, 3)):0.312727;
          params->y = avs_defined(avs_array_elt(args, 4))?avs_as_float(avs_array_elt(args, 4)):0.329023;



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
