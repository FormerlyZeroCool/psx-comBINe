# psx-comBINe
<b> (c) ADBeta 2023 </b>  

psx-comBINe is a program to combine/merge multiple .bin files from a psx .cue  
.bin CD-ROM dump, into a single .cue and .bin file.  

This function is commonly done via CDMage, or other tools in the PSX community,  
but having a purpose built tools seems like a useful addition to the community  
I love so much.  

This project is just a hobby, and is designed for Linux to begin with. I will  
eventually port it to a Windows exe, feel free to do this with the files if you  
want to test it out.  

This program depends boost::filesystem, make sure this is installed before use  
If this causes problems with older systems, feel free to create your own fork,  
as my code needs to be as universal as possible - fileHandler.cpp uses boost  
for directory creation and detection.  

## Compilation

## How To Use

## TO-DO
* binary file append handler
* help flag
* user input protection
* if a .cue FILE is not recognised, pause and ask for confirm rather than exit.
* ^^ Add a 'continue y/N' prompt function to call any time
* make sure program can handle .BIN .bin .CUE and .cue files.
* Parent directory handler may not play nice with windows. make a test for this  
before full deploy.  
* Create user input handler for program args
* Add some error checking for dots and slashes in a few functions that can take  
user input - setupOutputFiles  

## Changelog (Reminder for self)
* 0.3.1 - .cue file lines get detected and stripped of file info, with error  
checking  
stored in the binFile vector ready for append and dump.
* 0.4.3 - Moved some variables and types to common.h
* 0.4.5 - Included boost in make and src files. 
* 0.5.5 - Implimented filesystem managment and file creation
* 0.6.5 - Added error message system
* 0.7.5 - Added continue prompting function.

## Licence
This software is under the GPL (GPL3.0), please see LICENCE for information  
