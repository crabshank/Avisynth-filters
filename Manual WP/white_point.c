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
        int bb_off;
        int ed_base;
        int bb_base;
        int* ed_Red;
        int* ed_Green;
        int* ed_Blue;
		double* ed_Red_dbl;
        double* ed_Green_dbl;
        double* ed_Blue_dbl;
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
		double* ed_Red2_dbl;
        double* ed_Green2_dbl;
        double* ed_Blue2_dbl;
        double* ed_x2;
        double* ed_y2;
        int* ed_start_fr2;
        int* ed_end_fr2;
        int* ed_switch2;
        int* ed_lookup2;
        int ed_lim2;
		char* edits2;
        int* bb_Red;
        int* bb_Green;
        int* bb_Blue;
		double* bb_Red_dbl;
        double* bb_Green_dbl;
        double* bb_Blue_dbl;
        double* bb_x;
        double* bb_y;
        int* bb_start_fr;
        int* bb_end_fr;
        int* bb_switch;
        int* bb_lookup;
        int bb_lim;
        char* bb;
        int* bb_Red2;
        int* bb_Green2;
        int* bb_Blue2;
		double* bb_Red2_dbl;
        double* bb_Green2_dbl;
        double* bb_Blue2_dbl;
        double* bb_x2;
        double* bb_y2;
        int* bb_start_fr2;
        int* bb_end_fr2;
        int* bb_switch2;
        int* bb_lookup2;
        int bb_lim2;
		char* bb2;
		double *bb_R;
	   double *bb_G;
	   double *bb_B;
	   double *WP_R_lin;
	   double *WP_G_lin;
	   double *WP_B_lin;
		int approxPow;
        char** split;
        char** split2;
        char** split_bb;
        char** split_bb2;
        double* hueCount;
        double* hueCount_sat;
        double* hueCount_val;
        double* hueCount_prp;
        double* hueCount_wht;
        double* hueCount_wht_prp;
		int pxls;
} Manual_WP;


AVS_VideoFrame * AVSC_CC Manual_WP_get_frame (AVS_FilterInfo * p, int n)
{
  AVS_VideoFrame * src;
   Manual_WP* params = (Manual_WP*) p->user_data;

  src = avs_get_frame(p->child, n);

   int row_size, height, src_pitch,x, y,dbg,mde,sxf,ato,lnr,edlm,edlm2,bblm,bblm2,ed_offst,bb_offst,fv_swt,ed_bse,bb_bse,aprxPw,p_ix,bb_curr_clip,found2_bb;
   BYTE* srcp;
   const BYTE* rrcp;
     char* nm;
     char* lid;
     char* eds;
     char* eds2;
   double rOG,bOG,gOG,cust_x,cust_y,cust_x_bb,cust_y_bb,amp,D65_x,D65_y, to_x, to_y,sat_dbg_six,r_dbg_six,g_dbg_six,b_dbg_six;

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
bblm=params->bb_lim;
bblm2=params->bb_lim2;
ed_offst=params->ed_off;
bb_offst=params->bb_off;
ed_bse=params->ed_base;
bb_bse=params->bb_base;
aprxPw=params->approxPow;
found2_bb=0;
int b_bse=n;
if(((params->bb!="")&&(params->bb!="NULL"))||((params->bb2!="")&&(params->bb2!="NULL"))){
    bb_curr_clip=-1;

	 if((params->bb2!="")&&(params->bb2!="NULL")){
        bb_curr_clip=params->bb_lookup2[b_bse];
        found2_bb=(bb_curr_clip==-1)?0:1;
	 }

	 if(((params->bb!="")&&(params->bb!="NULL"))&&(found2_bb==0)){
		b_bse=(bb_bse>=0)?bb_bse:n;
		bb_curr_clip=params->bb_lookup[b_bse];

	if(bb_curr_clip !=-1){
		if(bb_curr_clip+bb_offst>bblm-1){
			bb_curr_clip=bblm-1;
		}else if(bb_curr_clip+bb_offst<0){
			bb_curr_clip=0;
		}else{
			bb_curr_clip+=bb_offst;
		}
	}

      }

}else{
    bb_curr_clip=-1;
}

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

       // params->x=xyY_ato[0];
      //  params->y=xyY_ato[1];
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


p_ix=0;

      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

int intRed, intGreen, intBlue;

					if(bb_curr_clip!=-1){
	                intBlue=(sxf==1)?rrcp[x]+rrcp[x+1]*256:rrcp[x];
                  intGreen=(sxf==1)?rrcp[x+2]+rrcp[x+3]*256:rrcp[x+1];
                  intRed=(sxf==1)?rrcp[x+4]+rrcp[x+5]*256:rrcp[x+2];
					}else{
					intBlue=(sxf==1)?srcp[x]+srcp[x+1]*256:srcp[x];
                  intGreen=(sxf==1)?srcp[x+2]+srcp[x+3]*256:srcp[x+1];
                  intRed=(sxf==1)?srcp[x+4]+srcp[x+5]*256:srcp[x+2];
					}


				 double currBlue=(double)intBlue;
				 double currGreen=(double)intGreen;
				 double currRed=(double)intRed;

int greyOG=((intRed==intGreen)&&(intGreen==intBlue))?1:0;
int blackOG=((greyOG==1)&&(intRed==0))?1:0;


    bOG=(sxf==1)?currBlue*rcptHiBit:currBlue*rcptwoFiveFive;     // B
       gOG=(sxf==1)?currGreen*rcptHiBit:currGreen*rcptwoFiveFive;   //G
         rOG=(sxf==1)?currRed*rcptHiBit:currRed*rcptwoFiveFive;     // R

double rgbXYZ[3];
double rgbXYZGrey[3];
double WPConvXYZ[3];
double OG_RGB[3]={rOG,gOG,bOG};
double OG_RGB_lin[3]={rOG,gOG,bOG};
double WPchgRGB[3]={rOG,gOG,bOG};

if(lnr==0){
    Linearise(OG_RGB,OG_RGB_lin,mde,aprxPw);
}

rgb2XYZ(OG_RGB_lin,rgbXYZ,rgbXYZGrey,mde,0,1,aprxPw);

if(blackOG){
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

				//params->x=params->ed_x[curr_clip];
				//params->y=params->ed_y[curr_clip];
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
							double xyY_rgb_ed[3];

						double rgb_ed[3];

						rgb_ed[0]=params->ed_Red_dbl[curr_clip];
						rgb_ed[1]=params->ed_Green_dbl[curr_clip];
						rgb_ed[2]=params->ed_Blue_dbl[curr_clip];

							if(dbg==5){
							rOG=rgb_ed[0];
							gOG=rgb_ed[1];
							bOG=rgb_ed[2];
							fv_swt=2;
							}

						get_xy(rgb_ed, xyY_rgb_ed , mde,lnr,aprxPw);

						//params->x=xyY_rgb[0];
						//params->y=xyY_rgb[1];
						cust_x=xyY_rgb_ed[0];
						cust_y=xyY_rgb_ed[1];
				}

			  }
		}else{
									if (params->ed_switch2[curr_clip]==1){

				//params->x=params->ed_x2[curr_clip];
				//params->y=params->ed_y2[curr_clip];
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
							double xyY_rgb_ed2[3];

						double rgb_ed2[3];

                        rgb_ed2[0]=params->ed_Red2_dbl[curr_clip];
						rgb_ed2[1]=params->ed_Green2_dbl[curr_clip];
						rgb_ed2[2]=params->ed_Blue2_dbl[curr_clip];

							if(dbg==5){
							rOG=rgb_ed2[0];
							gOG=rgb_ed2[1];
							bOG=rgb_ed2[2];
							fv_swt=2;
							}

						get_xy(rgb_ed2, xyY_rgb_ed2 , mde,lnr,aprxPw);

						//params->y=xyY_rgb[1];
						//params->x=xyY_rgb[0];
						cust_x=xyY_rgb_ed2[0];
						cust_y=xyY_rgb_ed2[1];
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

if(bb_curr_clip!=-1){

	 if(lnr==0){
                if(blackOG==0){
                    double lin_rgb_bb[3];
                    XYZ2rgb(WPConvXYZ,lin_rgb_bb,mde,1,aprxPw);
                    params->WP_R_lin[p_ix]=lin_rgb_bb[0];
                    params->WP_G_lin[p_ix]=lin_rgb_bb[1];
                    params->WP_B_lin[p_ix]=lin_rgb_bb[2];
                }else{
                    params->WP_R_lin[p_ix]=0;
                    params->WP_G_lin[p_ix]=0;
                    params->WP_B_lin[p_ix]=0;
                }
        }else{
			params->WP_R_lin[p_ix]=WPchgRGB[0];
			params->WP_G_lin[p_ix]=WPchgRGB[1];
			params->WP_B_lin[p_ix]=WPchgRGB[2];
		}

	params->bb_R[p_ix]=OG_RGB_lin[0];
	params->bb_G[p_ix]=OG_RGB_lin[1];
	params->bb_B[p_ix]=OG_RGB_lin[2];


if(dbg==6){
    double mx=MAX(WPchgRGB[0],MAX(WPchgRGB[1],WPchgRGB[2]));
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
}

}else{

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
}else if(dbg==2||dbg==8){
    double mx=MAX(WPchgRGB[0],MAX(WPchgRGB[1],WPchgRGB[2]));
    double sat=(mx==0)?0:(mx-MIN(WPchgRGB[0],MIN(WPchgRGB[1],WPchgRGB[2])))/mx;

    WPchgRGB[0]=(sat>=amp)?WPchgRGB[0]:((dbg==8)?0.5:0);
    WPchgRGB[1]=(sat>=amp)?WPchgRGB[1]:((dbg==8)?0.5:0);
    WPchgRGB[2]=(sat>=amp)?WPchgRGB[2]:((dbg==8)?0.5:0);
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
	  }

x=(sxf==1)?x+7:x+3;
p_ix++;

      }
		  if(bb_curr_clip!=-1){
				rrcp += src_pitch;
		  }else{
				srcp += src_pitch;
		  }
      }

rrcp=avs_get_read_ptr(src);

if(bb_curr_clip!=-1){

if(found2_bb==1){

if (params->bb_switch2[bb_curr_clip]==1){

				cust_x_bb=params->bb_x2[bb_curr_clip];
				cust_y_bb=params->bb_y2[bb_curr_clip];

}else{

				if((params->bb_Red2[bb_curr_clip]!=0) || (params->bb_Green2[bb_curr_clip]!=0) || (params->bb_Blue2[bb_curr_clip]!=0)){
							double xyY_rgb_bb[3];

						double rgb_bb[3];
						 rgb_bb[0]=params->bb_Red2_dbl[bb_curr_clip];
						 rgb_bb[1]=params->bb_Green2_dbl[bb_curr_clip];
						 rgb_bb[2]=params->bb_Blue2_dbl[bb_curr_clip];

						get_xy(rgb_bb, xyY_rgb_bb , mde,1,aprxPw);

						cust_x_bb=xyY_rgb_bb[0];
						cust_y_bb=xyY_rgb_bb[1];
				}

}

}else{
    if (params->bb_switch[bb_curr_clip]==1){

				cust_x_bb=params->bb_x[bb_curr_clip];
				cust_y_bb=params->bb_y[bb_curr_clip];

}else{

				if((params->bb_Red[bb_curr_clip]!=0) || (params->bb_Green[bb_curr_clip]!=0) || (params->bb_Blue[bb_curr_clip]!=0)){
							double xyY_rgb_bb[3];

						double rgb_bb[3];
						 rgb_bb[0]=params->bb_Red_dbl[bb_curr_clip];
						 rgb_bb[1]=params->bb_Green_dbl[bb_curr_clip];
						 rgb_bb[2]=params->bb_Blue_dbl[bb_curr_clip];

						get_xy(rgb_bb, xyY_rgb_bb , mde,1,aprxPw);

						cust_x_bb=xyY_rgb_bb[0];
						cust_y_bb=xyY_rgb_bb[1];
				}

}
}

	p_ix=0;

      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

		  double rgb_out[3]={0,0,0};
            double rgb_og_lin[3];
             rgb_og_lin[0]=params->bb_R[p_ix];
             rgb_og_lin[1]=params->bb_G[p_ix];
             rgb_og_lin[2]=params->bb_B[p_ix];

                         double rgb_WP_lin[3];
             rgb_WP_lin[0]=params->WP_R_lin[p_ix];
             rgb_WP_lin[1]=params->WP_G_lin[p_ix];
             rgb_WP_lin[2]=params->WP_B_lin[p_ix];

		  double rgb_out_lin[3];

            if((rgb_og_lin[0]!=0) || (rgb_og_lin[1]!=0) || (rgb_og_lin[2]!=0)){ //not black

		  double WPConvXYZ_bb[3];

double rgbXYZ_bb[3];
double rgbXYZGrey_bb_plh[3];

rgb2XYZ(rgb_og_lin,rgbXYZ_bb,rgbXYZGrey_bb_plh,mde,0,1,aprxPw);

if (cust_x_bb!=D65_x || (cust_y_bb!=D65_y)){
    double cust_xy_bb[2]={cust_x_bb,cust_y_bb};
    double cust_XYZ_bb[3];
    xy2XYZ(cust_xy_bb,cust_XYZ_bb);
    WPconv(rgbXYZ_bb,D65XYZ,cust_XYZ_bb,WPConvXYZ_bb);

    if (to_x!=D65_x || (to_y!=D65_y)){
        double dst_xy_bb[2]={to_x,to_y};
        double dst_XYZ_bb[3];
        xy2XYZ(dst_xy_bb,dst_XYZ_bb);
        double WPConvXYZ2_bb[3]={WPConvXYZ_bb[0],WPConvXYZ_bb[1],WPConvXYZ_bb[2]};
        WPconv(WPConvXYZ2_bb,cust_XYZ_bb,dst_XYZ_bb,WPConvXYZ_bb);
    }
        XYZ2rgb(WPConvXYZ_bb,rgb_out_lin,mde,1,aprxPw);

}else{

    if (to_x!=D65_x || (to_y!=D65_y)){
        double dst_xy_bb[2]={to_x,to_y};
        double dst_XYZ_bb[3];
        xy2XYZ(dst_xy_bb,dst_XYZ_bb);
        WPconv(rgbXYZ_bb,D65XYZ,dst_XYZ_bb,WPConvXYZ_bb);
        XYZ2rgb(WPConvXYZ_bb,rgb_out_lin,mde,1,aprxPw);
    }


}
		  }
//BB applied to OG lin colours => rgb_out_lin
//Linear WP colours: rgb_WP_lin
//Lerp between BB and WP based on measurements from WP then BB

        double mn=MIN(rgb_WP_lin[0],MIN(rgb_WP_lin[1],rgb_WP_lin[2]));
		double mx=MAX(rgb_WP_lin[0],MAX(rgb_WP_lin[1],rgb_WP_lin[2]));
		double chr=mx-mn;
		double sat=(mx==0)?0:chr/mx;


		double mn_bb=MIN(rgb_out_lin[0],MIN(rgb_out_lin[1],rgb_out_lin[2]));
		double mx_bb=MAX(rgb_out_lin[0],MAX(rgb_out_lin[1],rgb_out_lin[2]));
		double chr_bb=mx_bb-mn_bb;
		double sat_bb=(mx_bb==0)?0:chr_bb/mx_bb;

		double r=rgb_WP_lin[0];
		double g=rgb_WP_lin[1];
		double b=rgb_WP_lin[2];
		double h=0;

            if(chr!=0){

                if ((r >= g) && (r >= b)) {
                    h=(g - b) / chr;
                } else if ((g >= r) && (g >= b)) {
                   h = (b - r) / chr + 2.0;
                } else {
                    h = (r - g) / chr + 4.0;
                }
                h*=60;
                h=(h < 0) ? h + 360 : h;

            }
            double mxs=MAX(sat_bb,sat);
            double mss=MIN(sat_bb,sat);
            double mcs=MIN(sat,chr);
            double lrp=(MIN(chr,MAX(sat_bb,mcs))*(2-MIN(MAX(0,1-sat-chr),1-sat)))*0.5;
double rgb_WP_lin_adj_hsv[3]={h, MIN(sat,lerp(mss,MIN(1,2*mxs-mss),lrp)) , mx};

hsv2rgb_360(rgb_WP_lin_adj_hsv,rgb_out_lin);

		  if(lnr==0){
			  Apply_gamma(rgb_out_lin,rgb_out,mde,aprxPw);

		  }else{
			  rgb_out[0]=rgb_out_lin[0];
			  rgb_out[1]=rgb_out_lin[1];
			  rgb_out[2]=rgb_out_lin[2];
		  }

		  if(dbg==1){
    double mx=MAX(rgb_out[0],MAX(rgb_out[1],rgb_out[2]));
    double sat=(mx==0)?0:(mx-MIN(rgb_out[0],MIN(rgb_out[1],rgb_out[2])))/mx;

    double dbg_out;
	if(aprxPw==1){
     dbg_out=(amp==1)?sat:fastPrecisePow(sat,amp);
	}else{
		dbg_out=(amp==1)?sat:pow(sat,amp);
	}

    rgb_out[0]=dbg_out;
    rgb_out[1]=dbg_out;
    rgb_out[2]=dbg_out;
}else if(dbg==2 || dbg==8){
    double mx=MAX(rgb_out[0],MAX(rgb_out[1],rgb_out[2]));
    double sat=(mx==0)?0:(mx-MIN(rgb_out[0],MIN(rgb_out[1],rgb_out[2])))/mx;

    rgb_out[0]=(sat>=amp)?rgb_out[0]:((dbg==8)?0.5:0);
    rgb_out[1]=(sat>=amp)?rgb_out[1]:((dbg==8)?0.5:0);
    rgb_out[2]=(sat>=amp)?rgb_out[2]:((dbg==8)?0.5:0);
}else if (dbg==3){
    double mx=MAX(rgb_out[0],MAX(rgb_out[1],rgb_out[2]));
    double sat=(mx==0)?0:(mx-MIN(rgb_out[0],MIN(rgb_out[1],rgb_out[2])))/mx;

    double mxOG=MAX(rOG,MAX(gOG,bOG));
    double satOG=(mxOG==0)?0:(mxOG-MIN(rOG,MIN(gOG,bOG)))/mxOG;
satOG=(amp>=0)?satOG*(1-amp):satOG;
    rgb_out[0]=(sat>=satOG)?rgb_out[0]:0;
    rgb_out[1]=(sat>=satOG)?rgb_out[1]:0;
    rgb_out[2]=(sat>=satOG)?rgb_out[2]:0;
}else if (dbg==4){
    double mx=MAX(rgb_out[0],MAX(rgb_out[1],rgb_out[2]));
    double sat=(mx==0)?0:(mx-MIN(rgb_out[0],MIN(rgb_out[1],rgb_out[2])))/mx;

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
    hsv2rgb_360(hsv_dbg,rgb_out);

}else if (dbg==5){
double x_shift=(double)(x)/(double)(row_size);

if(fv_swt==1){
    rgb_out[0]=(x_shift<0.5)?rOG:bOG;
    rgb_out[1]=(x_shift<0.5)?rOG:bOG;
    rgb_out[2]=(x_shift<0.5)?rOG:bOG;
}else if(fv_swt==2){
    rgb_out[0]=rOG;
    rgb_out[1]=gOG;
    rgb_out[2]=bOG;

}

}

int wp_b=MAX(MIN(round(rgb_out[2]*255),255),0);
int wp_g=MAX(MIN(round(rgb_out[1]*255),255),0);
int wp_r=MAX(MIN(round(rgb_out[0]*255),255),0);

srcp[x] =wp_b; //blue : blue
srcp[x+1] =(sxf==1)?wp_b:wp_g; // blue : green
srcp[x+2] =(sxf==1)? wp_g:wp_r; // green: red
srcp[x+3] =(sxf==1)? wp_g:srcp[x+3]; //green : self
srcp[x+4] =(sxf==1)? wp_r:srcp[x+4]; //red : self
srcp[x+5] =(sxf==1)? wp_r:srcp[x+5]; //red : self
		  x=(sxf==1)?x+7:x+3;
		  p_ix++;
}
	  srcp += src_pitch;
}


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
               if(params->debug==2 || params->debug==4 || params->debug==6 || params->debug==8){
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

char* bbs ="";
bbs = ((avs_as_string(avs_array_elt(args, 22)))&&(avs_as_string(avs_array_elt(args, 22))!="NULL"))?avs_as_string(avs_array_elt(args, 22)):bbs;
params->bb = bbs;

char* bbs2 ="";
bbs2 = ((avs_as_string(avs_array_elt(args, 23)))&&(avs_as_string(avs_array_elt(args, 23))!="NULL"))?avs_as_string(avs_array_elt(args, 23)):edts2;
params->bb2 = bbs2;

params->bb_off=  avs_defined(avs_array_elt(args,24))?avs_as_int(avs_array_elt(args, 24)):0;
params->bb_base=  avs_defined(avs_array_elt(args,25))?avs_as_int(avs_array_elt(args, 25)):-1;

    if (params->ed_base<-1) {
    return avs_new_value_error ("'ed_base' must be >= -1");
    }else if (params->bb_base<-1) {
    return avs_new_value_error ("'bb_base' must be >= -1");
    }else{
      if ((params->debug<0)||(params->debug>8)){
            return avs_new_value_error ("Allowed debug settings are between 0 and 8!");
            return avs_new_value_error ("Allowed debug settings are between 0 and 8!");
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

  if((bbs!="")&&(bbs!="NULL")){

    params->split_bb=(char*)malloc(sizeof(char*)*strlen(bbs));
    char *dup = strdup(bbs);
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

        params->split_bb[tkn]=token;
        tkn++;
    }
    params->bb_lim=tkn;
    int no_clips=tkn;


            params->bb_Red=(int*)malloc(sizeof(int)*no_clips);
        params->bb_Green=(int*)malloc(sizeof(int)*no_clips);
        params->bb_Blue=(int*)malloc(sizeof(int)*no_clips);

			   params->bb_Red_dbl=(double*)malloc(sizeof(double)*no_clips);
      params->bb_Green_dbl=(double*)malloc(sizeof(double)*no_clips);
       params->bb_Blue_dbl=(double*)malloc(sizeof(double)*no_clips);

        params->bb_x=(double*)malloc(sizeof(double)*no_clips);
        params->bb_y=(double*)malloc(sizeof(double)*no_clips);

        params->bb_start_fr=(int*)malloc(sizeof(int)*no_clips);
        params->bb_end_fr=(int*)malloc(sizeof(int)*no_clips);
        params->bb_switch=(int*)malloc(sizeof(int)*no_clips);
        params->bb_lookup=(int*)malloc(sizeof(int)*vi->num_frames);

        memset(params->bb_lookup,-1,sizeof(params->bb_lookup[0])*vi->num_frames); //initialise to -1


        tkn-=1;

    while (tkn>=0){
     params->split_bb[tkn]=strtok(strdup(params->split_bb[tkn]),"}");
        tkn--;
    }

    tkn=0;
    int cnt=0;

    while (tkn<no_clips){
    while ((token = strtok_r(params->split_bb[tkn], ",", &params->split_bb[tkn]))){
int intg;
switch(cnt){
        case 0:
        intg=atoi(token);
        params->bb_Red[tkn]=intg;
        params->bb_Red_dbl[tkn]=(params->sixtyFour==true)?((double)(intg))*rcptHiBit:((double)(intg))*rcptwoFiveFive;
        break;

    case 1:
    intg=atoi(token);
    params->bb_Green[tkn]=intg;
	params->bb_Green_dbl[tkn]=(params->sixtyFour==true)?((double)(intg))*rcptHiBit:((double)(intg))*rcptwoFiveFive;
    break;

        case 2:
        intg=atoi(token);
        params->bb_Blue[tkn]=intg;
		params->bb_Blue_dbl[tkn]=(params->sixtyFour==true)?((double)(intg))*rcptHiBit:((double)(intg))*rcptwoFiveFive;
        break;

    case 3:
    intg=atoi(token);
    intg=(intg<0)?0:intg;
    params->bb_start_fr[tkn]=intg;
    break;

        case 4:
        intg=atoi(token);
        intg=(intg>(vi->num_frames)-1)?0:intg;
        params->bb_end_fr[tkn]=intg;
        break;

    case 5:
    params->bb_x[tkn]=atof(token);
    break;

        case 6:
        params->bb_y[tkn]=atof(token);
        break;

    default:
    ;


}
 cnt=(cnt==6)?0:cnt+1;
    }


    if((params->bb_Red[tkn]<0)||(params->bb_Green[tkn]<0)||(params->bb_Blue[tkn]<0)){
            params->bb_switch[tkn]=1;
    }else{

        if(params->sixtyFour==false){
            if((params->bb_Red[tkn]>255)||(params->bb_Green[tkn]>255)||(params->bb_Blue[tkn]>255)){
                params->bb_switch[tkn]=1;
            }else{
                params->bb_switch[tkn]=0;
            }
        }else{
            if((params->bb_Red[tkn]>65535)||(params->bb_Green[tkn]>65535)||(params->bb_Blue[tkn]>65535)){
            params->bb_switch[tkn]=1;
            }else{
            params->bb_switch[tkn]=0;
            }
        }

    }

    int bb_frm=params->bb_end_fr[tkn];

    if(bb_frm==0){
        bb_frm=(params->bb_start_fr[tkn]==0)?0:(vi->num_frames)-1;
    }

    for(int k=params->bb_start_fr[tkn]; k<=bb_frm; k++){
        params->bb_lookup[k]=tkn;
    }

      tkn++;
    }

free(dup);

  }

   if((bbs2!="")&&(bbs2!="NULL")){

    params->split_bb2=(char*)malloc(sizeof(char*)*strlen(bbs2));
    char *dup = strdup(bbs2);
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

        params->split_bb2[tkn]=token;
        tkn++;
    }
    params->bb_lim2=tkn;
    int no_clips=tkn;


        params->bb_Red2=(int*)malloc(sizeof(int)*no_clips);
      params->bb_Green2=(int*)malloc(sizeof(int)*no_clips);
       params->bb_Blue2=(int*)malloc(sizeof(int)*no_clips);

	   params->bb_Red2_dbl=(double*)malloc(sizeof(double)*no_clips);
      params->bb_Green2_dbl=(double*)malloc(sizeof(double)*no_clips);
       params->bb_Blue2_dbl=(double*)malloc(sizeof(double)*no_clips);

        params->bb_x2=(double*)malloc(sizeof(double)*no_clips);
        params->bb_y2=(double*)malloc(sizeof(double)*no_clips);

      params->bb_start_fr2=(int*)malloc(sizeof(int)*no_clips);
        params->bb_end_fr2=(int*)malloc(sizeof(int)*no_clips);
        params->bb_switch2=(int*)malloc(sizeof(int)*no_clips);
        params->bb_lookup2=(int*)malloc(sizeof(int)*vi->num_frames);

         memset(params->bb_lookup2,-1,sizeof(params->bb_lookup2[0])*vi->num_frames); //initialise to -1

        tkn-=1;

    while (tkn>=0){
     params->split_bb2[tkn]=strtok(strdup(params->split_bb2[tkn]),"}");
        tkn--;
    }

    tkn=0;
    int cnt=0;

    while (tkn<no_clips){
    while ((token = strtok_r(params->split_bb2[tkn], ",", &params->split_bb2[tkn]))){
int intg;
switch(cnt){
        case 0:
        intg=atoi(token);
        params->bb_Red2[tkn]=intg;
        params->bb_Red2_dbl[tkn]=(params->sixtyFour==true)?((double)(intg))*rcptHiBit:((double)(intg))*rcptwoFiveFive;
        break;

    case 1:
    intg=atoi(token);
    params->bb_Green2[tkn]=intg;
	params->bb_Green2_dbl[tkn]=(params->sixtyFour==true)?((double)(intg))*rcptHiBit:((double)(intg))*rcptwoFiveFive;
    break;

        case 2:
        intg=atoi(token);
        params->bb_Blue2[tkn]=intg;
        params->bb_Blue2_dbl[tkn]=(params->sixtyFour==true)?((double)(intg))*rcptHiBit:((double)(intg))*rcptwoFiveFive;
        break;

    case 3:
    intg=atoi(token);
    intg=(intg<0)?0:intg;
    params->bb_start_fr2[tkn]=intg;
    break;

        case 4:
        intg=atoi(token);
        intg=(intg>(vi->num_frames)-1)?0:intg;
        params->bb_end_fr2[tkn]=intg;
        break;

    case 5:
    params->bb_x2[tkn]=atof(token);
    break;

        case 6:
        params->bb_y2[tkn]=atof(token);
        break;

    default:
    ;


}
 cnt=(cnt==6)?0:cnt+1;
    }


    if((params->bb_Red2[tkn]<0)||(params->bb_Green2[tkn]<0)||(params->bb_Blue2[tkn]<0)){
            params->bb_switch2[tkn]=1;
    }else{

        if(params->sixtyFour==false){
            if((params->bb_Red2[tkn]>255)||(params->bb_Green2[tkn]>255)||(params->bb_Blue2[tkn]>255)){
                params->bb_switch2[tkn]=1;
            }else{
                params->bb_switch2[tkn]=0;
            }
        }else{
            if((params->bb_Red2[tkn]>65535)||(params->bb_Green2[tkn]>65535)||(params->bb_Blue2[tkn]>65535)){
            params->bb_switch2[tkn]=1;
            }else{
            params->bb_switch2[tkn]=0;
            }
        }

    }

    int bb_frm2=params->bb_end_fr2[tkn];

    if(bb_frm2==0){
        bb_frm2=(params->bb_start_fr2[tkn]==0)?0:(vi->num_frames)-1;
    }

    for(int k=params->bb_start_fr2[tkn]; k<=bb_frm2; k++){
    params->bb_lookup2[k]=tkn;
    }
      tkn++;
    }
free(dup);
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

	   params->ed_Red2_dbl=(double*)malloc(sizeof(double)*no_clips);
      params->ed_Green2_dbl=(double*)malloc(sizeof(double)*no_clips);
       params->ed_Blue2_dbl=(double*)malloc(sizeof(double)*no_clips);

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
        params->ed_Red2_dbl[tkn]=(params->sixtyFour==true)?((double)(intg))*rcptHiBit:((double)(intg))*rcptwoFiveFive;
        break;

    case 1:
    intg=atoi(token);
    params->ed_Green2[tkn]=intg;
	params->ed_Green2_dbl[tkn]=(params->sixtyFour==true)?((double)(intg))*rcptHiBit:((double)(intg))*rcptwoFiveFive;
    break;

        case 2:
        intg=atoi(token);
        params->ed_Blue2[tkn]=intg;
        params->ed_Blue2_dbl[tkn]=(params->sixtyFour==true)?((double)(intg))*rcptHiBit:((double)(intg))*rcptwoFiveFive;
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
free(dup);
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

			   params->ed_Red_dbl=(double*)malloc(sizeof(double)*no_clips);
      params->ed_Green_dbl=(double*)malloc(sizeof(double)*no_clips);
       params->ed_Blue_dbl=(double*)malloc(sizeof(double)*no_clips);

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
        params->ed_Red_dbl[tkn]=(params->sixtyFour==true)?((double)(intg))*rcptHiBit:((double)(intg))*rcptwoFiveFive;
        break;

    case 1:
    intg=atoi(token);
    params->ed_Green[tkn]=intg;
	params->ed_Green_dbl[tkn]=(params->sixtyFour==true)?((double)(intg))*rcptHiBit:((double)(intg))*rcptwoFiveFive;
    break;

        case 2:
        intg=atoi(token);
        params->ed_Blue[tkn]=intg;
		params->ed_Blue_dbl[tkn]=(params->sixtyFour==true)?((double)(intg))*rcptHiBit:((double)(intg))*rcptwoFiveFive;
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

free(dup);

  //Write int to error

/*char *str1 = malloc(MAX_PATH);
sprintf(str1,"%d: %d %d %d - %d %d",bb_curr_clip,params->bb_Red[bb_curr_clip],params->bb_Green[bb_curr_clip],params->bb_Blue[bb_curr_clip],params->bb_switch [bb_curr_clip], params->bb_lookup2[bse]);
return avs_new_value_error (str1);
*/

  }

if((params->auto_WP==1)&&((params->edits=="")||(params->edits=="NULL"))&&((params->edits2=="")||(params->edits2=="NULL"))){
params->hueCount=(double*)malloc(sizeof(double)*361);
params->hueCount_sat=(double*)malloc(sizeof(double)*361);
params->hueCount_val=(double*)malloc(sizeof(double)*361);
params->hueCount_prp=(double*)malloc(sizeof(double)*361);
params->hueCount_wht=(double*)malloc(sizeof(double)*361);
params->hueCount_wht_prp=(double*)malloc(sizeof(double)*361);
  }
   params->pxls=fi->vi.width*fi->vi.height;

	   params->bb_R = (double*)malloc( params->pxls* sizeof(double));
	   params->bb_G = (double*)malloc( params->pxls* sizeof(double));
	   params->bb_B = (double*)malloc( params->pxls* sizeof(double));

	   params->WP_R_lin = (double*)malloc( params->pxls* sizeof(double));
	   params->WP_G_lin = (double*)malloc( params->pxls* sizeof(double));
	   params->WP_B_lin = (double*)malloc( params->pxls* sizeof(double));

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
   avs_add_function(env, "Manual_WP", "c[x]f[y]f[R]i[G]i[B]i[mode]i[debug]i[debug_val]f[sixtyFour]b[dst_x]f[dst_y]f[auto_WP]b[file]s[log_id]s[overwrite]b[linear]b[edits]s[ed_off]i[ed_base]i[edits2]s[approxPow]b[bb]s[bb2]s[bb_off]i[bb_base]i", create_Manual_WP, 0);
   return "Manual_WP C plugin";
}
