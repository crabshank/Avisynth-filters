#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../avisynth_c.h"
#include "functions_c.h"

typedef struct Manual_WP {
        double x;
        double y;
        int debug;
        double debug_val;
} Manual_WP;


AVS_VideoFrame* AVSC_CC Manual_WP_get_frame(AVS_FilterInfo* fi, int n)
{
 Manual_WP* params = (Manual_WP*) fi->user_data;

   AVS_VideoFrame* src = avs_get_frame(fi->child, n);

   int row_size, height, src_pitch,x, y,dbg;
   BYTE* srcp;
   double rOG,bOG,gOG,cust_x,cust_y,amp,D65_x,D65_y;

cust_x=params->x;
cust_y=params->y;
dbg=params->debug;
amp=params->debug_val;

D65_x= 0.312727;
   D65_y= 0.329023;
      double D65XYZ[3]={0.95047,1,1.08883};
            int planes[] ={AVS_CS_BGR32};
src = avs_get_frame(fi->child, n);
   avs_make_writable(fi->env, &src);

      srcp = avs_get_write_ptr_p(src, planes[0]);
      src_pitch = avs_get_pitch_p(src, planes[0]);
      row_size = avs_get_row_size_p(src, planes[0]);
      height = avs_get_height_p(src, planes[0]);


      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {



//double x_shift=(double)x/(double)row_size;
                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcptwoFiveFive;     // B
       gOG=currGreen*rcptwoFiveFive;   //G
         rOG=currRed*rcptwoFiveFive;     // R

    double rgbxyY[3];
double rgbXYZ[3];
double WPConvXYZ[3];
double WPConvXYZ_xyY[3];
double OG_RGB[3]={rOG,gOG,bOG};
double WPchgRGB[3]={rOG,gOG,bOG};
rgb2xyY(OG_RGB,rgbxyY);
xyY2XYZ(rgbxyY,rgbXYZ);

if(rOG==0 && (gOG==0) && (bOG==0)){
    WPchgRGB[0]=0;
    WPchgRGB[1]=0;
    WPchgRGB[2]=0;
}else{
if (cust_x!=D65_x || (cust_y!=D65_y)){
double cust_xy[2]={cust_x,cust_y};
double cust_XYZ[3];
xy2XYZ(cust_xy,cust_XYZ);

WPconv(rgbXYZ,D65XYZ,cust_XYZ,WPConvXYZ);

XYZ2xyY(WPConvXYZ,WPConvXYZ_xyY);

xyY2rgb(WPConvXYZ_xyY,WPchgRGB);
}

}

if(dbg==1){
        double mx=MAX(WPchgRGB[0],MAX(WPchgRGB[1],WPchgRGB[2]));
    double sat=(mx==0)?0:(mx-MIN(WPchgRGB[0],MIN(WPchgRGB[1],WPchgRGB[2])))/mx;
    double dbg_out=(amp==1)?sat:fastPrecisePow(sat,amp);
    WPchgRGB[0]=dbg_out;
    WPchgRGB[1]=dbg_out;
    WPchgRGB[2]=dbg_out;
}else if(dbg==2){
        double mx=MAX(WPchgRGB[0],MAX(WPchgRGB[1],WPchgRGB[2]));
    double sat=(mx==0)?0:(mx-MIN(WPchgRGB[0],MIN(WPchgRGB[1],WPchgRGB[2])))/mx;

    WPchgRGB[0]=(sat>=amp)?WPchgRGB[0]:0;
    WPchgRGB[1]=(sat>=amp)?WPchgRGB[1]:0;
    WPchgRGB[2]=(sat>=amp)?WPchgRGB[2]:0;
}else if (dbg==3){
        double mx=MAX(WPchgRGB[0],MAX(WPchgRGB[1],WPchgRGB[2]));
    double sat=(mx==0)?0:(mx-MIN(WPchgRGB[0],MIN(WPchgRGB[1],WPchgRGB[2])))/mx;


            double mxOG=MAX(rOG,MAX(gOG,bOG));
    double satOG=(mxOG==0)?0:(mxOG-MIN(rOG,MIN(gOG,bOG)))/mxOG;
satOG=(amp>=0)?satOG*(1-amp):satOG;
    WPchgRGB[0]=(sat>=satOG)?WPchgRGB[0]:0;
    WPchgRGB[1]=(sat>=satOG)?WPchgRGB[1]:0;
    WPchgRGB[2]=(sat>=satOG)?WPchgRGB[2]:0;
}



                srcp[x] = MAX(MIN(round(WPchgRGB[2]*255),255),0);
             srcp[x+1] =MAX(MIN(round(WPchgRGB[1]*255),255),0);
        srcp[x+2] = MAX(MIN(round(WPchgRGB[0]*255),255),0);


x+=3;
      }
            srcp += src_pitch;
      }



   return src;
}


AVS_Value AVSC_CC create_Manual_WP(AVS_ScriptEnvironment* env, AVS_Value args, void* user_data)
{
   AVS_Value v;
   AVS_FilterInfo* fi;


   AVS_Clip* new_clip = avs_new_c_filter(env, &fi, avs_array_elt(args, 0), 1);

Manual_WP *params = (Manual_WP*)malloc(sizeof(Manual_WP));

if (!params)
      return avs_void;



   if (!avs_is_rgb(&fi->vi)) {
      return avs_new_value_error("Input video must be in RGB format!");
   }

          params->x = avs_defined(avs_array_elt(args, 1))?avs_as_float(avs_array_elt(args, 1)):0.312727;
          params->y = avs_defined(avs_array_elt(args, 2))?avs_as_float(avs_array_elt(args, 2)):0.329023;
          params->debug = avs_defined(avs_array_elt(args, 3))?avs_as_int(avs_array_elt(args, 3)):0;
          params->debug_val = avs_defined(avs_array_elt(args, 4))?avs_as_float(avs_array_elt(args, 4)):1;



   fi->user_data = (void*) params;
   fi->get_frame = Manual_WP_get_frame;


   v = avs_new_value_clip(new_clip);

   avs_release_clip(new_clip);
  // free(params);
   return v;
}

const char* AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment* env)
{
   avs_add_function(env, "Manual_WP", "c[x]f[y]f[debug]i[debug_val]f", create_Manual_WP, 0);
   return "Manual_WP sample C plugin";
}
