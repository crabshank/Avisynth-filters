import re
bookmarks = avsp.GetBookmarkList()
curr=avsp.GetFrameNumber()
found=0
txt=avsp.GetText(index=None, clean=False)
txl=txt.splitlines()
txl.reverse()

if bookmarks:
		bookmarks = list(set(bookmarks))
		bookmarks.sort(reverse=True) 
		for k in range(len(bookmarks)): 
			if bookmarks[k] <= curr:
				curr=bookmarks[k]
				break

for x in range(len(txl)):
	y=re.findall('{\s*\-{0,1}\d{1,5}\s*,\s*\-{0,1}\d{1,5}\s*,\s*\-{0,1}\d{1,5}\s*,\s*\d+\s*,\s*\d+\s*,\s*\d+\.*\d*\s*,\s*\d+\.*\d*\s*}',txl[x])
	if len(y)>0: #if find match
		ys=y[0].split(',')
		if ys[3].strip()==str(curr):
			found=1
			break

if found==0:
	avsp.InsertText('%i'%curr,pos=None)
else:
	avsp.InsertText('%i [WP]'%curr,pos=None)