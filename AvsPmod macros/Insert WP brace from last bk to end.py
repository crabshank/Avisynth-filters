import re
import wx

wnd = avsp.GetWindow()
script = wnd.currentScript

wx.TheClipboard.Open()
pasteText = wx.TextDataObject('')
if wx.TheClipboard.GetData(pasteText):
	pasteText = pasteText.GetText()
# Source - https://gist.github.com/line0/11328940

curr=avsp.GetFrameNumber()
bookmarks = avsp.GetBookmarkList()
#wnd.zoomfactor = 0.9
if bookmarks:
		bookmarks = list(set(bookmarks))
		bookmarks.sort(reverse=True) 
		for k in range(len(bookmarks)): 
			if bookmarks[k] <= curr:
				curr=bookmarks[k] #curr := last bookmark if there are any
				break

frms=avsp.GetVideoFramecount(index=None)-1
bookmarks = avsp.GetBookmarkList()
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
	y=re.findall('.*\s+last_bk\s*=\s*\d{1,}',txl[x])
	if len(y)>0: #if find match
		ff=int(y[0].split('=')[1].strip())
		if cg==1:
			curr=ff
			frms=0
		break

for x in range(len(txl)):
	y=re.findall('{\s*\-{0,1}\d{1,5}\s*,\s*\-{0,1}\d{1,5}\s*,\s*\-{0,1}\d{1,5}\s*,\s*\d+\s*,\s*\d+\s*,\s*\d+\.*\d*\s*,\s*\d+\.*\d*\s*}',txl[x])
	if len(y)>0: #if find match
		ys=y[0].split(',')
		x2=x+1 if x<len(txl)-1 else x
		y2=re.findall('{\s*\-{0,1}\d{1,5}\s*,\s*\-{0,1}\d{1,5}\s*,\s*\-{0,1}\d{1,5}\s*,\s*\d+\s*,\s*\d+\s*,\s*\d+\.*\d*\s*,\s*\d+\.*\d*\s*}',txl[x2])
		#y2s=y2[0].split(',')
		if len(y2)>0:
			p1=str(ys[0])+','+str(ys[1])+','+str(ys[2])+','+str(ys[3])+','+str(ys[4])+','+str(ys[5])+','+str(ys[6])          
			p2='{'+pasteText+', '+str(curr)+','+str(frms)+',0.312727,0.329023}'
			if str(ys[3]).strip()==str(curr).strip():
				p3=txt.replace(y[0], p2)
			else:
				p3=txt.replace(y[0], p1+'\n'+p2)

			avsp.SetText(p3)
			script.GotoLine(len(txl)-x)
			avsp.WriteToScrap('On brace from '+str(curr).strip()+' -> '+pasteText+'\n', pos=-1)
			avsp.ShowVideoFrame(forceRefresh=True)
			break