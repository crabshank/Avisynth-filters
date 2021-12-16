#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "..\avisynth_c.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define lerp(a,b,t) ((1 - (t)) * (a) + (t) * (b) )
#define rcptwoFiveFive 1.0/255.0
#define rcptHiBit 1.0/65535.0

typedef struct Luma_Smooth {
    int surr;
    double var;
    double smooth;
    int debug;
    int sixtyFour;
    double *ys;
    double *out_ys;
    double *rs;
    double *gs;
    double *bs;
    int** nbrsl;
    int** nbrs_rankl;
    int pxels;
    int surr_p;
} Luma_Smooth;

AVS_VideoFrame * AVSC_CC Luma_Smooth_get_frame (AVS_FilterInfo * p, int n)
{
  AVS_VideoFrame *src, *dst;
double vr, smo;
int l_sur,surr_pl,wdt,pxls,sxf,dbg,row_size, height, src_pitch,dst_pitch;
BYTE* srcp;
BYTE* dstp;
Luma_Smooth* params = (Luma_Smooth*) p->user_data;

vr=params->var;
smo=params->smooth;
l_sur=params->surr;
surr_pl=params->surr_p;
dbg=params->debug;

src = avs_get_frame(p->child, n);

srcp = avs_get_read_ptr(src);
src_pitch = avs_get_pitch(src);
row_size = avs_get_row_size(src);
height = avs_get_height(src);

wdt=p->vi.width;

pxls=params->pxels;

dst = avs_new_video_frame(p->env, &p->vi);
dst_pitch = avs_get_pitch(dst);
dstp = avs_get_write_ptr(dst);
dst->height=height;
dst->row_size=row_size;

sxf=params->sixtyFour;

    double *ys=params->ys;
    double *out_ys=params->out_ys;
    double *rs=params->rs;
    double *gs=params->gs;
    double *bs=params->bs;
    int** nbrsl=params->nbrsl;
    int** nbrs_rankl=params->nbrs_rankl;

int p_ix=0;
int xarr=0;

  for (int y=0; y<height; y++) {
      for (int x=0; x<row_size; x++) {

            int intBlue=(sxf==1)?srcp[x]+srcp[x+1]*256:srcp[x];
            int intGreen=(sxf==1)?srcp[x+2]+srcp[x+3]*256:srcp[x+1];
            int intRed=(sxf==1)?srcp[x+4]+srcp[x+5]*256:srcp[x+2];

        double currBlue=(double)(intBlue);
        double currGreen=(double)(intGreen);
        double currRed=(double)(intRed);

        double bOG=(sxf==1)?currBlue*rcptHiBit:currBlue*rcptwoFiveFive;     // B
        double gOG=(sxf==1)?currGreen*rcptHiBit:currGreen*rcptwoFiveFive;   //G
        double rOG=(sxf==1)?currRed*rcptHiBit:currRed*rcptwoFiveFive;     // R

            double v=MAX(rOG,MAX(gOG,bOG));
            ys[p_ix]=v;
            out_ys[p_ix]=v;

            rs[p_ix]=rOG;
            gs[p_ix]=gOG;
            bs[p_ix]=bOG;

        int nbrs_cntl=0;
        for(int i=xarr-l_sur; i<=xarr+l_sur; i++){
        for(int k=y-l_sur; k<=y+l_sur; k++){
            if((i>=0)&&(i<wdt)&&(k>=0)&&(k<height)){
                int rnk=MAX(abs(xarr-i),abs(y-k));
                int nbr_ix=k*wdt+i;
                nbrsl[p_ix][nbrs_cntl]=nbr_ix;
                nbrs_rankl[p_ix][nbrs_cntl]=rnk;
                nbrs_cntl++;
            }
        }
        }

        x=(sxf==1)?x+7:x+3;
        xarr++;
        p_ix++;
      }
      srcp += src_pitch;
      xarr=0;
      }

xarr=0;


for(int x=0; x<pxls; x++){

    //Luma

    double variance;
    double cnt=0;
    double mean=0;
    double old_mean=0;
    double s=0;
    int ix=0;

    while(nbrsl[x][ix]!=-1){
                int nb_ix=nbrsl[x][ix];
            double y_val=ys[nb_ix];
        double cmx=fabs(y_val-ys[x]);
    if(cmx<=smo){
            cnt+=1;
        //Welford's algorithm for variance:
        old_mean=mean;
        mean+=(y_val-mean)/cnt;
        s+=(y_val-mean)*(y_val-old_mean);
    }
    ix++;
    }
    if(cnt>0){
    variance=s/cnt;

                if(variance<vr){
                    double oy=(fabs(mean-ys[x])<=smo)?mean:ys[x];
                        out_ys[x]=(dbg==1)?-2:oy;
                }
             }
    }



    p_ix=0;

  for (int y=0; y<height; y++) {
      for (int x=0; x<row_size; x++) {

            double mx=out_ys[p_ix];
            double mxOG=ys[p_ix];
            double rgb_out[3];
            if(mx<-1){
                rgb_out[0]=0;
                rgb_out[1]=0.63;
                rgb_out[2]=0;
            }else{
                rgb_out[0]=(mxOG==0)?0:(rs[p_ix]/mxOG)*mx;
                rgb_out[1]=(mxOG==0)?0:(gs[p_ix]/mxOG)*mx;
                rgb_out[2]=(mxOG==0)?0:(bs[p_ix]/mxOG)*mx;

            }

int wp_b=MAX(MIN(round(rgb_out[2]*255),255),0);
int wp_g=MAX(MIN(round(rgb_out[1]*255),255),0);
int wp_r=MAX(MIN(round(rgb_out[0]*255),255),0);

          dstp[x] =wp_b; //blue : blue
        dstp[x+1] =(sxf==1)?wp_b:wp_g; // blue : green
        dstp[x+2] =(sxf==1)? wp_g:wp_r; // green: red
        dstp[x+3] =(sxf==1)? wp_g:dstp[x+3]; //green : self
        dstp[x+4] =(sxf==1)? wp_r:dstp[x+4]; //red : self
        dstp[x+5] =(sxf==1)? wp_r:dstp[x+5]; //red : self
        x=(sxf==1)?x+7:x+3;
        p_ix++;
      }
        dstp += dst_pitch;
      }


 avs_release_frame(src);
  return dst;

}

void AVSC_CC free_Luma_Smooth(AVS_FilterInfo* fi)
{
   Luma_Smooth* params = (Luma_Smooth*) fi->user_data;
    free(params->rs);
     free(params->gs);
     free(params->bs);
     free(params->ys);
     free(params->out_ys);

    for (int n = 0; n <params->pxels; ++n){
        free(params->nbrsl[n]);
    }
    free(params->nbrsl);

    for (int n = 0; n <params->pxels; ++n){
        free(params->nbrs_rankl[n]);
    }
    free(params->nbrs_rankl);
    free(params);
}

AVS_Value AVSC_CC create_Luma_Smooth (AVS_ScriptEnvironment * env,
                            AVS_Value args, void * dg)
{
  AVS_Value v;
  AVS_FilterInfo * fi;
  AVS_Clip * new_clip = avs_new_c_filter(env, &fi, avs_array_elt(args, 0), 1);
  Luma_Smooth *params = (Luma_Smooth*)malloc(sizeof(Luma_Smooth));

if (!params)
      return avs_void;

        params->surr= avs_defined(avs_array_elt(args, 1))?avs_as_int(avs_array_elt(args, 1)):3;
        params->var= avs_defined(avs_array_elt(args, 2))?avs_as_float(avs_array_elt(args, 2)):0.14;
        params->smooth= avs_defined(avs_array_elt(args, 3))?avs_as_float(avs_array_elt(args, 3)):0.11;
        params->debug= avs_defined(avs_array_elt(args,4))?avs_as_bool(avs_array_elt(args, 4)):false;

    if ((params->surr<1)||(params->surr>MAX(fi->vi.width-1,fi->vi.height-1))) {
        return avs_new_value_error ("'surr' must be between 1 and MAX(video_width-1,video_height-1)!");
    }

     if (params->var<0) {
        return avs_new_value_error ("'var' must be >=0!");
    }

     if ((params->smooth<0)||(params->smooth>1)) {
        return avs_new_value_error ("'smooth' must be between 0 and 1!");
    }

  if (!((avs_is_rgb32(&fi->vi))||(avs_is_rgb64(&fi->vi)))) {
    return avs_new_value_error ("Input video must be in RGB32 OR RGB64 format!");
  } else {

     if(avs_defined(avs_array_elt(args, 5))){
        params->sixtyFour =avs_as_bool(avs_array_elt(args, 5));
     }else{
       params->sixtyFour = (avs_is_rgb64(&fi->vi))?true:false;
     }

    params->pxels=fi->vi.width*fi->vi.height;
    params->surr_p=1+((4* params->surr)*(params->surr+1));

      params->ys = (double*)malloc( params->pxels* sizeof(double));
 params->out_ys = (double*)malloc( params->pxels* sizeof(double));

 params->rs = (double*)malloc( params->pxels* sizeof(double));
 params->gs = (double*)malloc( params->pxels* sizeof(double));
 params->bs = (double*)malloc( params->pxels* sizeof(double));

params->nbrsl  = malloc(params->pxels * sizeof(int*));    // rows
for (int r = 0; r < params->pxels; ++r) {
    params->nbrsl[r] = malloc((1+params->surr_p) * sizeof(int)); // columns
    memset(params->nbrsl[r],-1,sizeof(params->nbrsl[r][0])*(1+params->surr_p));
}

params->nbrs_rankl  = malloc(params->pxels * sizeof(int*));    // rows
for (int r = 0; r < params->pxels; ++r) {
    params->nbrs_rankl[r] = malloc((1+params->surr_p) * sizeof(int)); // columns
    memset(params->nbrs_rankl[r],-1,sizeof(params->nbrs_rankl[r][0])*(1+params->surr_p));
}

    fi->user_data = (void*) params;

    fi->get_frame = Luma_Smooth_get_frame;

    v = avs_new_value_clip(new_clip);
    fi->free_filter = free_Luma_Smooth;

  avs_release_clip(new_clip);
  return v;
  }
}

const char * AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment * env)
{
   avs_add_function(env, "Luma_Smooth", "c[surr]i[var]f[smooth]f[debug]b[sixtyFour]b", create_Luma_Smooth, 0);
   return "Luma_Smooth C plugin";
}
