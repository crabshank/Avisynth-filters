Suggested use:
Create a copy of your clip and apply 'Connected white patches_Avisynth.hlsl' to it.
Set up buttons to jump x frames back/forwards and set bookmarks (simple in AvsPmod).
Jump those x frames and set a bookmark when the desired grey points show up.

Then a file which you specify (file="...") will contain the x and y values and the frame number.

Then compare the generated text file with the frame numbers of the bookmarks (AvsPmod macros/Bookmarks to frame.py will generate a list of the frame numbers of the bookmarks in AvsPmod).

The "thresh" parameter is the saturation value at, and under, which the pixel doesn't show up black (default=0.02).

N.B. If the debug output (most pixels black) doesn't show up with debug=true, refresh or re-open the preview.

Also, if your Avisynth crashes, try changing the file location to one which does not require admin privileges.
