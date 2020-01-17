#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../avisynth_c.h"
#include "functions_c.h"

typedef struct WhitePoint {
    int post;
        double x;
        double y;
        double contrast;
        double desat;
        int debug;
        double debug_pwr;
} WhitePoint;


AVS_VideoFrame* AVSC_CC WhitePoint_get_frame(AVS_FilterInfo* fi, int n)
{
 WhitePoint* params = (WhitePoint*) fi->user_data;

   AVS_VideoFrame* src = avs_get_frame(fi->child, n);

   int row_size, height, src_pitch,x, y, p,pst,dbg;
   BYTE* srcp;
   double CIEx,CIEy,rOG,bOG,gOG,cust_x,cust_y,cont,dest,pwr;

pst=params->post;
cust_x=params->x;
cust_y=params->y;
cont=params->contrast;
dest=params->desat;
dbg=params->debug;
pwr=params->debug_pwr;

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


double Sc_sum=0;
double Sc_avg=0;
    double avg_rgb_gc[3];
    double avg_hsv_gc[3];
    double avg_Y=0;
    double sum_Y=0;
    double sum_invK=0;
    double avg_invK=0;
    double sum_white=0;
    double avg_white=0;

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

double Sc=1-(fmax(curr_rgb_dst_prp[0],fmax(curr_rgb_dst_prp[1],curr_rgb_dst_prp[2]))-fmin(curr_rgb_dst_prp[0],fmin(curr_rgb_dst_prp[1],curr_rgb_dst_prp[2])));
sum_Y+=0.212673*rOG+0.715152*gOG+0.072175*bOG;
Sc_sum+=Sc;
sumR_gc+=curr_rgb_dst[0];
sumG_gc+= curr_rgb_dst[1];
sumB_gc+=curr_rgb_dst[2];
     sum_invK+=1-fmin(1-rOG,fmin(1-gOG,1-bOG));
     sum_white+=fmin(rOG,fmin(gOG,bOG));

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
avg_Y=sum_Y*rcp_counterAll;
avg_invK=sum_invK*rcp_counterAll;
avg_white=sum_white*rcp_counterAll;
}


      for (y=0; y<height; y++) {

      for (x=0; x<row_size; x++) {

                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcptwoFiveFive;     // B
       gOG=currGreen*rcptwoFiveFive;   //G
         rOG=currRed*rcptwoFiveFive;     // R
           double WPchgRGB_lst[3];
        if(rOG==0 && (gOG==0) && (bOG==0)){
    WPchgRGB_lst[0]=0;
    WPchgRGB_lst[1]=0;
    WPchgRGB_lst[2]=0;

}else{




double curr_rgb_dst_lst[3]={rOG,gOG,bOG};

if(pst==1){



double curr_rgb_dst_lst_hsv[3];


double curr_rgb_dst_lst_prp[3];
RGB2rgb(curr_rgb_dst_lst,curr_rgb_dst_lst_prp);

rgb2hsv(curr_rgb_dst_lst,curr_rgb_dst_lst_hsv);
 double Y=0.212673*rOG+0.715152*gOG+0.072175*bOG;
double whiteB=fmin(rOG,fmin(gOG,bOG));
double invK=1-fmin(1-rOG,fmin(1-gOG,1-bOG));
double initSat=curr_rgb_dst_lst_hsv[1];

    double Sc_lst=1-(fmax(curr_rgb_dst_lst_prp[0],fmax(curr_rgb_dst_lst_prp[1],curr_rgb_dst_lst_prp[2]))-fmin(curr_rgb_dst_lst_prp[0],fmin(curr_rgb_dst_lst_prp[1],curr_rgb_dst_lst_prp[2])));

    double Sc_diff_scr=(1-(fabs(Sc_avg-Sc_lst)/(fmax(Sc_avg,fmax(1-Sc_avg,fmax(Sc_lst,1-Sc_lst))))));
 double Y_diff_scr=(1-(fabs(Y-avg_Y)/(fmax(avg_Y,fmax(1-avg_Y,fmax(Y,1-Y))))));
 double invK_diff_scr=(1-(fabs(invK-avg_invK)/(fmax(avg_invK,fmax(1-avg_invK,fmax(invK,1-invK))))));
 double white_diff_scr=(1-(fabs(whiteB-avg_white)/(fmax(avg_white,fmax(1-avg_white,fmax(whiteB,1-whiteB))))));

    double Sat_diff_scr=(1-(fabs(initSat-avg_hsv_gc[1])/(fmax(avg_hsv_gc[1],fmax(1-avg_hsv_gc[1],fmax(initSat,1-initSat))))));
    double dnm=1-0.5*(Sc_diff_scr+Sc_lst);
    double dnm2=(dnm==0)?0:pow(Sc_lst,1.0/dnm);
double dst=pow(0.5*(0.5*(Sc_diff_scr*Sc_lst)*dnm2+Sc_lst),1-Sc_lst);
double curr_sat=initSat*(1-(dst));


curr_sat=curr_sat*(0.5*(Sc_diff_scr+initSat)*Sat_diff_scr);

curr_sat=lerp_clamp(curr_sat,initSat,third*(Sc_lst*Sc_diff_scr*initSat+ Sat_diff_scr+initSat*(1-Sc_diff_scr)));

double curr_diff=fabs(curr_sat-initSat)/(fmax(curr_sat,fmax(1-curr_sat,fmax(initSat,1-initSat))));

curr_sat=lerp_clamp(0,initSat,1-    0.5*((1-curr_diff)*(1-    initSat)+Sc_lst*(Sat_diff_scr)));
curr_sat=(curr_sat==1)?1:fmin(initSat,lerp_clamp(0,initSat,1-pow(curr_sat,initSat))/(1-curr_sat));
curr_sat=(initSat==0)?0:pow(curr_sat,(1.0/(2*initSat)));


curr_rgb_dst_lst_hsv[1]=curr_sat;


double man_dst=initSat;
if(dest!=0){

man_dst=curr_rgb_dst_lst_hsv[1];
double dst_cnt=(man_dst==1)?0:1-pow(man_dst,initSat)/(1-man_dst);
man_dst=-dest*Y_diff_scr*white_diff_scr*invK_diff_scr*dst_cnt+dest*man_dst*Y_diff_scr*white_diff_scr*invK_diff_scr*dst_cnt  +man_dst;
double man_dst_sat=fmax(0,man_dst);
    curr_rgb_dst_lst_hsv[1]=man_dst_sat;
    double pst_dst_rgb[3];
    hsv2rgb(curr_rgb_dst_lst_hsv,pst_dst_rgb);
 Y=0.212673*pst_dst_rgb[0]+0.715152*pst_dst_rgb[1]+0.072175*pst_dst_rgb[2];
 Y_diff_scr=(1-(fabs(Y-avg_Y)/(fmax(avg_Y,fmax(1-avg_Y,fmax(Y,1-Y))))));
 whiteB=fmin(pst_dst_rgb[0],fmin(pst_dst_rgb[1],pst_dst_rgb[2]));
invK=1-fmin(1-pst_dst_rgb[0],fmin(1-pst_dst_rgb[1],1-pst_dst_rgb[2]));
 invK_diff_scr=(1-(fabs(invK-avg_invK)/(fmax(avg_invK,fmax(1-avg_invK,fmax(invK,1-invK))))));
 white_diff_scr=(1-(fabs(whiteB-avg_white)/(fmax(avg_white,fmax(1-avg_white,fmax(whiteB,1-whiteB))))));
}


if(cont!=0){
        double col_scr=pow(cont,1-Y_diff_scr*white_diff_scr*invK_diff_scr)    ;
double cnt=(curr_rgb_dst_lst_hsv[1]==1)?1:pow(curr_rgb_dst_lst_hsv[1],initSat)/(1-curr_rgb_dst_lst_hsv[1]);
curr_rgb_dst_lst_hsv[1]=fmin(initSat,curr_rgb_dst_lst_hsv[1]+cnt*col_scr*cont*initSat);

}


        double curr_hsv_rgb[3] ;
  double invhuehsv_rgb[3] ;
  double invhuehsv_bk[3] ;


hsv2rgb(curr_rgb_dst_lst_hsv,curr_hsv_rgb);

     double invhuehsv[3] ={mod(curr_rgb_dst_lst_hsv[0]-0.25,1),curr_rgb_dst_lst_hsv[1],curr_rgb_dst_lst_hsv[2]};
hsv2rgb(invhuehsv,invhuehsv_rgb);

double col=(initSat==0)?1:0.5*((initSat-curr_rgb_dst_lst_hsv[1]/(initSat)));

  double shifted_rgb[3]={lerp(curr_hsv_rgb[0],invhuehsv_rgb[0],col),lerp(curr_hsv_rgb[1],invhuehsv_rgb[1],col),lerp(curr_hsv_rgb[2],invhuehsv_rgb[2],col)};
  rgb2hsv(shifted_rgb,invhuehsv_bk);
curr_rgb_dst_lst_hsv[0]=invhuehsv_bk[0];


double rgb_xyY[3];
double xyY_OG[3];
double xyY_dst[3];
hsv2rgb(curr_rgb_dst_lst_hsv,rgb_xyY);

rgb2xyY(rgb_xyY,xyY_dst);
rgb2xyY(curr_rgb_dst_lst,xyY_OG);
double xyY_dst_bk[3]={xyY_dst[0],xyY_dst[1],xyY_OG[2]};
xyY2rgb(xyY_dst_bk,WPchgRGB_lst);

if (cust_x!=CIEx || (cust_y!=CIEy)){

        if(rOG==0 && (gOG==0) && (bOG==0)){
    WPchgRGB_lst[0]=0;
    WPchgRGB_lst[1]=0;
    WPchgRGB_lst[2]=0;

}else{
    double rgbxyY_lst[3];
    double rgbXYZ_lst[3];
double rgbxyY_lst_bk[3];
double rgb_lst_bk[3];
rgb2xyY(WPchgRGB_lst,rgbxyY_lst);
xyY2XYZ(rgbxyY_lst,rgbXYZ_lst);

double cust_xy_lst[2]={cust_x,cust_y};
double cust_XYZ_lst[3];
xy2XYZ(cust_xy_lst,cust_XYZ_lst);
double WPConvXYZ4[3];
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

double cust_xy_lst[2]={cust_x,cust_y};
double cust_XYZ_lst[3];
double rgbxyY_lst[3];
double rgbXYZ_lst[3];
double WPConvXYZ4[3];
double rgbxyY_lst_bk[3];
double rgb_lst_bk[3];
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
}
      if (dbg==1){
            double chroma=pow(fmax(WPchgRGB_lst[0],fmax(WPchgRGB_lst[1],WPchgRGB_lst[2]))-fmin(WPchgRGB_lst[0],fmin(WPchgRGB_lst[1],WPchgRGB_lst[2])),pwr);

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
                    params->desat = avs_defined(avs_array_elt(args, 4))?avs_as_float(avs_array_elt(args, 4)):0;
          params->contrast = avs_defined(avs_array_elt(args, 5))?avs_as_float(avs_array_elt(args, 5)):0;

          params->debug = avs_defined(avs_array_elt(args, 6))?avs_as_bool(avs_array_elt(args, 6)):false;
          params->debug_pwr = avs_defined(avs_array_elt(args, 7))?avs_as_float(avs_array_elt(args, 7)):1;

   fi->user_data = (void*) params;
   fi->get_frame = WhitePoint_get_frame;


   v = avs_new_value_clip(new_clip);

   avs_release_clip(new_clip);
  // free(params);
   return v;
}

const char* AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment* env)
{
   avs_add_function(env, "WhitePoint", "c[post]b[x]f[y]f[desat]f[contrast]f[debug]b[debug_pwr]f", create_WhitePoint, 0);
   return "WhitePoint sample C plugin";
}
