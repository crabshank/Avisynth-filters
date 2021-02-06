fr=avsp.GetSelectedText(index=None)
if not fr:
	avsp.ShowVideoFrame(framenum=None, index=None, forceRefresh=True)
else:
	avsp.ShowVideoFrame(framenum= int(fr), index=None, forceRefresh=True)