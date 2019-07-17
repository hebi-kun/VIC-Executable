//fileoperations.cpp

#define _CRT_SECURE_NO_DEPRECATE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileOperations.h"

float heightField[MAXWIDTH][MAXDEPTH];
int heightField_width;
int heightField_depth;

char* findFile(char *fname)
{
   // Find a file 'fname', searching in current directory and also parent.
   // This helps beat Visual Studio's irritating habit of putting the
   // .EXE in a different folder to all the other files.
   // leaks small amout of memory - dont put it in a large loop
	FILE *stream;
   char *fileFound;

   fileFound = (char *)(calloc(strlen(fname)+4, sizeof(char)));
   // First search for './fname'...
	strcpy(fileFound, "./");
	strcat(fileFound, fname);
	if( (stream  = fopen( fileFound, "r" )) != NULL )
	{
	   fclose(stream);
		return fileFound;
	}

   // If got to here, then 'fname' wasn't found in current directory.
   // So search for '../fname'...
   strcpy(fileFound, "../");
	strcat(fileFound, fname);	
	if( (stream  = fopen( fileFound, "r" )) != NULL )
		{
			fclose(stream);
			return fileFound;
		}
   
   // If got to here, then 'fname' wasn't found at all.
   printf("findFile: Unable to find file %s\n", fname);
	fileFound[0]=0; // return empty string
	return fileFound;
}    // End of findFile().



int readHeightField (const char *filename)
// readHeightField reads in height field data from a file
// return value is 0 if successful, 1 if fail.
// Width, depth & heights are returned as a call-by-reference.
#define giveUp(message) {fclose(inputFile); printf("%s\n", message); return false;}
{

  FILE   *inputFile;
  int    nRead, i, j;

  // Open height field data file for input, barf on error...
  //printf("Opening height field file \"%s\" for input...\n", filename);
  inputFile = fopen(filename, "r");
  if (inputFile == NULL)             
  {
    //printf("  Unable to open file for input\n");
    return false;
  }

  // Read first line, it should be width & depth...
  nRead = fscanf(inputFile, "%d %d", &heightField_width, &heightField_depth);
  if (nRead != 2 )
    giveUp("  Couldn't read width & height from file.");
  //printf("  width, depth:  %d x %d\n", heightField_width, heightField_depth);
  if (heightField_width <= 0 || heightField_depth <= 0)
    giveUp("  Invalid value for width or depth..");

  // Now loop over lines containing height data...
  printf("  Reading heights...\n");
  for (j=0; j<heightField_depth; j++)
  {
    for (i=0; i<heightField_width; i++)
    {
      nRead = fscanf_s(inputFile, "%f", &heightField[i][j]);
      if (nRead != 1)
        giveUp("  Bad or incomplete height data.");
	  //printf("%6.1f ",heightField[i][j]);
    }
	//printf("\n\n");
  }
  fclose(inputFile);
  //printf("  Height field done. %d x %d\n",heightField_width, heightField_depth);
  return true;
}                         // End of readHeightField
