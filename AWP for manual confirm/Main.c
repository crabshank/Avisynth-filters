#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "../avisynth_c.h"
#include "functions_c.h"

typedef struct White_Point {
   int debug;
   double thresh;
   char* file;
} White_Point;

AVS_VideoFrame* AVSC_CC WhitePoint_get_frame(AVS_FilterInfo* fi, int n)
{
 White_Point* params = (White_Point*) fi->user_data;

   AVS_VideoFrame* src = avs_get_frame(fi->child, n);


   int row_size, height, src_pitch,x, y,avgCountAll,dbg;
   BYTE* srcp;
  char* nm;

double D65XYZ[3]={0.95047,1,1.08883};



      int planes[] ={AVS_CS_BGR32};
src = avs_get_frame(fi->child, n);
   avs_make_writable(fi->env, &src);

      srcp = avs_get_write_ptr_p(src, planes[0]);
      src_pitch = avs_get_pitch_p(src, planes[0]);
      row_size = avs_get_row_size_p(src, planes[0]);
      height = avs_get_height_p(src, planes[0]);
double  bOG,gOG,rOG,sumR_,sumG_,sumB_,thrsh;

//strt=params->start;
thrsh=params->thresh;
dbg=params->debug;
nm=params->file;




avgCountAll=0;
sumR_=0;
sumG_=0;
sumB_=0;
//POLL FRAME/////////////////////////////////////////////////////////
      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcptwoFiveFive;     // B
       gOG=currGreen*rcptwoFiveFive;   //G
         rOG=currRed*rcptwoFiveFive;     // R
if ((rOG==0)&&(gOG==0)&&(bOG==0)){
        avgCountAll+=1;
}else{

double curr_rgb_dst[3]={rOG,gOG,bOG};
double curr_rgb_dst_lin[3];


double curr_rgb_dst_lin_xyY[3];
sRGB2Linear(curr_rgb_dst,curr_rgb_dst_lin);
LinRGB2xyY(curr_rgb_dst_lin,curr_rgb_dst_lin_xyY);
double curr_rgb_dst_lin_prp[3];

RGB2rgb(curr_rgb_dst_lin,curr_rgb_dst_lin_prp);
double mx_prp=MAX(curr_rgb_dst_lin_prp[0],MAX(curr_rgb_dst_lin_prp[1],curr_rgb_dst_lin_prp[2]));
double Sc=(mx_prp==0)?0:1-(mx_prp-MIN(curr_rgb_dst_lin_prp[0],MIN(curr_rgb_dst_lin_prp[1],curr_rgb_dst_lin_prp[2])))/mx_prp;

  Sc*=Sc*curr_rgb_dst_lin_xyY[2];


sumR_+=curr_rgb_dst_lin[0]*Sc;
sumG_+= curr_rgb_dst_lin[1]*Sc;
sumB_+=curr_rgb_dst_lin[2]*Sc;
        avgCountAll+=1;

      }

         x+=3;
      }

      }
if((avgCountAll>0) && (sumR_+sumG_+sumB_!=0) ){

double rcp_avgCountAll=1.0/avgCountAll;
double rgbForGrey[3]={sumR_*rcp_avgCountAll,sumG_*rcp_avgCountAll,sumB_*rcp_avgCountAll};
double XYZ_orig[3];
double XYZ_grey[3];
double XYZ_conv2grey[3];
double XYZ_Forgrey_xy[3];
double WPConvXYZ[3];

LinRGB2Grey_XYZ(rgbForGrey,XYZ_orig,XYZ_grey);
WPconv2Grey(XYZ_orig,XYZ_grey,XYZ_conv2grey);
XYZ2xyY(XYZ_conv2grey,XYZ_Forgrey_xy);

   //int num;
   FILE *fptr;
   // use appropriate location if you are using MacOS or Linux
   fptr = fopen(nm,"a");
   if(fptr == NULL)
   {
      printf("Error!");
      exit(1);
   }

   fprintf(fptr,"%f %f %d\n",XYZ_Forgrey_xy[0],XYZ_Forgrey_xy[1],n );
   fclose(fptr);


      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {


bOG=(double)srcp[x]*pow(255,-1);     // B
        gOG=(double)srcp[x+1]*pow(255,-1);  //G
         rOG=(double)srcp[x+2]*pow(255,-1); // R

double WPchgRGB[3];

if ((rOG==0)&&(gOG==0)&&(bOG==0)){
    WPchgRGB[0]=0;
    WPchgRGB[1]=0;
    WPchgRGB[2]=0;
}else{
double curr_rgb_dst_4xyY[3]={rOG,gOG,bOG};
    double curr_rgb_dst_fnl_xyY[3];
    double curr_rgb_dst_fnl_XYZ[3];
    rgb2xyY(curr_rgb_dst_4xyY,curr_rgb_dst_fnl_xyY);
    xyY2XYZ(curr_rgb_dst_fnl_xyY,curr_rgb_dst_fnl_XYZ);
WPconv(curr_rgb_dst_fnl_XYZ,D65XYZ,XYZ_conv2grey,WPConvXYZ);
double WPConvXYZ_xyY[3];
XYZ2xyY(WPConvXYZ,WPConvXYZ_xyY);
//FINAL OUTPUT
xyY2rgb(WPConvXYZ_xyY,WPchgRGB);


if(dbg==1){
    double mx=MAX(WPchgRGB[0],MAX(WPchgRGB[1],WPchgRGB[2]));
    double dbg_sat=(mx==0)?0:(mx-MIN(WPchgRGB[0],MIN(WPchgRGB[1],WPchgRGB[2])))/mx;
if (dbg_sat>thrsh){
    WPchgRGB[0]=0;
    WPchgRGB[1]=0;
    WPchgRGB[2]=0;
}

}

                srcp[x] = MAX(MIN(round(WPchgRGB[2]*255),255),0);
             srcp[x+1] =MAX(MIN(round(WPchgRGB[1]*255),255),0);
        srcp[x+2] = MAX(MIN(round(WPchgRGB[0]*255),255),0);
}
        x+=3;
}
 srcp += src_pitch;
}
}else{



      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {
if(dbg==1){
            srcp[x] = 0;
            srcp[x+1] = 0;
            srcp[x+2] = 0;
}
         x+=3;
      }
        srcp += src_pitch;
}

}


/////////////////DRAW PIXELS END/////////////////////////////////



   return src;
}



AVS_Value AVSC_CC create_WhitePoint(AVS_ScriptEnvironment* env, AVS_Value args, void* user_data)
{
   AVS_Value v;
   AVS_FilterInfo* fi;


   AVS_Clip* new_clip = avs_new_c_filter(env, &fi, avs_array_elt(args, 0), 1);

White_Point *params = (White_Point*)malloc(sizeof(White_Point));

if (!params)
      return avs_void;



   if (!avs_is_rgb(&fi->vi)) {
      return avs_new_value_error("Input video must be in RGB format!");
   }


  char* file_name ="";

       params->debug = (avs_as_bool(avs_array_elt(args, 1)))?avs_as_bool(avs_array_elt(args, 1)):true;
       params->thresh = (avs_as_float(avs_array_elt(args, 2)))?avs_as_float(avs_array_elt(args, 2)):0.03;
      file_name = (avs_as_string(avs_array_elt(args, 3)))?avs_as_string(avs_array_elt(args, 3)):"C:\\xy.txt";
       params->file = file_name;

   fi->user_data = (void*) params;



      FILE *fptr;
   fptr = fopen(file_name,"w");
   if(fptr == NULL)
   {
      printf("Error!");
      exit(1);
   }

   fprintf(fptr,"%s\n",file_name);
   fclose(fptr);


   fi->get_frame = WhitePoint_get_frame;


   v = avs_new_value_clip(new_clip);

   avs_release_clip(new_clip);
   free(params);
   return v;
}

const char* AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment* env)
{
   avs_add_function(env, "AWP_For_Manual", "c[debug]b[thresh]f[file]s", create_WhitePoint, 0);
   return "WhitePoint sample C plugin";
}
