curr_1=avsp.GetFrameNumber()-1
curr_1 = 0 if curr_1<=0 else curr_1
avsp.InsertText('%i' %  curr_1,pos=None)