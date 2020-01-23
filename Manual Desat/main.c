#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../avisynth_c.h"
#include "functions_c.h"

typedef struct Manual_Desat {
    int debug;
        double start;
        double min_chroma;
        double desat;
        double neg_bias;
} Manual_Desat;

AVS_VideoFrame* AVSC_CC Manual_Desat_get_frame(AVS_FilterInfo* fi, int n)
{
 Manual_Desat* params = (Manual_Desat*) fi->user_data;

   AVS_VideoFrame* src = avs_get_frame(fi->child, n);

   int row_size, height, src_pitch,x,y,dbg,bis;
   BYTE* srcp;
   double rOG,bOG,gOG,mnch,dest;

dbg=params->debug;
mnch=params->min_chroma ;
dest=params-> desat ;
bis=params->neg_bias;

   //CIEx= 0.312727;
     // CIEy= 0.329023;
     // double D65XYZ[3]={0.95047,1,1.08883};
            int planes[] ={AVS_CS_BGR32};
src = avs_get_frame(fi->child, n);
   avs_make_writable(fi->env, &src);

      srcp = avs_get_write_ptr_p(src, planes[0]);
      src_pitch = avs_get_pitch_p(src, planes[0]);
      row_size = avs_get_row_size_p(src, planes[0]);
      height = avs_get_height_p(src, planes[0]);

/*
double counterAll=0;

double sumR=0;
double sumG=0;
double sumB=0;

double sumR_dst=0;
double sumG_dst=0;
double sumB_dst=0;
*/
//double hueCount[360];
//double hueCount_prp[360];
double sat_Count[101];
double sat_Count_prp[101];
double sat_Count_l[101];
double sat_Count_l_prp[101];
double ds=0;
double ds_l=0;
//double raw_sat_sum=0;
//double raw_sat_avg=0;
//double counter=0;
/*double hueCount_sat[360];
double hueCount_sat_avg[360];
double hueCount_sat_dst[360];
double hueCount_sat_sum[360];
double last_hueCount_sd[360];
*/
/*
for (int i=359; i>=0; i--){
    hueCount[i]=0;
    hueCount_prp[i]=0;
   // hueCount_sat_sum[i]=0;
    //hueCount_sat[i]=0;
    //last_hueCount_sd[i]=0;
}*/

for (int i=100; i>=0; i--){
    sat_Count[i]=0;
    sat_Count_prp[i]=0;
    sat_Count_l[i]=0;
    sat_Count_l_prp[i]=0;
   // hueCount_sat_sum[i]=0;
    //hueCount_sat[i]=0;
    //last_hueCount_sd[i]=0;
}
///////////////ACTUALLY DRAW PIXELS///////////////////////////////////////

      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcptwoFiveFive;     // B
       gOG=currGreen*rcptwoFiveFive;   //G
         rOG=currRed*rcptwoFiveFive;     // R



double curr_rgb_dst[3]={rOG,gOG,bOG};
double curr_rgb_dst_lin[3];

sRGB2Linear(curr_rgb_dst,curr_rgb_dst_lin);
        double curr_rgb_dst_hsvnc[5];
rgb2hsv_min_chr(curr_rgb_dst_lin,curr_rgb_dst_hsvnc);

double hsvightness=0.5*(curr_rgb_dst_hsvnc[2]+curr_rgb_dst_hsvnc[3]);
double sat_l=((hsvightness==1)||(hsvightness==0))?0:curr_rgb_dst_hsvnc[4]/(1-fabs(2*hsvightness-1));

/*
int hueEl=round(curr_rgb_dst_hsvnc[0]*360);
hueEl=(hueEl==360)?0:hueEl;
*/
int hsv_el=round(curr_rgb_dst_hsvnc[1]*100);
int hsl_el=round(sat_l*100);
//raw_sat_sum+=curr_rgb_dst_hsvnc[1];
//counter+=1;

sat_Count[hsv_el]+=1;
sat_Count_l[hsl_el]+=1;
//hueCount[hueEl]+=1;

      }

x+=3;

}



//double hueSum=0;
double hsvSum=0;
double hslSum=0;
/*
raw_sat_avg=raw_sat_sum/counter;
for (int i=359; i>=0; i--){
    hueSum+=hueCount[i];
   // last_hueCount_sd[i]*=(hueCount[i])/(hueCount[i]-1); //Bessel correct
}
for (int i=359; i>=0; i--){
    hueCount_prp[i]=hueCount[i]/hueSum;
   // hueCount_sat_avg[i]=(hueCount[i]==0)?0:hueCount_sat_sum[i]/hueCount[i];
 //   hueCount_sat_dst[i]= hueCount_sat_avg[i]+ last_hueCount_sd[i]; //x standard devs from mean
}*/

for (int i=100; i>=0; i--){
    hsvSum+=sat_Count[i];
    hslSum+=sat_Count_l[i];
}


double i_dbl=100;
double intp;

for (int i=100; i>=0; i--){
         intp=(1-(i_dbl+0.5)*0.01);

sat_Count_prp[i]=sat_Count[i]/hsvSum;
        ds=(i<100)?ds+fabs(sat_Count_prp[i+1]-sat_Count_prp[i])*intp:ds;
      sat_Count_l_prp[i]=sat_Count_l[i]/hslSum;
    ds_l=(i<100)?ds_l+fabs(sat_Count_l_prp[i+1]-sat_Count_l_prp[i]):ds_l;

i_dbl-=1;
}

ds=ds*0.01;
ds_l=ds_l*0.01;
            for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

//double x_shift=(double)x/(double)row_size;
                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcptwoFiveFive;
       gOG=currGreen*rcptwoFiveFive;
         rOG=currRed*rcptwoFiveFive;

double curr_rgb_dst_fnl[3];
double WPchgRGB_bk[3];
double WPchgRGB_bk_gc[3];
if ((rOG==0)&&(gOG==0)&&(bOG==0)){

    WPchgRGB_bk_gc[0]=0;
    WPchgRGB_bk_gc[1]=0;
    WPchgRGB_bk_gc[2]=0;
}else{

curr_rgb_dst_fnl[0]=rOG;
curr_rgb_dst_fnl[1]=gOG;
curr_rgb_dst_fnl[2]=bOG;
double curr_rgb_dst_fnl_Lin[3];
sRGB2Linear(curr_rgb_dst_fnl,curr_rgb_dst_fnl_Lin);

        double curr_rgb_dst_fnl_hsvnc[5];
rgb2hsv_min_chr(curr_rgb_dst_fnl_Lin,curr_rgb_dst_fnl_hsvnc);

 double hsvightness_fnl=0.5*(curr_rgb_dst_fnl_hsvnc[2]+curr_rgb_dst_fnl_hsvnc[3]);
double satL_fnl=((hsvightness_fnl==1)||(hsvightness_fnl==0))?0:curr_rgb_dst_fnl_hsvnc[4]/(1-fabs(2*hsvightness_fnl-1));

double init_Sat=curr_rgb_dst_fnl_hsvnc[1];
double init_chr=curr_rgb_dst_fnl_hsvnc[4];
double curr_rgb_dst_fnl_xyY[3];
//double curr_rgb_dst_fnl_prp[3];
LinRGB2xyY(curr_rgb_dst_fnl_Lin,curr_rgb_dst_fnl_xyY);
double invK=fastPrecisePow(1-MIN(1-(curr_rgb_dst_fnl_Lin[0]),MIN(1-(curr_rgb_dst_fnl_Lin[1]),1-(curr_rgb_dst_fnl_Lin[2]))),3.38392888);
//RGB2rgb(curr_rgb_dst_fnl_Lin,curr_rgb_dst_fnl_prp);

//double Sc=MAX(curr_rgb_dst_fnl_prp[0],MAX(curr_rgb_dst_fnl_prp[1],curr_rgb_dst_fnl_prp[2]))-MIN(curr_rgb_dst_fnl_prp[0],MIN(curr_rgb_dst_fnl_prp[1],curr_rgb_dst_fnl_prp[2]));

/*int hue=round(curr_rgb_dst_fnl_hsvnc[0]*360);
hue=(hue==360)?0:hue;
 double hue_pr=1-fastPrecisePow(hueCount_prp[hue],0.1807568);*/
int hsv_fnl_el=round(curr_rgb_dst_fnl_hsvnc[1]*100);
int hsL_fnl_el=round(satL_fnl*100);
double sat_pr=fastPrecisePow(sat_Count_prp[hsv_fnl_el],0.23533962);
double satl_pr=sat_Count_l_prp[hsL_fnl_el];
  double dlpw=fastPrecisePow(init_Sat,ds);
  double dlpw_l=fastPrecisePow(satL_fnl,ds_l);

    if(dest>0){
double sat1=curr_rgb_dst_fnl_hsvnc[1]-(dest*(dlpw)*dlpw_l*(1-sat_pr)*(1-satl_pr));
curr_rgb_dst_fnl_hsvnc[1]= MAX(0,MIN(lerp(init_Sat,sat1,(1-init_Sat)*(invK)*(            satL_fnl)*(1-sat1)*(1-satL_fnl)*(1-(satl_pr)*( 1-sat_pr))),1));
}

if (mnch<1){
        double chr=curr_rgb_dst_fnl_hsvnc[1]*curr_rgb_dst_fnl_hsvnc[2];
    double chr_dff=((init_chr==0)||(init_chr<chr))?0:(init_chr - chr)/init_chr;
//double lrp_chr=(init_chr==0)?0:(chr_dff* (invK - 1)* (satL_fnl - 1)* (1 - ds))/init_chr + invK;
        curr_rgb_dst_fnl_hsvnc[1]=(chr_dff>mnch)?MIN(init_Sat,MAX(0,init_chr*(1-mnch)*curr_rgb_dst_fnl_hsvnc[2])):  curr_rgb_dst_fnl_hsvnc[1];
//curr_rgb_dst_fnl_hsvnc[1]=(raw_sat_avg==0)?init_Sat-dest:lerp_clamp(init_Sat,0 ,mnch+((sat_pr)*hue_pr*(1-init_Sat)*(1-satL_fnl)*(1-invK)*(curr_rgb_dst_fnl_xyY[2])*(1-Sc)*ds*(1-init_Sat))*(1-raw_sat_avg));
}

double rel_sat_redu=(init_Sat==0)?1:fabs(init_Sat-curr_rgb_dst_fnl_hsvnc[1])/init_Sat;

double pre_shift_rgb_Lin[3];
double pre_shift_rgb_gc[3];
hsvnc2rgb(curr_rgb_dst_fnl_hsvnc,pre_shift_rgb_Lin);

double pre_shift_rgb_bk_Lin[3];

pre_shift_rgb_bk_Lin[0]=MAX(pre_shift_rgb_Lin[0],curr_rgb_dst_fnl_Lin[0]);
pre_shift_rgb_bk_Lin[1]=MAX(pre_shift_rgb_Lin[1],curr_rgb_dst_fnl_Lin[1]);
pre_shift_rgb_bk_Lin[2]=MAX(pre_shift_rgb_Lin[2],curr_rgb_dst_fnl_Lin[2]);

rgb2hsv_min_chr(pre_shift_rgb_bk_Lin,curr_rgb_dst_fnl_hsvnc);

double hued=curr_rgb_dst_fnl_hsvnc[0];
double bias=-third*rel_sat_redu;

if(  ((hued>0)&&(hued<sixty_deg)) || ((hued>third)&&(hued<0.5))  || ((hued>2*third)&&(hued<threeHun_deg)) ) {

    bias=-third*rel_sat_redu;
}else if (     ((hued>sixty_deg)&&(hued<third)) || ((hued>0.5)&&(hued<2*third))  || ((hued>threeHun_deg)&&(hued<1))  || (bis==0) ){
bias=third*rel_sat_redu;
}

double col=1-   0.5*(rel_sat_redu);
curr_rgb_dst_fnl_hsvnc[0]=mod(curr_rgb_dst_fnl_hsvnc[0]+bias,1);

/*
double sat_diff=fabs(init_Sat-curr_rgb_dst_fnl_hsv[1]);
double sat_diff_prp=(init_Sat==0)?0:sat_diff/(init_Sat);
double chr_chg=sat_diff_prp*curr_rgb_dst_fnl_hsv[2];
if(dest>0){
curr_rgb_dst_fnl_hsv[1]=(curr_rgb_dst_fnl_hsv[2]==0)?curr_rgb_dst_fnl_hsv[1]:MAX(curr_rgb_dst_fnl_hsv[1],MIN(init_Sat,lerp(curr_rgb_dst_fnl_hsv[1],curr_rgb_dst_fnl_hsv[1]+sat_diff*(dest/curr_rgb_dst_fnl_hsv[2]),   lrp)));
}
*/
 //subt=MIN(1,(fastPrecisePow(fastPrecisePow(0.25*(2*Sc+Y+invK),1-curr_rgb_dst_fnl_hsv[1]),1.08880107)));

//curr_rgb_dst_fnl_hsv[1]=MAX(0,(curr_rgb_dst_fnl_hsv[1]-mnch*subt*(1-hueCount_prp[hue])));
//double lrp=(init_Sat==0)?1-subt:(1-(1-hueCount_prp[hue])*(fabs(curr_rgb_dst_fnl_hsv[1]-init_Sat)/init_Sat))*(1-subt);
//curr_rgb_dst_fnl_hsv[1]=lerp_clamp(curr_rgb_dst_fnl_hsv[1],init_Sat,(1-hueCount_prp[hue])*pow((curr_rgb_dst_fnl_hsv[1]*init_Sat),pow(lrp,dest) ));

double WPchgRGB_Lin_shft[3];
double WPchgRGB_Lin_shft_h[3];
double WPchgRGB_Lin[3];
double WPchgRGB_xyY[3];

hsvnc2rgb(curr_rgb_dst_fnl_hsvnc,WPchgRGB_Lin_shft);

  WPchgRGB_Lin_shft[0]=lerp(WPchgRGB_Lin_shft[0],pre_shift_rgb_Lin[0],col);
  WPchgRGB_Lin_shft[1]=lerp(WPchgRGB_Lin_shft[1],pre_shift_rgb_Lin[1],col);
  WPchgRGB_Lin_shft[2]=lerp(WPchgRGB_Lin_shft[2],pre_shift_rgb_Lin[2],col);


rgb2hsv(WPchgRGB_Lin_shft,WPchgRGB_Lin_shft_h);
curr_rgb_dst_fnl_hsvnc[0]=WPchgRGB_Lin_shft_h[0];

hsvnc2rgb(curr_rgb_dst_fnl_hsvnc,WPchgRGB_Lin);

LinRGB2xyY(WPchgRGB_Lin,WPchgRGB_xyY);
WPchgRGB_xyY[2]=curr_rgb_dst_fnl_xyY[2];
xyY2LinRGB(WPchgRGB_xyY,WPchgRGB_bk);
Linear2sRGB(WPchgRGB_bk,WPchgRGB_bk_gc);
}

if(dbg==1){
        double dbg_mx=MAX(WPchgRGB_bk_gc[0],MAX(WPchgRGB_bk_gc[1],WPchgRGB_bk_gc[2]));
        double dbg_mn=MIN(WPchgRGB_bk_gc[0],MIN(WPchgRGB_bk_gc[1],WPchgRGB_bk_gc[2]));
    double sat_dbg=(dbg_mx==0)?0:(dbg_mx-dbg_mn)/dbg_mx    ;
 //double dbg_light=0.5*(dbg_mx+dbg_mn);
//double dbg_satL=((dbg_light==1)||(dbg_light==0))?0:chroma/(1-fabs(2*dbg_light-1));

                srcp[x] = MAX(MIN(round(sat_dbg*255),255),0);
             srcp[x+1] =MAX(MIN(round(sat_dbg*255),255),0);
        srcp[x+2] = MAX(MIN(round(sat_dbg*255),255),0);

}else{
                srcp[x] = MAX(MIN(round(WPchgRGB_bk_gc[2]*255),255),0);
             srcp[x+1] =MAX(MIN(round(WPchgRGB_bk_gc[1]*255),255),0);
        srcp[x+2] = MAX(MIN(round(WPchgRGB_bk_gc[0]*255),255),0);
}



x+=3;
}
   srcp += src_pitch;
      }






   return src;
}


AVS_Value AVSC_CC create_Manual_Desat(AVS_ScriptEnvironment* env, AVS_Value args, void* user_data)
{
   AVS_Value v;
   AVS_FilterInfo* fi;


   AVS_Clip* new_clip = avs_new_c_filter(env, &fi, avs_array_elt(args, 0), 1);

Manual_Desat *params = (Manual_Desat*)malloc(sizeof(Manual_Desat));

if (!params)
      return avs_void;


   if (!avs_is_rgb(&fi->vi)) {
      return avs_new_value_error("Input video must be in RGB format!");
   }

          params->debug = avs_defined(avs_array_elt(args, 1))?avs_as_bool(avs_array_elt(args, 1)):false;

          params->desat = avs_defined(avs_array_elt(args, 2))?avs_as_float(avs_array_elt(args, 2)):0.0;
                  params->min_chroma = avs_defined(avs_array_elt(args, 3))?avs_as_float(avs_array_elt(args, 3)):1.0;
          params->neg_bias = avs_defined(avs_array_elt(args, 4))?avs_as_bool(avs_array_elt(args, 4)):true;

   fi->user_data = (void*) params;
   fi->get_frame = Manual_Desat_get_frame;


   v = avs_new_value_clip(new_clip);

   avs_release_clip(new_clip);
  // free(params);
   return v;
}

const char* AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment* env)
{
   avs_add_function(env, "Manual_Desat", "c[debug]b[desat]f[min_chroma]f[neg_bias]b", create_Manual_Desat, 0);
   return "Manual_Desat sample C plugin";
}
