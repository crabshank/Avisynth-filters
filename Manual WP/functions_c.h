#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define lerp(a,b,t) ((1 - (t)) * (a) + (t) * (b) )

void RGB_prp(double rgb[3], double rgb_prp[3])
{
	double rgbSum=rgb[0]+rgb[1]+rgb[2];

	rgb_prp[0]=(rgbSum==0)?third:rgb[0]/rgbSum;
	rgb_prp[1]=(rgbSum==0)?third:rgb[1]/rgbSum;
	rgb_prp[2]=(rgbSum==0)?third:rgb[2]/rgbSum;
}

double rgb2hue_360(double rgb[3])
{
    double r=rgb[0];
    double g=rgb[1];
    double b=rgb[2];

    double mn=MIN(r,MIN(g,b));
    double mx=MAX(r,MAX(g,b));
    double chr=mx-mn;
    double hue_d=0;

    if (chr!=0){
        if ((r>=g)&&(r>=b)){
            hue_d =(g - b) / chr;
        }else if ((g>=r)&&(g>=b)){
            hue_d = 2.0 + (b - r) / chr;
        }else{
            hue_d = 4.0 + (r - g) / chr;
        }
            hue_d=(hue_d==6)?0:hue_d;
            hue_d*=60;
            hue_d =(hue_d < 0)?hue_d + 360:hue_d;
    }
        return hue_d;

}

void rgb2hsvc_360 (double rgb[3],double hsvc[4])
{
    double r=rgb[0];
    double g=rgb[1];
    double b=rgb[2];

    double mn=MIN(r,MIN(g,b));
    double mx=MAX(r,MAX(g,b));
    double chr=mx-mn;
    double sat=(mx==0)?0:chr/mx;
    double hue_d=0;

    if (chr!=0){
        if ((r>=g)&&(r>=b)){
            hue_d =(g - b) / chr;
        }else if ((g>=r)&&(g>=b)){
            hue_d = 2.0 + (b - r) / chr;
        }else{
            hue_d = 4.0 + (r - g) / chr;
        }
            hue_d=(hue_d==6)?0:hue_d;
            hue_d*=60;
            hue_d =(hue_d < 0)?hue_d + 360:hue_d;
    }

    hsvc[0]=hue_d;
    hsvc[1]=sat;
    hsvc[2]=mx;
    hsvc[3]=chr;
}

void hsv2rgb_360(double hsv[3], double rgb[3])
{
    double h=hsv[0]/360.0;
    double s=hsv[1];
    double v=hsv[2];

    int i = floor(h * 6);
    double f = h * 6 - i;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);
    switch (i % 6) {
        case 0: rgb[0] = v, rgb[1] = t, rgb[2] = p; break;
        case 1: rgb[0] = q, rgb[1] = v, rgb[2] = p; break;
        case 2: rgb[0] = p, rgb[1] = v, rgb[2] = t; break;
        case 3: rgb[0] = p, rgb[1] = q, rgb[2] = v; break;
        case 4: rgb[0] = t, rgb[1] = p, rgb[2] = v; break;
        case 5: rgb[0] = v, rgb[1] = p, rgb[2] = q; break;
    }
}

void hsvc2rgb_360(double hsvc[4], double rgb[3])
{
    double hsv[3]={hsvc[0], hsvc[1], hsvc[2]};
    hsv2rgb_360(hsv, rgb);
}

void debug_1(double rgb[3], double rgb_out[3], double amp,int aprxPw)
{
    double mx=MAX(rgb[0],MAX(rgb[1],rgb[2]));
    double sat=(mx==0)?0:(mx-MIN(rgb[0],MIN(rgb[1],rgb[2])))/mx;

    double dbg_out;
	if(aprxPw==1){
     dbg_out=(amp==1)?sat:fastPrecisePow(sat,amp);
	}else{
		dbg_out=(amp==1)?sat:pow(sat,amp);
	}

    rgb_out[0]=dbg_out;
    rgb_out[1]=dbg_out;
    rgb_out[2]=dbg_out;
}

void debug_2_8(double rgb[3], double rgb_out[3], double amp, int dbg)
{
    double mx=MAX(rgb[0],MAX(rgb[1],rgb[2]));
    double sat=(mx==0)?0:(mx-MIN(rgb[0],MIN(rgb[1],rgb[2])))/mx;

    rgb_out[0]=(sat>=amp)?rgb[0]:((dbg==8)?0.5:0);
    rgb_out[1]=(sat>=amp)?rgb[1]:((dbg==8)?0.5:0);
    rgb_out[2]=(sat>=amp)?rgb[2]:((dbg==8)?0.5:0);
}

void debug_2(double rgb[3], double rgb_out[3], double amp)
{
    debug_2_8(rgb, rgb_out, amp, 2);
}

void debug_8(double rgb[3], double rgb_out[3], double amp)
{
    debug_2_8(rgb, rgb_out, amp, 8);
}

void debug_3(double rgb[3], double rgb_out[3], double amp, double rOG, double gOG, double bOG)
{
    double mx=MAX(rgb[0],MAX(rgb[1],rgb[2]));
    double sat=(mx==0)?0:(mx-MIN(rgb[0],MIN(rgb[1],rgb[2])))/mx;

    double mxOG=MAX(rOG,MAX(gOG,bOG));
    double satOG=(mxOG==0)?0:(mxOG-MIN(rOG,MIN(gOG,bOG)))/mxOG;
    satOG=(amp>=0)?satOG*(1-amp):satOG;
    rgb_out[0]=(sat>=satOG)?rgb[0]:0;
    rgb_out[1]=(sat>=satOG)?rgb[1]:0;
    rgb_out[2]=(sat>=satOG)?rgb[2]:0;
}

void debug_4(double rgb[3], double rOG, double gOG, double bOG, double rgb_out[3], double amp)
{
    double mx=MAX(rgb[0],MAX(rgb[1],rgb[2]));
    double sat=(mx==0)?0:(mx-MIN(rgb[0],MIN(rgb[1],rgb[2])))/mx;

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
}

void debug_5(double rgb_out[3],int x, int row_size, double rOG, double gOG, double bOG, int fv_swt)
{
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

void debug_6(double rgb[3], double rgb_out[3], double rgbs_dbg_six[4], double amp, double rOG, double gOG, double bOG)
{
    double mx=MAX(rgb[0],MAX(rgb[1],rgb[2]));
    double sat=(mx==0)?0:(mx-MIN(rgb[0],MIN(rgb[1],rgb[2])))/mx;
    double OGmx=MAX(rOG,MAX(gOG,bOG));
    double OGsat=(mx==0)?0:(OGmx-MIN(rOG,MIN(gOG,bOG)))/OGmx;
    double redu=OGsat-sat;
    if((redu>rgbs_dbg_six[3])&&(sat<=amp)){
        rgbs_dbg_six[0]=rOG;
        rgbs_dbg_six[1]=gOG;
        rgbs_dbg_six[2]=bOG;
        rgbs_dbg_six[3]=redu;
    }
    rgb_out[0]=(sat<=amp)?rOG:0;
    rgb_out[1]=(sat<=amp)?gOG:0;
    rgb_out[2]=(sat<=amp)?bOG:0;
}

void debug_7(double rgb[3], double rgb_out[3], double amp)
{

int grey=((rgb[0]==rgb[1])&&(rgb[1]==rgb[2]))?1:0;

if (grey==0){

    double dbgHSV[3];
    dbgHSV[1]=1.0;
    dbgHSV[2]=1-amp;

    double dbgRGB[3];

    double hue_d=rgb2hue_360(rgb);
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
    rgb_out[0]=dbgRGB[0];
    rgb_out[1]=dbgRGB[1];
    rgb_out[2]=dbgRGB[2];

}else{
    rgb_out[0]=1.0;
    rgb_out[1]=1.0;
    rgb_out[2]=1.0;
}

}

void debug_9(double rgb[3], double rgb_out[3], double amp, double amp2)
{
    double mx=MAX(rgb[0],MAX(rgb[1],rgb[2]));
    double sat=(mx==0)?0:(mx-MIN(rgb[0],MIN(rgb[1],rgb[2])))/mx;

    rgb_out[0]=(sat>=amp2)?rgb[0]:((sat<amp)?0:0.5);
    rgb_out[1]=(sat>=amp2)?rgb[1]:((sat<amp)?0:0.5);
    rgb_out[2]=(sat>=amp2)?rgb[2]:((sat<amp)?0:0.5);
}
