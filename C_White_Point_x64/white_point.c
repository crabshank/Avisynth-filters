#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../avisynth_c.h"
#include "functions_c.h"

typedef struct WhitePoint {
    int post;
        double x;
        double y;
        double scurve;
        double desat;
        int debug;
} WhitePoint;


AVS_VideoFrame* AVSC_CC WhitePoint_get_frame(AVS_FilterInfo* fi, int n)
{
 WhitePoint* params = (WhitePoint*) fi->user_data;

   AVS_VideoFrame* src = avs_get_frame(fi->child, n);

   int row_size, height, src_pitch,x, y, p,pst,dbg;
   BYTE* srcp;
   double CIEx,CIEy,rOG,bOG,gOG,cust_x,cust_y,lrp,scrv,dest;

pst=params->post;
cust_x=params->x;
cust_y=params->y;
scrv=params->scurve;
dest=params->desat;
dbg=params->debug;


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

double sumR_gc=0;
double sumG_gc=0;
double sumB_gc=0;

double sumR_gc_prp=0;
double sumG_gc_prp=0;
double sumB_gc_prp=0;

double Sc_sum=0;
double Sc_avg=0;
    double avg_rgb_gc[3];
    double avg_hsv_gc[3];
if(pst==1){


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
double curr_rgb_dst_prp[3];
RGB2rgb(curr_rgb_dst,curr_rgb_dst_prp);

double Sc=1-(MAX(curr_rgb_dst_prp[0],MAX(curr_rgb_dst_prp[1],curr_rgb_dst_prp[2]))-MIN(curr_rgb_dst_prp[0],MIN(curr_rgb_dst_prp[1],curr_rgb_dst_prp[2])));

Sc_sum+=Sc;
sumR_gc+=curr_rgb_dst[0];
sumG_gc+= curr_rgb_dst[1];
sumB_gc+=curr_rgb_dst[2];


        counterAll+=1;


x+=3;

}



      }



      double rcp_counterAll=(counterAll==0)?1:pow(counterAll,-1);



avg_rgb_gc[0]=sumR_gc*rcp_counterAll;
avg_rgb_gc[1]=sumG_gc*rcp_counterAll;
avg_rgb_gc[2]=sumB_gc*rcp_counterAll;


rgb2hsv(avg_rgb_gc,avg_hsv_gc);
Sc_avg=Sc_sum*rcp_counterAll;


}

double rgbxyY_lst[3];
double rgbXYZ_lst[3];
double rgbxyY_lst_bk[3];
double rgb_lst_bk[3];
double WPConvXYZ4[3];
double cust_xy_lst[2]={cust_x,cust_y};
double cust_XYZ_lst[3];
double Y;
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
double curr_rgb_dst_lst_prp[3];
RGB2rgb(curr_rgb_dst_lst,curr_rgb_dst_lst_prp);
double curr_rgb_dst_lst_hsv[3];
rgb2hsv(curr_rgb_dst_lst,curr_rgb_dst_lst_hsv);
Y=0.212673*rOG+0.715152*gOG+0.072175*bOG;

double initSat=curr_rgb_dst_lst_hsv[1];

    double Sc_lst=1-(MAX(curr_rgb_dst_lst_prp[0],MAX(curr_rgb_dst_lst_prp[1],curr_rgb_dst_lst_prp[2]))-MIN(curr_rgb_dst_lst_prp[0],MIN(curr_rgb_dst_lst_prp[1],curr_rgb_dst_lst_prp[2])));

    double Sc_diff_scr=(1-(fabs(Sc_avg-Sc_lst)/(MAX(Sc_avg,MAX(1-Sc_avg,MAX(Sc_lst,1-Sc_lst))))));
    double Sat_diff_scr=(1-(fabs(initSat-avg_hsv_gc[1])/(MAX(avg_hsv_gc[1],MAX(1-avg_hsv_gc[1],MAX(initSat,1-initSat))))));
    double dnm=1-0.5*(Sc_diff_scr+Sc_lst);
    dnm=(dnm==0)?0:pow(Sc_lst,1.0/dnm);
double dst=pow(0.5*(0.5*(Sc_diff_scr*Sc_lst)*dnm+Sc_lst),1-Sc_lst);
double curr_sat=initSat*(1-(dst));
curr_sat=MIN(initSat,lerp(curr_sat,initSat,0.5*(Sc_diff_scr+initSat)*Sat_diff_scr));

curr_sat=MIN(initSat,lerp(curr_sat,initSat,0.5*(initSat+(1-Sc_diff_scr))));
double curr_diff=fabs(curr_sat-initSat)/(MAX(curr_sat,MAX(1-curr_sat,MAX(initSat,1-initSat))));
curr_rgb_dst_lst_hsv[1]=MAX(0,MIN(initSat,lerp(curr_sat,initSat,0.5*(((curr_sat)+(curr_diff))))));

if(dest!=0){
double man_dst=curr_rgb_dst_lst_hsv[1];

//man_dst=MAX(0,lerp(man_dst,-dest+dest*man_dst+man_dst,(Y*(curr_rgb_dst_lst_hsv[2]*man_dst))));

man_dst=MAX(0,(man_dst,-dest+dest*man_dst+man_dst));
    curr_rgb_dst_lst_hsv[1]=man_dst;
    double pst_dst_rgb[3];
    hsv2rgb(curr_rgb_dst_lst_hsv,pst_dst_rgb);
 Y=0.212673*pst_dst_rgb[0]+0.715152*pst_dst_rgb[1]+0.072175*pst_dst_rgb[2];
}


if(scrv!=-1){
double post_sat=curr_rgb_dst_lst_hsv[1];
double scrv_sat=curr_rgb_dst_lst_hsv[1]*2;
scrv_sat=(scrv_sat<0.5)?pow(fabs(0.5*scrv_sat),scrv):1-(0.5*pow(fabs(2-scrv_sat),scrv));
curr_rgb_dst_lst_hsv[1]=lerp(MAX(scrv_sat,post_sat),MIN(scrv_sat,post_sat),Y);
}


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

      if (dbg==1){
            double chroma=(MAX(WPchgRGB_lst[0],MAX(WPchgRGB_lst[1],WPchgRGB_lst[2]))-MIN(WPchgRGB_lst[0],MIN(WPchgRGB_lst[1],WPchgRGB_lst[2])));

        WPchgRGB_lst[0]=chroma;
        WPchgRGB_lst[1]=chroma;
        WPchgRGB_lst[2]=chroma;
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
          params->scurve = avs_defined(avs_array_elt(args, 4))?avs_as_float(avs_array_elt(args, 4)):-1;
          params->desat = avs_defined(avs_array_elt(args, 5))?avs_as_float(avs_array_elt(args, 5)):0;
          params->debug = avs_defined(avs_array_elt(args, 6))?avs_as_bool(avs_array_elt(args, 6)):false;


   fi->user_data = (void*) params;
   fi->get_frame = WhitePoint_get_frame;


   v = avs_new_value_clip(new_clip);

   avs_release_clip(new_clip);
  // free(params);
   return v;
}

const char* AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment* env)
{
   avs_add_function(env, "WhitePoint", "c[post]b[x]f[y]f[scurve]f[desat]f[debug]b", create_WhitePoint, 0);
   return "WhitePoint sample C plugin";
}
