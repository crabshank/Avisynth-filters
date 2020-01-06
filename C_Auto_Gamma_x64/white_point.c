#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../avisynth_c.h"
#include "functions_c.h"

typedef struct WhitePoint {
    int debug;
        double start;
        double x;
        double y;
       double tint;
      //  int precision;
} WhitePoint;


AVS_VideoFrame* AVSC_CC WhitePoint_get_frame(AVS_FilterInfo* fi, int n)
{
 WhitePoint* params = (WhitePoint*) fi->user_data;

   AVS_VideoFrame* src = avs_get_frame(fi->child, n);

   int row_size, height, src_pitch,x, y, p,dbg;
   BYTE* srcp;
   double CIEx,CIEy,rOG,bOG,gOG,strt,cust_x,cust_y;

dbg=params->debug;
strt=params->start;
cust_x=params->x;
cust_y=params->y;
double tntp=params->tint;
//int prc=params->precision;

   CIEx= 0.312727;
      CIEy= 0.329023;
      double D65XYZ[3]={0.95047,1,1.08883};
            int planes[] ={AVS_CS_BGR32};
src = avs_get_frame(fi->child, n);
   avs_make_writable(fi->env, &src);

p=0;
      srcp = avs_get_write_ptr_p(src, planes[p]);
      src_pitch = avs_get_pitch_p(src, planes[p]);
      row_size = avs_get_row_size_p(src, planes[p]);
      height = avs_get_height_p(src, planes[p]);

/*
double r_avg_sum_gm= 0;
double g_avg_sum_gm= 0;
double b_avg_sum_gm= 0;
*/
double counterAll=0;

//double counter=0;
//double counterSc=0;

double sumR=0;
double sumG=0;
double sumB=0;

double sumR_=0;
double sumG_=0;
double sumB_=0;

/*
double sumMin=0;
double sumMax=0;*/
double sumChroma=0;

double rcp_twoFiveFive=pow(255,-1);


///////////////ACTUALLY DRAW PIXELS///////////////////////////////////////

      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

//double x_shift=(double)x/(double)row_size;
                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcp_twoFiveFive;     // B
       gOG=currGreen*rcp_twoFiveFive;   //G
         rOG=currRed*rcp_twoFiveFive;     // R

double curr_rgb_dst[3]={rOG,gOG,bOG};

double curr_rgb_dst_lin[3];
sRGB2Linear(curr_rgb_dst,curr_rgb_dst_lin);

double curr_rgb_dst_lin_prp[3];
//double curr_rgb_dst_lin_wht[3];
//double curr_rgb_dst_lin_hsv[3];
RGB2rgb(curr_rgb_dst_lin,curr_rgb_dst_lin_prp);
/*
rgb2hsv(curr_rgb_dst_lin,curr_rgb_dst_lin_hsv);
double curr_rgb_dst_hsi[3];
RGB2HSI(curr_rgb_dst_lin,curr_rgb_dst_hsi);
*/
/*
rgb2RGB_White(curr_rgb_dst_lin_prp,curr_rgb_dst_lin_wht);
*/


/*




curr_rgb_dst_hwb[2]=1;

double curr_rgb_dst_hwb_hsv[3];
hwb2hsv(curr_rgb_dst_hwb,curr_rgb_dst_hwb_hsv);

double curr_rgb_dst_hwb_rgb[3];
hsv2rgb(curr_rgb_dst_hwb_hsv,curr_rgb_dst_hwb_rgb);
*/
double Sc=(MAX(curr_rgb_dst_lin_prp[0],MAX(curr_rgb_dst_lin_prp[1],curr_rgb_dst_lin_prp[2]))-MIN(curr_rgb_dst_lin_prp[0],MIN(curr_rgb_dst_lin_prp[1],curr_rgb_dst_lin_prp[2])));
//double satSc=curr_rgb_dst_hsv[1];


double rootSat=sqrt(  pow(curr_rgb_dst_lin[0]-curr_rgb_dst_lin[1],2) + pow(curr_rgb_dst_lin[0]-curr_rgb_dst_lin[2],2)  +pow(curr_rgb_dst_lin[1]-curr_rgb_dst_lin[2],2))/sqrt(3);
 // Sc=(1-curr_rgb_dst_hsv[1])*(curr_rgb_dst_hsi[1])*(1-0.5*(Sc+(1-curr_rgb_dst_hwb[1])))*2;
//double min=MIN(fabs(curr_rgb_dst_lin[0]-curr_rgb_dst_lin[1]),MIN(fabs(curr_rgb_dst_lin[0]-curr_rgb_dst_lin[2]), fabs(curr_rgb_dst_lin[1]-curr_rgb_dst_lin[2])  ));
 Sc=MIN(MAX((Sc*rootSat),0),1);
if (1-Sc<=strt){
/*
double curr_rgb_dst_hsv[3];
rgb2hsv(curr_rgb_dst_lin,curr_rgb_dst_hsv);
double curr_rgb_dst_hwb[3];
hsv2hwb(curr_rgb_dst_hsv,curr_rgb_dst_hwb);





curr_rgb_dst_hwb[1]=0;
curr_rgb_dst_hwb[2]= MIN(MAX(curr_rgb_dst_hwb[2]+tntp,0),1); ;

double curr_rgb_dst_adj_hsv[3];
double curr_rgb_dst_adj_rgb_lin[3];
hwb2hsv(curr_rgb_dst_hwb,curr_rgb_dst_adj_hsv);
hsv2rgb(curr_rgb_dst_adj_hsv,curr_rgb_dst_adj_rgb_lin);
*/
//if (curr_rgb_dst_adj_rgb_lin[0]+curr_rgb_dst_adj_rgb_lin[1]+curr_rgb_dst_adj_rgb_lin[2]>0){
    sumR+=(Sc==0)?1:MIN(MAX(pow(curr_rgb_dst_lin[0],Sc),0),1);
        sumG+=(Sc==0)?1:MIN(MAX(pow(curr_rgb_dst_lin[1],Sc),0),1);
        sumB+=(Sc==0)?1:MIN(MAX(pow(curr_rgb_dst_lin[2],Sc),0),1);
        //sumChroma+= rootSat;
        counterAll+=1;
//}


/*
        sumR_+=rs_rgb_lin_bk[0];
        sumG_+=rs_rgb_lin_bk[1];
        sumB_+=rs_rgb_lin_bk[2];

*/


       /* double min=MIN(curr_rgb_dst_lin[0],MIN(curr_rgb_dst_lin[1],curr_rgb_dst_lin[2]))*Sc;
sumMax+=max;
sumMin+=min;
*/

//if(curr_rgb_dst_hwb_rgb[0]+curr_rgb_dst_hwb_rgb[1]+curr_rgb_dst_hwb_rgb[2]>0){

/*
     r_avg_sum_gm+= pow(curr_rgb_dst_lin_prp[0],Sc);
     g_avg_sum_gm+= pow(curr_rgb_dst_lin_prp[1],Sc);
     b_avg_sum_gm+= pow(curr_rgb_dst_lin_prp[2],Sc);

counter+=1;*/
//counterSc+=Sc;
/*}else{
    curr_rgb_dst_hwb_rgb[0]=0;
    curr_rgb_dst_hwb_rgb[1]=0;
    curr_rgb_dst_hwb_rgb[2]=0;
}*/
      }


x+=3;

}

 // srcp += src_pitch;

      }

      //double rcp_counter=pow(counter,-1);
      double rcp_counterAll=(counterAll==0)?1:pow(counterAll,-1);
    //  double rcp_counterSc=pow(sumChroma,-1);

    double avg_rgb[3];
    //double avg_rgb_[3];
   // double avg_rgb_hsv[3];
  /*   double avg_rgb1[3];
      double avg_rgb2[3];
     double avg_rgb3[3];
      double avg_rgb4[3];
      double avg_rgb5[3];
      double avg_rgb6[3];*/
   //   double avg_chroma=sumChroma*rcp_counterAll;
      //double avg_sat=(sumMax==0)?0:((sumMax-sumMin)/(sumMax))*rcp_counterAll;


avg_rgb[0]=sumR*rcp_counterAll;
avg_rgb[1]=sumG*rcp_counterAll;
avg_rgb[2]=sumB*rcp_counterAll;
/*
double avg_rgb_prp[3];
RGB2rgb(avg_rgb,avg_rgb_prp);

double avg_rgb_wht[3];
double avg_rgb_prp_hsv[3];
rgb2RGB_White(avg_rgb_prp   ,avg_rgb_wht);
*/double avg_rgb_hsv[3];
rgb2hsv(avg_rgb,avg_rgb_hsv);


double h_a=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (60*pow(360,-1))  ), 1) - 0.5);
double h_b=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (120*pow(360,-1))  ), 1) - 0.5);
double h_c=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (180*pow(360,-1))  ), 1) - 0.5);
double h_d=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (240*pow(360,-1))  ), 1) - 0.5);
double h_e=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (300*pow(360,-1))  ), 1) - 0.5);
double h_f=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (1)  ), 1) - 0.5);


double hd_min=MIN(h_a,MIN(h_b,MIN(h_c,MIN(h_d,MIN(h_e,MIN(h_f,avg_rgb_hsv[0]))))));

if (hd_min=h_a){
    avg_rgb_hsv[0]=60*pow(360,-1);
}else if (hd_min=h_b){
    avg_rgb_hsv[0]=120*pow(360,-1);
}else if (hd_min=h_c){
    avg_rgb_hsv[0]=180*pow(360,-1);
}else if (hd_min=h_d){
    avg_rgb_hsv[0]=240*pow(360,-1);
}else if (hd_min=h_e){
    avg_rgb_hsv[0]=300*pow(360,-1);
}else{
    avg_rgb_hsv[0]=0;
}

/*
double h_a=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (40*pow(360,-1) )), 1) - 0.5);
double h_b=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (60*pow(360,-1)  )), 1) - 0.5);
double h_c=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (80*pow(360,-1)  )), 1) - 0.5);
double h_d=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (120*pow(360,-1)  )), 1) - 0.5);
double h_e=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (160*pow(360,-1)  )), 1) - 0.5);
double h_f=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (180*pow(360,-1  ))), 1) - 0.5);
double h_g=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (200*pow(360,-1)  )), 1) - 0.5);
double h_h=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (240*pow(360,-1)  )), 1) - 0.5);
double h_i=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (280*pow(360,-1)  )), 1) - 0.5);
double h_j=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (300*pow(360,-1)  )), 1) - 0.5);
double h_k=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (320*pow(360,-1  ))), 1) - 0.5);
double h_l=0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - (1)  ), 1) - 0.5);


double hd_min=MIN(h_a,MIN(h_b,MIN(h_c,MIN(h_d,MIN(h_e,MIN(h_f,MIN(h_g,MIN(h_h,MIN(h_i,MIN(h_j,MIN(h_k,h_l)))))))))));

if (hd_min=h_a){
    avg_rgb_hsv[0]=40*pow(360,-1);
}else if (hd_min=h_b){
    avg_rgb_hsv[0]=60*pow(360,-1);
}else if (hd_min=h_c){
    avg_rgb_hsv[0]=80*pow(360,-1);
}else if (hd_min=h_d){
    avg_rgb_hsv[0]=120*pow(360,-1);
}else if (hd_min=h_e){
    avg_rgb_hsv[0]=160*pow(360,-1);
}else if (hd_min=h_f){
    avg_rgb_hsv[0]=180*pow(360,-1);
}else if (hd_min=h_g){
    avg_rgb_hsv[0]=200*pow(360,-1);
}else if (hd_min=h_h){
    avg_rgb_hsv[0]=240*pow(360,-1);
}else if (hd_min=h_i){
    avg_rgb_hsv[0]=280*pow(360,-1);
}else if (hd_min=h_j){
    avg_rgb_hsv[0]=300*pow(360,-1);
}else if (hd_min=h_k){
    avg_rgb_hsv[0]=320*pow(360,-1);
}else{
    avg_rgb_hsv[0]=0;
}

*/

double avg_rgb_bk[3];
double avg_rgb_prp_bk[3];
double avg_rgb_wht_bk[3];
double avg_rgb_prp_bk2[3];
double avg_rgb_prp_bk3[3];
double avg_rgb_wht_bk2[3];
double avg_rgb_wht_bk3[3];


hsv2rgb(avg_rgb_hsv,avg_rgb_bk );


RGB2rgb(avg_rgb_bk,avg_rgb_prp_bk);
rgb2RGB_White(avg_rgb_prp_bk,avg_rgb_wht_bk);

avg_rgb_prp_bk2[0]=(avg_rgb_prp_bk[0]>pow(3,-1))?1-2*avg_rgb_prp_bk[0]:0.5*(avg_rgb_prp_bk[0]-pow(3,-1));
avg_rgb_prp_bk2[1]=(avg_rgb_prp_bk[1]>pow(3,-1))?1-2*avg_rgb_prp_bk[1]:0.5*(avg_rgb_prp_bk[1]-pow(3,-1));
avg_rgb_prp_bk2[2]=(avg_rgb_prp_bk[2]>pow(3,-1))?1-2*avg_rgb_prp_bk[2]:0.5*(avg_rgb_prp_bk[2]-pow(3,-1));

avg_rgb_wht_bk3[0]=1-avg_rgb_prp_bk2[0];
avg_rgb_wht_bk3[1]=1-avg_rgb_prp_bk2[1];
avg_rgb_wht_bk3[2]=1-avg_rgb_prp_bk2[2];

rgb2RGB_White(avg_rgb_prp_bk3,avg_rgb_wht_bk2);


/*
avg_rgb_[0]=sumR_*rcp_counterAll;
avg_rgb_[1]=sumG_*rcp_counterAll;
avg_rgb_[2]=sumB_*rcp_counterAll;
*/
/*

   double avg_rgb_lin_hsv[3];
   double avg_rgb_lin_hwb[3];
   double avg_rgb_lin_hsv_bk[3];
   double avg_rgb_lin_bk[3];



   rgb2hsv(avg_rgb_prp_wht,avg_rgb_lin_hsv);
   avg_rgb_lin_hsv[1]=0;
   */
   /*
double avg_rgb_lin_hsv_dst[3]={avg_rgb_lin_hsv[0],MIN(MAX(avg_rgb_lin_hsv[1]-avg_chroma   ,0),1),avg_rgb_lin_hsv[2]};
   hsv2rgb(avg_rgb_lin_hsv_dst,avg_rgb_lin_bk);*/
/*
hsv2hwb(avg_rgb_lin_hsv,avg_rgb_lin_hwb);

avg_rgb_lin_hwb[1]=MIN(MAX(avg_rgb_lin_hwb[1]+avg_chroma   ,0),1);
//avg_rgb_lin_hwb[2]=MIN(MAX(avg_rgb_lin_hwb[2]-avg_chroma   ,0),1);
hwb2hsv(avg_rgb_lin_hwb,avg_rgb_lin_hsv_bk);

hsv2rgb(avg_rgb_lin_hsv,avg_rgb_lin_bk);
double avg_rgb_lin_bk_prp[3];
RGB2rgb(avg_rgb_lin_bk,avg_rgb_lin_bk_prp);
double avg_rgb_lin_bk_wht[3];
rgb2RGB_White(avg_rgb_lin_bk_prp   ,avg_rgb_lin_bk_wht);
*/
/*avg_rgb1[0]=r_avg_sum_gm*rcp_counter;
avg_rgb1[1]=g_avg_sum_gm*rcp_counter;
avg_rgb1[2]=b_avg_sum_gm*rcp_counter;
double avg_rgb1_prp_wht[3];*/
/*
double avg_rgb1_gc[3];
Linear2sRGB(avg_rgb1,avg_rgb1_gc);
double avg_rgb1_prp[3];
RGB2rgb(avg_rgb1,avg_rgb1_prp);

rgb2RGB_White(avg_rgb1_prp,avg_rgb1_prp_wht);*/

/*
double avg_rgb3_inv[3]={1-avg_rgb1_prp_wht[0],1-avg_rgb1_prp_wht[1],1-avg_rgb1_prp_wht[2]};
double avg_rgb3_prp[3];
RGB2rgb(avg_rgb3_inv,avg_rgb3_prp);
double avg_rgb3_prp_wht[3];
rgb2RGB_White(avg_rgb3_prp,avg_rgb3_prp_wht);
rgb2RGB_White(avg_rgb3_prp,avg_rgb3_prp_wht);
*/
/*


double avg_rgb_prp_[3];
RGB2rgb(avg_rgb_,avg_rgb_prp_);
double avg_rgb_prp_wht_[3];
rgb2RGB_White(avg_rgb_prp_,avg_rgb_prp_wht_);

*/


/*
double rgb_corr_plm[3];
double rgb_corr_plm_prp[3];
double rgb_corr_plm_wht[3];
double rgb_corr_plm_hsv[3];
double rgb_corr_plm_inv_wht[3];
double rgb_corr_plm_inv[3];
double rgb_corr_plm_inv_prp[3];
double rgb_corr_plm_hsv_inv[3];
double rgb_corr[3];
double min_corr=MIN(avg_rgb[0],MIN(avg_rgb[1],avg_rgb[2]));
double max_corr=MAX(avg_rgb[0],MAX(avg_rgb[1],avg_rgb[2]));
double cfn=(min_corr==0)?0:lerp(-0.44510405283723776*max_corr+max_corr,min_corr,avg_chroma);
//double cfn_inv=(min_corr==0)?0:1-cfn;
double diff=max_corr-cfn;

double lrp0=(diff==0)?1:((avg_rgb[0]-cfn)/diff);
double lrp1=(diff==0)?1:((avg_rgb[1]-cfn)/diff);
double lrp2=(diff==0)?1:((avg_rgb[2]-cfn)/diff);


rgb_corr_plm[0]=lerp(cfn,max_corr,lrp0);
rgb_corr_plm[1]=lerp(cfn,max_corr,lrp1);
rgb_corr_plm[2]=lerp(cfn,max_corr,lrp2);
RGB2rgb(rgb_corr_plm,rgb_corr_plm_prp);
rgb2RGB_White(rgb_corr_plm_prp,rgb_corr_plm_wht);

rgb_corr_plm_inv[0]=1-rgb_corr_plm[0];
rgb_corr_plm_inv[1]=1-rgb_corr_plm[1];
rgb_corr_plm_inv[2]=1-rgb_corr_plm[2];
RGB2rgb(rgb_corr_plm_inv,rgb_corr_plm_inv_prp);
rgb2RGB_White(rgb_corr_plm_inv_prp,rgb_corr_plm_inv_wht);

rgb2hsv(avg_rgb,avg_rgb_hsv);
rgb2hsv(rgb_corr_plm_wht,rgb_corr_plm_hsv);
rgb2hsv(rgb_corr_plm_inv_wht,rgb_corr_plm_hsv_inv);
double hueDiff1 =  0.5 - fabs(fmod(fabs(avg_rgb_hsv[0] - rgb_corr_plm_hsv[0]), 2*0.5) - 0.5);
double hueDiff2 =  0.5 - fabs(fmod(fabs(avg_rgb_hsv[0] - rgb_corr_plm_hsv_inv[0]), 2*0.5) - 0.5);



double lrp_corr=(hueDiff2<=hueDiff1)?0.25+(hueDiff1-0.25):0.75+(hueDiff2-0.25);
rgb_corr[0]=lerp(rgb_corr_plm_wht[0],rgb_corr_plm_inv_wht[0],lrp_corr);
rgb_corr[1]=lerp(rgb_corr_plm_wht[1],rgb_corr_plm_inv_wht[1],lrp_corr);
rgb_corr[2]=lerp(rgb_corr_plm_wht[2],rgb_corr_plm_inv_wht[2],lrp_corr);



double rgb_corr_prp[3];
double rgb_corr_wht[3];
RGB2rgb(rgb_corr,rgb_corr_prp);
rgb2RGB_White(rgb_corr_prp,rgb_corr_wht);

*/


/*
double rgb_corr_wht_avg=(rgb_corr_wht[0]+rgb_corr_wht[1]+rgb_corr_wht[2])*pow(3,-1);
double rgb_corr_wht_gry[3]={rgb_corr_wht_avg,rgb_corr_wht_avg,rgb_corr_wht_avg};*/
/*


double avg_lrp=(avg_rgb_prp_wht[1]-avg_rgb_prp_wht[0])/(avg_rgb_inv_prp_wht[0]-avg_rgb_inv_prp_wht[1]+avg_rgb_prp_wht[1]-avg_rgb_prp_wht[0]);
*/
/*
avg_rgb5[0]=1-lerp(avg_rgb_prp_wht[0],avg_rgb_inv_prp_wht[0],tnt);
avg_rgb5[1]=1-lerp(avg_rgb_prp_wht[1],avg_rgb_inv_prp_wht[1],tnt);
avg_rgb5[2]=1-lerp(avg_rgb_prp_wht[2],avg_rgb_inv_prp_wht[2],tnt);
*/
/*
double tint_lrp=((avg_rgb1_prp_wht[1]-avg_rgb1_prp_wht[0])/(avg_rgb3_prp_wht[0]-avg_rgb3_prp_wht[1]+avg_rgb1_prp_wht[1]-avg_rgb1_prp_wht[0]));
avg_rgb2[0]=1-lerp(avg_rgb1_prp_wht[0],avg_rgb3_prp_wht[0],tnt);
avg_rgb2[1]=1-lerp(avg_rgb1_prp_wht[1],avg_rgb3_prp_wht[1],tnt);
avg_rgb2[2]=1-lerp(avg_rgb1_prp_wht[2],avg_rgb3_prp_wht[2],tnt);

RGB2rgb(avg_rgb2,avg_rgb3);

double avg_rgb2_inv[3]={1-avg_rgb2[0],1-avg_rgb2[1],1-avg_rgb2[2]};

rgb2RGB_White(avg_rgb3,avg_rgb4);

rgb2RGB_White(avg_rgb2_inv,avg_rgb6);
*/


//double avg_rgb_hsv[3];

/*
double avg_rgb_sc_lin[3];

sRGB2Linear(avg_rgb_sc,avg_rgb_sc_lin);
*/
/*
sRGB2Linear(avg_rgb,avg_rgb_sc_lin);*/
/*double avg_rgb_prp[3];
RGB2rgb(avg_rgb,avg_rgb_prp);*/
/*
double avg_rgb_wht[3];
rgb2RGB_White(avg_rgb,avg_rgb_wht);

double avg_rgb_sc_wht[3];
rgb2RGB_White(avg_rgb_sc,avg_rgb_sc_wht);

rgb2hsv(avg_rgb,avg_rgb_hsv);
double avg_rgb_hsmv[3];
hsv2hmv(avg_rgb_hsv,avg_rgb_hsmv);
double avg_rgb_avg=(avg_rgb[0]+avg_rgb[1]+avg_rgb[2])*pow(3,-1);

double avg_rgb_hmv_bk[3];
double corr_pwr=1.00827;
double corr_m[1]={pow(avg_rgb_avg,corr_pwr)};
mchg2hsv(avg_rgb_hsmv,corr_m,avg_rgb_hmv_bk);

double avg_rgb_hmv_fix[3];
//avg_rgb_hmv_bk[0]=mod(avg_rgb_hmv_bk[0]-0.1,1);
hsv2rgb(avg_rgb_hmv_bk,avg_rgb_hmv_fix);/*
double avg_rgb_hmv_fix_lin[3];
sRGB2Linear(avg_rgb_hmv_fix,avg_rgb_hmv_fix_lin);*/

/*
double avg_rgb_hmv_fix_plus[3]={(avg_rgb_hmv_fix[0])+1,(avg_rgb_hmv_fix[1])+1,(avg_rgb_hmv_fix[2])+1};
double avg_rgb_hmv_fix_prp[3];
RGB2rgb(avg_rgb_hmv_fix_plus,avg_rgb_hmv_fix_prp);
double avg_rgb_hmv_fix_wht[3];
rgb2RGB_White(avg_rgb_hmv_fix_prp,avg_rgb_hmv_fix_wht);

double avg_rgb_hmv_fix_inv[3]={1-avg_rgb_hmv_fix[0],1-avg_rgb_hmv_fix[1],1-avg_rgb_hmv_fix[2]};
double avg_rgb_hmv_fix_inv_prp[3];
double avg_rgb_hmv_fix_inv_wht[3];
RGB2rgb(avg_rgb_hmv_fix_inv,avg_rgb_hmv_fix_inv_prp);
rgb2RGB_White(avg_rgb_hmv_fix_inv_prp,avg_rgb_hmv_fix_inv_wht);

double avg_rgb_inv[3]={1-avg_rgb_hmv_fix[0],1-avg_rgb_hmv_fix[1],1-avg_rgb_hmv_fix[2]};
double  avg_rgb_wht_inv_hsv[3];
rgb2hsv(avg_rgb_inv,avg_rgb_wht_inv_hsv);
double avg_rgb_wht_inv_prp[3];
RGB2rgb(avg_rgb_inv,avg_rgb_wht_inv_prp);
double avg_rgb_inv_prp_wht[3];
rgb2RGB_White(avg_rgb_wht_inv_prp,avg_rgb_inv_prp_wht);

double avg_rgb_hmv_fix_plus2[3]={1-(avg_rgb_hmv_fix[0]),1-(avg_rgb_hmv_fix[1]),1-(avg_rgb_hmv_fix[2])};
double avg_rgb_hmv_fix_prp2[3];
RGB2rgb(avg_rgb_hmv_fix_plus2,avg_rgb_hmv_fix_prp2);
double avg_rgb_hmv_fix_wht2[3];
rgb2RGB_White(avg_rgb_hmv_fix_prp2,avg_rgb_hmv_fix_wht2);

double avg_rgb_hmv_fix_inv2[3]={1-avg_rgb_hmv_fix_plus2[0],1-avg_rgb_hmv_fix_plus2[1],1-avg_rgb_hmv_fix_plus2[2]};
double avg_rgb_hmv_fix_inv_prp2[3];
double avg_rgb_hmv_fix_inv_wht2[3];
RGB2rgb(avg_rgb_hmv_fix_inv2,avg_rgb_hmv_fix_inv_prp2);
rgb2RGB_White(avg_rgb_hmv_fix_inv_prp2,avg_rgb_hmv_fix_inv_wht2);




double rgb_trns_wht1[3];
double rgb_trns_wht2[3];
double rgb_trns_wht3[3];
double rgb_trns_wht4[3];
//double rgb_trns_wht1_inv[3];

double rgb_trns_wht1_prp[3];
double rgb_trns_wht2_prp[3];
double rgb_trns_wht3_prp[3];
double rgb_trns_wht4_prp[3];
double rgb_trns_wht1_prp_wht[3];

double rgb_trns_wht2_prp_wht[3];
double rgb_trns_wht3_prp_wht[3];
double rgb_trns_wht4_prp_wht[3];
double avg_rgb_hmv_fix_grey=(avg_rgb_hmv_fix[0]+avg_rgb_hmv_fix[1]+avg_rgb_hmv_fix[2])*pow(3,1);

double hueDiff1 =  0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - avg_rgb_hmv_bk[0]), 1) - 0.5);
double hueDiff2 =  0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - avg_rgb_wht_inv_hsv[0]), 1) - 0.5);


double diff_sm=fabs(avg_rgb_hmv_bk[1]-avg_rgb_hsv[1]);
double lrp=tntp;


//0.5*((1-(0.5*(0.5*(diff_sm-avg_chroma+1)+diff_sm))*(1-(1-diff_sm)*(1-avg_chroma)))+(1-avg_chroma));

//lrp=(hueDiff1<=hueDiff2)?0.75+(0.5*lrp-0.25):0.25+(-0.5*lrp+0.25);


*/
/*
rgb_trns_wht1[0]=lerp(avg_rgb_wht_inv[0],avg_rgb_hmv_fix[0],lrp);
rgb_trns_wht1[1]=lerp(avg_rgb_wht_inv[1],avg_rgb_hmv_fix[1],lrp);
rgb_trns_wht1[2]=lerp(avg_rgb_wht_inv[2],avg_rgb_hmv_fix[2],lrp);
*/

/*

rgb_trns_wht1[0]=lerp(avg_rgb_hmv_fix_lin[0],avg_rgb_hmv_fix_grey,lrp);
rgb_trns_wht1[1]=lerp(avg_rgb_hmv_fix_lin[1],avg_rgb_hmv_fix_grey,lrp);
rgb_trns_wht1[2]=lerp(avg_rgb_hmv_fix_lin[2],avg_rgb_hmv_fix_grey,lrp);

rgb_trns_wht2[0]=lerp(avg_rgb_inv_lin[0],1-avg_rgb_hmv_fix_grey,lrp);
rgb_trns_wht2[1]=lerp(avg_rgb_inv_lin[1],1-avg_rgb_hmv_fix_grey,lrp);
rgb_trns_wht2[2]=lerp(avg_rgb_inv_lin[2],1-avg_rgb_hmv_fix_grey,lrp);

double rgb_trns_wht1_inv[3]={1-rgb_trns_wht1[0],1-rgb_trns_wht1[1],1-rgb_trns_wht1[2]};
double rgb_trns_wht2_inv[3]={1-rgb_trns_wht2[0],1-rgb_trns_wht2[1],1-rgb_trns_wht2[2]};

if(hueDiff1<=hueDiff2){

    rgb_trns_wht3[0]=rgb_trns_wht1[0];
    rgb_trns_wht3[1]=rgb_trns_wht1[1];
    rgb_trns_wht3[2]=rgb_trns_wht1[2];

    rgb_trns_wht4[0]=rgb_trns_wht1_inv[0];
    rgb_trns_wht4[1]=rgb_trns_wht1_inv[1];
    rgb_trns_wht4[2]=rgb_trns_wht1_inv[2];



}else{


    rgb_trns_wht3[0]=rgb_trns_wht2[0];
    rgb_trns_wht3[1]=rgb_trns_wht2[1];
    rgb_trns_wht3[2]=rgb_trns_wht2[2];

    rgb_trns_wht4[0]=rgb_trns_wht2_inv[0];
    rgb_trns_wht4[1]=rgb_trns_wht2_inv[1];
    rgb_trns_wht4[2]=rgb_trns_wht2_inv[2];


}

*/

//
/*

   RGB2rgb(rgb_trns_wht1,rgb_trns_wht1_prp);
rgb2RGB_White(rgb_trns_wht1_prp,rgb_trns_wht1_prp_wht);

   RGB2rgb(rgb_trns_wht2,rgb_trns_wht2_prp);
rgb2RGB_White(rgb_trns_wht2_prp,rgb_trns_wht2_prp_wht);
*/
/*
  RGB2rgb(rgb_trns_wht3,rgb_trns_wht3_prp);
rgb2RGB_White(rgb_trns_wht3_prp,rgb_trns_wht3_prp_wht);

  RGB2rgb(rgb_trns_wht4,rgb_trns_wht4_prp);
rgb2RGB_White(rgb_trns_wht4_prp,rgb_trns_wht4_prp_wht);

*/
/*
double rgb_trns_wht1_hsv[3];
rgb2hsv(rgb_trns_wht1,rgb_trns_wht1_hsv);

rgb_trns_wht1_inv[0]=lerp(avg_rgb_hmv_fix_wht[0],1,lrp);
rgb_trns_wht1_inv[1]=lerp(avg_rgb_hmv_fix_wht[1],1,lrp);
rgb_trns_wht1_inv[2]=lerp(avg_rgb_hmv_fix_wht[2],1,lrp);

double rgb_trns_wht1_inv_hsv[3];
rgb2hsv(rgb_trns_wht1_inv,rgb_trns_wht1_inv_hsv);


double hueDiff1 =  0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - rgb_trns_wht1_hsv[0]), 1) - 0.5);
double hueDiff2 =  0.5 - fabs(mod(fabs(avg_rgb_hsv[0] - rgb_trns_wht1_inv_hsv[0]),1) - 0.5);

 // LinRGB2Grey_XYZ(avg_rgb_prp_wht,XYZ_convert_OG,XYZ_convert2);
double rgb_trns_wht1_inv_prp[3];
double rgb_trns_wht1_inv_wht[3];



   RGB2rgb(rgb_trns_wht1_inv,rgb_trns_wht1_inv_prp);
rgb2RGB_White(rgb_trns_wht1_inv_prp,rgb_trns_wht1_inv_wht);
*/
double XYZ_convert_OG[3];
double XYZ_convert2[3];
double XYZ_convert[3];
/*
double XYZ_convert_OG_inv[3];
double XYZ_convert2_inv[3];
double XYZ_convert_inv[3];
*/


double white[3]={1,1,1};/*
double rgb_trns_wht1_prp[3];
double rgb_trns_wht1_wht[3];
   RGB2rgb(rgb_trns_wht1,rgb_trns_wht1_prp);
rgb2RGB_White(rgb_trns_wht1_prp,rgb_trns_wht1_wht);

*/


LinRGB2Other_XYZ (avg_rgb_wht_bk3,white,XYZ_convert_OG,XYZ_convert2);
//LinRGB2Other_XYZ (white,avg_rgb_hmv_fix_inv_wht,XYZ_convert_OG,XYZ_convert2);



   //LinRGB2Other_XYZ (avg_rgb_hmv_fix_inv_wht2,white,XYZ_convert_OG_inv,XYZ_convert2_inv);


WPconv2Grey (XYZ_convert_OG,XYZ_convert2 ,XYZ_convert);


/*
RGB2rgb(rgb_trns_wht2,rgb_trns_wht2_prp);
rgb2RGB_White(rgb_trns_wht2_prp,rgb_trns_wht2_prp_wht);
*/


  //  LinRGB2Grey_XYZ(rgb_corr_wht,XYZ_convert_OG,XYZ_convert2);
/*

WPconv2Grey (XYZ_convert_OG_inv,XYZ_convert2_inv ,XYZ_convert_inv);

double avg_rgbxyY[3];
double avg_rgbXYZ[3];
double avg_WPConvXYZ[3];
double avg_WPConvXYZ2[3];
double avg_WPConvXYZ_xyY[3];
double avg_WPConvXYZ_xyY2[3];
double avg_WPchgRGB[3];
double avg_WPchgRGB_hsv[3];
double avg_WPchgRGB_Lin[3];
double avg_WPchgRGB2[3];
double avg_WPchgRGB2_hsv[3];
double avg_WPchgRGB_Lin2[3];

double avg_rgb_frm[3];
double avg_rgb_frm_lin[3];

 avg_rgb_frm_lin[0]=avg_rgb[0];
 avg_rgb_frm_lin[1]=avg_rgb[1];
 avg_rgb_frm_lin[2]=avg_rgb[2];

Linear2sRGB(avg_rgb_frm_lin,avg_rgb_frm);

rgb2xyY(avg_rgb_frm,avg_rgbxyY);
xyY2XYZ(avg_rgbxyY,avg_rgbXYZ);

WPconv(avg_rgbXYZ,D65XYZ,XYZ_convert,avg_WPConvXYZ);
WPconv(avg_rgbXYZ,D65XYZ,XYZ_convert_inv,avg_WPConvXYZ2);

XYZ2xyY(avg_WPConvXYZ,avg_WPConvXYZ_xyY);
XYZ2xyY(avg_WPConvXYZ2,avg_WPConvXYZ_xyY2);

xyY2rgb(avg_WPConvXYZ_xyY,avg_WPchgRGB);
xyY2rgb(avg_WPConvXYZ_xyY2,avg_WPchgRGB2);
*/
/*
sRGB2Linear(avg_WPchgRGB,avg_WPchgRGB_Lin);
sRGB2Linear(avg_WPchgRGB2,avg_WPchgRGB_Lin2);
*/
/*
rgb2hsv(avg_WPchgRGB,avg_WPchgRGB_hsv);
rgb2hsv(avg_WPchgRGB2,avg_WPchgRGB2_hsv);

if (avg_WPchgRGB2_hsv[1]<avg_WPchgRGB_hsv[1]){

    XYZ_convert[0]=XYZ_convert_inv[0];
    XYZ_convert[1]=XYZ_convert_inv[1];
    XYZ_convert[2]=XYZ_convert_inv[2];

}

*/
      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {


double x_shift=(double)x/(double)row_size;
double y_shift=(double)y/(double)height;
                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue*rcp_twoFiveFive;     // B
       gOG=currGreen*rcp_twoFiveFive;   //G
         rOG=currRed*rcp_twoFiveFive;     // R

double curr_rgb_dst_fnl[3]={rOG,gOG,bOG};

double curr_rgb_dst_lin_fnl[3];
sRGB2Linear(curr_rgb_dst_fnl,curr_rgb_dst_lin_fnl);

double curr_rgb_dst_lin_prp_fnl[3];
RGB2rgb(curr_rgb_dst_lin_fnl,curr_rgb_dst_lin_prp_fnl);

double curr_rgb_dst_lin_wht_fnl[3];

rgb2RGB_White(curr_rgb_dst_lin_prp_fnl,curr_rgb_dst_lin_wht_fnl);

/*
double curr_rgb_dst_hsi_fnl[3];
RGB2HSI(curr_rgb_dst_lin_fnl,curr_rgb_dst_hsi_fnl);

double curr_rgb_dst_hsv_fnl[3];
rgb2hsv(curr_rgb_dst_lin_fnl,curr_rgb_dst_hsv_fnl);
double curr_rgb_dst_hwb_fnl[3];
hsv2hwb(curr_rgb_dst_hsv_fnl,curr_rgb_dst_hwb_fnl);


curr_rgb_dst_hwb_fnl[2]=1;

double curr_rgb_dst_hwb_hsv_fnl[3];
hwb2hsv(curr_rgb_dst_hwb_fnl,curr_rgb_dst_hwb_hsv_fnl);

double curr_rgb_dst_hwb_rgb_fnl[3];
hsv2rgb(curr_rgb_dst_hwb_hsv_fnl,curr_rgb_dst_hwb_rgb_fnl);
*/

double Sc_fnl=(MAX(curr_rgb_dst_lin_prp_fnl[0],MAX(curr_rgb_dst_lin_prp_fnl[1],curr_rgb_dst_lin_prp_fnl[2]))-MIN(curr_rgb_dst_lin_prp_fnl[0],MIN(curr_rgb_dst_lin_prp_fnl[1],curr_rgb_dst_lin_prp_fnl[2])));

double rootSat_fnl=sqrt(  pow(curr_rgb_dst_lin_fnl[0]-curr_rgb_dst_lin_fnl[1],2) + pow(curr_rgb_dst_lin_fnl[0]-curr_rgb_dst_lin_fnl[2],2)  +pow(curr_rgb_dst_lin_fnl[1]-curr_rgb_dst_lin_fnl[2],2))/sqrt(3);

 Sc_fnl=MIN(MAX((Sc_fnl*rootSat_fnl),0),1);


if(dbg==1){

    if (1-Sc_fnl>strt){

        srcp[x] = 127;
                srcp[x+1] =255;
        srcp[x+2] = 0;
    }
}else{
    double WPchgRGB[3];

    double rgbxyY[3];
double rgbXYZ[3];
double WPConvXYZ[3];
double WPConvXYZ_xyY[3];

rgb2xyY(curr_rgb_dst_fnl,rgbxyY);
xyY2XYZ(rgbxyY,rgbXYZ);

WPconv(rgbXYZ,D65XYZ,XYZ_convert,WPConvXYZ);

if (cust_x!=CIEx || (cust_y!=CIEy)){

double WPConvXYZ2[3];
double cust_xy[2]={cust_x,cust_y};
double cust_XYZ[3];
xy2XYZ(cust_xy,cust_XYZ);

WPconv(WPConvXYZ,D65XYZ,cust_XYZ,WPConvXYZ2);
WPConvXYZ[0]=WPConvXYZ2[0];
WPConvXYZ[1]=WPConvXYZ2[1];
WPConvXYZ[2]=WPConvXYZ2[2];

}

XYZ2xyY(WPConvXYZ,WPConvXYZ_xyY);

xyY2rgb(WPConvXYZ_xyY,WPchgRGB);



if(rOG==0 && (gOG==0) && (bOG==0)){
    WPchgRGB[0]=0;
    WPchgRGB[1]=0;
    WPchgRGB[2]=0;

}

/*

double WPchgRGB_lin[3];
double WPchgRGB_lin_prp[3];
double WPchgRGB_lin_wht[3];
sRGB2Linear(WPchgRGB,WPchgRGB_lin);
RGB2rgb(WPchgRGB_lin,WPchgRGB_lin_prp);
rgb2RGB_White(WPchgRGB_lin_prp,WPchgRGB_lin_wht);


double WPchgRGB_lin2[3];
double WPchgRGB_lin_prp2[3];
double WPchgRGB_lin_wht2[3];
sRGB2Linear(WPchgRGB2,WPchgRGB_lin2);
RGB2rgb(WPchgRGB_lin2,WPchgRGB_lin_prp2);
rgb2RGB_White(WPchgRGB_lin_prp2,WPchgRGB_lin_wht2);


double output[3];
*/
/*
double dnm_rg=WPchgRGB_lin_wht2[0]-WPchgRGB_lin_wht[0]-WPchgRGB_lin_wht2[1]+WPchgRGB_lin_wht[1];
double dnm_rb=WPchgRGB_lin_wht2[0]-WPchgRGB_lin_wht[0]-WPchgRGB_lin_wht2[2]+WPchgRGB_lin_wht[2];
double dnm_gb=WPchgRGB_lin_wht2[1]-WPchgRGB_lin_wht[1]-WPchgRGB_lin_wht2[2]+WPchgRGB_lin_wht[2];
double t_zero_rg=(dnm_rg==0)?1:(WPchgRGB_lin_wht2[0]-WPchgRGB_lin_wht2[1])/dnm_rg;
double t_zero_rb=(dnm_rb==0)?1:(WPchgRGB_lin_wht2[0]-WPchgRGB_lin_wht2[2])/dnm_rb;
double t_zero_gb=(dnm_gb==0)?1:(WPchgRGB_lin_wht2[1]-WPchgRGB_lin_wht2[2])/dnm_gb;

 t_zero_rg=MAX(MIN(t_zero_rg,1),0);
 t_zero_rb=MAX(MIN(t_zero_rb,1),0);
 t_zero_gb=MAX(MIN(t_zero_gb,1),0);

double lrp_rg_r=lerp(WPchgRGB_lin_wht2[0],WPchgRGB_lin_wht[0],t_zero_rg);
double lrp_rg_g=lerp(WPchgRGB_lin_wht2[1],WPchgRGB_lin_wht[1],t_zero_rg);
double lrp_rg_b=lerp(WPchgRGB_lin_wht2[2],WPchgRGB_lin_wht[2],t_zero_rg);

double lrp_rb_r=lerp(WPchgRGB_lin_wht2[0],WPchgRGB_lin_wht[0],t_zero_rb);
double lrp_rb_g=lerp(WPchgRGB_lin_wht2[1],WPchgRGB_lin_wht[1],t_zero_rb);
double lrp_rb_b=lerp(WPchgRGB_lin_wht2[2],WPchgRGB_lin_wht[2],t_zero_rb);

double lrp_gb_r=lerp(WPchgRGB_lin_wht2[0],WPchgRGB_lin_wht[0],t_zero_gb);
double lrp_gb_g=lerp(WPchgRGB_lin_wht2[1],WPchgRGB_lin_wht[1],t_zero_gb);
double lrp_gb_b=lerp(WPchgRGB_lin_wht2[2],WPchgRGB_lin_wht[2],t_zero_gb);

double mix_lrp_rg= MAX(lrp_rg_r,MAX(lrp_rg_g,lrp_rg_b))-MIN(lrp_rg_r,MIN(lrp_rg_g,lrp_rg_b));
double mix_lrp_rb= MAX(lrp_rb_r,MAX(lrp_rb_g,lrp_rb_b))-MIN(lrp_rb_r,MIN(lrp_rb_g,lrp_rb_b));
double mix_lrp_gb= MAX(lrp_gb_r,MAX(lrp_gb_g,lrp_gb_b))-MIN(lrp_gb_r,MIN(lrp_gb_g,lrp_gb_b));
double mix_lrp_zero= MAX(WPchgRGB_lin_wht2[0],MAX(WPchgRGB_lin_wht2[1],WPchgRGB_lin_wht2[2]))-MIN(WPchgRGB_lin_wht2[0],MIN(WPchgRGB_lin_wht2[1],WPchgRGB_lin_wht2[2]));
double mix_lrp_one= MAX(WPchgRGB_lin_wht[0],MAX(WPchgRGB_lin_wht[1],WPchgRGB_lin_wht[2]))-MIN(WPchgRGB_lin_wht[0],MIN(WPchgRGB_lin_wht[1],WPchgRGB_lin_wht[2]));

double mix_lrp_min=MIN(mix_lrp_rg,MIN(mix_lrp_rb,MIN(mix_lrp_zero,MIN(mix_lrp_one,mix_lrp_gb))));
double tnt=1;

if(mix_lrp_min==mix_lrp_rg){
        tnt=t_zero_rg;
}

if(mix_lrp_min==mix_lrp_rb){
        tnt=t_zero_rb;
}

if(mix_lrp_min==mix_lrp_gb){
        tnt=t_zero_gb;
}

if(mix_lrp_min==mix_lrp_zero){
        tnt=0;
}

if(mix_lrp_min==mix_lrp_one){
        tnt=1;
}



output[0]=lerp(WPchgRGB_lin2[0],WPchgRGB_lin[0],tnt);
output[1]=lerp(WPchgRGB_lin2[1],WPchgRGB_lin[1],tnt);
output[2]=lerp(WPchgRGB_lin2[2],WPchgRGB_lin[2],tnt);

double output_gc[3];

Linear2sRGB(output,output_gc);
*/

                srcp[x] = MAX(MIN(round(WPchgRGB[2]*255),255),0);
             srcp[x+1] =MAX(MIN(round(WPchgRGB[1]*255),255),0);
        srcp[x+2] = MAX(MIN(round(WPchgRGB[0]*255),255),0);


       /*
                if(y_shift<0.09){
                            srcp[x] = MAX(MIN(round(avg_rgb_lin_hwb[1]*255),255),0); //B
             srcp[x+1] =MAX(MIN(round(avg_rgb_lin_hwb[1]*255),255),0); //G
        srcp[x+2] = MAX(MIN(round(avg_rgb_lin_hwb[1]*255),255),0); //R
        }             if(y_shift<0.06){
                            srcp[x] = MAX(MIN(round(avg_chroma*255),255),0); //B
             srcp[x+1] =MAX(MIN(round(avg_chroma*255),255),0); //G
        srcp[x+2] = MAX(MIN(round(avg_chroma*255),255),0); //R
        }   if(y_shift<0.03){
                            srcp[x] = MAX(MIN(round(avg_chroma*255),255),0); //B
             srcp[x+1] =MAX(MIN(round(avg_chroma*255),255),0); //G
        srcp[x+2] = MAX(MIN(round(avg_chroma*255),255),0); //R
        } /*
*/


  /*      double tint_map[3];


tint_map[0]=1-lerp(rgb_trns_wht1_wht[0],rgb_trns_wht1_inv_wht[0],x_shift);
tint_map[1]=1-lerp(rgb_trns_wht1_wht[1],rgb_trns_wht1_inv_wht[1],x_shift);
tint_map[2]=1-lerp(rgb_trns_wht1_wht[2],rgb_trns_wht1_inv_wht[2],x_shift);

double tint_map_prp[3];
RGB2rgb(tint_map,tint_map_prp);
double tint_map_white[3];
rgb2RGB_White(tint_map_prp,tint_map_white);
double tint_map_white_gc[3];
Linear2sRGB(tint_map_white,tint_map_white_gc);
   if(y_shift<0.07){
                            srcp[x] = MAX(MIN(round((lrp)*255),255),0);
             srcp[x+1] =MAX(MIN(round((lrp)*255),255),0);
        srcp[x+2] = MAX(MIN(round(( lrp)*255),255),0);

        }

        if(y_shift<0.05){
                            srcp[x] = MAX(MIN(round(tint_map_white[2]*255),255),0);
             srcp[x+1] =MAX(MIN(round(tint_map_white[1]*255),255),0);
        srcp[x+2] = MAX(MIN(round(tint_map_white  [0]*255),255),0);

        }*/
/*
        double tint_map1[3];

tint_map1[0]=1-lerp(avg_rgb_prp_wht[0],avg_rgb_inv_prp_wht[0],1-x_shift);
tint_map1[1]=1-lerp(avg_rgb_prp_wht[1],avg_rgb_inv_prp_wht[1],1-x_shift);
tint_map1[2]=1-lerp(avg_rgb_prp_wht[2],avg_rgb_inv_prp_wht[2],1-x_shift);

double tint_map1_prp[3];
RGB2rgb(tint_map1,tint_map1_prp);
double tint_map1_white[3];
rgb2RGB_White(tint_map1_prp,tint_map1_white);
double tint_map1_white_gc[3];
Linear2sRGB(tint_map1_white,tint_map1_white_gc);
 if(y_shift<0.15){
                            srcp[x] = MAX(MIN(round((avg_rgb6[2])*255),255),0);
             srcp[x+1] =MAX(MIN(round((avg_rgb6[1])*255),255),0);
        srcp[x+2] = MAX(MIN(round((avg_rgb6  [0])*255),255),0);

        }

*/
        /*
        if(y_shift<0.05){
                            srcp[x] = MAX(MIN(round(rgb_corr[2]*255),255),0);
             srcp[x+1] =MAX(MIN(round(rgb_corr[1]*255),255),0);
        srcp[x+2] = MAX(MIN(round(rgb_corr  [0]*255),255),0);

        }
*/
}
////////////////////////////////////////////////////

x+=3;
      }
            srcp += src_pitch;
      } //END OF IMAGE DRAWING LOOP

/////////////////DRAW PIXELS END/////////////////////////////////



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

          params->debug = avs_defined(avs_array_elt(args, 1))?avs_as_bool(avs_array_elt(args, 1)):false;
          params->start = avs_defined(avs_array_elt(args, 2))?avs_as_float(avs_array_elt(args, 2)):1;
          params->x = avs_defined(avs_array_elt(args, 3))?avs_as_float(avs_array_elt(args, 3)):0.312727;
          params->y = avs_defined(avs_array_elt(args, 4))?avs_as_float(avs_array_elt(args, 4)):0.329023;
          //params->precision = avs_defined(avs_array_elt(args, 5))?avs_as_int(avs_array_elt(args, 5)):0;
         params->tint = avs_defined(avs_array_elt(args, 5))?avs_as_float(avs_array_elt(args, 5)):0;



   fi->user_data = (void*) params;
   fi->get_frame = WhitePoint_get_frame;


   v = avs_new_value_clip(new_clip);

   avs_release_clip(new_clip);
  // free(params);
   return v;
}

const char* AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment* env)
{
   avs_add_function(env, "WhitePoint", "c[debug]b[start]f[x]f[y]f[tint]f", create_WhitePoint, 0);
   return "WhitePoint sample C plugin";
}