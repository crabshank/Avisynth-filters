import re
import wx
import ctypes as ct

wnd = avsp.GetWindow()
script = wnd.currentScript

wx.TheClipboard.Open()
pasteText = wx.TextDataObject('')
if wx.TheClipboard.GetData(pasteText):
	pasteText = pasteText.GetText()
# Source - https://gist.github.com/line0/11328940

curr=avsp.GetFrameNumber()

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
			p1='{'+pasteText+', '+str(st)+','+str(ed)+','+ys[5]+','+ys[6]
			p2=txt.replace(y[0], p1)
			avsp.SetText(p2)
			script.GotoLine(len(txl)-x)
			avsp.WriteToScrap(str(st)+' - '+str(ed)+' -> '+pasteText+'\n', pos=-1)
			avsp.ShowVideoFrame(forceRefresh=True)
			break