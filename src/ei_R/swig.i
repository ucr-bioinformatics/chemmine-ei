%{
#include "desc.h"
#include "simpledb.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include "db_build.h"
#include <vector>
#include <iterator>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include "solver.h"


int db2db_distance2file(char *dbFile, char *iddb1File,char *iddb2File,char *outfile);
int db2db_distance2file(char *dbFile, char *db2File,char *outfile);
int binaryCoord(char *inFile,char *outFile,int dim);
Solver* getSolver(int r,int d, double *refCoords);
int db_subset(char* dbFile, char* iddbFile, char* outputFile);
%}

int batch_sdf_parse(const char* sdfile, const char* dbfile);
int db2db_distance2file(char *dbFile, char *iddb1File,char *iddb2File,char *outfile);
int db2db_distance2file(char *dbFile, char *db2File,char *outfile);
int binaryCoord(char *inFile,char *outFile,int dim);
int db_subset(char* dbFile, char* iddbFile, char* outputFile);
Solver* getSolver(int r,int d, double *refCoords);
