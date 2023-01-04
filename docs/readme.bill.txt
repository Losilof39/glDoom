
This program is a VERY early alpha of glDoom.

It will NOT play through levels properly. (go from one level to another)

There are a LOT of things that need work or just have not been done so don't take
anything you see here as finished.

The 3Dfx version using MESA has a lot of visual artifact problems and inaccuracies
in its calculations.  The "cracks" you see in the sky seams will not be there in the
final version.

The renderer is using a very low resolution span renderer for VSD and so does
not properly draw everything.  Also, the floors/ceilings are not optimized at all
so they all draw all the time.

There are still some levels which just will not work at all.  One of these is E1M2.
Don't even try it.  The other levels in Episode 1 of Doom will work though.  My
suggestion is to setup your shortcut to go directly to E1M1 or E1M7 and use
the -nomonsters flag. 

It seems there are even more errors in Doom II levels than there are in Doom I
levels (geometry wise).  Therefore over a third of the Doom II levels won't run
at all.  I'm working on this.

There ARE levels which will work for each of the skies.  You will have to do
some experimentation to find the ones you need.

There are a number of features that are turned off right now to make this a
more stable platform for testing the skies so don't be surprised if they
arent' there.  (like the dynamic sprite loader)

The key for switching between a 4 sided and an 8 sided sky is the apostrope ' key. 
The default keys for looking up and down are the PAGEDN and DECIMAL keys on
the numeric keypad.  These can be rebound through the console using the key
definitions of key_lookup and key_lookdown.  The key_mlook is disabled right
now because of a problem with the mouse code.

The rendering engine has not been updated to alter the VSD to account for the
ability to look and down so don't be surprised when walls "disappear" if you
look up or down when close to them.

Also, DirectX 5 is required to run this because of DirectInput.

The skytop texture should be in a file called "sky1top.bmp" and should be in
the program directory.  I have a scheme for loading the skies into new PAK
type files for this but have not implemented it yet. Also, the BMP file must
not be a compressed BMP. I didn't bother with the code to unpack an RLE
format. (The sky1top.bmp you sent me was uncompressed so this should not be
a problem.)

The file "opengl32.dll" which is included with this is the MESA version of an
OpenGL like driver for 3Dfx cards only.  The program defaults to 640x480
resolution so you don't have to give a command line parameter for that.

Here is the suggested command line:

gldoom -nomonsters -warp 1 1

Let me know which you think looks better (4 or 8 sided sky).

Cheers,
Bruce