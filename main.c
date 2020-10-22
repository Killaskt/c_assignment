// Include Block
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Preprocessor declarations
#define BUF_SIZE 10 /* global constant buffer size:
						  Generally this would be much larger, but I want to show you
						  that it works in a loop until the entire file is read.*/

// main function
int main(int argc, char *argv[])
{	

	// Variable declarations
	int fd;						// file descripter
	char buffer[BUF_SIZE];		// string for buffer between read and write
	int start = atoi(argv[1]);
	int end = atoi(argv[2]);
	long n;		// amount read by read()
	int m = 1;
	
  // error handling checks before actually running code
    if (argc != 4) {
		write(STDOUT_FILENO, "Needs to be in following format: <output file> <start line> <end line> <file to be read>\n", 150);
		return 1;
	}
					
	if (start > end) {
		write(STDOUT_FILENO, "the 1st arg cannot be more than the 2nd\n", 50);
		return 1;
	}

	if (start < 1) {
		// Report error opening
		write(STDERR_FILENO, "1st arg cant be 0 or less than 0\n", 50); 
		return 1;
	}

	// Open the file given as an argument by the user
	if ((fd = open(argv[3], O_RDONLY)) == -1 ) {
		// Report error opening
		write(STDERR_FILENO, "Error opening file\n", 30); 
		return 1;
	}

	// Loop through until nothing is read
	while ( (n = read(fd, buffer, 1)) > 0 ) {	

		if (buffer[0] == '\n') { //check for the end of the line being read
			m++;
			if ((m > start) && (m <= end)) { //if between n and m print a newline
				write(STDERR_FILENO, "\n",1); //print it
			} 
		}
		
		else if (m < start) {
            continue;
		}

		else if (m > end){
			write(STDOUT_FILENO, "\n", 1);
            break;
		}

		else{
			if( write(STDOUT_FILENO, buffer, n) != n )
			{
				//report error writing
				write(STDERR_FILENO, "Error writing character.\n", 40);
				return 3;
			}
		}
	}

	// Error check
	if(n == -1)
	{
		// report error reading
		write(STDERR_FILENO, "Error reading character.\n", 24);
		return 2;
	}

	/* If we got here that means readAmt=0, meaning it successfully read and wrote
	   the whole file without error, and there is nothing left to read. */

	// Close the file
	close(fd);

	// Success:
	return 0;
}
