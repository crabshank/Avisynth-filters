import re
import wx
import ctypes as ct

Xstart = 0
Xend = 1.0/2.0
#Frames around top covers the first third of the output width

Ystart = 0
Yend = 0.5
#Frames around top covers the whole output height

class GetPoint(ct.Structure):
    _fields_ = [("x", ct.c_long), ("y", ct.c_long)]

def get_mousepos():
    pt = GetPoint()
    ct.windll.user32.GetCursorPos(ct.byref(pt))
    return int(pt.x), int(pt.y)

wnd = avsp.GetWindow()
script = wnd.currentScript
w, h = script.AVI.Width, script.AVI.Height
dc = wx.ClientDC(wnd.videoWindow)
dc.SetDeviceOrigin(wnd.xo, wnd.yo)
try: # DoPrepareDC causes NameError in wx2.9.1 and fixed in wx2.9.2
    wnd.videoWindow.DoPrepareDC(dc)
except:
    wnd.videoWindow.PrepareDC(dc)
zoomfactor = wnd.zoomfactor
if zoomfactor != 1:
    dc.SetUserScale(zoomfactor, zoomfactor)
xpos,ypos = wnd.videoWindow.ScreenToClient(get_mousepos())
x = dc.DeviceToLogicalX(xpos)
y = dc.DeviceToLogicalY(ypos)

rgb = dc.GetPixel(x, y)
R,G,B = rgb.Get()

# Source - https://gist.github.com/line0/11328940

curr=avsp.GetFrameNumber()
frms=avsp.GetVideoFramecount(index=None)-1

f_x=float(x)
f_y=float(y)
f_w=float(w)
f_h=float(h)

fat_w=Xend-Xstart
fat_h=Yend-Ystart

normX=f_x/f_w #0-1
normY=f_y/f_h #0-1

if normX >= Xstart and normX <= Xend and normY >= Ystart and normY <= Yend:
	normX_fat=(normX-Xstart)/fat_w
	normY_fat=(normY-Ystart)/fat_h
	
	x_ix=0.0
	
	if normX_fat <= 0.2:
		x_ix = 0.0
	elif normX_fat <= 0.4:
		x_ix = 1.0
	elif normX_fat <= 0.6:
		x_ix = 2.0
	elif normX_fat <= 0.8:
		x_ix = 3.0
	else:
		x_ix = 4.0		
		
	y_ix=0.0

	if normY_fat <= 0.2:
		y_ix = 0.0
	elif normY_fat <= 0.4:
		y_ix = 1.0
	elif normY_fat <= 0.6:
		y_ix = 2.0
	elif normY_fat <= 0.8:
		y_ix = 3.0
	else:
		y_ix = 4.0
		
	fr=round(y_ix*5+x_ix-4+float(curr))
	fr=0 if fr<0 else fr
	fr=frms if fr>frms else fr
	fr=int(fr)
	avsp.ShowVideoFrame(framenum= fr, index=None, forceRefresh=False)
	avsp.WriteToScrap('Jumped to: '+str(fr)+'\n', pos=-1)