#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "avisynth_c.h"
#include "functions_c.h"

typedef struct WhitePoint {
      double step;
      double start;
      double sensitivity;
      int debug;
      int fails;
} WhitePoint;

AVS_VideoFrame* AVSC_CC WhitePoint_get_frame(AVS_FilterInfo* fi, int n)
{
 WhitePoint* params = (WhitePoint*) fi->user_data;

   AVS_VideoFrame* src = avs_get_frame(fi->child, n);



   int row_size, height, src_pitch,x, y, p,dbg,fail;
   BYTE* srcp;
   double CIEx,CIEy,rOG,bOG,gOG,stp,strt,sns,HWblack,grey_metric,mn,mx,sat;

stp =   params->step;
strt=params->start;
dbg=params->debug;
sns=params->sensitivity;
fail=params->fails;

   CIEx= 0.312727;
      CIEy= 0.329023;
            int planes[] ={AVS_CS_BGR32};
src = avs_get_frame(fi->child, n);
   avs_make_writable(fi->env, &src);

p=0;
      srcp = avs_get_write_ptr_p(src, planes[p]);
      src_pitch = avs_get_pitch_p(src, planes[p]);
      row_size = avs_get_row_size_p(src, planes[p]);
      height = avs_get_height_p(src, planes[p]);
/*
double **avrg;

avrg = (double**)malloc(stp*sizeof(double));
for(p = 0; p < stp; p++)
{
    avrg[p] = (double*)malloc(10*sizeof(double));

}

for (p=0; p<stp; p++){
        for (q=0; q<10; q++){

                avrg[p][q]=0;
}
}
*/
double avrg[]={0,0,0,0,0,0};

double bestSSD;
double bestx=0.312727;
double besty=0.329023;
double best_x=0;
double best_y=0;

double curr=strt;

int count_ssd=0;

int loopCount=1;

double last_ssd_r=0;
double last_sum_r=0;

double last_sum_g=0;
double last_ssd_g=0;

double last_sum_b=0;
double last_ssd_b=0;

int run_fail=0;

if (dbg!=1){


while(curr>0){
//POLL FRAME/////////////////////////////////////////////////////////
      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue/255.0;     // B
       gOG=currGreen/255.0;   //G
         rOG=currRed/255.0;  // R

         mn=MIN(rOG,MIN(gOG,bOG));
 mx=MAX(rOG,MAX(gOG,bOG));

sat=(mx==0)?0:(mx-mn)/mx;


HWblack=1-mx;
//double HwhiteB=(1-sat)*mx;
grey_metric=1-(sat*HWblack);


        double currTst=(sns==1)?curr:pow(curr,sns);

 if(grey_metric>=currTst&&(grey_metric<=strt) && (grey_metric<1)){
double rgbxyY[]={rOG,gOG,bOG};
double xyY[3];
rgb2xyY(rgbxyY,xyY);
     avrg[0]+=xyY[0];
      avrg[1]+=xyY[1];

double dist_r=sqrt(pow(0.64-xyY[0],2)+pow(0.33-xyY[1],2));
double dist_g=sqrt(pow(0.3-xyY[0],2)+pow(0.6-xyY[1],2));
double dist_b=sqrt(pow(0.15-xyY[0],2)+pow(0.06-xyY[1],2));

if(count_ssd<1){
last_sum_r=dist_r;
last_sum_g=dist_g;
last_sum_b=dist_b;

}else{
last_ssd_r+=(dist_r-((last_sum_r+dist_r)/(count_ssd+1)))*(dist_r-(last_sum_r/count_ssd));
last_sum_r+=dist_r;

last_ssd_g+=(dist_g-((last_sum_g+dist_g)/(count_ssd+1)))*(dist_g-(last_sum_g/count_ssd));
last_sum_g+=dist_g;

last_ssd_b+=(dist_b-((last_sum_b+dist_b)/(count_ssd+1)))*(dist_b-(last_sum_b/count_ssd));
last_sum_b+=dist_b;



     avrg[3]=last_ssd_r;
           avrg[4]=last_ssd_g;
avrg[5]=last_ssd_b;

}
count_ssd++;




       avrg[2]+=1;
   }

        x=x+3;

      }





      }
//y+=smp;


/////////////POLL FRAME END///////////////////////

double lpx=(avrg[2]==0)?bestx:avrg[0]/avrg[2];
double lpy=(avrg[2]==0)?besty:avrg[1]/avrg[2];
double lpSSDr=avrg[3];
double lpSSDb=avrg[4];
double lpSSDg=avrg[5];
double cumulSSD=lpSSDr+lpSSDb+lpSSDg;

if(loopCount>=2){
            if(cumulSSD<=bestSSD ){
                run_fail=0;
                if (avrg[2]>=2){
                bestSSD=cumulSSD;
                best_x=lpx;
                best_y=lpy;
                }

            }else{
                run_fail++;
                curr=(run_fail>=fail)?0:curr; //Early termination if no. of consecutive fails achieved
            }

}else{
        bestSSD=6;
        best_x=lpx;
        best_y=lpy;

}

avrg[0]=0;
avrg[1]=0;
avrg[2]=0;
avrg[3]=0;
avrg[4]=0;
avrg[5]=0;
count_ssd=0;
loopCount++;
last_sum_r=0;
last_ssd_r=0;
last_sum_g=0;
last_ssd_g=0;
last_sum_b=0;
last_ssd_b=0;

curr-=stp;
}


//AVERAGE RGB to grey xy/////////////////////////////////////////
double XYZ_orig[3];
double XYZ_conv2grey[3];
double XYZ_Forgrey_xy[3];


double xyForGrey[]={best_x,best_y};
double XYZ_grey[]={0.95047,1,1.08883};
xy2XYZ(xyForGrey,XYZ_orig);
WPconv2Grey(XYZ_orig,XYZ_grey,XYZ_conv2grey);
XYZ2xyY_Grey(XYZ_conv2grey,XYZ_Forgrey_xy);

bestx=XYZ_Forgrey_xy[0];
besty=XYZ_Forgrey_xy[1];



///////////////////////////////////////////////////////
/////////////SET RGB AVGS END///////////////////////


///////////////ACTUALLY DRAW PIXELS///////////////////////////////////////
      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {


                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue/255.0;     // B
       gOG=currGreen/255.0;   //G
         rOG=currRed/255.0;  // R
CIEx=bestx;
CIEy=besty;
///////////TO CHANGE WHITE POINT////////////////////////

double Customxy[2]={CIEx,CIEy};
double CustomXYZ[3];
double D65XYZ[3]={0.95047,1,1.08883};

xy2XYZ(Customxy,CustomXYZ);

double OGrgb[3]={rOG,gOG,bOG};
double rgbxyY[3];
rgb2xyY(OGrgb,rgbxyY);
double rgbXYZ[3];
xyY2XYZ(rgbxyY,rgbXYZ);
double WPConvXYZ[3];
WPconv(rgbXYZ,D65XYZ,CustomXYZ,WPConvXYZ);
double WPConvXYZ_xyY[3];
XYZ2xyY(WPConvXYZ,WPConvXYZ_xyY);

double WPchgRGB[3]; //FINAL OUTPUT
xyY2rgb(WPConvXYZ_xyY,WPchgRGB);

////////////////////////////////////////////////////

                srcp[x] = MAX(MIN(round(WPchgRGB[2]*255),255),0);
             srcp[x+1] =MAX(MIN(round(WPchgRGB[1]*255),255),0);
        srcp[x+2] = MAX(MIN(round(WPchgRGB[0]*255),255),0);


/*

                srcp[x] = MAX(MIN(round(bestSSD*255),255),0);
             srcp[x+1] =MAX(MIN(round(best_x*255),255),0);
        srcp[x+2] = MAX(MIN(round(best_y*255),255),0);
*/
             x=x+3;
      }
            srcp += src_pitch;
      } //END OF IMAGE DRAWING LOOP

/////////////////DRAW PIXELS END/////////////////////////////////
}else{
    for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue/255.0;     // B
       gOG=currGreen/255.0;   //G
         rOG=currRed/255.0;  // R

         mn=MIN(rOG,MIN(gOG,bOG));
 mx=MAX(rOG,MAX(gOG,bOG));

sat=(mx==0)?0:(mx-mn)/mx;




HWblack=1-mx;
//double HwhiteB=(1-sat)*mx;
grey_metric=1-(sat*HWblack);

if(grey_metric>strt){
                srcp[x] = MAX(MIN(round(0*255),255),0);
             srcp[x+1] =MAX(MIN(round(0*255),255),0);
        srcp[x+2] = MAX(MIN(round(0*255),255),0);
}

             x=x+3;
      }
             srcp += src_pitch;
}
}


/*
for (p = 0; p < stp; p++){
    free(avrg[p]);
}
free(avrg);
*/
 avs_release_frame(src);

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



           params->fails = (avs_as_int(avs_array_elt(args, 5)))?avs_as_int(avs_array_elt(args, 5)):1;

       params->debug = avs_defined(avs_array_elt(args, 4))?avs_as_bool(avs_array_elt(args, 4)):false;
        params->start = avs_defined(avs_array_elt(args, 2))?avs_as_float(avs_array_elt(args, 2)):1;
         params->step = avs_defined(avs_array_elt(args, 1))?avs_as_float(avs_array_elt(args, 1)):1;
                 params->sensitivity = avs_defined(avs_array_elt(args, 3))?avs_as_float(avs_array_elt(args, 3)):0.02;


   fi->user_data = (void*) params;
   fi->get_frame = WhitePoint_get_frame;


   v = avs_new_value_clip(new_clip);

   avs_release_clip(new_clip);
  free(params);
   return v;
}

const char* AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment* env)
{
   avs_add_function(env, "WhitePoint", "c[step]f[start]f[sensitivity]f[debug]b[fails]i", create_WhitePoint, 0);
   return "WhitePoint sample C plugin";
}
