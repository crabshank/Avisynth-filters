import re

frms=avsp.GetVideoFramecount(index=None)
bookmarks = avsp.GetBookmarkList()
curr=avsp.GetFrameNumber()
txt=avsp.GetText(index=None, clean=False)
txl=txt.splitlines()
txl.reverse()
for x in range(len(txl)):
	y=re.findall('{\s*\-{0,1}\d{0,5}\s*,\s*\-{0,1}\d{0,5}\s*,\s*\-{0,1}\d{0,5}\s*,\s*\d+\s*,\s*\d+\s*,\s*\d+\.*\d*\s*,\s*\d+\.*\d*\s*}',txl[x])
	if len(y)>0: #if find match
		ys=y[0].split(',')
		x2=x+1 if x<len(txl)-1 else x
		y2=re.findall('{\s*\-{0,1}\d{0,5}\s*,\s*\-{0,1}\d{0,5}\s*,\s*\-{0,1}\d{0,5}\s*,\s*\d+\s*,\s*\d+\s*,\s*\d+\.*\d*\s*,\s*\d+\.*\d*\s*}',txl[x2])
		y2s=y2[0].split(',')
		if len(y2)>0:
			p1=str(ys[0])+','+str(ys[1])+','+str(ys[2])+','+str(ys[3])+','+str(curr-1)+','+str(ys[5])+','+str(ys[6])          
			p2='{ -1, -1, -1 ,'+str(curr)+',0,0.312727,0.329023}'
			p3=txt.replace(y[0], p1+'\n'+p2)
			avsp.SetText(p3)
			break