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

double r_desat_sum_gm= 0;
double g_desat_sum_gm= 0;
double b_desat_sum_gm= 0;

double r_avg_sum_gm= 0;
double g_avg_sum_gm= 0;
double b_avg_sum_gm= 0;

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

double curr_rgb_dst_lin_xyY[3];

LinRGB2xyY(curr_rgb_dst_lin,curr_rgb_dst_lin_xyY);

double curr_rgb_dst_lin_prp[3];
RGB2rgb(curr_rgb_dst_lin,curr_rgb_dst_lin_prp);

double Sc=1-(MAX(curr_rgb_dst_lin_prp[0],MAX(curr_rgb_dst_lin_prp[1],curr_rgb_dst_lin_prp[2]))-MIN(curr_rgb_dst_lin_prp[0],MIN(curr_rgb_dst_lin_prp[1],curr_rgb_dst_lin_prp[2])));

double Y=curr_rgb_dst_lin_xyY[2];



if(Sc<=strt) {

r_avg_sum_gm+=curr_rgb_dst_lin[0]*(Sc)*Y;
g_avg_sum_gm+=curr_rgb_dst_lin[1]*(Sc)*Y;
b_avg_sum_gm+=curr_rgb_dst_lin[2]*(Sc)*Y;

r_avg_sum+=curr_rgb_dst_lin[0];
g_avg_sum+=curr_rgb_dst_lin[1];
b_avg_sum+=curr_rgb_dst_lin[2];
grey_metric_avg_sum+=Sc*Y;
counter=counter+1;
}


x+=3;

}

 // srcp += src_pitch;

      }
      double rcp_counter=(counter==0)?1:pow(counter,-1);
      double rcp_counter2=(counter==0)?1:pow(grey_metric_avg_sum,-1);
      double avg_Sc=rcp_counter*grey_metric_avg_sum;
      double avg_rgb[3];
      double avg_rgb1[3];
      double avg_rgb2[3];
      double avg_rgb3[3];
      double avg_rgb_diff;

  avg_rgb1[0]=1-r_avg_sum_gm*rcp_counter2;
 avg_rgb1[1]=1-g_avg_sum_gm*rcp_counter2;
 avg_rgb1[2]=1-b_avg_sum_gm*rcp_counter2;

 avg_rgb3[0]=1-r_avg_sum*rcp_counter;
 avg_rgb3[1]=1-g_avg_sum*rcp_counter;
 avg_rgb3[2]=1-b_avg_sum*rcp_counter;

 avg_rgb_diff=1-(0.5*((avg_rgb1[0]-avg_rgb3[0])+1)+0.5*((avg_rgb1[1]-avg_rgb3[1])+1)+0.5*((avg_rgb1[2]-avg_rgb3[2])+1))/3;
 double avg_rgb_diff2=1-(fabs(avg_rgb1[0]-avg_rgb3[0])  + fabs(avg_rgb1[1]-avg_rgb3[1])  + fabs(avg_rgb1[2]-avg_rgb3[2])           )/3;
 double lrp=-(avg_rgb_diff)*(1-avg_Sc)*avg_rgb_diff2;

   avg_rgb[0]=1-lerp(0.5*((avg_rgb1[0]-avg_rgb3[0])+1),1-avg_rgb1[0],lrp);
   avg_rgb[1]=1-lerp(0.5*((avg_rgb1[1]-avg_rgb3[1])+1),1-avg_rgb1[1],lrp);
   avg_rgb[2]=1-lerp(0.5*((avg_rgb1[2]-avg_rgb3[2])+1),1-avg_rgb1[2],lrp);


double avg_rgb_xyY[3];
LinRGB2xyY(avg_rgb,avg_rgb_xyY);
double avg_rgb_xy[2]={avg_rgb_xyY[0],avg_rgb_xyY[1]};
double avg_rgb_XYZ[3];
xy2XYZ(avg_rgb_xy,avg_rgb_XYZ);

double XYZ_convert[3];

WPconv2Grey (avg_rgb_XYZ,D65XYZ,XYZ_convert);


      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {
                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcp_twoFiveFive;     // B
       gOG=currGreen*rcp_twoFiveFive;   //G
         rOG=currRed*rcp_twoFiveFive;     // R

double curr_rgb_dst_fnl[3]={rOG,gOG,bOG};
if(dbg==1){
double curr_rgb_dst_lin_fnl[3];
sRGB2Linear(curr_rgb_dst_fnl,curr_rgb_dst_lin_fnl);

double curr_rgb_dst_lin_prp_fnl[3];
RGB2rgb(curr_rgb_dst_lin_fnl,curr_rgb_dst_lin_prp_fnl);

double Sc_fnl=1-(MAX(curr_rgb_dst_lin_prp_fnl[0],MAX(curr_rgb_dst_lin_prp_fnl[1],curr_rgb_dst_lin_prp_fnl[2]))-MIN(curr_rgb_dst_lin_prp_fnl[0],MIN(curr_rgb_dst_lin_prp_fnl[1],curr_rgb_dst_lin_prp_fnl[2])));

double curr_rgb_dst_lin_xyY_fnl[3];

LinRGB2xyY(curr_rgb_dst_lin_fnl,curr_rgb_dst_lin_xyY_fnl);

double Y_fnl=curr_rgb_dst_lin_xyY_fnl[2];


Sc_fnl*=Y_fnl;

    if (Sc_fnl>strt){

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
