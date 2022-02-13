curr=avsp.GetFrameNumber()
frms=avsp.GetVideoFramecount(index=None)-1
bookmarks = avsp.GetBookmarkList()
#wnd.zoomfactor = 0.9
if bookmarks:
		bookmarks = list(set(bookmarks))
		bookmarks.sort(reverse=True) 
		for k in range(len(bookmarks)): 
			if bookmarks[k] <= curr:
				curr=bookmarks[k] #curr := last bookmark if there are any
				break

avsp.InsertText('\n{-1,-1,-1 ,%i,%i,0.312727,0.329023}'%(curr,frms),pos=None)