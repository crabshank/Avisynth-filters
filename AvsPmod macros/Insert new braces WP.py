frms=avsp.GetVideoFramecount(index=None)
bookmarks = avsp.GetBookmarkList()
curr=avsp.GetFrameNumber()
if bookmarks:
		bookmarks = list(set(bookmarks))
		bookmarks.sort(reverse=True) 
		for k in range(len(bookmarks)): 
			if bookmarks[k] <= curr:
				curr=bookmarks[k]
				break
		avsp.InsertText('\n{ ,%i,%i,0.312727,0.329023}' % (curr,frms),pos=None)
else:
	avsp.InsertText('\n{ ,%i,%i,0.312727,0.329023}' % (curr,frms),pos=None)