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
   double patch;
} White_Point;

AVS_VideoFrame * AVSC_CC WhitePoint_get_frame (AVS_FilterInfo * p, int n)
{
  AVS_VideoFrame * src;
   White_Point* params = (White_Point*) p->user_data;

  src = avs_get_frame(p->child, n);

   int row_size, height, src_pitch,x, y,dbg;
   BYTE* srcp;
  char* nm;

double D65XYZ[3]={0.95047,1,1.08883};
  avs_make_writable(p->env, &src);

      srcp = avs_get_write_ptr(src);
      src_pitch = avs_get_pitch(src);
      row_size = avs_get_row_size(src);
      height = avs_get_height(src);

double  bOG,gOG,rOG,thrsh,ptch;

//strt=params->start;
thrsh=params->thresh;
dbg=params->debug;
dbg=round(dbg);
nm=params->file;
ptch=params->patch;
ptch=MAX(0,MIN(1,ptch));

double sc_max=0;
double rf=1;
double gf=1;
double bf=1;
//POLL FRAME/////////////////////////////////////////////////////////
      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcptwoFiveFive;     // B
       gOG=currGreen*rcptwoFiveFive;   //G
         rOG=currRed*rcptwoFiveFive;     // R

if ((round(currRed)==0)&&(round(currGreen)==0)&&(round(currBlue)==0)){
  x+=3;
}else{

 double mn=MIN(rOG,MIN(gOG,bOG));
 double mx=MAX(rOG,MAX(gOG,bOG));
 double sc=mn*(1-((mx-mn)/mx));
 if(sc>sc_max){
 sc_max=sc;
 rf=rOG;
 gf=gOG;
 bf=bOG;
 }
  x+=3;
}

      }

      }

double rgbForGrey[3]={rf,gf,bf};
double XYZ_orig[3];
double XYZ_grey[3];
double XYZ_conv2grey[3];
double XYZ_Forgrey_xy[3];
double WPConvXYZ[3];

LinRGB2Grey_XYZ(rgbForGrey,XYZ_orig,XYZ_grey);
WPconv2Grey(XYZ_orig,XYZ_grey,XYZ_conv2grey);
XYZ2xyY(XYZ_conv2grey,XYZ_Forgrey_xy);

if (nm!=""){
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

}
      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {


                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcptwoFiveFive;     // B
       gOG=currGreen*rcptwoFiveFive;   //G
         rOG=currRed*rcptwoFiveFive;     // R


double WPchgRGB[3];

if ((round(currRed)==0)&&(round(currGreen)==0)&&(round(currBlue)==0)){
    WPchgRGB[0]=0;
    WPchgRGB[1]=0;
    WPchgRGB[2]=0;
}else{
double curr_rgb_dst_4XYZ[3]={rOG,gOG,bOG};

    double curr_rgb_dst_fnl_XYZ[3];
    rgb2XYZ(curr_rgb_dst_4XYZ,curr_rgb_dst_fnl_XYZ);
WPconv(curr_rgb_dst_fnl_XYZ,D65XYZ,XYZ_conv2grey,WPConvXYZ);

XYZ2rgb(WPConvXYZ,WPchgRGB);

if(dbg==1){
    double mx=MAX(WPchgRGB[0],MAX(WPchgRGB[1],WPchgRGB[2]));
    double dbg_sat=(mx==0)?0:(mx-MIN(WPchgRGB[0],MIN(WPchgRGB[1],WPchgRGB[2])))/mx;

    if( ((double)(x)<=(double)(row_size)*ptch) && (((double)(y)>=(double)(height)*(1-ptch)))  ){
    WPchgRGB[0]=rf;
    WPchgRGB[1]=gf;
    WPchgRGB[2]=bf;
}else{
if (dbg_sat<=thrsh){
    WPchgRGB[0]=0;
    WPchgRGB[1]=0;
    WPchgRGB[2]=0;
}
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



/////////////////DRAW PIXELS END/////////////////////////////////


  return src;
}

AVS_Value AVSC_CC create_WhitePoint (AVS_ScriptEnvironment * env,AVS_Value args, void * dg)
{
  AVS_Value v;
  AVS_FilterInfo * fi;
  AVS_Clip * new_clip = avs_new_c_filter(env, &fi, avs_array_elt(args, 0), 1);
  White_Point *params = (White_Point*)malloc(sizeof(White_Point));

if (!params)
      return avs_void;



  char* file_name ="";

       params->debug = (avs_as_bool(avs_array_elt(args, 1)))?avs_as_bool(avs_array_elt(args, 1)):false;
       params->thresh = (avs_as_float(avs_array_elt(args, 2)))?avs_as_float(avs_array_elt(args, 2)):0.015;
      file_name = (avs_as_string(avs_array_elt(args, 3)))?avs_as_string(avs_array_elt(args, 3)):file_name;
       params->file = file_name;
              params->patch = (avs_as_float(avs_array_elt(args, 4)))?avs_as_float(avs_array_elt(args, 4)):0.08;
    fi->user_data = (void*) params;
if(file_name!=""){
      FILE *fptr;
   fptr = fopen(file_name,"w");
   if(fptr == NULL)
   {
      printf("Error!");
      exit(1);
   }

   fprintf(fptr,"%s\n",file_name);
   fclose(fptr);
   }

  if (!avs_is_rgb32(&fi->vi)) {
    return avs_new_value_error ("Input video must be in RGB format!");
  } else {
    fi->get_frame = WhitePoint_get_frame;
    v = avs_new_value_clip(new_clip);
  }
  avs_release_clip(new_clip);
  return v;
}


const char * AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment * env)
{
   avs_add_function(env, "AWP_For_Manual", "c[debug]b[thresh]f[file]s[patch]f", create_WhitePoint, 0);
   return "WhitePoint sample C plugin";
}
