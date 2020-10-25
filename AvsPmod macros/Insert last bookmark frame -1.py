bookmarks = avsp.GetBookmarkList()
curr=avsp.GetFrameNumber()
if bookmarks:
		bookmarks = list(set(bookmarks))
		bookmarks.sort(reverse=True) 
		for k in range(len(bookmarks)): 
			if bookmarks[k] <= curr:
				curr=bookmarks[k]
				break
		curr = 0 if curr==0 else curr-1
		avsp.InsertText('%i' % 	curr,pos=None)
else:
	curr = 0 if curr==0 else curr-1
	avsp.InsertText('%i' %  curr,pos=None)