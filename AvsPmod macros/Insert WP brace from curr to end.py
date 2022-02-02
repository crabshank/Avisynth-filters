curr=avsp.GetFrameNumber()
frms=avsp.GetVideoFramecount(index=None)-1
avsp.InsertText('\n{-1,-1,-1 ,%i,%i,0.312727,0.329023}'%(curr,frms),pos=None)