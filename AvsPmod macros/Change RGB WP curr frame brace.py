import re
import wx
import ctypes as ct

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

frms=avsp.GetVideoFramecount(index=None)
sw=0
cg=0
txt=avsp.GetText(index=None, clean=False)
txl=txt.splitlines()
txl.reverse()

for x in range(len(txl)):
	y=re.findall('.*\s+focusser\s*=\s*\d{1}',txl[x])
	if len(y)>0: #if find match
		ys=y[0].split('=')
		if str(ys[1]).strip()!='0':
			cg=1
			break

for x in range(len(txl)):
	y=re.findall('.*\s+focus_frame\s*=\s*\d{1,}',txl[x])
	if len(y)>0: #if find match
		ff=int(y[0].split('=')[1].strip())
		if cg==1:
			curr=ff
		break

for x in range(len(txl)):
	y=re.findall('{\s*\-{0,1}\d{1,5}\s*,\s*\-{0,1}\d{1,5}\s*,\s*\-{0,1}\d{1,5}\s*,\s*\d+\s*,\s*\d+\s*,\s*\d+\.*\d*\s*,\s*\d+\.*\d*\s*}',txl[x])
	if len(y)>0: #if find match
		ys=y[0].split(',')
		st=int(ys[3].strip())
		ed=int(ys[4].strip())
		if curr >= st and curr<=ed:
			sw=1
		elif curr >= st and ed==0:
			sw=1
		
		if sw==1:
			p1='{'+str(R)+', '+str(G)+', '+str(B)+', '+str(st)+','+str(ed)+','+ys[5]+','+ys[6]
			p2=txt.replace(y[0], p1)
			avsp.SetText(p2)
			script.GotoLine(len(txl)-x)
			avsp.WriteToScrap(str(st)+' - '+str(ed)+' -> '+str(R)+','+str(G)+','+str(B)+'\n', pos=-1)
			avsp.ShowVideoFrame(forceRefresh=True)
			break