import re

wnd = avsp.GetWindow()
script = wnd.currentScript

curr=avsp.GetFrameNumber()
bk=curr
txt=avsp.GetText(index=None, clean=False)
txl=txt.splitlines()
txl.reverse()
swt=0;
for x in range(len(txl)):
	y=re.findall('.*\s+focusser\s*=\s*\d{1}',txl[x])
	if len(y)>0: #if find match
		ys=y[0].split('=')
		pos = script.GetSelectionEnd()
		if str(ys[1]).strip()=='0':
			avsp.SetText(txt.replace(y[0], 'global focusser = 1'))
			swt=1
		else:
			avsp.SetText(txt.replace(y[0], 'global focusser = 0'))
			swt=0
		script.GotoPos(pos)
		break
		
txt=avsp.GetText(index=None, clean=False)
txl=txt.splitlines()
txl.reverse()
		
for x in range(len(txl)):
	y=re.findall('.*\s+focus_frame\s*=\s*\d{1,}',txl[x])
	if len(y)>0: #if find match
		ff=int(y[0].split('=')[1].strip())
		if swt==1:
			avsp.SetText(txt.replace(y[0], 'global focus_frame = '+str(curr)))
		break
		

bookmarks = avsp.GetBookmarkList()

if bookmarks:
		bookmarks = list(set(bookmarks))
		bookmarks.sort(reverse=True) 
		for k in range(len(bookmarks)): 
			if bookmarks[k] <= curr:
				bk=bookmarks[k] #curr := last bookmark if there are any
				break

txt=avsp.GetText(index=None, clean=False)
txl=txt.splitlines()
txl.reverse()

for x in range(len(txl)):
	y=re.findall('.*\s+last_bk\s*=\s*\d{1,}',txl[x])
	if len(y)>0: #if find match
		ff=int(y[0].split('=')[1].strip())
		posi=len(txl)-x
		if swt==1:
			avsp.SetText(txt.replace(y[0], 'global last_bk = '+str(bk)))
		break
		
if swt==0:		
	txt=avsp.GetText(index=None, clean=False)
	txl=txt.splitlines()
	txl.reverse()

	for x in range(len(txl)):
		y=re.findall('.*\s+focus_frame\s*=\s*\d{1,}',txl[x])
		if len(y)>0: #if find match
			ff=int(y[0].split('=')[1].strip())
			posi=len(txl)-x
			avsp.ShowVideoFrame(framenum=ff , index=None, forceRefresh=True)
			script.GotoLine(posi)
			break
else:
	avsp.ShowVideoFrame(framenum=ff , index=None, forceRefresh=True)
	script.GotoLine(posi)
