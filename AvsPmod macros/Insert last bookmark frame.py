bookmarks = avsp.GetBookmarkList()
if bookmarks:
	bookmarks = list(set(bookmarks))
	bookmarks.sort() 
	avsp.InsertText('%i' % bookmarks[-1],pos=None)
else:
	avsp.InsertText('%i' %  avsp.GetFrameNumber(),pos=None)
