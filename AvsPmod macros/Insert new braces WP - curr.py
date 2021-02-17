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
bookmarks = avsp.GetBookmarkList()

if bookmarks:
		bookmarks = list(set(bookmarks))
		bookmarks.sort(reverse=True) 
		for k in range(len(bookmarks)): 
			if bookmarks[k] <= curr:
				curr=bookmarks[k] #curr := last bookmark if there are any
				break

frms=avsp.GetVideoFramecount(index=None)
bookmarks = avsp.GetBookmarkList()
txt=avsp.GetText(index=None, clean=False)
txl=txt.splitlines()
txl.reverse()
for x in range(len(txl)):
	y=re.findall('{\s*\-{0,1}\d{1,5}\s*,\s*\-{0,1}\d{1,5}\s*,\s*\-{0,1}\d{1,5}\s*,\s*\d+\s*,\s*\d+\s*,\s*\d+\.*\d*\s*,\s*\d+\.*\d*\s*}',txl[x])
	if len(y)>0: #if find match
		ys=y[0].split(',')
		x2=x+1 if x<len(txl)-1 else x
		y2=re.findall('{\s*\-{0,1}\d{1,5}\s*,\s*\-{0,1}\d{1,5}\s*,\s*\-{0,1}\d{1,5}\s*,\s*\d+\s*,\s*\d+\s*,\s*\d+\.*\d*\s*,\s*\d+\.*\d*\s*}',txl[x2])
		#y2s=y2[0].split(',')
		if len(y2)>0:
			p1=str(ys[0])+','+str(ys[1])+','+str(ys[2])+','+str(ys[3])+','+str(curr-1)+','+str(ys[5])+','+str(ys[6])          
			p2='{'+str(R)+', '+str(G)+', '+str(B)+', '+str(curr)+','+str(frms)+',0.312727,0.329023}'
			if str(ys[3]).strip()==str(curr).strip():
				p3=txt.replace(y[0], p2)
			else:
				p3=txt.replace(y[0], p1+'\n'+p2)

			pos = script.GetSelectionEnd()
			avsp.SetText(p3)
			script.GotoPos(pos)
			avsp.ShowVideoFrame(forceRefresh=True)
			break
