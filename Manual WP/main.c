#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include "../avisynth_c.h"
#include "functions_c.h"

typedef struct Manual_WP {
        double x;
        double y;
        int R;
        int G;
        int B;
        int mode;
        int debug;
        double debug_val;
        int sixtyFour;
        double dst_x;
        double dst_y;
        int auto_WP;
        char* file;
        char* log_id;
} Manual_WP;


AVS_VideoFrame * AVSC_CC Manual_WP_get_frame (AVS_FilterInfo * p, int n)
{
  AVS_VideoFrame * src;
   Manual_WP* params = (Manual_WP*) p->user_data;

  src = avs_get_frame(p->child, n);

   int row_size, height, src_pitch,x, y,dbg,mde,sxf,ato;
   BYTE* srcp;
     char* nm;
     char* lid;
   double rOG,bOG,gOG,cust_x,cust_y,amp,D65_x,D65_y, to_x, to_y;


  avs_make_writable(p->env, &src);

      srcp = avs_get_write_ptr(src);
      src_pitch = avs_get_pitch(src);
      row_size = avs_get_row_size(src);
      height = avs_get_height(src);
cust_x=params->x;
cust_y=params->y;
mde=params->mode;
dbg=params->debug;
amp=params->debug_val;
sxf=params->sixtyFour;
to_x=params->dst_x;
to_y=params->dst_y;
ato=params->auto_WP;
nm=params->file;
lid=params->log_id;

D65_x= 0.312727;
   D65_y= 0.329023;

if(ato==1){
    double sc_max=0;
    double rf=1;
    double gf=1;
    double bf=1;
    int rfi=255;
    int gfi=255;
    int bfi=255;
//POLL FRAME/////////////////////////////////////////////////////////
      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

                 double currBlue=(sxf==1)?(double)srcp[x]+srcp[x+1]*256:(double)srcp[x];
                 double currGreen=(sxf==1)?(double)srcp[x+2]+srcp[x+3]*256:(double)srcp[x+1];
                 double currRed=(sxf==1)?(double)srcp[x+4]+srcp[x+5]*256:(double)srcp[x+2];


    bOG=(sxf==1)?currBlue*rcptHiBit:currBlue*rcptwoFiveFive;     // B
       gOG=(sxf==1)?currGreen*rcptHiBit:currGreen*rcptwoFiveFive;   //G
         rOG=(sxf==1)?currRed*rcptHiBit:currRed*rcptwoFiveFive;     // R



 double mn=MIN(rOG,MIN(gOG,bOG));
 double mx=MAX(rOG,MAX(gOG,bOG));
  double sat=(mx==0)?0:(mx-mn)/mx;

  double mn_diff=MIN(fabs(currRed-currGreen),MIN(fabs(currRed-currBlue),fabs(currGreen-currBlue)));
   double sc=mn*(1-mn_diff);
 if(sc>sc_max){
 sc_max=sc;

 rf=rOG;
 gf=gOG;
 bf=bOG;

 rfi=currBlue;
 gfi=currGreen;
 bfi=currRed;
 }



x=(sxf==1)?x+7:x+3;


      }

      }

      double rgb_ato[3]={rf,gf,bf};
      double xyY_ato[3];
       get_xy(rgb_ato, xyY_ato , params->mode);

        params->x=xyY_ato[0];
        params->y=xyY_ato[1];
        cust_x=xyY_ato[0];
        cust_y=xyY_ato[1];

if ((nm!="")&&(nm!="NULL")){
   //int num;
   FILE *fptr;
   // use appropriate location if you are using MacOS or Linux
   fptr = fopen(nm,"a");
   if(fptr == NULL)
   {
      printf("Error!");
      exit(1);
   }

if ((lid!="")&&(lid!="NULL")){
    fprintf(fptr,"%s: %i, %i, %i - %i\n", lid,rfi, gfi, bfi,n );
}else{
    fprintf(fptr,"%i, %i, %i - %i\n", rfi, gfi, bfi,n );
}

   fclose(fptr);

}
}


      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

//double x_shift=(double)x/(double)row_size;

                 double currBlue=(sxf==1)?(double)srcp[x]+srcp[x+1]*256:(double)srcp[x];
                 double currGreen=(sxf==1)?(double)srcp[x+2]+srcp[x+3]*256:(double)srcp[x+1];
                 double currRed=(sxf==1)?(double)srcp[x+4]+srcp[x+5]*256:(double)srcp[x+2];


    bOG=(sxf==1)?currBlue*rcptHiBit:currBlue*rcptwoFiveFive;     // B
       gOG=(sxf==1)?currGreen*rcptHiBit:currGreen*rcptwoFiveFive;   //G
         rOG=(sxf==1)?currRed*rcptHiBit:currRed*rcptwoFiveFive;     // R

double rgbXYZ[3];
double rgbXYZGrey[3];
double WPConvXYZ[3];
double OG_RGB[3]={rOG,gOG,bOG};
double WPchgRGB[3]={rOG,gOG,bOG};

rgb2XYZ(OG_RGB,rgbXYZ,rgbXYZGrey,mde,0);

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

    if (to_x!=D65_x || (to_y!=D65_y)){
        double dst_xy[2]={to_x,to_y};
        double dst_XYZ[3];
        xy2XYZ(dst_xy,dst_XYZ);
        double WPConvXYZ2[3]={WPConvXYZ[0],WPConvXYZ[1],WPConvXYZ[2]};
        WPconv(WPConvXYZ2,cust_XYZ,dst_XYZ,WPConvXYZ);
    }
        XYZ2rgb(WPConvXYZ,WPchgRGB,mde);

}else{

    if (to_x!=D65_x || (to_y!=D65_y)){
        double dst_xy[2]={to_x,to_y};
        double dst_XYZ[3];
        xy2XYZ(dst_xy,dst_XYZ);
        WPconv(rgbXYZ,D65XYZ,dst_XYZ,WPConvXYZ);
        XYZ2rgb(WPConvXYZ,WPchgRGB,mde);
    }


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

int wp_b=MAX(MIN(round(WPchgRGB[2]*255),255),0);
int wp_g=MAX(MIN(round(WPchgRGB[1]*255),255),0);
int wp_r=MAX(MIN(round(WPchgRGB[0]*255),255),0);

srcp[x] =wp_b; //blue : blue
srcp[x+1] =(sxf==1)?wp_b:wp_g; // blue : green
srcp[x+2] =(sxf==1)? wp_g:wp_r; // green: red
srcp[x+3] =(sxf==1)? wp_g:srcp[x+3]; //green : self
srcp[x+4] =(sxf==1)? wp_r:srcp[x+4]; //red : self
srcp[x+5] =(sxf==1)? wp_r:srcp[x+5]; //red : self


x=(sxf==1)?x+7:x+3;

      }
            srcp += src_pitch;
      }

  return src;
}

AVS_Value AVSC_CC create_Manual_WP (AVS_ScriptEnvironment * env,AVS_Value args, void * dg)
{
  AVS_Value v;
  AVS_FilterInfo * fi;
  AVS_Clip * new_clip = avs_new_c_filter(env, &fi, avs_array_elt(args, 0), 1);
  Manual_WP *params = (Manual_WP*)malloc(sizeof(Manual_WP));

if (!params){
      return avs_void;
}

                   params->x = avs_defined(avs_array_elt(args, 1))?avs_as_float(avs_array_elt(args, 1)):0.312727;
                   params->y = avs_defined(avs_array_elt(args, 2))?avs_as_float(avs_array_elt(args, 2)):0.329023;
                   params->R = avs_defined(avs_array_elt(args, 3))?avs_as_int(avs_array_elt(args, 3)):-1;
                   params->G = avs_defined(avs_array_elt(args, 4))?avs_as_int(avs_array_elt(args, 4)):-1;
                   params->B = avs_defined(avs_array_elt(args, 5))?avs_as_int(avs_array_elt(args, 5)):-1;
                params->mode = avs_defined(avs_array_elt(args, 6))?avs_as_int(avs_array_elt(args, 6)):0;
               params->debug = avs_defined(avs_array_elt(args, 7))?avs_as_int(avs_array_elt(args, 7)):0;
            params->debug_val=  avs_defined(avs_array_elt(args,8))?avs_as_float(avs_array_elt(args, 8)):1;
                params->dst_x=  avs_defined(avs_array_elt(args,10))?avs_as_float(avs_array_elt(args, 10)):0.312727;
                params->dst_y=  avs_defined(avs_array_elt(args,11))?avs_as_float(avs_array_elt(args, 11)):0.329023;
                params->auto_WP=  avs_defined(avs_array_elt(args,12))?avs_as_bool(avs_array_elt(args, 12)):false;

char* file_name ="";
file_name = ((avs_as_string(avs_array_elt(args, 13)))&&(avs_as_string(avs_array_elt(args, 13))!="NULL"))?avs_as_string(avs_array_elt(args, 13)):file_name;
params->file = file_name;

char* log_idt ="";
log_idt = ((avs_as_string(avs_array_elt(args, 14)))&&(avs_as_string(avs_array_elt(args, 14))!="NULL"))?avs_as_string(avs_array_elt(args, 14)):log_idt;
params->log_id = log_idt;

          if ((params->mode<0)||(params->mode>10)){
            return avs_new_value_error ("Allowed modes are between 0 and 10!");
          }else{
  if (!((avs_is_rgb32(&fi->vi))||(avs_is_rgb64(&fi->vi)))) {
    return avs_new_value_error ("Input video must be in RGB32 OR RGB64 format!");
  }else {

    if((params->auto_WP==true)&&((file_name!="")&&(file_name!="NULL"))){
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

     if(avs_defined(avs_array_elt(args, 9))){
        params->sixtyFour =avs_as_bool(avs_array_elt(args, 9));
     }else{
       params->sixtyFour = (avs_is_rgb64(&fi->vi))?true:false;
     }

 if((params->R>=0)&&(params->G>=0)&&((params->B>=0))){
      if((params->sixtyFour==true)&&((params->R>65535)||(params->G>65535)||(params->B>65535))){
        return avs_new_value_error ("RGB arguments must be between 0 and 65535");
      }else if((params->sixtyFour==false)&&((params->R>255)||(params->G>255)||(params->B>255))){
        return avs_new_value_error ("RGB arguments must be between 0 and 255");
      }else if (params->auto_WP==false){

        double xyY_rgb[3];

        double rgb[3];

        rgb[0]=(params->sixtyFour==true)?(double)(params->R)*rcptHiBit:(double)(params->R)*rcptwoFiveFive;
        rgb[1]=(params->sixtyFour==true)?(double)(params->G)*rcptHiBit:(double)(params->G)*rcptwoFiveFive;
        rgb[2]=(params->sixtyFour==true)?(double)(params->B)*rcptHiBit:(double)(params->B)*rcptwoFiveFive;

        get_xy(rgb, xyY_rgb , params->mode);

        params->x=xyY_rgb[0];
        params->y=xyY_rgb[1];

      }

  }

         fi->user_data = (void*) params;
    fi->get_frame = Manual_WP_get_frame;
    v = avs_new_value_clip(new_clip);
   }
   }
  avs_release_clip(new_clip);
  return v;

}


const char * AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment * env)
{
   avs_add_function(env, "Manual_WP", "c[x]f[y]f[R]i[G]i[B]i[mode]i[debug]i[debug_val]f[sixtyFour]b[dst_x]f[dst_y]f[auto_WP]b[file]s[log_id]s", create_Manual_WP, 0);
   return "Manual_WP C plugin";
}
