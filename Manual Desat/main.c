#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../avisynth_c.h"
#include "functions_c.h"

typedef struct Manual_Desat {
    int debug;
        double start;
        double contrast;
        double desat;
        double neg_bias;
} Manual_Desat;

AVS_VideoFrame* AVSC_CC Manual_Desat_get_frame(AVS_FilterInfo* fi, int n)
{
 Manual_Desat* params = (Manual_Desat*) fi->user_data;

   AVS_VideoFrame* src = avs_get_frame(fi->child, n);

   int row_size, height, src_pitch,x,y,dbg,bis;
   BYTE* srcp;
   double rOG,bOG,gOG,cont,dest;

dbg=params->debug;
cont=params->contrast ;
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
double hueCount[360];
double hueCount_prp[360];
/*
double hueCount_b[121];
double hueCount_b_prp[121];
double hueCount_g[121];
double hueCount_g_prp[121];
double hueCount_r[121];
double hueCount_r_prp[121];
*/
/*
double sat_Count[101];
double sat_Count_prp[101];
double sat_Count_l[101];
double sat_Count_l_prp[101];
double ds=0;
double ds_l=0;*/
//double raw_sat_sum=0;
//double raw_sat_avg=0;
//double counter=0;
/*double hueCount_sat[360];
double hueCount_sat_avg[360];
double hueCount_sat_dst[360];
double hueCount_sat_sum[360];
double last_hueCount_sd[360];
*/

for (int i=359; i>=0; i--){
    hueCount[i]=0;
    hueCount_prp[i]=0;
   // hueCount_sat_sum[i]=0;
    //hueCount_sat[i]=0;
    //last_hueCount_sd[i]=0;
}
/*
for (int i=120; i>=0; i--){
 hueCount_b[i]=0;
 hueCount_b_prp[i]=0;
  hueCount_g[i]=0;
 hueCount_b_prp[i]=0;
  hueCount_r[i]=0;
 hueCount_r_prp[i]=0;
}*/


/*
for (int i=100; i>=0; i--){
    sat_Count[i]=0;
    sat_Count_prp[i]=0;
    sat_Count_l[i]=0;
    sat_Count_l_prp[i]=0;
   // hueCount_sat_sum[i]=0;
    //hueCount_sat[i]=0;
    //last_hueCount_sd[i]=0;
}*/
///////////////ACTUALLY DRAW PIXELS///////////////////////////////////////

      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcptwoFiveFive;     // B
       gOG=currGreen*rcptwoFiveFive;   //G
         rOG=currRed*rcptwoFiveFive;     // R


         if(!((currRed==currGreen)&&(currGreen==currBlue))){

        double curr_rgb_dst_OG[3]={rOG,gOG,bOG};
        double curr_rgb_dst[3];

         sRGB2Linear(curr_rgb_dst_OG,curr_rgb_dst);
double curr_rgb_dst_prp[3];
//double curr_rgb_dst_lin[3];

//sRGB2Linear(curr_rgb_dst,curr_rgb_dst_lin);
RGB2rgb(curr_rgb_dst,curr_rgb_dst_prp);
        double curr_rgb_dst_hsvnc[5];
rgb2hsv_min_chr(curr_rgb_dst,curr_rgb_dst_hsvnc);
//double invK=1-MIN(1-curr_rgb_dst[0],MIN(1-curr_rgb_dst[1],1-curr_rgb_dst[2]));
double Sc=1-(MAX(curr_rgb_dst_prp[0],MAX(curr_rgb_dst_prp[1],curr_rgb_dst_prp[2]))-MIN(curr_rgb_dst_prp[0],MIN(curr_rgb_dst_prp[1],curr_rgb_dst_prp[2])));

int hueEl=round(curr_rgb_dst_hsvnc[0]*360);
hueEl=(hueEl==360)?0:hueEl;



//double hsvightness=0.5*(curr_rgb_dst_hsvnc[2]+curr_rgb_dst_hsvnc[3]);
//double sat_l=((hsvightness==1)||(hsvightness==0))?0:curr_rgb_dst_hsvnc[4]/(1-fabs(2*hsvightness-1));


/*
int hsv_el=round(curr_rgb_dst_hsvnc[1]*100);
int hsl_el=round(sat_l*100);
*/
//raw_sat_sum+=curr_rgb_dst_hsvnc[1];
//counter+=1;

//sat_Count[hsv_el]+=1;
//sat_Count_l[hsl_el]+=1;
hueCount[hueEl]+=Sc;

      }

      }

x+=3;

}

//double hueSum=0;
//double hsvSum=0;
//double hslSum=0;

double h_sum=0;

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
/*
for (int i=100; i>=0; i--){
    hsvSum+=sat_Count[i];
    hslSum+=sat_Count_l[i];
}*/

for (int i=359; i>=0; i--){
    h_sum+=hueCount[i];


}
double min_hue=1;
double max_hue=0;

for (int i=359; i>=0; i--){

double curr=1-hueCount[i]/h_sum;
min_hue=(curr<min_hue)?curr:min_hue;
max_hue=(curr>max_hue)?curr:max_hue;
hueCount_prp[i]=curr;
}

            for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {


//double x_shift=(double)x/(double)row_size;
                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcptwoFiveFive;
       gOG=currGreen*rcptwoFiveFive;
         rOG=currRed*rcptwoFiveFive;
double curr_hue;
        double curr_rgb_dst_OG[3]={rOG,gOG,bOG};
        double curr_rgb_dst[3];
         double curr_rgb_dst_fnl[3];
         sRGB2Linear(curr_rgb_dst_OG,curr_rgb_dst);
double invK;
double dist;
double Sc;
double init_Sat;
           if((currRed==currGreen)&&(currGreen==currBlue)){
curr_rgb_dst_fnl[0]=curr_rgb_dst_OG[0];
curr_rgb_dst_fnl[1]=curr_rgb_dst_OG[1];
curr_rgb_dst_fnl[2]=curr_rgb_dst_OG[2];
           }else{
double curr_rgb_dst_xyY[3];

LinRGB2xyY(curr_rgb_dst,curr_rgb_dst_xyY);



        double curr_rgb_dst_hsvnc[5];
rgb2hsv_min_chr(curr_rgb_dst,curr_rgb_dst_hsvnc);
 init_Sat=curr_rgb_dst_hsvnc[1];
double init_chr=curr_rgb_dst_hsvnc[4];
int hueEl=round(curr_rgb_dst_hsvnc[0]*360);
hueEl=(hueEl==360)?0:hueEl;
double rgb_avg=(curr_rgb_dst[0]+curr_rgb_dst[1]+curr_rgb_dst[2])*third;
 dist=fastPrecisePow(sqrt((curr_rgb_dst[0]-rgb_avg)*(curr_rgb_dst[0]-rgb_avg) + (curr_rgb_dst[1]-rgb_avg)*(curr_rgb_dst[1]-rgb_avg) + (curr_rgb_dst[2]-rgb_avg)*(curr_rgb_dst[2]-rgb_avg))/sqrt(3),0.4711087);
 curr_hue=(max_hue==0)?0:((hueCount_prp[hueEl]-min_hue)/max_hue);
//double delt_sgn=(curr_hue<h_rgb_avg)?1:-1;
/*
 Sc=(MAX(curr_rgb_dst[0],MAX(curr_rgb_dst[1],curr_rgb_dst[2]))-MIN(curr_rgb_dst[0],MIN(curr_rgb_dst[1],curr_rgb_dst[2])));
double wht=(MIN(curr_rgb_dst[0],MIN(curr_rgb_dst[1],curr_rgb_dst[2])));

 double hsLightness_fnl=0.5*(curr_rgb_dst_hsvnc[2]+curr_rgb_dst_hsvnc[3]);
double satL_fnl=((hsLightness_fnl==1)||(hsLightness_fnl==0))?0:curr_rgb_dst_hsvnc[4]/(1-fabs(2*hsLightness_fnl-1));
*/

curr_rgb_dst_hsvnc[1]-=dest*(1-fastPrecisePow(0.5*(init_Sat+(1-dist)),0.5*((1-init_chr)+(1-curr_hue))))     ;
curr_rgb_dst_hsvnc[1]=MIN(init_Sat,MAX(0, curr_rgb_dst_hsvnc[1]));
double rel_sat_redu=(init_Sat==0)?1:fabs(init_Sat-curr_rgb_dst_hsvnc[1])/init_Sat;
double pre_shift_rgb[3];
hsvnc2rgb(curr_rgb_dst_hsvnc,pre_shift_rgb);
double hued=curr_rgb_dst_hsvnc[0];
double bias=-third*rel_sat_redu;

if(  ((hued>0)&&(hued<sixty_deg)) || ((hued>third)&&(hued<0.5))  || ((hued>2*third)&&(hued<threeHun_deg)) ) {

    bias=-third*rel_sat_redu;
}else if (     ((hued>sixty_deg)&&(hued<third)) || ((hued>0.5)&&(hued<2*third))  || ((hued>threeHun_deg)&&(hued<1))  || (bis==0) ){
bias=third*rel_sat_redu;
}

double col=1-   0.5*(rel_sat_redu);
curr_rgb_dst_hsvnc[0]=mod(hued+bias,1);

double WPchgRGB_shft[3];
double WPchgRGB_shft_h[3];

hsvnc2rgb(curr_rgb_dst_hsvnc,WPchgRGB_shft);

  WPchgRGB_shft[0]=lerp(WPchgRGB_shft[0],pre_shift_rgb[0],col);
  WPchgRGB_shft[1]=lerp(WPchgRGB_shft[1],pre_shift_rgb[1],col);
  WPchgRGB_shft[2]=lerp(WPchgRGB_shft[2],pre_shift_rgb[2],col);

rgb2hsv(WPchgRGB_shft,WPchgRGB_shft_h);
curr_rgb_dst_hsvnc[0]=WPchgRGB_shft_h[0];

/*
if (cont<1){
        double chr=curr_rgb_dst_hsvnc[1]*curr_rgb_dst_hsvnc[2];
    double chr_dff=((init_chr==0)||(init_chr<chr))?0:(init_chr - chr)/init_chr;
        curr_rgb_dst_hsvnc[1]=(chr_dff>cont)?MIN(init_Sat,MAX(0,init_chr*(1-cont)*curr_rgb_dst_hsvnc[2])):  curr_rgb_dst_hsvnc[1];
}
*/

double curr_rgb_dst_4xyY[3];
hsvnc2rgb(curr_rgb_dst_hsvnc,curr_rgb_dst_4xyY);

double curr_rgb_dst_fnl_xyY[3];
    LinRGB2xyY(curr_rgb_dst_4xyY,curr_rgb_dst_fnl_xyY);
curr_rgb_dst_fnl_xyY[2]=curr_rgb_dst_xyY[2];
xyY2rgb(curr_rgb_dst_fnl_xyY,curr_rgb_dst_fnl);



           }


if (dbg==1){

            double dbg_mx=MAX(curr_rgb_dst_fnl[0],MAX(curr_rgb_dst_fnl[1],curr_rgb_dst_fnl[2]));
        double dbg_mn=MIN(curr_rgb_dst_fnl[0],MIN(curr_rgb_dst_fnl[1],curr_rgb_dst_fnl[2]));
    double sat_dbg=(dbg_mx==0)?0:(dbg_mx-dbg_mn)/dbg_mx;


                srcp[x] = MAX(MIN(round(sat_dbg*255),255),0);
             srcp[x+1] =MAX(MIN(round(sat_dbg*255),255),0);
        srcp[x+2] = MAX(MIN(round(sat_dbg*255),255),0);


}else{

                srcp[x] = MAX(MIN(round(curr_rgb_dst_fnl[2]*255),255),0);
             srcp[x+1] =MAX(MIN(round(curr_rgb_dst_fnl[1]*255),255),0);
        srcp[x+2] = MAX(MIN(round(curr_rgb_dst_fnl[0]*255),255),0);

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
                  params->contrast = avs_defined(avs_array_elt(args, 3))?avs_as_float(avs_array_elt(args, 3)):1.0;
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
   avs_add_function(env, "Manual_Desat", "c[debug]b[desat]f[contrast]f[neg_bias]b", create_Manual_Desat, 0);
   return "Manual_Desat sample C plugin";
}
