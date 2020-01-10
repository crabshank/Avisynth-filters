To sort the issue of colours flickering from frame to frame, write your script like so:

Input= //OG Clip
SetFilterMTMode("WhitePoint",...)     //Enable multithreading
 
 Input2=WhitePoint(Input)
Input3=Overlay(Input2,Input,  mode="chroma",ignore_conditional =true,pc_range=true)
Input4=Overlay(Input,Input2,  mode="luma",ignore_conditional =true,pc_range=true)
Input5=Overlay(Input4,Input3,  mode="luma",ignore_conditional =true,pc_range=true)

Input5
   Prefetch(threads=6) //For multithreading
