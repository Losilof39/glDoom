DISCLAIMER

I make absolutely NO warrantees of any kind for this
program.  I am not responsible for any consequences
of anyone using this program. No animals were harmed
in porting this program. Batteries not included. Don't
run with scissors. You're ugly and your mother dresses
you funny. Who the heck IS John Gault?  Is this thing
on? I can't drive 55! Trespassers will be eaten. Beware
of the cat. 42. Technology, no place for wimps.  I also
have management style hair! So, there!


HELLO


This is my version (so far) of a Win32 port of the Linux
Doom source code released by id Software.  I have gotten
most of the features of the original program working but
it isn't quite ready for primetime.  It does odd things
occasionally, and sometimes sounds just disappear. Probably
because of the sound buffer manager being non-existent.

Please don't E-Mail me with problems you've had with this
unless you can't get it working at all. I already know
of most of the problems you may encounter and I am already
addressing them.  I did want to share what I have done so
far, though.

Also, please don't send me E-Mails asking for features or
changes.  There will be time enough for that after the
game actually works properly.

New revisions will be forthcoming soon.

Also, when I've got all the original Doom features working
and the program stabilized, I will release my source code
for the game.

Also, after I get all the enhancements added to the game
that I want to add, I will release the source code to
that as well.  I've detailed a couple of them below. This
probably won't be before the summer, though. I've got a
LOT planned.

Also, if you're a Doom purist.  Why are you reading this?


REQUIREMENTS


This program is a Win32 application and REQUIRES DirectX 5
(sorry NT folks).  It would have been possible to do what
was done with DirectX 3 but I've already installed DirectX
5 and wanted to use it.  DirectX 5 will be supported in the
5.0 version of NT. (Yes, I KNOW Microsoft says you can write
DirectX 3 compatible software using DirectX 5.  I didn't want
to.)  If you don't have DirectX 5, you can get it here:

http://www.microsoft.com/msdownload/directx5/directx5.htm

If you want to run ANYTHING under Windows95 you really should
have at least 16 Meg of RAM.  32 Meg is MUCH better.

WHAT'S WORKING?

Video - DirectDraw is being used as a flipping surface with
        two surfaces.  The game is rendered into a memory
        area then copied to the back DD buffer.  The palette
        is updated through the DirectDraw surface attached
        palette. All video works.  An initial attempt was
        made to increase the resolution but was not entirely
        successful.  Current screen resolution is 320x200.

Sound - DirectSound is being used for playing the sounds.
        Currently, the sounds play until they are finished
        and do not loop as in the original game.  Nor can
        they currently be stopped. This is being worked on
        and will be functional in a few days.  Volume control
        and panning are both working adequately.  Buffer
        management is non-existent at this point.  It is
        being worked and a proper sound buffer manager will
        be forthcoming.  The volume controls on the menu
        DO NOT WORK YET.  I haven't even looked at them.

Network - The network code is there and appears to be
        functional but no attempt has been made as yet for
        a network game.

Music - I haven't had to the time to get a MUS to MIDI con-
        version written, yet although I do have code to
        play a midi file "doomsong.mid".  There is also a
        little surprise as you read on.


Input - DirectInput and the Windows message loop are used to
        handle keyboard, mouse and joystick input.  Not all
        the keys on the keyboard have been mapped.  Notably
        missing, so far are most of the keys in the numeric
        keypad and some of the navigation keys.  Pause DOES
        work.  The page-up and down keys as well as home,
        end, insert and delete are not normally used anyway.


HOW DO I USE IT?

Create a directory and put the WinDoom.exe program in it
along with the Doom IWAD of your choice  (I've tested this
with all three of the Doom I wads (doom1.wad, doom.wad and
doomu.wad) as well as doom2.wad, tnt.wad and plutonia.wad.

The order of precedence in looking for an IWAD is this:

doom2.wad - retail Doom II wad file
doomu.wad - ultimate Doom I retail wad file
doom.wad  - Doom I registered shareware wad
doom1.wad - Doom I unregistered shareware wad
tnt.wad   - Another Doom II IWAD from id
plutonia.wad - Another Doom II IWAD from id

If you have "Ultimate Doom" you need to rename your IWAD
to doomu.wad.  (it's dumb I know) If you don't, the program
will think that you have the original Doom I loaded and
will try to cycle to a screen that isn't there and die.

When the program is first run, it will create a section in
the win.ini file for WINDOOM that stores the location of
the WinDoom program.  If you move the program or change the
name of the directory in which the program is installed you
need to update this ini file entry or remove it.

An ini file will be created (or updated) in the directory
in which WinDoom is run with the configuration information
for the program. All numeric entries are either boolean
(0 = false, 1 = true) or keyboard scancodes. Modifying
this file is currently the only way to change some of the
keyboard/mouse/joystick mappings.  A way to modify this
in the game or a setup program will be forthcoming.

Make sure your shortcut, if you create one, has the directory
where you installed the program as the working directory.  It
probably won't run otherwise.


BEFORE SOME OF YOU ASK


If you only have the Doom shareware IWAD, you CANNOT use
the Plasma gun or the BFG9000. They just aren't in the
WAD file to use.  You also can't use the double-barreled
shotgun in Doom I.  There are no sprites for those things
in those wads.  It's not something you can change in the
program.


WHAT'S NEW?


A few new things have been added along the way.

1. A print screen key is now always available by pressing
   the (Print Screen/Sys Rq) key on your keyboard.  If you
   aren't playing a game, no message will appear on the
   screen but a print screen PCX file WILL be generated.

2. I have added a console similar to the Quake/Quake II
   console.  It isn't as fancy and no doubt not as functional
   as the Quake console but it works and any keyboard command
   such as the cheats can be entered in the console.  Also,
   all messages that appear on the screen briefly will be
   displayed in the console.  The console key is the "`"
   key.  The following commands are currently implemented:

   iddqd         - god mode
   idfa          - full ammo
   idkfa         - keys full ammo
   idclev        - change level
   idchoppers    - chainsaw and invulnerability
   idclip        - no clipping toggle
   idspispopd    - no clipping toggle
   idbeholdi     - invisibility
   idbeholdv     - invulnerability
   idbeholds     - strength (berserk pack)
   idbeholdr     - radiation/environment suit
   idbeholdl     - light amplification goggles
   idbeholda     - all map cheat
   cd on         - starts your cd player (switches from midi)
   cd off        - stops your cd player (switches to midi)
   cd pause      - pause/resume cd music toggle
   cd resume     - pause/resume cd music toggle
   cd next       - play next song on cd
   cd +          - play next song on cd
   cd prev       - play previous song on cd
   cd -          - play previous song on cd
   cd XX         - plays song number specified in XX 00 to 99
   midi pause    - pauses/resumes the midi playback (toggle)
   midi resume   - pauses/resumes the midi playback (toggle)
   quit          - quits the game (immediately)
   god           - enable god mode (degreelessness?) toggle
   map           - e?m? or map?? (Doom I or Doom II map names)
   give all      - gives full ammo,armor,health,weapons,keys
   give ammo     - gives full ammo
   give armor    - gives full armor
   give health   - give 100% health
   give weapons  - gives all weapons
   give keys     - gives all keys
   give backpack - gives ammo backpack

   A number of other commands are planned for this. Mostly
   just game variables that you can set.

   All game input is dead while the console is active, so
   be warned! The <ESCAPE> key as well as the "`" key will
   terminate the console.  Any command that you have not
   pressed <ENTER> to complete will be thrown away.  The
   menu and console shut each other down when you call
   them up.  They are mutually exclusive.

   The console was done because I wanted to support the CD
   player and there simply weren't enough keys available to
   assign keys to handle this and keep the original cheat
   codes.  Also no menus can be added without hacking the
   original menus to bits and reassembling them.  That was
   too much trouble.  Besides the console is very flexible.
   All sorts of new commands can now be implemented.

3. As you see above, a CD player is now supported.  A number
   of features are planned for this (playlists, directory,
   naming, song naming, etc.)  Trying to play a CD with no
   CD in the drive will do nothing.  Removing the CD from the
   drive while playing will result in the MIDI song being
   restarted.  The CD player will detect if the CD has been
   changed and start playing on the first music track (if
   there is one) and will not play if there are no music
   tracks.

4. Midi file playing. Currently a single MIDI file will be played
   (if it exists) in the game directory. The name of this file is
   doomsong.mid.  It can be any valid MIDI file.  It will be played
   ALL the time unless you turn on the CD player.


WARNINGS!


Context switching (switching to other programs) while playing this
program MAY LOCK YOUR MACHINE UP.  (This is being looked into.)
The fix is probably very simple.  I haven't looked very hard.

This program is by no means ready for prime time but it does work
mostly.  The original music and network play were a large part
of Doom for most of us and this game isn't complete without it. It
IS being worked on.  It may not be possible, however, to play
against other versions of Doom with this code.  I do not have the
specifications for the IPX protocol used by the original Doom.

Supporting DOS Doom in network games is, therefore, doubtful.

I have not tried most of the original command line options to see
if they work or not.  I have spent most of my time writing code
to get the program working.  I will be testing all of these soon.


WHAT'S COMING?


Note the version is on the console on the bottom right.  Something
I "stole" from Quake's console.  It's a good idea with a Win32
game to have SOME way to see what version you are running.

The volume control for music will control which ever music mode
is currently selected when it's done.  It isn't done yet.

I am going to add my OpenGL Doom level viewer as the video output
portion of Doom.  I already have ALL the code to generate the levels
as polygons, OpenGL texture creation and rendering of the level.

I have enclosed a screen shot of the OpenGL Level Viewer so you can
get an idea of what that Doom will look like with an OpenGL renderer.

This will mean replacing most of the current renderer or at least
making major modifications to it so that will be fairly slow in
coming.

Once the game has been converted to a polygon renderer (not as
simple as it sounds), I am going to add the ability to look
(and aim) up and down.  I am also going to add a crosshair.

Before I get ahead of myself, let me say that I have a zillion
other changes in mind but the program needs to get back to the
level of the commercial release before any other modifications
are made.

The CD player was added as a replacement for the missing Midi music.
I know it's not the same but you can pick whatever music you want
while you're playing Doom, now. I like cut 6 on the Quake II CD!

The console was necessary to make the CD interface possible. It
was ridiculously easy to do.  I guess id just hadn't thought of
it when Doom was released or maybe it didn't fit with their
design.  Who knows. You don't have to use if you don't want to.
It's ALWAYS available, too.

There will probably be a Win32 dialog box interface created as
the front-end of the program similar to the Doom95 dialog box.

Most of the features (if not all) of that dialog will be duplicated
since most of them are necessary.  This will be available before
the move to OpenGL or any other modifications.  It's a pity we
didn't get that source code so we wouldn't have to recreate
so much of it.  Oh, well, we learn more this way...


THAT'S ALL FOLKS


Notice the DOOM98 in the top right corner of the screen? Please
don't copy that if you are working on your own Win32 port. Or,
at least make yours look different.  (The console I mean.)  The
look of mine is set and I am not going to change it.

Enjoy it if you can...

Bruce Lewis 
brucel@abraxis.com

