#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "..\avisynth_c.h"
#include "functions_c.h"

typedef struct Circle_Warp {
      double expn;
      int sixtyFour;
} Circle_Warp;

AVS_VideoFrame * AVSC_CC Circle_Warp_get_frame (AVS_FilterInfo * p, int n)
{
  AVS_VideoFrame *src, *dst;
double xp;
BYTE* srcp;
BYTE* dstp;
Circle_Warp* params = (Circle_Warp*) p->user_data;
xp=params->expn;

src = avs_get_frame(p->child, n);
if(xp!=1){
int row_size, height, src_pitch,dst_pitch,sxf,wdt;
srcp = avs_get_read_ptr(src);
src_pitch = avs_get_pitch(src);
row_size = avs_get_row_size(src);
height = avs_get_height(src);
wdt=p->vi.width;


dst = avs_new_video_frame(p->env, &p->vi);
dst_pitch = avs_get_pitch(dst);
dstp = avs_get_write_ptr(dst);
dst->height=height;
dst->row_size=row_size;

sxf=params->sixtyFour;



int** rds  = malloc(wdt * sizeof(int*));    // rows
for (int r = 0; r < wdt; ++r) {
    rds[r] = malloc(height * sizeof(int)); // columns
}

int** grs  = malloc(wdt * sizeof(int*));    // rows
for (int r = 0; r < wdt; ++r) {
    grs[r] = malloc(height * sizeof(int)); // columns
}

int** bls  = malloc(wdt * sizeof(int*));    // rows
for (int r = 0; r < wdt; ++r) {
    bls[r] = malloc(height * sizeof(int)); // columns
}


int xarr=0;

  for (int y=0; y<height; y++) {
      for (int x=0; x<row_size; x++) {

            int currBlue=(sxf==1)?srcp[x]+srcp[x+1]*256:srcp[x];
            int currGreen=(sxf==1)?srcp[x+2]+srcp[x+3]*256:srcp[x+1];
            int currRed=(sxf==1)?srcp[x+4]+srcp[x+5]*256:srcp[x+2];

            rds[xarr][y]=currRed;
            grs[xarr][y]=currGreen;
            bls[xarr][y]=currBlue;

        x=(sxf==1)?x+7:x+3;
        xarr++;
      }
      srcp += src_pitch;
      xarr=0;
      }

     // srcp-=height*src_pitch;

xarr=0;


      for (int y=0; y<height; y++) {
      for (int x=0; x<row_size; x++) {

            double d_x=(double)(xarr);
            double d_y=(double)(y);

            double cd[2]={d_x+0.5,d_y+0.5};
                double cen[2];
                double madr[4];
    adjCenDist(wdt, height, cd,madr,cen);
      double r_n=pow(madr[3],xp)*MAX(madr[0],madr[2]);

         double x_n=r_n*cos(madr[1])+cen[0]; //+ cen[0], gives orign based co-ords
   double  y_n=r_n*sin(madr[1])+cen[1];

   int j=MIN(wdt-1,MAX(floor(x_n-0.5),0));
   int   k=MIN(height-1,MAX(floor(y_n-0.5),0));

            int wp_b=bls[j][k];
            int wp_g=grs[j][k];
            int wp_r=rds[j][k];


dstp[x] =wp_b; //blue : blue
dstp[x+1] =(sxf==1)?wp_b:wp_g; // blue : green
dstp[x+2] =(sxf==1)? wp_g:wp_r; // green: red
dstp[x+3] =(sxf==1)? wp_g:dstp[x+3]; //green : self
dstp[x+4] =(sxf==1)? wp_r:dstp[x+4]; //red : self
dstp[x+5] =(sxf==1)? wp_r:dstp[x+5]; //red : self
x=(sxf==1)?x+7:x+3;
xarr++;
      }
       dstp += dst_pitch;
       xarr=0;
      }


for (int n = 0; n < wdt; ++n){
    free(rds[n]);
}
free(rds);

for (int n = 0; n < wdt; ++n){
    free(grs[n]);
}
free(grs);

for (int n = 0; n < wdt; ++n){
    free(bls[n]);
}
free(bls);

 avs_release_frame(src);
  return dst;
}else{
return src;
}
}

void AVSC_CC free_Circle_Warp(AVS_FilterInfo* fi)
{
   Circle_Warp* params = (Circle_Warp*) fi->user_data;
   free(params);
}

AVS_Value AVSC_CC create_Circle_Warp (AVS_ScriptEnvironment * env,
                            AVS_Value args, void * dg)
{
  AVS_Value v;
  AVS_FilterInfo * fi;
  AVS_Clip * new_clip = avs_new_c_filter(env, &fi, avs_array_elt(args, 0), 1);
  Circle_Warp *params = (Circle_Warp*)malloc(sizeof(Circle_Warp));

if (!params)
      return avs_void;

        params->expn= avs_defined(avs_array_elt(args, 1))?avs_as_float(avs_array_elt(args, 1)):1;

  if (!((avs_is_rgb32(&fi->vi))||(avs_is_rgb64(&fi->vi)))) {
    return avs_new_value_error ("Input video must be in RGB32 OR RGB64 format!");
  } else {

     if(avs_defined(avs_array_elt(args, 2))){
        params->sixtyFour =avs_as_bool(avs_array_elt(args, 2));
     }else{
       params->sixtyFour = (avs_is_rgb64(&fi->vi))?true:false;
     }

    fi->user_data = (void*) params;

    fi->get_frame = Circle_Warp_get_frame;

    v = avs_new_value_clip(new_clip);
    fi->free_filter = free_Circle_Warp;

  avs_release_clip(new_clip);
  return v;
  }
}

const char * AVSC_CC avisynth_c_plugin_init(AVS_ScriptEnvironment * env)
{
   avs_add_function(env, "Circle_Warp", "c[expn]f[sixtyFour]b", create_Circle_Warp, 0);
   return "Circle_Warp C plugin";
}
