#pragma once

#define CURRENT_FILE_PATH Path_To_File_h

#define COMBINE_MACRO_3(A,B,C) A##B##C

#define MSTRUCT(...) COMBINE_MACRO_3(CURRENT_FILE_PATH,_,__LINE__)