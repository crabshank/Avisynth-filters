#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "..\avisynth_c.h"
#include "functions_c.h"

typedef struct Auto_Gamma {
      double bracketA;
      double bracketB;
      int tolerance;
      int crush;
      int limitedRange;
      int mode;
      int linear;
      int sixtyFour;
} Auto_Gamma;

AVS_VideoFrame * AVSC_CC Auto_Gamma_get_frame (AVS_FilterInfo * p, int n)
{
  AVS_VideoFrame * src;
   Auto_Gamma* params = (Auto_Gamma*) p->user_data;

  src = avs_get_frame(p->child, n);

    int row_size, height, src_pitch,x, y,max_iters,tol,tolr,opt,lmr,crs,lnr,mde,sxf;
   BYTE* srcp;
   double a,b,c,mx,runTot_r,runTot_g,runTot_b,bOG,gOG,rOG,f_c,gamma_high,gamma_high_tmp,gamma_low,f_a,R,G,B,counter,rLin,gLin,bLin;

a =   params->bracketA;
b=params->bracketB;
tol=params->tolerance;
crs=params->crush;
lmr=params->limitedRange;
mde=params->mode;
lnr=params->linear;
sxf=params->sixtyFour;

  avs_make_writable(p->env, &src);

      srcp = avs_get_write_ptr(src);
      src_pitch = avs_get_pitch(src);
      row_size = avs_get_row_size(src);
      height = avs_get_height(src);

runTot_r=0;
runTot_g=0;
runTot_b=0;
counter=0;


      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {



//double x_shift=(double)x/(double)row_size;
                 double currBlue=(sxf==1)?(double)srcp[x]+srcp[x+1]*256:(double)srcp[x];
                 double currGreen=(sxf==1)?(double)srcp[x+2]+srcp[x+3]*256:(double)srcp[x+1];
                 double currRed=(sxf==1)?(double)srcp[x+4]+srcp[x+5]*256:(double)srcp[x+2];


    bOG=(sxf==1)?currBlue*rcptHiBit:currBlue*rcptwoFiveFive;     // B
       gOG=(sxf==1)?currGreen*rcptHiBit:currGreen*rcptwoFiveFive;   //G
         rOG=(sxf==1)?currRed*rcptHiBit:currRed*rcptwoFiveFive;     // R

    if ((lnr!=2)&&(lnr!=4)&&(lnr!=6)&&(lnr!=8)){
        if ((((mde>=1)&&(mde<=4))||(mde==8))||(lnr==3)){
           bOG=(bOG < recBetaLin )?rcpFourFive*bOG:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-bOG)),rcpTxFourFive);
           gOG=(gOG < recBetaLin )?rcpFourFive*gOG:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-gOG)),rcpTxFourFive);
           rOG=(rOG < recBetaLin )?rcpFourFive*rOG:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-rOG)),rcpTxFourFive);
        }else if (((mde==5)||(mde==10))||(lnr==7)){
          bOG=fastPrecisePow(bOG,2.6);
          gOG=fastPrecisePow(gOG,2.6);
          rOG=fastPrecisePow(rOG,2.6);
        }else if ((mde==7)||(lnr==5)){
          bOG=fastPrecisePow(bOG,2.2);
          gOG=fastPrecisePow(gOG,2.2);
          rOG=fastPrecisePow(rOG,2.2);
        }else{
                bOG=(bOG > 0.0404482362771082 )?fastPrecisePow(fabs((bOG+0.055)*rcpOFiveFive),2.4):bOG*rcpTwelveNineTwo;
                gOG=(gOG > 0.0404482362771082 )?fastPrecisePow(fabs((gOG+0.055)*rcpOFiveFive),2.4):gOG*rcpTwelveNineTwo;
                rOG=(rOG > 0.0404482362771082 )?fastPrecisePow(fabs((rOG+0.055)*rcpOFiveFive),2.4):rOG*rcpTwelveNineTwo;
            }
    }

    if(mde==1){
        runTot_r+=rOG*0.2124132;
        runTot_g+=gOG*0.7010437;
        runTot_b+=bOG*0.0865432;
    }else if(mde==2){
        runTot_r+=rOG*0.2220379;
        runTot_g+=gOG*0.7066384;
        runTot_b+=bOG*0.0713236;
    }else if(mde==5){ //DCI-P3
        runTot_r+=rOG*0.209491677912731;
        runTot_g+=gOG*0.721595254161044;
        runTot_b+=bOG*0.068913067926226;
    }else if(mde==6){
    runTot_r+=rOG*0.2290036;
    runTot_g+=gOG*0.691726725;
    runTot_b+=bOG*0.079269675;
    }else if (mde==4){
        runTot_r+=rOG*0.26272171736164;
        runTot_g+=gOG*0.677989275502262;
        runTot_b+=bOG*0.059289007136098;
    }else if (mde==7){ //Original NTSC
        runTot_r+=0.2896886;
        runTot_g+=0.6056356;
        runTot_b+=0.1046758;
    }else if (mde==8){ //Rec 601 D93
        runTot_r+=0.1767506;
        runTot_g+=0.7072321;
        runTot_b+=0.1160173;
    }else if (mde==9){ //Rec 709 D93
        runTot_r+=0.1799632;
        runTot_g+=0.7231169;
        runTot_b+=0.0969199;
    }else if (mde==10){ //DCI-P3 D60/ACES
        runTot_r+=0.23762331020788;
        runTot_g+=0.689170669198985;
        runTot_b+=0.073206020593136;
    }else{
        runTot_r+=rOG*0.2126729;
        runTot_g+=gOG*0.7151522;
        runTot_b+=bOG*0.072175;
    }

counter+=1;

        x=x+3;
      }
      }


//Bisection method solver/////////////////////////////////////////
p=1;
tolr=1.0/fastPrecisePow(10,(double)tol);
max_iters=ceil((log10(b-a)-log10(tolr))/log10(2));
opt=0;
double mxMean[3]={runTot_r/counter,runTot_g/counter,runTot_b/counter};
while(p<=max_iters){
    c=0.5*(a+b);
    f_gammaLow(mxMean, c,gamma_high,f_c);

    if(f_c==0||(0.5*(b-a)<tolr)){
    p=max_iters;
    gamma_low=c;
    opt=1;
    }

    p++;
    f_gammaLow(mxMean, a,gamma_high_tmp,f_a );
   // sgn_c=(f_c<0)?-1:1;
    //sgn_a=(f_a<0)?-1:1;
    if(f_a>f_c){
        a=c;
    }else{
    b=c;
    }

}
///////////////////////////////////////////////////////


///////////////ACTUALLY DRAW PIXELS///////////////////////////////////////
      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

//double x_shift=(double)x/(double)row_size;

                 double currBlue=(sxf==1)?(double)srcp[x]+srcp[x+1]*256:(double)srcp[x];
                 double currGreen=(sxf==1)?(double)srcp[x+2]+srcp[x+3]*256:(double)srcp[x+1];
                 double currRed=(sxf==1)?(double)srcp[x+4]+srcp[x+5]*256:(double)srcp[x+2];


    bOG=(sxf==1)?currBlue*rcptHiBit:currBlue*rcptwoFiveFive;     // B
       gOG=(sxf==1)?currGreen*rcptHiBit:currGreen*rcptwoFiveFive;   //G
         rOG=(sxf==1)?currRed*rcptHiBit:currRed*rcptwoFiveFive;     // R

        if ((((mde>=1)&&(mde<=4))||(mde==8))||(lnr==3)){
           bLin=(bOG < recBetaLin )?rcpFourFive*bOG:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-bOG)),rcpTxFourFive);
           gLin=(gOG < recBetaLin )?rcpFourFive*gOG:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-gOG)),rcpTxFourFive);
           rLin=(rOG < recBetaLin )?rcpFourFive*rOG:fastPrecisePow(-1*(rcpRecAlpha*(1-recAlpha-rOG)),rcpTxFourFive);
        }else if (((mde==5)||(mde==10))||(lnr==7)){
          bLin=fastPrecisePow(bLin,2.6);
          gLin=fastPrecisePow(gLin,2.6);
          rLin=fastPrecisePow(rLin,2.6);
        }else if ((mde==7)||(lnr==5)){
          bLin=fastPrecisePow(bLin,2.2);
          gLin=fastPrecisePow(gLin,2.2);
          rLin=fastPrecisePow(rLin,2.2);
        }else{
          bLin=(bOG > 0.0404482362771082 )?fastPrecisePow(fabs((bOG+0.055)*rcpOFiveFive),2.4):bOG*rcpTwelveNineTwo;
          gLin=(gOG > 0.0404482362771082 )?fastPrecisePow(fabs((gOG+0.055)*rcpOFiveFive),2.4):gOG*rcpTwelveNineTwo;
          rLin=(rOG > 0.0404482362771082 )?fastPrecisePow(fabs((rOG+0.055)*rcpOFiveFive),2.4):rOG*rcpTwelveNineTwo;
        }

                  if ((lnr==1)||(lnr==3)||(lnr==5)||(lnr==7)){
                        bOG=bLin;
                        gOG=gLin;
                        rOG=rLin;
         }else if((lnr==2)||(lnr==4)||(lnr==6)||(lnr==8)){

        if (((mde>=1)&&(mde<=4))||(mde==8)){
    rOG=(rOG< recBeta)?4.5*rOG:recAlpha*fastPrecisePow(rOG,0.45)-(recAlpha-1);
    rOG=(gOG< recBeta)?4.5*gOG:recAlpha*fastPrecisePow(gOG,0.45)-(recAlpha-1);
    rOG=(bOG< recBeta)?4.5*bOG:recAlpha*fastPrecisePow(bOG,0.45)-(recAlpha-1);
        }else if ((mde==5)||(mde==10)){
          bOG=fastPrecisePow(bOG,invTwoSix);
          gOG=fastPrecisePow(gOG,invTwoSix);
          rOG=fastPrecisePow(rOG,invTwoSix);
        }else if (mde==7){
          bOG=fastPrecisePow(bOG,invTwoTwo);
          gOG=fastPrecisePow(gOG,invTwoTwo);
          rOG=fastPrecisePow(rOG,invTwoTwo);
        }else{
 rOG=(rOG> 0.00313066844250063)?1.055 * fastPrecisePow(rOG,rcpTwoFour) - 0.055:12.92 *rOG;
 bOG=(gOG> 0.00313066844250063)?1.055 * fastPrecisePow(gOG,rcpTwoFour) - 0.055:12.92 *gOG;
 gOG=(bOG> 0.00313066844250063)?1.055 * fastPrecisePow(bOG,rcpTwoFour) - 0.055:12.92 *bOG;

            }
         }
R=rOG;
G=gOG;
B=bOG;

double Yog;

    if(mde==1){
       Yog=rOG*0.2124132+gOG*0.7010437+bOG*0.0865432;
    }else if(mde==2){
         Yog=rOG*0.2220379+gOG*0.7066384+bOG*0.0713236;
    }else if(mde==5){
        Yog=rOG*0.209491677912731+gOG*0.721595254161044+bOG*0.068913067926226;
    }else if(mde==6){
        Yog=rOG*0.2290036+gOG*0.691726725+bOG*0.079269675;
    }else if (mde==4){
        Yog=rOG*0.26272171736164+gOG*0.677989275502262+bOG*0.059289007136098;
    }else if (mde==7){
        Yog=rOG*0.2896886+gOG*0.6056356+bOG*0.1046758;
    }else if (mde==8){
        Yog=rOG*0.1767506+gOG*0.7072321+bOG*0.1160173;
    }else if (mde==9){
        Yog=rOG*0.1799632+gOG*0.7231169+bOG*0.0969199;
    }else if (mde==10){
        Yog=rOG*0.23762331020788+gOG*0.689170669198985+bOG*0.073206020593136;
    }else{
        Yog=rOG*0.2126729+gOG*0.7151522+bOG*0.072175;
        }


if(opt==1){

R=lerp(fastPrecisePow(rOG,gamma_low),fastPrecisePow(rOG,gamma_high),rOG);
G=lerp(fastPrecisePow(gOG,gamma_low),fastPrecisePow(gOG,gamma_high),gOG);
B=lerp(fastPrecisePow(bOG,gamma_low),fastPrecisePow(bOG,gamma_high),bOG);

double Ynw;

            if(mde==1){
       Ynw=R*0.2124132+G*0.7010437+B*0.0865432;
    }else if(mde==2){
         Ynw=R*0.2220379+G*0.7066384+B*0.0713236;
    }else if(mde==5){
        Ynw=R*0.209491677912731+G*0.721595254161044+B*0.068913067926226;
    }else if(mde==6){
        Ynw=R*0.2290036+G*0.691726725+B*0.079269675;
    }else if (mde==4){
        Ynw=R*0.26272171736164+G*0.677989275502262+B*0.059289007136098;
    }else if (mde==7){
        Ynw=R*0.2896886+G*0.6056356+B*0.1046758;
    }else if (mde==8){
        Ynw=R*0.1767506+G*0.7072321+B*0.1160173;
    }else if (mde==9){
        Ynw=R*0.1799632+G*0.7231169+B*0.0969199;
    }else if (mde==10){
        Ynw=R*0.23762331020788+G*0.689170669198985+B*0.073206020593136;
    }else{
        Ynw=R*0.2126729+G*0.7151522+B*0.072175;
        }

if (crs==0){
if (Ynw<Yog){
R=rOG;
G=gOG;
B=bOG;
}
}else if (crs==1){
if (Ynw>Yog){
R=rOG;
G=gOG;
B=bOG;
}
}

if(lmr==1){
    R=(( R*255*(235-16)*rcptwoFiveFive )+16)*rcptwoFiveFive;
     G=(( G*255*(235-16)*rcptwoFiveFive )+16)*rcptwoFiveFive;
      B=(( B*255*(235-16)*rcptwoFiveFive )+16)*rcptwoFiveFive;
}

         }

////////////////////////////////////////////////////
//if (x_shift>0.5){
int wp_b=MAX(MIN(round(B*255),255),0);
int wp_g=MAX(MIN(round(G*255),255),0);
int wp_r=MAX(MIN(round(R*255),255),0);

srcp[x] =wp_b; //blue : blue
srcp[x+1] =(sxf==1)?wp_b:wp_g; // blue : green
srcp[x+2] =(sxf==1)? wp_g:wp_r; // green: red
srcp[x+3] =(sxf==1)? wp_g:srcp[x+3]; //green : self
srcp[x+4] =(sxf==1)? wp_r:srcp[x+4]; //red : self
srcp[x+5] =(sxf==1)? wp_r:srcp[x+5]; //red : self

x=(sxf==1)?x+7:x+3;
//}
      }
            srcp += src_pitch;
      }

  return src;
}

AVS_Value AVSC_CC create_Auto_Gamma (AVS_ScriptEnvironment * env,AVS_Value args, void * dg)
{
  AVS_Value v;
  AVS_FilterInfo * fi;
  AVS_Clip * new_clip = avs_new_c_filter(env, &fi, avs_array_elt(args, 0), 1);
  Auto_Gamma *params = (Auto_Gamma*)malloc(sizeof(Auto_Gamma));

if (!params)
      return avs_void;
        params->bracketA = avs_defined(avs_array_elt(args, 1))?avs_as_float(avs_array_elt(args, 1)):0;
        params->bracketB = avs_defined(avs_array_elt(args, 2))?avs_as_float(avs_array_elt(args, 2)):11;
        params->tolerance = avs_defined(avs_array_elt(args, 3))?avs_as_int(avs_array_elt(args, 3)):3;
        params->crush = avs_defined(avs_array_elt(args, 4))?avs_as_int(avs_array_elt(args, 4)):0;
        params->limitedRange= avs_defined(avs_array_elt(args, 5))?avs_as_bool(avs_array_elt(args, 5)):false;
        params->mode = avs_defined(avs_array_elt(args, 6))?avs_as_int(avs_array_elt(args, 6)):0;
        params->linear= avs_defined(avs_array_elt(args, 7))?avs_as_int(avs_array_elt(args, 7)):0;

          if ((params->mode<0)||(params->mode>10)){
            return avs_new_value_error ("Allowed modes are between 0 and 10!");
          } else if ((params->linear<0)||(params->linear>8)){
            return avs_new_value_error ("Allowed linear values are between 0 and 8!");
          }else{
  if (!((avs_is_rgb32(&fi->vi))||(avs_is_rgb64(&fi->vi)))) {
    return avs_new_value_error ("Input video must be in RGB32 OR RGB64 format!");
  } else {

     if(avs_defined(avs_array_elt(args, 8))){
        params->sixtyFour =avs_as_bool(avs_array_elt(args, 8));
     }else{
       params->sixtyFour = (avs_is_rgb64(&fi->vi))?true:false;
     }
         fi->user_data = (void*) params;
    fi->get_frame = Auto_Gamma_get_frame;
    v = avs_new_value_clip(new_clip);
  }
          }
  avs_release_clip(new_clip);
  return v;
}



const char * AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment * env)
{
   avs_add_function(env, "Auto_Gamma", "c[a]f[b]f[tolerance]i[crush]i[limitedRange]b[mode]i[linear]i[sixtyFour]b", create_Auto_Gamma, 0);
   return "Auto_Gamma sample C plugin";
}
