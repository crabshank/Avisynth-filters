#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "avisynth_c.h"
#include "functions_c.h"

typedef struct WhitePoint {
      int iterations;
      double start;
      int debug;
} WhitePoint;

AVS_VideoFrame* AVSC_CC WhitePoint_get_frame(AVS_FilterInfo* fi, int n)
{
 WhitePoint* params = (WhitePoint*) fi->user_data;

   AVS_VideoFrame* src = avs_get_frame(fi->child, n);



   int row_size, height, src_pitch,x, y, p,dbg,itr,bestEl;
   BYTE* srcp;
   double CIEx,CIEy,rOG,bOG,gOG,strt,HWblack,grey_metric,mn,mx,sat,cumulSSD,dblItr,dblP,bestSSD;

itr =   params->iterations;
strt=params->start;
dbg=params->debug;


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


      double sum_x[itr];
double sum_y[itr];
double count_ssd[itr];
double last_ssd_r[itr];
double last_ssd_g[itr];
double last_ssd_b[itr];
double min_x[itr];
double min_y[itr];
double max_x[itr];
double max_y[itr];
double last_sum_r[itr];
double last_sum_g[itr];
double last_sum_b[itr];
double curr_thr[itr];


for (p=0; p<itr; p++){

sum_x[p]=0;
sum_y[p]=0;
count_ssd[p]=0;
last_ssd_r[p]=0;
last_ssd_g[p]=0;
last_ssd_b[p]=0;
last_sum_r[p]=0;
last_sum_g[p]=0;
last_sum_b[p]=0;
dblP=(double)p;
dblItr=(double)itr;
curr_thr[p]=strt-(strt/dblItr)*(dblP+1);
min_x[p]=CIEx;
max_x[p]=CIEx;
min_y[p]=CIEy;
max_y[p]=CIEy;
}




double best_x=CIEx;
double best_y=CIEy;

if (dbg!=1){



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

double rgbxyY[]={rOG,gOG,bOG};
double xyY[3];
rgb2xyY(rgbxyY,xyY);

double dist_r=sqrt(pow(0.64-xyY[0],2)+pow(0.33-xyY[1],2));
double dist_g=sqrt(pow(0.3-xyY[0],2)+pow(0.6-xyY[1],2));
double dist_b=sqrt(pow(0.15-xyY[0],2)+pow(0.06-xyY[1],2));


for(p=0; p<itr;p++){

 if(grey_metric>=curr_thr[p]&&(grey_metric<=strt) && (grey_metric<1)){

if(x==0 && y==0){

max_y[p]=xyY[1];

min_x[p]=xyY[0];

min_y[p]=xyY[1];

max_x[p]=xyY[0];

}else{

    if (xyY[1]>max_y[p]){
        max_y[p]=xyY[1];
    }

    if (xyY[0]<min_x[p]){
        min_x[p]=xyY[0];
    }

    if (xyY[1]<min_y[p]){
        min_y[p]=xyY[1];
    }

    if(xyY[0]>max_x[p]){
        max_x[p]=xyY[0];
    }

 }

if(count_ssd[p]<1){ //If first eligible pixel, add current dist to sum
last_sum_r[p]=dist_r;
last_sum_g[p]=dist_g;
last_sum_b[p]=dist_b;

}else{
last_ssd_r[p]+=(dist_r-((last_sum_r[p]+dist_r)/(count_ssd[p]+1)))*(dist_r-(last_sum_r[p]/count_ssd[p]));
last_sum_r[p]+=dist_r;

last_ssd_g[p]+=(dist_g-((last_sum_g[p]+dist_g)/(count_ssd[p]+1)))*(dist_g-(last_sum_g[p]/count_ssd[p]));
last_sum_g[p]+=dist_g;

last_ssd_b[p]+=(dist_b-((last_sum_b[p]+dist_b)/(count_ssd[p]+1)))*(dist_b-(last_sum_b[p]/count_ssd[p]));
last_sum_b[p]+=dist_b;


}
sum_x[p]+=xyY[0];
sum_y[p]+=xyY[1];
        count_ssd[p]++;

   }



      }




   x=x+3;
      }

      }

/////////////POLL FRAME END///////////////////////

for(p=0; p<itr;p++){ //Bessel correct standard deviations
    last_ssd_r[p]*=(count_ssd[p])/(count_ssd[p]-1);
    last_ssd_g[p]*=(count_ssd[p])/(count_ssd[p]-1);
    last_ssd_b[p]*=(count_ssd[p])/(count_ssd[p]-1);
    }

bestSSD=last_ssd_r[itr-1]+last_ssd_g[itr-1]+last_ssd_b[itr-1];
bestEl=-1;
for(p=0; p<itr;p++){ //Search for sample withe lowest standard deviation and at least 2 points
  cumulSSD= last_ssd_r[p]+last_ssd_g[p]+last_ssd_b[p];
           if(cumulSSD<bestSSD && (count_ssd[p]>=2)){
           bestSSD=cumulSSD;
           bestEl=p;
		   }
    }



best_x=(bestEl==-1)?CIEx:0.5*(max_x[bestEl]+min_x[bestEl]);//sum_x[bestEl]/count_ssd[bestEl];
best_y=(bestEl==-1)?CIEy:0.5*(max_y[bestEl]+min_y[bestEl]);//sum_x[bestEl]/count_ssd[bestEl];

//best_x=CIEx;
//best_y=CIEy;

if (best_x !=CIEx || best_y !=CIEy) {

double XYZ_orig[3];
double XYZ_conv2grey[3];
double XYZ_Forgrey_xy[3];


double xyForGrey[]={best_x,best_y};
double XYZ_grey[]={0.95047,1,1.08883};
xy2XYZ(xyForGrey,XYZ_orig);
WPconv2Grey(XYZ_orig,XYZ_grey,XYZ_conv2grey);
XYZ2xyY_Grey(XYZ_conv2grey,XYZ_Forgrey_xy);

double Customxy[2]={XYZ_Forgrey_xy[0],XYZ_Forgrey_xy[1]};
double CustomXYZ[3];
double D65XYZ[3]={0.95047,1,1.08883};
xy2XYZ(Customxy,CustomXYZ);

///////////////ACTUALLY DRAW PIXELS///////////////////////////////////////
      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {


                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue/255.0;     // B
       gOG=currGreen/255.0;   //G
         rOG=currRed/255.0;  // R

///////////TO CHANGE WHITE POINT////////////////////////


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



x+=3;
      }
            srcp += src_pitch;
      } //END OF IMAGE DRAWING LOOP

/////////////////DRAW PIXELS END/////////////////////////////////
}

/*else{
          for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {


                srcp[x] = srcp[x];
             srcp[x+1] = srcp[x+1];
        srcp[x+2] = srcp[x+2];



x+=3;
      }
            srcp += src_pitch;
      }

}
*/

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




       params->debug = avs_defined(avs_array_elt(args, 3))?avs_as_bool(avs_array_elt(args, 3)):false;
        params->start = avs_defined(avs_array_elt(args, 2))?avs_as_float(avs_array_elt(args, 2)):1;
         params->iterations = avs_defined(avs_array_elt(args, 1))?avs_as_int(avs_array_elt(args, 1)):50;



   fi->user_data = (void*) params;
   fi->get_frame = WhitePoint_get_frame;


   v = avs_new_value_clip(new_clip);

   avs_release_clip(new_clip);
  // free(params);
   return v;
}

const char* AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment* env)
{
   avs_add_function(env, "WhitePoint", "c[iterations]i[start]f[debug]b", create_WhitePoint, 0);
   return "WhitePoint sample C plugin";
}
