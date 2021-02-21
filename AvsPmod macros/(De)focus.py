import re

wnd = avsp.GetWindow()
script = wnd.currentScript

curr=avsp.GetFrameNumber()

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
		pos = script.GetSelectionEnd()
		script.GotoPos(pos)
		avsp.ShowVideoFrame(framenum=ff , index=None, forceRefresh=True)
		break