#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "..\avisynth_c.h"
#include "functions_c.h"

typedef struct Auto_Gamma {
      double bracketA;
      double bracketB;
      int tolerance;
      int limitedRange;
      int sat;
} Auto_Gamma;

AVS_VideoFrame* AVSC_CC Auto_Gamma_get_frame(AVS_FilterInfo* fi, int n)
{
 Auto_Gamma* params = (Auto_Gamma*) fi->user_data;

   AVS_VideoFrame* src = avs_get_frame(fi->child, n);



   int row_size, height, src_pitch,x, y, p,count,max_iters,tol,tolr,sgn_c,sgn_a,opt,lmr,satu;
   BYTE* srcp;
   double a,b,c,mx,runTot_r,runTot_g,runTot_b,runTot_s,bOG,gOG,rOG,f_c,gamma_high,gamma_high_tmp,gamma_low,f_a,R,G,B,weights;

a =   params->bracketA;
b=params->bracketB;
tol=params->tolerance;
lmr=params->limitedRange;
satu=params->sat;

            int planes[] ={AVS_CS_BGR32};
src = avs_get_frame(fi->child, n);
   avs_make_writable(fi->env, &src);

p=0;
      srcp = avs_get_write_ptr_p(src, planes[p]);
      src_pitch = avs_get_pitch_p(src, planes[p]);
      row_size = avs_get_row_size_p(src, planes[p]);
      height = avs_get_height_p(src, planes[p]);

runTot_r=0;
runTot_g=0;
runTot_b=0;
weights=0;
//POLL FRAME/////////////////////////////////////////////////////////
      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue/255.0;     // B
       gOG=currGreen/255.0;   //G
         rOG=currRed/255.0;  // R
double ogRGB[3]={bOG,gOG,rOG};
double lnRGB[3];

   double      mn=MIN(rOG,MIN(gOG,bOG));
 double mx=MAX(rOG,MAX(gOG,bOG));

double satr=(mx==0)?0:(mx-mn)/mx;


double HWblack=1-mx;
double HwhiteB=(1-satr)*mx;

double weight=1+(1-(sqrt(pow(HwhiteB,2)+pow(HWblack,2)))/sqrt((2)));
sRGB2Linear(ogRGB,lnRGB);

runTot_r+=lnRGB[0]*weight;
runTot_g+=lnRGB[1]*weight;
runTot_b+=lnRGB[2]*weight;
runTot_s+=satr;
weights=(satu==1)?weights+1:weights+weight;

        x=x+3;

      }

      }

//Bisection method solver/////////////////////////////////////////
p=1;
tolr=pow(1,(double)tol*-1);
max_iters=ceil((log10(b-a)-log10(tolr))/log10(2));
opt=0;
double mxMean[3];
if(satu==1){
double avg_sat=runTot_s/weights;
mxMean[0]=avg_sat;
mxMean[1]=avg_sat;
mxMean[2]=avg_sat;
}else{
 mxMean[0]=runTot_r/weights;
 mxMean[1]=runTot_g/weights;
 mxMean[2]=runTot_b/weights;
}
double mxMeanGC[3];
Linear2sRGB(mxMean,mxMeanGC);
while(p<=max_iters){
    c=0.5*(a+b);
    f_gammaLow(mxMeanGC, c,gamma_high,f_c);

    if(f_c==0||(0.5*(b-a)<tolr)){
    p=max_iters;
    gamma_low=c;
    opt=1;
    }

    p++;
    f_gammaLow(mxMeanGC, a,gamma_high_tmp,f_a );
   // sgn_c=(f_c<0)?-1:1;
    //sgn_a=(f_a<0)?-1:1;
    if(f_a>f_c){
        a=c;
    }else{
    b=c;
    }

}
///////////////////////////////////////////////////////

double x_shift;
///////////////ACTUALLY DRAW PIXELS///////////////////////////////////////
      for (y=0; y<height; y++) {
      for (x=0; x<row_size; x++) {

x_shift=(double)x/(double)row_size;

                 double currBlue=(double)srcp[x];
                double currGreen=(double)srcp[x+1];
                double currRed=(double)srcp[x+2];

bOG=currBlue/255.0;     // B
       gOG=currGreen/255.0;   //G
         rOG=currRed/255.0;  // R
R=rOG;
G=gOG;
B=bOG;


if(satu==1){
    double hsv[3];
    double rgb[3]={R,G,B};
    double rgb_bk[3];
    rgb2hsv(rgb,hsv);
    hsv[1]=lerp(pow( hsv[1],gamma_low),pow(hsv[1],gamma_high),hsv[1]);
    hsv2rgb(hsv,rgb_bk);
    R=rgb_bk[0];
    G=rgb_bk[1];
    B=rgb_bk[2];


}else{
           if(opt==1){

R=lerp(pow(rOG,gamma_low),pow(rOG,gamma_high),rOG);
G=lerp(pow(gOG,gamma_low),pow(gOG,gamma_high),gOG);
B=lerp(pow(bOG,gamma_low),pow(bOG,gamma_high),bOG);


         }



}
if(lmr==1){
    R=(( R*255*(235-16)*pow(255,-1) )+16)*pow(255,-1);
     G=(( G*255*(235-16)*pow(255,-1) )+16)*pow(255,-1);
      B=(( B*255*(235-16)*pow(255,-1) )+16)*pow(255,-1);
}


////////////////////////////////////////////////////
//if (x_shift>0.5){
                srcp[x] = MAX(MIN(round(B*255),255),0);
             srcp[x+1] =MAX(MIN(round(G*255),255),0);
        srcp[x+2] = MAX(MIN(round(R*255),255),0);
//}

             x=x+3;
      }
            srcp += src_pitch;
      } //END OF IMAGE DRAWING LOOP

/////////////////DRAW PIXELS END/////////////////////////////////

 avs_release_frame(src);

   return src;
}


AVS_Value AVSC_CC create_Auto_Gamma(AVS_ScriptEnvironment* env, AVS_Value args, void* user_data)
{
   AVS_Value v;
   AVS_FilterInfo* fi;


   AVS_Clip* new_clip = avs_new_c_filter(env, &fi, avs_array_elt(args, 0), 1);

Auto_Gamma *params = (Auto_Gamma*)malloc(sizeof(Auto_Gamma));

if (!params)
      return avs_void;



   if (!avs_is_rgb(&fi->vi)) {
      return avs_new_value_error("Input video must be in RGB format!");
   }


        params->bracketB = avs_defined(avs_array_elt(args, 2))?avs_as_float(avs_array_elt(args, 2)):12;
         params->bracketA = avs_defined(avs_array_elt(args, 1))?avs_as_float(avs_array_elt(args, 1)):0;
                params->tolerance = avs_defined(avs_array_elt(args, 3))?avs_as_int(avs_array_elt(args, 3)):2;
params->limitedRange= avs_defined(avs_array_elt(args, 4))?avs_as_bool(avs_array_elt(args, 4)):false;
params->sat= avs_defined(avs_array_elt(args, 5))?avs_as_bool(avs_array_elt(args, 5)):false;

   fi->user_data = (void*) params;
   fi->get_frame = Auto_Gamma_get_frame;


   v = avs_new_value_clip(new_clip);

   avs_release_clip(new_clip);
  // free(params);
   return v;
}

const char* AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment* env)
{
   avs_add_function(env, "Auto_Gamma", "c[a]f[b]f[tolerance]i[limitedRange]b[sat]b", create_Auto_Gamma, 0);
   return "Auto_Gamma sample C plugin";
}
