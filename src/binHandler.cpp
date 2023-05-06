/*******************************************************************************
* This file is part of psx-comBINe. Please see the github:
* https://github.com/ADBeta/psx-comBINe
*
* psx-comBINe is a simple program to combine multiple .bin files into a single
* file, and modified the .cue file indexing, this is ideal for PSX/PS1 CD-ROMs
* e.g. Rayman to get them ready for cue2pops or some emulators. I also find it 
* improves reliabilty when buring to a disk to only have one .bin file.
*
* (c) ADBeta
*******************************************************************************/
#include <iostream>
#include <vector>
#include <fstream>

#include "helpers.hpp"
#include "binHandler.hpp"
//Each entity is at which byte a file transistion to the next file. This is for
//.cue output INDEX value (in time format which gets converted later)
std::vector<unsigned long> binOffsetBytes;

int dumpBinFiles(std::vector<std::string> &binVect, const std::string& outFn) {
	/*** Setup ***/
	//Input and output .bin files.
	std::fstream binFileOut(outFn.c_str(), std::ios::out | std::ios::binary);

	std::cout << "creating " << outFn << std::endl;
	if(!binFileOut) errorMsg(2, "dumpBinFiles", 
	                         "Cannot create output .bin file. Check privelages");
	
	//Total bytes from all files
	size_t totalBytes = 0;
	
	//Create a heap byte array (From defined size in header)
	std::string byteArray;
	byteArray.resize(_def_ARR_SIZE);
	//TODO error handle
	//errorMsg(2, "Failed to allocate the bin buffer array (Requires 100MiB free RAM)");
	
	
	/*** runtime ***/
	//Go through all bin filenames in the vector of strings
	for(size_t indx = 0; indx < binVect.size(); indx++) {
		//bytes in each file.
		size_t fileBytes = 0;
		//Print message about file (flush to ensure the text is printed first)
		std::cout << "Dumping: " << binVect.at(indx) << "    " << std::flush;
		
		//Open input binary file to the current string in the vecor.
		std::fstream binFileIn(binVect[indx], std::ios::in | std::ios::binary);
		//error check
		if(!binFileIn) {
			errorMsg(2, "dumpBinFiles", "Can not open " + binVect[indx]);
		}
		
		//Log the Offset byte of the current bin file (starts at 0)
		binOffsetBytes.push_back(totalBytes);
		
		//Get all the bytes from the current input and push them to output.

		while(binFileIn.read(byteArray.data(), byteArray.size())) 
		{
			//Current bytes in array
			const auto arrBytes = binFileIn.gcount();
			
			//Dump buffer to the output file.
			binFileOut.write(byteArray.data(), arrBytes);
			
			//Keep track of how many bytes read so far, fileByte gets reset at
			//next loop, totalBytes does not get reset 
			totalBytes += arrBytes;
			fileBytes += arrBytes;
		}
		
		
		//Report how many megabytes the file is, that it is done, then reset.
		std::cout << padMiBStr(fileBytes, 3) << std::endl;

	
		fileBytes = 0;
	}
	//Flush what is left of the byte array to the output file
	//if(arrBytes != 0) binFileOut.write(byteArray.data(), arrBytes);

	
	//Print message that the outfile is waiting to finish wiring
	std::cout << "\nFinishing write to file ..." << std::flush;
	
	//Flush the output file
	binFileOut.flush();
	
	//Report finished writing
	std::cout << " Done" << std::endl;
	
	//Report completion and bytes written to the output bin file
	std::cout << "Successfully dumped " << padByteStr(totalBytes) << " to " 
	          << outFn << std::endl;
	//Return 0 for success
	return 0;
}
