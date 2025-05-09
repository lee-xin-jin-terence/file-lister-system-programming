
/***********************************
*
* File name: myls.c
*
* author: Terence Lee Xin Jun
*
* Aim: List the file information
*
* Date: 25 April 2020
* 
***********************************/


//For opendir(), closedir(), readdir(), 
// rewinddir()
#include <dirent.h>


//For 'errno' variable
#include <errno.h>


//For getgrgid()
#include <grp.h>


//For getpwuid()
#include <pwd.h>


#include <stdio.h>


//for strerror(), strcmp(), strcpy()
#include <string.h>


//For lstat()
#include <sys/stat.h>


//For gnu_dev_major(), gnu_dev_minor()
#include <sys/sysmacros.h>


//For lstat(), opendir(), closedir(),
// getpwuid(), getgrgid(), rewinddir
#include <sys/types.h>


//For localtime_r()
#include <time.h>


//For lstat()
#include <unistd.h>




#define MAX_STRING_SIZE 1024



	/*------------------------------------------------
	 Brief: Display the file information of all the 
		files in the current directory

		If there is a problem in opening the
		current directory, an error message will
		be displayed instead
	------------------------------------------------*/
void displayCurrDirFilesInfo();


	/*-----------------------------------------------
	 Brief: Display the file information of the 
		current file. File information includes
		the user name, group name, type of file,
		full access permissions.

	 Parameters:
		fileName - the name of the file of 
			interest, where the information
			regarding the file will be
			displayed
	------------------------------------------------*/
void displayCurrFileInfo(const char * fileName);



	/*-----------------------------------------------
	 Brief: Converts a numerical value of file type
		to its corresponding string form


	 Parameters:
		fileTypeString - This contains the string
			form of a file type. This is the
			output of this function 

		fileTypeAndPermsFlags - this contains the
			numerical value of a file type,
			which is to be converted by the
			function into the corresponding
			file type in string form
	------------------------------------------------*/
void getFileTypeString(char * fileTypeString, 
		      int fileTypeAndPermsFlags);
		

	/*-------------------------------------------------
	 Brief: Converts a numerical value of file access 
		permissions to its corresponding string
		form

		Eg: In string form,
		'rwxrwxr-x'  represents 
		read-write-execute permissions for owner(user)
		read-write-execute permissions for group
		read-execute permissions for others
		

	 Parameters:
		filePermsString - this is the output of the
			function. It contains a nine-letter
			string containing the file access
			permissions of a file

		fileTypeAndPermsFlags - this is the
			 numerical value containing the
			 file access permissions. It is used
			 to convert to its corresponding 
			 string form and stored in 
			 'fileTypeString' 
	-------------------------------------------------*/
void getFilePermissionsString(char * filePermsString ,
			mode_t fileTypeAndPermsFlags);


	   

	/*--------------------------------------------------
	 Brief: Converts a numerical value of time in
		seconds away from 1 Jan 12:00am to a
		date and time in string form 


		If the year value in date and time
		is the same as the current year,
		the year will not be in the string 
		dateString
		E.g: 'May 25 00:30'


		If the year value in date and time
		is not the same as the current year,
		then the year replace the time
		in the string dateString
		E.g. 'May 25  2010'


	 Parameters:
		dateString : the date converted to
			human readable form from
			'givenTime'. This is the
			output of the function

		givenTime : time in seconds, which is
			the number of seconds elapsed
			since 1 Jan 1970 12.00am
	--------------------------------------------------*/
void convertTimeToDateString(char * dateString, 
			     time_t givenTime);

	
	/*------------------------------------------------
	 Brief: Converts a numerical month value to a
		three-letter word form of month
		E.g. 'Jan', 'Feb', 'Mar', 'Apr'

		If the user provided an invalid month
		value, the string "Inv" will be returned
		instead


	 Parameters:
		monthString - the word form of 'month'
			in exactly three letters. This is
			the return value of the function

		month - the numerical value of month.
			Eg:
			0 represents Jan
			1 represenrs Feb
			2 represents Mar
			...
			11 represents Dec
	--------------------------------------------------*/
void getMonthString(char * monthString, int month);





int main(int argc, char * argv[])
{

	
	/*=============================================
	 If the user did not provide any argument,
	 display the file information of all the files
	 in the current directory

	 Otherwise, display the file information of the
	 files provided by the user arguments
	==============================================*/
	if (argc == 1)
	{
		//display information of all files
		// in current directory
		displayCurrDirFilesInfo();

	}
	else
	{

		for (int index=1; index <argc; index++)
		{
			displayCurrFileInfo(argv[index]);
		}

	}


	return 0;
}



/*--------------------------------------------------------*/

void  displayCurrDirFilesInfo()
{
	
	/*============================================
	 SECTION 1: Declaration of variables
	=============================================*/
	
	DIR * dirHandle = NULL;

	struct dirent * direntPtr = NULL;



	/*============================================
	 SECTION 2: Opening the current directory
	 
	 If the current directory cannot be opened,
	  the error message is displayed. The function
	  then returns immediately
	=============================================*/

	dirHandle = opendir("./");

	
	if (dirHandle == NULL)
	{
		perror("Failed to open current directory");

		return;
	}


	

	/*===========================================
	 SECTION 3: Displaying file information of
	 	    each file

	 With the exception of the parent
	 directory('..') and the current
	 directory('.') itself, all the files in the 
	 current directory will be displayed
	============================================*/
	
	
	while((direntPtr = readdir(dirHandle)) != NULL)
	{

		if ( strcmp( direntPtr->d_name, "..") != 0
				&&
		     strcmp( direntPtr->d_name, "." ) != 0)
		{

			displayCurrFileInfo(
					direntPtr->d_name );
		}

	}//end of while loop

	

	/*===============================================
	 SECTION 4: Closing the current directory
	================================================*/
	closedir(dirHandle);

}


/*---------------------------------------------------------*/

void displayCurrFileInfo( const char * fileName)
{


	/*==============================================
	 SECTION 1: Declation of Variables
	===============================================*/

	struct passwd * passwdPtr = NULL;

	struct group * groupPtr = NULL;

	int lstatReturnValue;

	struct stat statBuf;

	uid_t userId;

	gid_t groupId;

	dev_t deviceNumbers;

	unsigned int deviceMajorNum;

	unsigned int deviceMinorNum;

	ino_t inodeNum;

	off_t fileSize;

	nlink_t numOfHardLinks;

	mode_t fileTypeAndPermsFlags;

	time_t lastAccessTime;

	time_t lastModTime;

	time_t lastStatChgTime;

	char userName[MAX_STRING_SIZE];	

	char groupName[MAX_STRING_SIZE];


	char lastAccessTimeString[MAX_STRING_SIZE];
	
	char lastModTimeString[MAX_STRING_SIZE];

	char lastStatChgTimeString[MAX_STRING_SIZE];

	char fileTypeString[MAX_STRING_SIZE];

	char filePermsString[MAX_STRING_SIZE];



	/*==============================================
	 SECTION 2: Retrieving the information about
		    the file

	 (a)First we make a system call using 'lstat()'
	 to retrieve a group of file data stored in
	 a struct. If lstat fails, it means the
	 file does not exists, and the function 
	 returns immediately


	 (b)Second, we retrieve the values stored in
	 the struct.Some of the values can be
	 retrieved directly, while some other values
	 require further processing/more library
	 function calls  to obtain the desired output/data


	 (b)(i) Values that be retrieved directly:
	 ----------------------------------
	 -Size of file (in bytes)
	 -i-node number
	 -number of (hard) links


	 (b)(ii)Values that require further processing/
	 library function calls
	 --------------------------------------
	 -user name of owner owner
	 -group name of group owner
	 -type of file
	 -full access permissions information
	 -device numbers (major and minor numbers)
	 -last access time
	 -last modification time
	 -last time file status changed
	===============================================*/



	/*-----------------------------------------
	 Part (a) Obtaining all the information
		  about the file and storing
		  them in a struct 'statBuf'

	  If the files does not exist or the
	  data cannot be accessed, then an
	  error message is printed. 

	  The function them immediately returns
	------------------------------------------*/
	lstatReturnValue = lstat(fileName,  &statBuf);
	


	if (lstatReturnValue == -1 )
	{
		fprintf(stderr,  
			"\nmyls: Cannot access '%s': %s\n\n", 
			 fileName, strerror(errno) );

		return;
	}

	


	/*------------------------------------
	Part (b)(i)  Values that are retrieved
	 directly
	-----------------------------------*/
	fileSize = statBuf.st_size;
	
	inodeNum = statBuf.st_ino;

	numOfHardLinks = statBuf.st_nlink;


	/*-------------------------------------
	 Part (b)(ii) Values that require
	 more processing/library calls
	--------------------------------------*/


	
	/*---------------------------------------
	 Part (b)(ii)(i) Retrieving User Name	

	 Retrieval of the user name may fail,
	 so the string 'userName' would be
	 set to 'Not Available' if it fails.

	 Otherwise, 'userName' would contain the
	 user name of the owner owner
	-----------------------------------------*/	
	userId = statBuf.st_uid;


	if ((passwdPtr = getpwuid(userId)) != NULL)
	{
		strcpy(userName,passwdPtr->pw_name);
				
	}
	else
	{
		strcpy(userName, "Not Available");
			
	}


	/*---------------------------------------
	 Part(b)(ii)(ii) Retrieving Group Name	

	 Retrieval of the group name may fail,
	 so the string 'groupName' would be
	 set to 'Not Available' if it fails

	 Otherwise, 'groupName' would contain
	 the group name of the group owner
	-----------------------------------------*/
	groupId = statBuf.st_gid;


	if ((groupPtr = getgrgid(groupId)) != NULL)
	{
		strcpy(groupName, groupPtr->gr_name);
			
	}
	else
	{
		strcpy(groupName, "Not Available");
	}
	
	

	/*---------------------------------------
	 Part(b)(ii)(iii) Retrieving file type
	 and file access permissions in string	

	 
	 The variable 'fileTypeAndPermsFlag'
	 contains the data of file type and file
	 access permissions in numerical form. 
	 
	 Further processing is required to obtain
	 these data in string form
	-----------------------------------------*/
	fileTypeAndPermsFlags = statBuf.st_mode;

	getFileTypeString(fileTypeString,
			  fileTypeAndPermsFlags);

	getFilePermissionsString(filePermsString,
				fileTypeAndPermsFlags);



	/*---------------------------------------
	 Part(b)(ii)(iv) Retrieving major and minor
	 device numbers

	 
	 The variable 'deviceNumbers'
	 contains both the device major and minor
	 numbers 
	 
	 Further function calls to gnu_dev_major() 
	 and gnu_dev_minor() are required to obtain the
	 major and minor numbers respectively
	-----------------------------------------*/
	deviceNumbers = statBuf.st_dev;

	deviceMajorNum = gnu_dev_major(deviceNumbers);

	deviceMinorNum = gnu_dev_minor(deviceNumbers);



	/*---------------------------------------
	 Part(b)(ii)(v) Retrieving last access,last
	 modification and last status change time

	 
	 The variables 'lastAccessTime',
	 'lastModTime' and 'lastStatChgTime' 
	 contains the information, but in number of
	 seconds from 12am 1970. 
	 
	 Further function calls are required to
	 to change them to user readable format
	 in string (in local time) 
	-----------------------------------------*/	
	lastAccessTime = statBuf.st_atime;

	lastModTime = statBuf.st_mtime;

	lastStatChgTime = statBuf.st_ctime;

		
	convertTimeToDateString(lastAccessTimeString,
				lastAccessTime);

	convertTimeToDateString(lastModTimeString,
				lastModTime);

	convertTimeToDateString(lastStatChgTimeString,
				lastStatChgTime);
	

	/*==============================================
	 SECTION 3: Displaying all the file information
	===============================================*/
	printf("\n");

	printf("File Name: %s\n", fileName);
	
	printf("User Name of Owner Owner: %s\n", userName);

	printf("Group Name of Group Owner: %s\n", groupName);

	printf("Type of file: %s\n", fileTypeString);

	printf("Full Access Permission: %s\n",
		filePermsString);	

	printf("Size of file (bytes): %ld\n", fileSize);

	printf("Inode num: %lu\n", inodeNum);

	printf("Device Major Number: %d\n",
		deviceMajorNum);

	printf("Device Minor Number: %d\n",
		deviceMinorNum);

	printf("No of links: %lu\n", numOfHardLinks);

	printf("Last Access Time: %s\n",
		lastAccessTimeString);

	printf("Last Modification Time: %s\n",
		lastModTimeString);
	
	printf("Last Time File Status Change: %s\n",
		lastStatChgTimeString);

	printf("\n");

}



/*---------------------------------------------------------*/

void getFileTypeString(char * fileTypeString, 
		      int fileTypeAndPermsFlags)
{

	
	/*===============================================
	 This program retrieves the type of file from
	 a numerical flag and returns the file type as
	 a human-readable string
	================================================*/

	
	if (S_ISREG(fileTypeAndPermsFlags))
	{
		//regular file
		strcpy(fileTypeString, "Regular");

	}
	else if (S_ISBLK(fileTypeAndPermsFlags))
	{
		//block device file
		strcpy(fileTypeString,
		       "Block Device"); 

	}
	else if (S_ISCHR(fileTypeAndPermsFlags))
	{
		//character device file
		strcpy(fileTypeString,
		       "Character Device");

	}
	else if (S_ISDIR(fileTypeAndPermsFlags))
	{
		//directory
		strcpy(fileTypeString, "Directory");

	}
	else if (S_ISLNK(fileTypeAndPermsFlags))
	{
		//symbolic link
		strcpy(fileTypeString, "Symbolic Link");

	}
	else if (S_ISFIFO(fileTypeAndPermsFlags))
	{
		//named pipe
		strcpy(fileTypeString, "Named Pipe");

	}
	else
	{
		//socket
		strcpy(fileTypeString, "Socket");

	}

}

/*---------------------------------------------------------*/

void getFilePermissionsString( char * filePermsString ,
				mode_t fileTypeAndPermsFlags)
{

	/*================================================
	 FILE PERMISSIONS IN LINUX
	---------------------------------------------
	 This function returns a string that contains
	 the information about the access permissions
	  of the file, eg 'rwxrwxr--'


	 This function uses a numerical flag value
	 as input to produce the string
	 version of file access permissions

	
	 ----------------------------------

	 SECTION 1: User-related Permissions

	 SECTION 2: Group-related Permissions

	 SECTION 3: Others-related Permissions
		

	 Each section consists of three parts, which
	 are:
	   (a) Read permission
	   (b) Write Permission
	   (c) Execute Permission

	If the user(User/Group/Others) does not have
	a particular permission, it would be labelled
	as '-', otherwise it would be labelled
	as 'r', 'w' or 'x' respectively.
	=================================================*/




	/*================================================
	 SECTION 1: User-related Permissions
	=================================================*/
	
	/*------------------------------
	  Part(a) User Read Permission
	-------------------------------*/
	if(fileTypeAndPermsFlags & S_IRUSR)
	{
		filePermsString[0] = 'r';
	}
	else
	{
		filePermsString[0] = '-';
	}


	
	/*-------------------------------
	 Part(b) User Write Permission
	--------------------------------*/
	if (fileTypeAndPermsFlags & S_IWUSR)
	{
		filePermsString[1] = 'w';
	}
	else
	{
		filePermsString[1] = '-';
	}



	/*---------------------------------
	 Part(c) User Execute Permission	
	----------------------------------*/

	if (fileTypeAndPermsFlags & S_IXUSR)
	{
		filePermsString[2] = 'x';
	}
	else
	{
		filePermsString[2] = '-';
	}




	/*================================================
	 SECTION 2: Group-related Permissions
	=================================================*/


	/*---------------------------------
	 Part(a) Group Read Permission
	----------------------------------*/
	if (fileTypeAndPermsFlags & S_IRGRP)
	{
		filePermsString[3] = 'r';
	}
	else
	{
		filePermsString[3] = '-';
	}



	/*---------------------------------
	 Part(b) Group Write Permission
	----------------------------------*/
	if (fileTypeAndPermsFlags & S_IWGRP)
	{
		filePermsString[4] = 'w';
	}
	else
	{
		filePermsString[4] = '-';
	}



	/*---------------------------------
	 Part(c) Group Execute Permission
	---------------------------------*/
	if (fileTypeAndPermsFlags & S_IXGRP)
	{
		filePermsString[5] = 'x';
	}
	else
	{
		filePermsString[5] = '-';
	}



	
	/*==============================================
	 SECTION 3: Others-related Permissions
	===============================================*/

	/*----------------------------------
	 Part (a) Others Read Permission
	----------------------------------*/
	if (fileTypeAndPermsFlags & S_IROTH)
	{
		filePermsString[6] = 'r';
	}
	else
	{
		filePermsString[6] = '-';
	}



	/*----------------------------------
	 Part (b) Others Write Permission
	-----------------------------------*/
	if (fileTypeAndPermsFlags & S_IWOTH)
	{
		filePermsString[7] = 'w';
	}
	else
	{
		filePermsString[7] = '-';
	}



	/*---------------------------------
	 Part(c) Others Execute Permission
	---------------------------------*/
	if (fileTypeAndPermsFlags & S_IXOTH)
	{
		filePermsString[8] = 'x';
	}
	else
	{
		filePermsString[8] = '-';
	}



	/*-------------------------------------
	 Appending of the null-terminating
	 character
	--------------------------------------*/	

	filePermsString[9] = '\0';
}



/*---------------------------------------------------------*/

void convertTimeToDateString(char * dateString, 
			    time_t givenTime)
{

	/*===========================================
	 This function converts the Unix time in 
	 seconds to its corresponding human-readable
	 time in string, and returns the string
	============================================*/



	/*===========================================
	 SECTION 1: Declatation of variables
	============================================*/

	struct  tm givenBrokenDownTime;

	struct  tm currentBrokenDownTime;

	time_t currentTime;

	int currentYear;

	int givenYear;

	int givenMonth;

	int givenDay;

	int givenHour;

	int givenMinute;

	char givenMonthString[4];




	/*================================================
	 SECTION 2: Converting the values to desired form
	=================================================*/


	/*------------------------------------------
	 Part (a) Breaking the time into different
	 	 parts, such as year and month,
		 and store them in structs
	-------------------------------------------*/
	currentTime = time(NULL);

	localtime_r(&currentTime, &currentBrokenDownTime);

	localtime_r(&givenTime, &givenBrokenDownTime);
	


	/*------------------------------------------
	 Part (b) Retrieving the desired values from
		 the structs
	-------------------------------------------*/
	currentYear = 1900 + currentBrokenDownTime.tm_year;

	givenYear = 1900 + givenBrokenDownTime.tm_year;
	
	givenMonth = givenBrokenDownTime.tm_mon;

	givenDay = givenBrokenDownTime.tm_mday;

	givenHour = givenBrokenDownTime.tm_hour;

	givenMinute = givenBrokenDownTime.tm_min;

	getMonthString(givenMonthString, givenMonth);
	


	
	/*=================================================
	 SECTION 3: Storing the string in the desired
		    format and returning the string


	 NOTE: that the format is in accordance to the
	 actual 'ls' program. 
	
 	 In the actual 'ls' program, if the date to be 
	 displayed has the same year as the current year,
	 the year will not be displayed and only the time
	 will be displayed.

	 However, if the date does not have the same year
	 as the current year, the year will be displayed
	 in place of the time
	==================================================*/
	if (currentYear == givenYear)
	{
		snprintf(dateString, 
			sizeof(char) *(MAX_STRING_SIZE - 1),
			 "%3s %2d %02d:%02d",
			 givenMonthString, givenDay,
			 givenHour, givenMinute);
	}
	else
	{
		snprintf(dateString,
			 sizeof(char) * (MAX_STRING_SIZE -1),
			 "%3s %2d %5d", 
			 givenMonthString, givenDay, 
			 givenYear); 
	}
	

}


/*---------------------------------------------------------*/

void getMonthString(char *  monthString, int month)
{

	/*=============================================
	 This function converts a numerical month
	 value into its string form, e.g "Jun", and
	 returns the string

	 If the month value is invalid, it returns
	 "N/A" instead
	==============================================*/

	switch(month)
	{
		case 0:
			strcpy(monthString, "Jan");

			break;

		case 1:
			strcpy(monthString, "Feb"); 
			
			break;

		case 2:
			strcpy(monthString, "Mar");

			break;

		case 3:
			strcpy(monthString, "Apr");
				
			break;

		case 4:
			strcpy(monthString, "May");
				
			break;

		case 5:
			strcpy(monthString, "Jun");

			break;

		case 6:
			strcpy(monthString, "Jul");
				
			break;

		case 7:
			strcpy(monthString, "Aug");

			break;

		case 8:
			strcpy(monthString, "Sep");

			break;

		case 9:
			strcpy(monthString, "Oct");

			break;

		case 10:
			strcpy(monthString, "Nov");

			break;

		case 11:
			strcpy(monthString, "Dec");

			break;
		
		default:
			//invalid month value
			strcpy(monthString, "N/A");
			

	}//end of switch case

}



