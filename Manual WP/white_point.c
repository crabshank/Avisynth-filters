#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include "../avisynth_c.h"
#include "xyY_funcs.h"
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
        double debug_val2;
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
	   double *WP_Y_lin;
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
     char* bbs;
     char* bbs2;
   double rOG,bOG,gOG,cust_x,cust_y,cust_x_bb,cust_y_bb,amp,amp2, to_x, to_y;
   double rgbs_dbg_six[4]={1.0, 1.0, 1.0, 0.0};

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
amp2=params->debug_val2;
sxf=params->sixtyFour;
to_x=params->dst_x;
to_y=params->dst_y;
ato=params->auto_WP;
nm=params->file;
lid=params->log_id;
lnr=params->linear;
eds=params->edits;
eds2=params->edits2;
bbs=params->bb;
bbs2=params->bb2;
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


if((ato==1)&&((eds=="")||(eds=="NULL"))&&((eds2=="")||(eds2=="NULL"))&&((bbs=="")||(bbs=="NULL"))&&((bbs2=="")||(bbs2=="NULL"))){

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

        double curr_rgb_dst_hsvc[4];
        double curr_rgb_dst[3]={rOG,gOG,bOG};
        double curr_rgb_prp[3];
rgb2hsvc_360(curr_rgb_dst,curr_rgb_dst_hsvc);
int hueEl=round(curr_rgb_dst_hsvc[0]);

double score=(1-curr_rgb_dst_hsvc[1])*curr_rgb_dst_hsvc[2]*curr_rgb_dst_hsvc[2]; //higher, more white

if((intBlue==intGreen)&&(intGreen==intRed)){ //grey
    hueEl=360;
score*=score;
//hueCount_sat[hueEl]+=0;

}else{
    hueEl=(hueEl==360)?0:hueEl;

RGB_prp(curr_rgb_dst,curr_rgb_prp);

score=(1-MAX(curr_rgb_dst_hsvc[1],curr_rgb_dst_hsvc[1]*curr_rgb_dst_hsvc[2]))*(1-(MAX(curr_rgb_prp[0],MAX(curr_rgb_prp[1],curr_rgb_prp[2]))-MIN(curr_rgb_prp[0],MIN(curr_rgb_prp[1],curr_rgb_prp[2]))))*(1-curr_rgb_dst_hsvc[1])*(score); //higher, more white
score*=score;

hueCount_sat[hueEl]+=curr_rgb_dst_hsvc[1];
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

      double xy_ato[2];
       get_xy(rgb_ato, xy_ato , mde,lnr,aprxPw);

        cust_x=xy_ato[0];
        cust_y=xy_ato[1];

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
double WPConvXYZ[3];
double OG_RGB[3]={rOG,gOG,bOG};
double OG_RGB_lin[3]={rOG,gOG,bOG};
double WPchgRGB[3]={rOG,gOG,bOG};

if(lnr==0){
    Linearise(OG_RGB,OG_RGB_lin,mde,aprxPw);
}

LinRGB2XYZ(OG_RGB_lin,rgbXYZ,mde);

if(bb_curr_clip!=-1){
params->bb_R[p_ix]=OG_RGB_lin[0];
params->bb_G[p_ix]=OG_RGB_lin[1];
params->bb_B[p_ix]=OG_RGB_lin[2];

params->WP_R_lin[p_ix]=0;
params->WP_G_lin[p_ix]=0;
params->WP_B_lin[p_ix]=0;
params->WP_Y_lin[p_ix]=0;
}

if(blackOG==1){
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
							double xy_rgb_ed[2];

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

						get_xy(rgb_ed, xy_rgb_ed, mde,lnr,aprxPw);

						cust_x=xy_rgb_ed[0];
						cust_y=xy_rgb_ed[1];
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
							double xy_rgb_ed2[2];

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

						get_xy(rgb_ed2, xy_rgb_ed2 , mde,lnr,aprxPw);

						cust_x=xy_rgb_ed2[0];
						cust_y=xy_rgb_ed2[1];
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

        XYZ2LinRGB(WPConvXYZ,WPchgRGB,mde);

        if(bb_curr_clip!=-1){
            params->WP_R_lin[p_ix]=WPchgRGB[0];
            params->WP_G_lin[p_ix]=WPchgRGB[1];
            params->WP_B_lin[p_ix]=WPchgRGB[2];
            params->WP_Y_lin[p_ix]=WPConvXYZ[1];
        }

        if(lnr==0){
            Apply_gamma(WPchgRGB,WPchgRGB,mde,aprxPw);
        }

}else{

    if (to_x!=D65_x || (to_y!=D65_y)){
        double dst_xy[2]={to_x,to_y};
        double dst_XYZ[3];
        xy2XYZ(dst_xy,dst_XYZ);
        WPconv(rgbXYZ,D65XYZ,dst_XYZ,WPConvXYZ);

        if(lnr==1){
            XYZ2LinRGB(WPConvXYZ,WPchgRGB,mde);
            if(bb_curr_clip!=-1){
                params->WP_R_lin[p_ix]=WPchgRGB[0];
                params->WP_G_lin[p_ix]=WPchgRGB[1];
                params->WP_B_lin[p_ix]=WPchgRGB[2];
                params->WP_Y_lin[p_ix]=WPConvXYZ[1];
            }
        }else{
            XYZ2LinRGB(WPConvXYZ,WPchgRGB,mde);
            if(bb_curr_clip!=-1){
                params->WP_R_lin[p_ix]=WPchgRGB[0];
                params->WP_G_lin[p_ix]=WPchgRGB[1];
                params->WP_B_lin[p_ix]=WPchgRGB[2];
                params->WP_Y_lin[p_ix]=WPConvXYZ[1];
            }
            Apply_gamma(WPchgRGB,WPchgRGB,mde,aprxPw);
        }

    }else if(bb_curr_clip!=-1){
        params->WP_R_lin[p_ix]=OG_RGB_lin[0];
        params->WP_G_lin[p_ix]=OG_RGB_lin[1];
        params->WP_B_lin[p_ix]=OG_RGB_lin[2];
        params->WP_Y_lin[p_ix]=LinRGB2Y(OG_RGB_lin,mde);
    }


}

}

if(dbg==6){
    debug_6(WPchgRGB, WPchgRGB, rgbs_dbg_six, amp, rOG, gOG, bOG);
}

if(bb_curr_clip==-1){

    if(dbg==1){
        debug_1(WPchgRGB, WPchgRGB, amp, aprxPw);
    }else if(dbg==9){
        debug_9(WPchgRGB, WPchgRGB, amp, amp2);
    }else if(dbg==2){
        debug_2(WPchgRGB, WPchgRGB, amp);
    }else if(dbg==8){
         debug_8(WPchgRGB, WPchgRGB, amp);
    }else if (dbg==3){
        debug_3(WPchgRGB, WPchgRGB, amp, rOG, gOG, bOG);
    }else if (dbg==4){
        debug_4(WPchgRGB, rOG, gOG, bOG, WPchgRGB, amp);
    }else if (dbg==5){
        debug_5(WPchgRGB, x, row_size, rOG, gOG, bOG, fv_swt);
    }else if(dbg==7){
        debug_7(WPchgRGB, WPchgRGB, amp);
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
							double xy_rgb_bb[2];

						double rgb_bb[3];
						 rgb_bb[0]=params->bb_Red2_dbl[bb_curr_clip];
						 rgb_bb[1]=params->bb_Green2_dbl[bb_curr_clip];
						 rgb_bb[2]=params->bb_Blue2_dbl[bb_curr_clip];

						get_xy(rgb_bb, xy_rgb_bb , mde,1,aprxPw);

						cust_x_bb=xy_rgb_bb[0];
						cust_y_bb=xy_rgb_bb[1];
				}

}

}else{
    if (params->bb_switch[bb_curr_clip]==1){

				cust_x_bb=params->bb_x[bb_curr_clip];
				cust_y_bb=params->bb_y[bb_curr_clip];

}else{

				if((params->bb_Red[bb_curr_clip]!=0) || (params->bb_Green[bb_curr_clip]!=0) || (params->bb_Blue[bb_curr_clip]!=0)){
							double xy_rgb_bb[2];

						double rgb_bb[3];
						 rgb_bb[0]=params->bb_Red_dbl[bb_curr_clip];
						 rgb_bb[1]=params->bb_Green_dbl[bb_curr_clip];
						 rgb_bb[2]=params->bb_Blue_dbl[bb_curr_clip];

						get_xy(rgb_bb, xy_rgb_bb , mde,1,aprxPw);

						cust_x_bb=xy_rgb_bb[0];
						cust_y_bb=xy_rgb_bb[1];
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

        double rgb_out_lin[3]={0,0,0};

        int rgb_og_lin_blk=((rgb_og_lin[0]!=0) || (rgb_og_lin[1]!=0) || (rgb_og_lin[2]!=0))?0:1;

if(rgb_og_lin_blk==0){ //not black

    double WPConvXYZ_bb[3];

    double rgbXYZ_bb[3];

    LinRGB2XYZ(rgb_og_lin,rgbXYZ_bb,mde);

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
            XYZ2LinRGB(WPConvXYZ_bb,rgb_out_lin,mde);
    }else{

        if (to_x!=D65_x || (to_y!=D65_y)){
            double dst_xy_bb[2]={to_x,to_y};
            double dst_XYZ_bb[3];
            xy2XYZ(dst_xy_bb,dst_XYZ_bb);
            WPconv(rgbXYZ_bb,D65XYZ,dst_XYZ_bb,WPConvXYZ_bb);
            XYZ2LinRGB(WPConvXYZ_bb,rgb_out_lin,mde);
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

            double h=rgb2hue_360(rgb_WP_lin);
            double mxs=MAX(sat_bb,sat);
            double mss=MIN(sat_bb,sat);
            double mcs=MIN(sat,chr);
            double lrp=(MIN(chr,MAX(sat_bb,mcs))*(2-MIN(MAX(0,1-sat-chr),1-sat)))*0.5;
double rgb_WP_lin_adj_hsv[3]={h, MIN(sat,lerp(mss,MIN(1,2*mxs-mss),lrp)) , mx};

hsv2rgb_360(rgb_WP_lin_adj_hsv,rgb_out_lin);

//take xy of rgb_out_lin, and Y of rgb_WP_lin:

double rgb_out_lin_xyY[3];
LinRGB2xyY(rgb_out_lin,rgb_out_lin_xyY,mde);

double out_xyY[3]={rgb_out_lin_xyY[0],rgb_out_lin_xyY[1],params->WP_Y_lin[p_ix]};
xyY2LinRGB(out_xyY,rgb_out_lin,mde);

		  if(lnr==0){
			  Apply_gamma(rgb_out_lin,rgb_out,mde,aprxPw);

		  }else{
			  rgb_out[0]=rgb_out_lin[0];
			  rgb_out[1]=rgb_out_lin[1];
			  rgb_out[2]=rgb_out_lin[2];
		  }

if(dbg==1){
    debug_1(rgb_out,rgb_out,amp,aprxPw);
}else if(dbg==9){
    debug_9(rgb_out, rgb_out, amp, amp2);
}else if(dbg==2){
    debug_2(rgb_out, rgb_out,amp);
}else if(dbg==8){
    debug_8(rgb_out, rgb_out,amp);
}else if (dbg==3){
    debug_3(rgb_out, rgb_out, amp, rOG, gOG, bOG);
}else if (dbg==4){
    debug_4(rgb_out, rOG, gOG, bOG, rgb_out, amp);
}else if (dbg==5){
    debug_5(rgb_out,x,row_size,rOG,gOG,bOG,fv_swt);
}else if(dbg==7){
    debug_7(rgb_out,rgb_out,amp);
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

   int redInt=(sxf==1)?round(rgbs_dbg_six[0]*65535):round(rgbs_dbg_six[0]*255);
   int greenInt=(sxf==1)?round(rgbs_dbg_six[1]*65535):round(rgbs_dbg_six[1]*255);
   int blueInt=(sxf==1)?round(rgbs_dbg_six[2]*65535):round(rgbs_dbg_six[2]*255);

if ((lid!="")&&(lid!="NULL")){
    fprintf(fptr,"%s: %d, %d, %d, %.11f, %d\n", lid,redInt, greenInt, blueInt, rgbs_dbg_six[3],n);
}else{
    fprintf(fptr,"%d, %d, %d, %.11f, %d\n",redInt, greenInt, blueInt, rgbs_dbg_six[3],n);
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
				   if(params->debug==2 || params->debug==4 || params->debug==6 || params->debug==8 || params->debug==9){
					dbg_v=0.03;
				   }else if(params->debug==3){
					dbg_v=0.97;
				   }else if(params->debug==7){
					dbg_v=0.37;
				   }
				params->debug_val=  avs_defined(avs_array_elt(args,8))?avs_as_float(avs_array_elt(args, 8)):dbg_v;
				params->debug_val2=  avs_defined(avs_array_elt(args,9))?avs_as_float(avs_array_elt(args, 9)):((params->debug==9)?0.17:1);

                params->dst_x=  avs_defined(avs_array_elt(args,11))?avs_as_float(avs_array_elt(args, 11)):0.312727;
                params->dst_y=  avs_defined(avs_array_elt(args,12))?avs_as_float(avs_array_elt(args, 12)):0.329023;
                params->auto_WP=  avs_defined(avs_array_elt(args,13))?avs_as_bool(avs_array_elt(args, 13)):false;

				char* file_name ="";
				file_name = ((avs_as_string(avs_array_elt(args, 14)))&&(avs_as_string(avs_array_elt(args, 14))!="NULL"))?avs_as_string(avs_array_elt(args, 14)):file_name;
				params->file = file_name;

				char* log_idt ="";
				log_idt = ((avs_as_string(avs_array_elt(args, 15)))&&(avs_as_string(avs_array_elt(args, 15))!="NULL"))?avs_as_string(avs_array_elt(args, 15)):log_idt;
				params->log_id = log_idt;

                params->overwrite=  avs_defined(avs_array_elt(args,16))?avs_as_bool(avs_array_elt(args, 16)):true;
                params->linear=  avs_defined(avs_array_elt(args,17))?avs_as_bool(avs_array_elt(args, 17)):false;

				char* edts ="";
				edts = ((avs_as_string(avs_array_elt(args, 18)))&&(avs_as_string(avs_array_elt(args, 18))!="NULL"))?avs_as_string(avs_array_elt(args, 18)):edts;
				params->edits = edts;

				params->ed_off=  avs_defined(avs_array_elt(args,19))?avs_as_int(avs_array_elt(args, 19)):0;
				params->ed_base=  avs_defined(avs_array_elt(args,20))?avs_as_int(avs_array_elt(args, 20)):-1;

				char* edts2 ="";
				edts2 = ((avs_as_string(avs_array_elt(args, 21)))&&(avs_as_string(avs_array_elt(args, 21))!="NULL"))?avs_as_string(avs_array_elt(args, 21)):edts2;
				params->edits2 = edts2;

				params->approxPow=  avs_defined(avs_array_elt(args,22))?avs_as_bool(avs_array_elt(args, 22)):true;

				char* bbs ="";
				bbs = ((avs_as_string(avs_array_elt(args, 23)))&&(avs_as_string(avs_array_elt(args, 23))!="NULL"))?avs_as_string(avs_array_elt(args, 23)):bbs;
				params->bb = bbs;

				char* bbs2 ="";
				bbs2 = ((avs_as_string(avs_array_elt(args, 24)))&&(avs_as_string(avs_array_elt(args, 24))!="NULL"))?avs_as_string(avs_array_elt(args, 24)):bbs2;
				params->bb2 = bbs2;

				params->bb_off=  avs_defined(avs_array_elt(args,25))?avs_as_int(avs_array_elt(args, 25)):0;
				params->bb_base=  avs_defined(avs_array_elt(args,26))?avs_as_int(avs_array_elt(args, 26)):-1;

    if (params->ed_base<-1) {
    return avs_new_value_error ("'ed_base' must be >= -1");
    }else if (params->bb_base<-1) {
    return avs_new_value_error ("'bb_base' must be >= -1");
    }else{
      if ((params->debug<0)||(params->debug>9)){
            return avs_new_value_error ("Allowed debug settings are between 0 and 9!");
            return avs_new_value_error ("Allowed debug settings are between 0 and 9!");
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

     if(avs_defined(avs_array_elt(args, 10))){
        params->sixtyFour =avs_as_bool(avs_array_elt(args, 10));
     }else{
       params->sixtyFour = (avs_is_rgb64(&fi->vi))?true:false;
     }

 if((params->R>=0)&&(params->G>=0)&&((params->B>=0))){
      if((params->sixtyFour==true)&&((params->R>65535)||(params->G>65535)||(params->B>65535))){
        return avs_new_value_error ("RGB arguments must be between 0 and 65535");
      }else if((params->sixtyFour==false)&&((params->R>255)||(params->G>255)||(params->B>255))){
        return avs_new_value_error ("RGB arguments must be between 0 and 255");
      }else if ((params->auto_WP==false)&&((params->edits=="")||(params->edits=="NULL"))){

        double xy_rgb[2];

        double rgb[3];

        rgb[0]=(params->sixtyFour==true)?(double)(params->R)*rcptHiBit:(double)(params->R)*rcptwoFiveFive;
        rgb[1]=(params->sixtyFour==true)?(double)(params->G)*rcptHiBit:(double)(params->G)*rcptwoFiveFive;
        rgb[2]=(params->sixtyFour==true)?(double)(params->B)*rcptHiBit:(double)(params->B)*rcptwoFiveFive;

        int linar=(params->linear==true)?1:0;
        int aprxPw=(params->approxPow==true)?1:0;

        get_xy(rgb, xy_rgb , params->mode,linar,aprxPw);

        params->x=xy_rgb[0];
        params->y=xy_rgb[1];

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
sprintf(str1,"%d",params->bb_Red[0]);
return avs_new_value_error (str1);
*/

  }

if((params->auto_WP==1)&&((params->edits=="")||(params->edits=="NULL"))&&((params->edits2=="")||(params->edits2=="NULL"))&&((params->bb=="")||(params->bb=="NULL"))&&((params->bb2=="")||(params->bb2=="NULL"))){
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
	   params->WP_Y_lin = (double*)malloc( params->pxls* sizeof(double));

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
   avs_add_function(env, "Manual_WP", "c[x]f[y]f[R]i[G]i[B]i[mode]i[debug]i[debug_val]f[debug_val2]f[sixtyFour]b[dst_x]f[dst_y]f[auto_WP]b[file]s[log_id]s[overwrite]b[linear]b[edits]s[ed_off]i[ed_base]i[edits2]s[approxPow]b[bb]s[bb2]s[bb_off]i[bb_base]i", create_Manual_WP, 0);
   return "Manual_WP C plugin";
}
