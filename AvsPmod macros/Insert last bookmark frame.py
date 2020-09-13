bookmarks = avsp.GetBookmarkList()
curr=avsp.GetFrameNumber()
if bookmarks:
		bookmarks = list(set(bookmarks))
		bookmarks.sort(reverse=True) 
		for k in range(len(bookmarks)): 
			if bookmarks[k] <= curr:
				curr=bookmarks[k]
				break
		avsp.InsertText('%i' % 	curr,pos=None)
else:
	avsp.InsertText('%i' %  curr,pos=None)