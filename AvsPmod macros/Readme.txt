Bookmarks to frame.py - Save a .txt list of the frame numbers of the bookmarks.

Insert last bookmark frame.py - Inserts the last bookmark's frame number, or if there are no bookmarks: the current frame's number.

Jump to selected frame(_no refresh).py - Select text containing a number and it will jump to that number frame with or without refreshing, if nothing is selected it will just refresh or do nothing. Jump to selected frame.py can replace 'Refresh preview' in AvsPmod.

Insert new braces WP - curr - focus.py - Adds brace and changes previous brace depending on the position of the last bookmark, and sets the latest brace to the RGB valuse that the cursor was over when the macro was triggered. Also checks the script for 'focusser' and 'last_bk' (for use with De(focus).py and focusOnFrame {function}). N.B. this macro needs at least 2 braces already there to work.

Mouse select frames around top.py - Set X/Ystart and X/Yend to specify what part of the of the video preview is covered by the framesAroundTop function, then you can place the cursor over the frame you want to jump to and trgger the macro to jumps to that frame.

Change RGB WP curr frame brace.py - Changes RGB value for the brace that contains the current frame (or focus_frame if focusser = 1) to the RGB value that the cursor is over.

Change RGB WP curr frame brace_paste.py - Changes RGB value for the brace that contains the current frame (or focus_frame if focusser = 1) to what is in the clipboard (should be an RGB triplet).

Insert last bookmark frame_match start brace.py & Insert last bookmark frame -1 plus newline last bk_match start brace.py - Insert the last bookmarked frame or, last bookmarked frame - 1 + new line + last bookmarked frame, and adds " [WP]" if the last bookmarked frame is a start frame for a brace of Manual_WP or just a space if not.
