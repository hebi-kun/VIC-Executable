#ifndef _FILEOPERATIONS_2009
#define _FILEOPERATIONS_2009

/* Header file that provides a read and storage for the structure heightFiled
 */

#define MAXWIDTH 257
#define MAXDEPTH 257

extern float heightField[MAXWIDTH][MAXDEPTH];
extern int heightField_width;
extern int heightField_depth;

// findFile:   Find a file, either in current directory, or in parent.
//     If we ask to find a file like "foo.bmp" the return value
//       will be "./foo.bmp"  or "../foo.bmp".
//       (or "" if file wasn't found).
char  *findFile        (char *fname);

// readHeightField:
//   Read a file containing a height field, returning
//     width / depth of the field through the parameter list,
//     along with the array of heights.
//     The caller should declare
//          int heights[MAXDEPTH][MAXDEPTH] 
//  Return TRUE if file was read, FALSE otherwise.
int readHeightField  (const char *filename);

#endif   /* _FILEOPERATIONS_ */

