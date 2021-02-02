#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

static inline double dist(double x1, double x2, double y1, double y2){
return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

void adjCenDist(int w, int h, double cd[2], double madr[3],double cen[2]){
        double d_w=(double)(w);
    double d_h=(double)(h);
    cen[0]=0.5*(d_w-1);
    cen[1]=0.5*(d_h-1);
    double brd[4]={0.5, d_w-0.5, d_h-0.5,0.5};
    double ds[4]={-1,-1,-1,-1};
    double ds1[4]={-1,-1,-1,-1};

    int flag=0;
    double m=0;
    double b=0;

    if(cd[0]==cen[0]){
        flag++;
        ds[2]=fabs(brd[2]-cen[1]); //te
        ds[3]=fabs(brd[3]-cen[1]); //be
    }

    if(cd[1]==cen[1]){
        flag++;
        ds[0]=fabs(brd[0]-cen[0]); //le
        ds[1]=fabs(brd[1]-cen[0]); //re
    }

    if(flag==0){
        m=(cd[1]-cen[1])/(cd[0]-cen[0]);
        b=-cd[0]*m+cd[1];
        ds[0]=m*brd[0]+b;
        ds[1]=m*brd[1]+b;
        ds[2]=(brd[2]-b)/m;
        ds[3]=(brd[3]-b)/m;

        ds1[0]=dist(cen[0],brd[0],cen[1],ds[0]); //le
        ds1[1]=dist(cen[0],brd[1],cen[1],ds[1]); //re
        ds1[2]=dist(cen[0],ds[2],cen[1],brd[2]); //te
        ds1[3]=dist(cen[0],ds[3],cen[1],brd[3]); //be
    }

    flag=0;

    for(int i=0; i<4; i++){
        if(ds1[i]!=-1){
            flag++;
            if(flag==1){
                madr[0]=ds1[i];
            }else{
            madr[0]=(ds1[i]<madr[0])?ds1[i]:madr[0];
            }
        }
    }
            madr[1]=atan2(cd[1]-cen[1],cd[0]-cen[0]);
            madr[2]=dist(cd[0],cen[0],cd[1],cen[1]);
            madr[3]=MIN(madr[0],madr[2])/MAX(madr[0],madr[2]); //0 to 1
}
