N.B. For speed use "White Point Avisynth.hlsl" with https://github.com/mysteryx93/AviSynthShader instead.

Change the white point of the frame. All the dependencies required to compile this can be found in Auto White Point's folder.

Debug modes:

0 - Off
1 - pow(Saturation, debug_val)
2 - Outputs all pixels >= debug_val (0-1), and blacks out all pixels < debug_val in saturation.
3 - Outputs, in black, all pixels that have had at least a (debug_val)% reduction in saturation.
