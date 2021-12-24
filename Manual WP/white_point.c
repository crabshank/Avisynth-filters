#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
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
        int overwrite;
        int linear;
        char* edits;
        int ed_off;
        int ed_base;
        int* ed_Red;
        int* ed_Green;
        int* ed_Blue;
        double* ed_x;
        double* ed_y;
        int* ed_start_fr;
        int* ed_end_fr;
        int* ed_switch;
        int* ed_lookup;
        int ed_lim;
		int* ed_Red2;
        int* ed_Green2;
        int* ed_Blue2;
        double* ed_x2;
        double* ed_y2;
        int* ed_start_fr2;
        int* ed_end_fr2;
        int* ed_switch2;
        int* ed_lookup2;
        int ed_lim2;
		char* edits2;
		int approxPow;
        char** split;
        char** split2;
        double* hueCount;
        double* hueCount_sat;
        double* hueCount_val;
        double* hueCount_prp;
        double* hueCount_wht;
        double* hueCount_wht_prp;
} Manual_WP;


AVS_VideoFrame * AVSC_CC Manual_WP_get_frame (AVS_FilterInfo * p, int n)
{
  AVS_VideoFrame * src;
   Manual_WP* params = (Manual_WP*) p->user_data;

  src = avs_get_frame(p->child, n);

   int row_size, height, src_pitch,x, y,dbg,mde,sxf,ato,lnr,edlm,edlm2,ed_offst,fv_swt,ed_bse,aprxPw;
   BYTE* srcp;
   const BYTE* rrcp;
     char* nm;
     char* lid;
     char* eds;
     char* eds2;
   double rOG,bOG,gOG,cust_x,cust_y,amp,D65_x,D65_y, to_x, to_y,sat_dbg_six,r_dbg_six,g_dbg_six,b_dbg_six;


  avs_make_writable(p->env, &src);

      srcp = avs_get_write_ptr(src);
      rrcp = avs_get_read_ptr(src);
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
lnr=params->linear;
eds=params->edits;
eds2=params->edits2;
edlm=params->ed_lim;
edlm2=params->ed_lim2;
ed_offst=params->ed_off;
ed_bse=params->ed_base;
aprxPw=params->approxPow;

sat_dbg_six=0.0;
r_dbg_six=1.0;
g_dbg_six=1.0;
b_dbg_six=1.0;

D65_x= 0.312727;
   D65_y= 0.329023;

if((ato==1)&&((eds=="")||(eds=="NULL"))&&((eds2=="")||(eds2=="NULL"))){

for (int i=360; i>=0; i--){
    params->hueCount[i]=0;
    params->hueCount_wht[i]=0;
    params->hueCount_prp[i]=0;
    params->hueCount_wht_prp[i]=0;
    params->hueCount_sat[i]=0;
    params->hueCount_val[i]=0;
}

double* hueCount=params->hueCount;
double* hueCount_sat=params->hueCount_sat;
double* hueCount_val=params->hueCount_val;
double* hueCount_prp=params->hueCount_prp;
double* hueCount_wht=params->hueCount_wht;
double* hueCount_wht_prp=params->hueCount_wht_prp;

//POLL FRAME/////////////////////////////////////////////////////////
      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

            int intBlue=(sxf==1)?rrcp[x]+rrcp[x+1]*256:rrcp[x];
            int intGreen=(sxf==1)?rrcp[x+2]+rrcp[x+3]*256:rrcp[x+1];
            int intRed=(sxf==1)?rrcp[x+4]+rrcp[x+5]*256:rrcp[x+2];

                 double currBlue=(double)(intBlue);
                 double currGreen=(double)(intGreen);
                 double currRed=(double)(intRed);



    bOG=(sxf==1)?currBlue*rcptHiBit:currBlue*rcptwoFiveFive;     // B
       gOG=(sxf==1)?currGreen*rcptHiBit:currGreen*rcptwoFiveFive;   //G
         rOG=(sxf==1)?currRed*rcptHiBit:currRed*rcptwoFiveFive;     // R

        double curr_rgb_dst_hsv[3];
        double curr_rgb_dst[3]={rOG,gOG,bOG};
        double curr_rgb_prp[3];
rgb2hsv_360(curr_rgb_dst,curr_rgb_dst_hsv);
int hueEl=round(curr_rgb_dst_hsv[0]);

double score=(1-curr_rgb_dst_hsv[1])*curr_rgb_dst_hsv[2]*curr_rgb_dst_hsv[2]; //higher, more white

if((intBlue==intGreen)&&(intGreen==intRed)){ //grey
    hueEl=360;
score*=score;
//hueCount_sat[hueEl]+=0;

}else{
    hueEl=(hueEl==360)?0:hueEl;

RGB_prp(curr_rgb_dst,curr_rgb_prp);

score=(1-MAX(curr_rgb_dst_hsv[1],curr_rgb_dst_hsv[1]*curr_rgb_dst_hsv[2]))*(1-(MAX(curr_rgb_prp[0],MAX(curr_rgb_prp[1],curr_rgb_prp[2]))-MIN(curr_rgb_prp[0],MIN(curr_rgb_prp[1],curr_rgb_prp[2]))))*(1-curr_rgb_dst_hsv[1])*(score); //higher, more white
score*=score;

hueCount_sat[hueEl]+=curr_rgb_dst_hsv[1];
}

hueCount_val[hueEl]+=score;
hueCount_wht[hueEl]+=score;
hueCount[hueEl]++;

x=(sxf==1)?x+7:x+3;


      }
      rrcp+=src_pitch;
    }

	rrcp=avs_get_read_ptr(src);

	double hueSum=0;
	double scoreSum=0;

for (int i=0; i<361; i++){
    double hueCnt_dbl=(double)(hueCount[i]);
    hueCount_sat[i]=(hueCount[i]==0 || i==360)?0:hueCount_sat[i]/hueCnt_dbl;
    hueCount_val[i]=(hueCount[i]==0)?0:hueCount_val[i]/hueCnt_dbl;
}
for (int i=0; i<361; i++){
    scoreSum+=hueCount_wht[i];
}
for (int i=0; i<361; i++){
    hueCount_wht_prp[i]=hueCount_wht[i]/scoreSum;
}
for (int i=0; i<361; i++){
    hueSum+=hueCount[i];
}
for (int i=0; i<361; i++){
    hueCount_prp[i]=hueCount[i]/hueSum;
}

      double maxHueScore=0.0;
      int maxHueScoreDeg=0;

for (int i=0; i<361; i++){
            double currScore=(1-hueCount_prp[i])*hueCount_wht_prp[i];
      if(currScore>maxHueScore){
    maxHueScore=currScore;
    maxHueScoreDeg=i;
      }
}

double maxHueScoreDeg_dbl=(double)(maxHueScoreDeg);
double greys_val=(double)(hueCount_val[maxHueScoreDeg]);
double rgb_ato[3]={greys_val,greys_val,greys_val};

if(maxHueScoreDeg!=360){ //not grey
        double hsv_ato[3]={maxHueScoreDeg_dbl,hueCount_sat[maxHueScoreDeg],greys_val};
 hsv2rgb_360(hsv_ato,rgb_ato);
}

      double rgbLin[3];

       rgbLin[0]=rgb_ato[0];
       rgbLin[1]=rgb_ato[1];
       rgbLin[2]=rgb_ato[2];

      if(lnr==0){
        Apply_gamma(rgbLin,rgb_ato, mde,aprxPw);
      }

      double xyY_ato[3];
       get_xy(rgb_ato, xyY_ato , params->mode,lnr,aprxPw);

        params->x=xyY_ato[0];
        params->y=xyY_ato[1];
        cust_x=xyY_ato[0];
        cust_y=xyY_ato[1];

int rfi=(sxf==1)?round(rgb_ato[0]*65535):round(rgb_ato[0]*255);
int gfi=(sxf==1)?round(rgb_ato[1]*65535):round(rgb_ato[1]*255);
int bfi=(sxf==1)?round(rgb_ato[2]*65535):round(rgb_ato[2]*255);


if ((nm!="")&&(nm!="NULL")&&(dbg!=6)){
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
    fprintf(fptr,"%s: %d, %d, %d - %d\n", lid,rfi, gfi, bfi,n );
}else{
    fprintf(fptr,"%d, %d, %d - %d\n", rfi, gfi, bfi,n );
}

   fclose(fptr);

}
}



      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {


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

rgb2XYZ(OG_RGB,rgbXYZ,rgbXYZGrey,mde,0,lnr,aprxPw);

if(rOG==0 && (gOG==0) && (bOG==0)){
    WPchgRGB[0]=0;
    WPchgRGB[1]=0;
    WPchgRGB[2]=0;
}else{
      if(((eds!="")&&(eds!="NULL")) || ((eds2!="")&&(eds2!="NULL"))){

    int curr_clip=-1;
	 int bse=n;
	 int found2=0;

	 if((eds2!="")&&(eds2!="NULL")){
        curr_clip=params->ed_lookup2[bse];
        found2=(curr_clip==-1)?0:1;
	 }

	 if(((eds!="")&&(eds!="NULL"))&&(found2==0)){
		bse=(ed_bse>=0)?ed_bse:n;

		curr_clip=params->ed_lookup[bse];

	if(curr_clip !=-1){
		if(curr_clip+ed_offst>edlm-1){
			curr_clip=edlm-1;
		}else if(curr_clip+ed_offst<0){
			curr_clip=0;
		}else{
			curr_clip+=ed_offst;
		}
	}

      }

		if(curr_clip==-1){
			if(dbg==5){
				rOG=cust_x;
				gOG=0;
				bOG=cust_y;
				fv_swt=1;
			}
		}else if(found2==0){
						if (params->ed_switch[curr_clip]==1){

				params->x=params->ed_x[curr_clip];
				params->y=params->ed_y[curr_clip];
				cust_x=params->ed_x[curr_clip];
				cust_y=params->ed_y[curr_clip];

							if(dbg==5){
							rOG=cust_x;
							gOG=0;
							bOG=cust_y;
							fv_swt=1;
							}

			}else{

				if((params->ed_Red[curr_clip]!=0) || (params->ed_Green[curr_clip]!=0) || (params->ed_Blue[curr_clip]!=0)){
							double xyY_rgb[3];

						double rgb[3];

						rgb[0]=(sxf==1)?(double)(params->ed_Red[curr_clip])*rcptHiBit:(double)(params->ed_Red[curr_clip])*rcptwoFiveFive;
						rgb[1]=(sxf==1)?(double)(params->ed_Green[curr_clip])*rcptHiBit:(double)(params->ed_Green[curr_clip])*rcptwoFiveFive;
						rgb[2]=(sxf==1)?(double)(params->ed_Blue[curr_clip])*rcptHiBit:(double)(params->ed_Blue[curr_clip])*rcptwoFiveFive;

							if(dbg==5){
							rOG=rgb[0];
							gOG=rgb[1];
							bOG=rgb[2];
							fv_swt=2;
							}

						get_xy(rgb, xyY_rgb , mde,lnr,aprxPw);

						params->x=xyY_rgb[0];
						params->y=xyY_rgb[1];
						cust_x=xyY_rgb[0];
						cust_y=xyY_rgb[1];
				}

			  }
		}else{
									if (params->ed_switch2[curr_clip]==1){

				params->x=params->ed_x2[curr_clip];
				params->y=params->ed_y2[curr_clip];
				cust_x=params->ed_x2[curr_clip];
				cust_y=params->ed_y2[curr_clip];

							if(dbg==5){
							rOG=cust_x;
							gOG=0;
							bOG=cust_y;
							fv_swt=1;
							}

			}else{

				if((params->ed_Red2[curr_clip]!=0) || (params->ed_Green2[curr_clip]!=0) || (params->ed_Blue2[curr_clip]!=0)){
							double xyY_rgb[3];

						double rgb[3];

						rgb[0]=(sxf==1)?(double)(params->ed_Red2[curr_clip])*rcptHiBit:(double)(params->ed_Red2[curr_clip])*rcptwoFiveFive;
						rgb[1]=(sxf==1)?(double)(params->ed_Green2[curr_clip])*rcptHiBit:(double)(params->ed_Green2[curr_clip])*rcptwoFiveFive;
						rgb[2]=(sxf==1)?(double)(params->ed_Blue2[curr_clip])*rcptHiBit:(double)(params->ed_Blue2[curr_clip])*rcptwoFiveFive;

							if(dbg==5){
							rOG=rgb[0];
							gOG=rgb[1];
							bOG=rgb[2];
							fv_swt=2;
							}

						get_xy(rgb, xyY_rgb , mde,lnr,aprxPw);

						params->x=xyY_rgb[0];
						params->y=xyY_rgb[1];
						cust_x=xyY_rgb[0];
						cust_y=xyY_rgb[1];
				}

			  }
		}

	  }

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
        XYZ2rgb(WPConvXYZ,WPchgRGB,mde,lnr,aprxPw);

}else{

    if (to_x!=D65_x || (to_y!=D65_y)){
        double dst_xy[2]={to_x,to_y};
        double dst_XYZ[3];
        xy2XYZ(dst_xy,dst_XYZ);
        WPconv(rgbXYZ,D65XYZ,dst_XYZ,WPConvXYZ);
        XYZ2rgb(WPConvXYZ,WPchgRGB,mde,lnr,aprxPw);
    }


}


}

if(dbg==1){
    double mx=MAX(WPchgRGB[0],MAX(WPchgRGB[1],WPchgRGB[2]));
    double sat=(mx==0)?0:(mx-MIN(WPchgRGB[0],MIN(WPchgRGB[1],WPchgRGB[2])))/mx;

    double dbg_out;
	if(aprxPw==1){
     dbg_out=(amp==1)?sat:fastPrecisePow(sat,amp);
	}else{
		dbg_out=(amp==1)?sat:pow(sat,amp);
	}

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
}else if (dbg==4){
    double mx=MAX(WPchgRGB[0],MAX(WPchgRGB[1],WPchgRGB[2]));
    double sat=(mx==0)?0:(mx-MIN(WPchgRGB[0],MIN(WPchgRGB[1],WPchgRGB[2])))/mx;

    double mxOG=MAX(rOG,MAX(gOG,bOG));
    double satOG=(mxOG==0)?0:(mxOG-MIN(rOG,MIN(gOG,bOG)))/mxOG;

    double hue_dbg=120;

    double abs_satDiff=(satOG==0)?fabs(satOG-sat):fabs(satOG-sat)/satOG;
    double satDiff1=(satOG==0)?satOG-sat:fabs(satOG-sat)/satOG;
     satDiff1=satDiff1*satDiff1;
    double satDiff2=(satOG==1)?sat-satOG:(sat-satOG)/(1-satOG);
     satDiff2=satDiff2*satDiff2;

    hue_dbg=(sat<satOG)?lerp(157.5,240,satDiff1):hue_dbg;  //Sat decreased, cyan to blue
    hue_dbg=(sat>satOG)?lerp(307.5,367.5,satDiff2):hue_dbg; //Sat increased, Magenta to Orange
    hue_dbg=(hue_dbg==360)?0:hue_dbg;
    hue_dbg=(hue_dbg>360)?hue_dbg-360:hue_dbg;
    double hsv_dbg[3]={hue_dbg,1,lerp(0.3*(1-amp),1-amp,abs_satDiff)};
    hsv2rgb_360(hsv_dbg,WPchgRGB);

}else if (dbg==5){
double x_shift=(double)(x)/(double)(row_size);

if(fv_swt==1){
    WPchgRGB[0]=(x_shift<0.5)?rOG:bOG;
    WPchgRGB[1]=(x_shift<0.5)?rOG:bOG;
    WPchgRGB[2]=(x_shift<0.5)?rOG:bOG;
}else if(fv_swt==2){
    WPchgRGB[0]=rOG;
    WPchgRGB[1]=gOG;
    WPchgRGB[2]=bOG;

}

}else if(dbg==6){
    double mx=MAX(WPchgRGB[0],MAX(  WPchgRGB[1],WPchgRGB[2]));
    double sat=(mx==0)?0:(mx-MIN(WPchgRGB[0],MIN(WPchgRGB[1],WPchgRGB[2])))/mx;
    double OGmx=MAX(rOG,MAX(gOG,bOG));
    double OGsat=(mx==0)?0:(OGmx-MIN(rOG,MIN(gOG,bOG)))/OGmx;
    double redu=OGsat-sat;
    if((redu>sat_dbg_six)&&(sat<=amp)){
      sat_dbg_six=redu;
        r_dbg_six=rOG;
        g_dbg_six=gOG;
        b_dbg_six=bOG;
    }
    WPchgRGB[0]=(sat<=amp)?rOG:0;
    WPchgRGB[1]=(sat<=amp)?gOG:0;
    WPchgRGB[2]=(sat<=amp)?bOG:0;
}else if(dbg==7){

    int grey=((WPchgRGB[0]==WPchgRGB[1])&&(WPchgRGB[1]==WPchgRGB[2]))?1:0;



if (grey==0){

double mn=MIN(WPchgRGB[0],MIN(WPchgRGB[1],WPchgRGB[2]));
double mx=MAX(WPchgRGB[0],MAX(WPchgRGB[1],WPchgRGB[2]));
double diff=mx-mn;

double dbgHSV[3];
dbgHSV[1]=1.0;
dbgHSV[2]=1-amp;

double dbgRGB[3];

    double hue_d;

        if ((WPchgRGB[0]>WPchgRGB[1])&&(WPchgRGB[0]>WPchgRGB[2])){
            hue_d =(WPchgRGB[1] - WPchgRGB[2]) / diff;
        }else if ((WPchgRGB[1]>WPchgRGB[0])&&(WPchgRGB[1]>WPchgRGB[2])){
            hue_d = 2.0 + (WPchgRGB[2] - WPchgRGB[0]) / diff;
        }else{
            hue_d = 4.0 + (WPchgRGB[0] - WPchgRGB[1]) / diff;
        }
            hue_d*=60;
            hue_d =(hue_d < 0)?hue_d + 360:hue_d;

            int hue=floor(hue_d*10);


if((hue>=3525)||(((hue>=0) && (hue<75)))){
dbgHSV[0]=0.0;
}else if((hue>=75) && (hue<375)){
dbgHSV[0]=30.0;
}else if((hue>=375) && (hue<675)){
dbgHSV[0]=60.0;
}else if((hue>=675) && (hue<975)){
dbgHSV[0]=90.0;
}else if((hue>=975) && (hue<1275)){
dbgHSV[0]=120.0;
}else if((hue>=1275) && (hue<1575)){
dbgHSV[0]=150.0;
}else if((hue>=1575) && (hue<1875)){
dbgHSV[0]=180.0;
}else if((hue>=1875) && (hue<2175)){
dbgHSV[0]=210.0;
}else if((hue>=2175) && (hue<2475)){
dbgHSV[0]=240.0;
}else if((hue>=2475) && (hue<3075)){
dbgHSV[0]=270.0;
}else if((hue>=3075) && (hue<3375)){
dbgHSV[0]=330.0;
dbgHSV[1]=0.8;
}else if((hue>=3375) && (hue<3525)){
dbgHSV[0]=345.0;
dbgHSV[1]=0.95;
}

 hsv2rgb_360(dbgHSV, dbgRGB);
    WPchgRGB[0]=dbgRGB[0];
    WPchgRGB[1]=dbgRGB[1];
    WPchgRGB[2]=dbgRGB[2];


}else{
    WPchgRGB[0]=1.0;
    WPchgRGB[1]=1.0;
    WPchgRGB[2]=1.0;
}


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

      if ((dbg==6)&&(nm!="")&&(nm!="NULL")){
   //int num;
   FILE *fptr;
   // use appropriate location if you are using MacOS or Linux
   fptr = fopen(nm,"a");
   if(fptr == NULL)
   {
      printf("Error!");
      exit(1);
   }

   int redInt=(sxf==1)?round(r_dbg_six*65535):round(r_dbg_six*255);
   int greenInt=(sxf==1)?round(g_dbg_six*65535):round(g_dbg_six*255);
   int blueInt=(sxf==1)?round(b_dbg_six*65535):round(b_dbg_six*255);

if ((lid!="")&&(lid!="NULL")){
    fprintf(fptr,"%s: %d, %d, %d, %.11f, %d\n", lid,redInt, greenInt, blueInt, sat_dbg_six,n);
}else{
    fprintf(fptr,"%d, %d, %d, %.11f, %d\n",redInt, greenInt, blueInt, sat_dbg_six,n);
}
   fclose(fptr);
}


  return src;
}

void AVSC_CC free_Manual_WP(AVS_FilterInfo* fi)
{
    Manual_WP* params = (Manual_WP*) fi->user_data;
    free(params);
}

AVS_Value AVSC_CC create_Manual_WP (AVS_ScriptEnvironment * env,AVS_Value args, void * dg)
{
  AVS_Value v;
  AVS_FilterInfo * fi;
  AVS_Clip * new_clip = avs_new_c_filter(env, &fi, avs_array_elt(args, 0), 1);
    const AVS_VideoInfo * vi=avs_get_video_info(new_clip);
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

               double dbg_v=1;
               if(params->debug==2 || params->debug==4 || params->debug==6){
                dbg_v=0.03;
               }else if(params->debug==3){
                dbg_v=0.97;
               }else if(params->debug==7){
                dbg_v=0.37;
               }
            params->debug_val=  avs_defined(avs_array_elt(args,8))?avs_as_float(avs_array_elt(args, 8)):dbg_v;

                params->dst_x=  avs_defined(avs_array_elt(args,10))?avs_as_float(avs_array_elt(args, 10)):0.312727;
                params->dst_y=  avs_defined(avs_array_elt(args,11))?avs_as_float(avs_array_elt(args, 11)):0.329023;
                params->auto_WP=  avs_defined(avs_array_elt(args,12))?avs_as_bool(avs_array_elt(args, 12)):false;
                params->overwrite=  avs_defined(avs_array_elt(args,15))?avs_as_bool(avs_array_elt(args, 15)):true;
                params->linear=  avs_defined(avs_array_elt(args,16))?avs_as_bool(avs_array_elt(args, 16)):false;
                params->ed_off=  avs_defined(avs_array_elt(args,18))?avs_as_int(avs_array_elt(args, 18)):0;
                params->ed_base=  avs_defined(avs_array_elt(args,19))?avs_as_int(avs_array_elt(args, 19)):-1;
				 params->approxPow=  avs_defined(avs_array_elt(args,21))?avs_as_bool(avs_array_elt(args, 21)):true;

char* file_name ="";
file_name = ((avs_as_string(avs_array_elt(args, 13)))&&(avs_as_string(avs_array_elt(args, 13))!="NULL"))?avs_as_string(avs_array_elt(args, 13)):file_name;
params->file = file_name;

char* log_idt ="";
log_idt = ((avs_as_string(avs_array_elt(args, 14)))&&(avs_as_string(avs_array_elt(args, 14))!="NULL"))?avs_as_string(avs_array_elt(args, 14)):log_idt;
params->log_id = log_idt;

char* edts ="";
edts = ((avs_as_string(avs_array_elt(args, 17)))&&(avs_as_string(avs_array_elt(args, 17))!="NULL"))?avs_as_string(avs_array_elt(args, 17)):edts;
params->edits = edts;

char* edts2 ="";
edts2 = ((avs_as_string(avs_array_elt(args, 20)))&&(avs_as_string(avs_array_elt(args, 20))!="NULL"))?avs_as_string(avs_array_elt(args, 20)):edts2;
params->edits2 = edts2;


    if (params->ed_base<-1) {
    return avs_new_value_error ("'ed_base' must be >= -1");
    }else{
      if ((params->debug<0)||(params->debug>7)){
            return avs_new_value_error ("Allowed debug settings are between 0 and 7!");
          }else{
          if ((params->mode<0)||(params->mode>11)){
            return avs_new_value_error ("Allowed modes are between 0 and 11!");
          }else{
  if (!((avs_is_rgb32(&fi->vi))||(avs_is_rgb64(&fi->vi)))) {
    return avs_new_value_error ("Input video must be in RGB32 OR RGB64 format!");
  }else {

    if(((params->auto_WP==true)||(params->debug==6))&&((file_name!="")&&(file_name!="NULL"))){
        FILE *fptr;

        if(params->overwrite==true){
        fptr = fopen(file_name,"w");
        }else{
        fptr = fopen(file_name,"a+");
        }

        if(fptr == NULL)
        {
          printf("Error!");
          exit(1);
        }

        //fprintf(fptr,"%s\n",file_name);
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
      }else if ((params->auto_WP==false)&&((params->edits=="")||(params->edits=="NULL"))){

        double xyY_rgb[3];

        double rgb[3];

        rgb[0]=(params->sixtyFour==true)?(double)(params->R)*rcptHiBit:(double)(params->R)*rcptwoFiveFive;
        rgb[1]=(params->sixtyFour==true)?(double)(params->G)*rcptHiBit:(double)(params->G)*rcptwoFiveFive;
        rgb[2]=(params->sixtyFour==true)?(double)(params->B)*rcptHiBit:(double)(params->B)*rcptwoFiveFive;

        int linar=(params->linear==true)?1:0;

        get_xy(rgb, xyY_rgb , params->mode,linar,((params->approxPow==true)?1:0));

        params->x=xyY_rgb[0];
        params->y=xyY_rgb[1];

      }

  }

 if((edts2!="")&&(edts2!="NULL")){

    params->split2=(char*)malloc(sizeof(char*)*strlen(edts2));
    char *dup = strdup(edts2);
    char* token;
    char* rest=dup;

int is = 0;
int js = 0;

while (dup[is]!='\0'){
        if ((isdigit(dup[is]))||(dup[is]==',')||(dup[is]=='-')||(dup[is]=='.')||(dup[is]=='}')||(dup[is]=='{')){
        rest[js]=dup[is];
        js++;
        }
        is++;
    }

rest[js]='\0';


      int tkn=0;
    while ((token = strtok_r(rest, "{", &rest))){

        params->split2[tkn]=token;
        tkn++;
    }
    params->ed_lim2=tkn;
    int no_clips=tkn;


        params->ed_Red2=(int*)malloc(sizeof(int)*no_clips);
      params->ed_Green2=(int*)malloc(sizeof(int)*no_clips);
       params->ed_Blue2=(int*)malloc(sizeof(int)*no_clips);

        params->ed_x2=(double*)malloc(sizeof(double)*no_clips);
        params->ed_y2=(double*)malloc(sizeof(double)*no_clips);

      params->ed_start_fr2=(int*)malloc(sizeof(int)*no_clips);
        params->ed_end_fr2=(int*)malloc(sizeof(int)*no_clips);
        params->ed_switch2=(int*)malloc(sizeof(int)*no_clips);
        params->ed_lookup2=(int*)malloc(sizeof(int)*vi->num_frames);

         memset(params->ed_lookup2,-1,sizeof(params->ed_lookup2[0])*vi->num_frames); //initialise to -1

        tkn-=1;

    while (tkn>=0){
     params->split2[tkn]=strtok(strdup(params->split2[tkn]),"}");
        tkn--;
    }

    tkn=0;
    int cnt=0;

    while (tkn<no_clips){
    while ((token = strtok_r(params->split2[tkn], ",", &params->split2[tkn]))){
int intg;
switch(cnt){
        case 0:
        intg=atoi(token);
        params->ed_Red2[tkn]=intg;
        break;

    case 1:
    intg=atoi(token);
    params->ed_Green2[tkn]=intg;
    break;

        case 2:
        intg=atoi(token);
        params->ed_Blue2[tkn]=intg;
        break;

    case 3:
    intg=atoi(token);
    intg=(intg<0)?0:intg;
    params->ed_start_fr2[tkn]=intg;
    break;

        case 4:
        intg=atoi(token);
        intg=(intg>(vi->num_frames)-1)?0:intg;
        params->ed_end_fr2[tkn]=intg;
        break;

    case 5:
    params->ed_x2[tkn]=atof(token);
    break;

        case 6:
        params->ed_y2[tkn]=atof(token);
        break;

    default:
    ;


}
 cnt=(cnt==6)?0:cnt+1;
    }


    if((params->ed_Red2[tkn]<0)||(params->ed_Green2[tkn]<0)||(params->ed_Blue2[tkn]<0)){
            params->ed_switch2[tkn]=1;
    }else{

        if(params->sixtyFour==false){
            if((params->ed_Red2[tkn]>255)||(params->ed_Green2[tkn]>255)||(params->ed_Blue2[tkn]>255)){
                params->ed_switch2[tkn]=1;
            }else{
                params->ed_switch2[tkn]=0;
            }
        }else{
            if((params->ed_Red2[tkn]>65535)||(params->ed_Green2[tkn]>65535)||(params->ed_Blue2[tkn]>65535)){
            params->ed_switch2[tkn]=1;
            }else{
            params->ed_switch2[tkn]=0;
            }
        }

    }

    int ed_frm2=params->ed_end_fr2[tkn];

    if(ed_frm2==0){
        ed_frm2=(params->ed_start_fr2[tkn]==0)?0:(vi->num_frames)-1;
    }

    for(int k=params->ed_start_fr2[tkn]; k<=ed_frm2; k++){
    params->ed_lookup2[k]=tkn;
    }
      tkn++;
    }

  }

  if((edts!="")&&(edts!="NULL")){

    params->split=(char*)malloc(sizeof(char*)*strlen(edts));
    char *dup = strdup(edts);
    char* token;
    char* rest=dup;

int is = 0;
int js = 0;

while (dup[is]!='\0'){
        if ((isdigit(dup[is]))||(dup[is]==',')||(dup[is]=='-')||(dup[is]=='.')||(dup[is]=='}')||(dup[is]=='{')){
        rest[js]=dup[is];
        js++;
        }
        is++;
    }

rest[js]='\0';


      int tkn=0;
    while ((token = strtok_r(rest, "{", &rest))){

        params->split[tkn]=token;
        tkn++;
    }
    params->ed_lim=tkn;
    int no_clips=tkn;


            params->ed_Red=(int*)malloc(sizeof(int)*no_clips);
        params->ed_Green=(int*)malloc(sizeof(int)*no_clips);
        params->ed_Blue=(int*)malloc(sizeof(int)*no_clips);

        params->ed_x=(double*)malloc(sizeof(double)*no_clips);
        params->ed_y=(double*)malloc(sizeof(double)*no_clips);

        params->ed_start_fr=(int*)malloc(sizeof(int)*no_clips);
        params->ed_end_fr=(int*)malloc(sizeof(int)*no_clips);
        params->ed_switch=(int*)malloc(sizeof(int)*no_clips);
        params->ed_lookup=(int*)malloc(sizeof(int)*vi->num_frames);

        memset(params->ed_lookup,-1,sizeof(params->ed_lookup[0])*vi->num_frames); //initialise to -1


        tkn-=1;

    while (tkn>=0){
     params->split[tkn]=strtok(strdup(params->split[tkn]),"}");
        tkn--;
    }

    tkn=0;
    int cnt=0;

    while (tkn<no_clips){
    while ((token = strtok_r(params->split[tkn], ",", &params->split[tkn]))){
int intg;
switch(cnt){
        case 0:
        intg=atoi(token);
        params->ed_Red[tkn]=intg;
        break;

    case 1:
    intg=atoi(token);
    params->ed_Green[tkn]=intg;
    break;

        case 2:
        intg=atoi(token);
        params->ed_Blue[tkn]=intg;
        break;

    case 3:
    intg=atoi(token);
    intg=(intg<0)?0:intg;
    params->ed_start_fr[tkn]=intg;
    break;

        case 4:
        intg=atoi(token);
        intg=(intg>(vi->num_frames)-1)?0:intg;
        params->ed_end_fr[tkn]=intg;
        break;

    case 5:
    params->ed_x[tkn]=atof(token);
    break;

        case 6:
        params->ed_y[tkn]=atof(token);
        break;

    default:
    ;


}
 cnt=(cnt==6)?0:cnt+1;
    }


    if((params->ed_Red[tkn]<0)||(params->ed_Green[tkn]<0)||(params->ed_Blue[tkn]<0)){
            params->ed_switch[tkn]=1;
    }else{

        if(params->sixtyFour==false){
            if((params->ed_Red[tkn]>255)||(params->ed_Green[tkn]>255)||(params->ed_Blue[tkn]>255)){
                params->ed_switch[tkn]=1;
            }else{
                params->ed_switch[tkn]=0;
            }
        }else{
            if((params->ed_Red[tkn]>65535)||(params->ed_Green[tkn]>65535)||(params->ed_Blue[tkn]>65535)){
            params->ed_switch[tkn]=1;
            }else{
            params->ed_switch[tkn]=0;
            }
        }

    }

    int ed_frm=params->ed_end_fr[tkn];

    if(ed_frm==0){
        ed_frm=(params->ed_start_fr[tkn]==0)?0:(vi->num_frames)-1;
    }

    for(int k=params->ed_start_fr[tkn]; k<=ed_frm; k++){
        params->ed_lookup[k]=tkn;
    }

      tkn++;
    }

  //Write int to error
 /* char *str1 = malloc(MAX_PATH);
sprintf(str1,"%d",params->ed_end_fr[0]);
  return avs_new_value_error (str1);*/

  }

if((params->auto_WP==1)&&((params->edits=="")||(params->edits=="NULL"))&&((params->edits2=="")||(params->edits2=="NULL"))){
params->hueCount=(double*)malloc(sizeof(double)*361);
params->hueCount_sat=(double*)malloc(sizeof(double)*361);
params->hueCount_val=(double*)malloc(sizeof(double)*361);
params->hueCount_prp=(double*)malloc(sizeof(double)*361);
params->hueCount_wht=(double*)malloc(sizeof(double)*361);
params->hueCount_wht_prp=(double*)malloc(sizeof(double)*361);

  }
         fi->user_data = (void*) params;
    fi->get_frame = Manual_WP_get_frame;
    v = avs_new_value_clip(new_clip);
     fi->free_filter = free_Manual_WP;
   }
   }
   }
   }
  avs_release_clip(new_clip);
  return v;

}

const char * AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment * env)
{
   avs_add_function(env, "Manual_WP", "c[x]f[y]f[R]i[G]i[B]i[mode]i[debug]i[debug_val]f[sixtyFour]b[dst_x]f[dst_y]f[auto_WP]b[file]s[log_id]s[overwrite]b[linear]b[edits]s[ed_off]i[ed_base]i[edits2]s[approxPow]b", create_Manual_WP, 0);
   return "Manual_WP C plugin";
}
