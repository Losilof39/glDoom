/* entity.c */

/* 
** This file is intended to take the place
** of the info.c file when it is finished.
*/

/* Theory

   We can take the data from info.c and put it into
   a compiled "lump" or series of lumps that controls
   the entities in the game.

   This can be accomplished by tokenizing all the data
   and the functions called then creating runtime "classes"
   that use the hardcoded function calls that were designated
   in the p-code.

   The controlling data would still be available by filling
   in the data structure for each entity type and individual
   "thinker" for the individual entities.

   But the data would be controllable by anyone with an editor.
   Care must be taken, though that a checksum of some sort be
   performed so that multiplayer games use exactly the same
   game data (and programming).

   The following is just some "noodling" on my part so far...

*/ 

/* structures would be moved into entity.h when the times comes */

/*
   Info.c describes a series of "states" for the entities in the
   game.  Each state carries certain information.

   Types of things controlled by info.c are:

   sounds produced
   sprite frames used
   sprite frame timing
   next "state"
   entity state function called
*/


typedef struct
   {
    char x;
   }action_t;

typedef struct
   {
    char x;
   }entity_t;
