> ⚠️ This project is part of an academic coursework.
> Authored by **Terence Lee Xin Jin** on 2020.

# System Programming: Unix `ls` Command Implementation (Academic Coursework)

## Test Documentation
[linux-file-lister-documentation.pdf](https://github.com/user-attachments/files/20114046/linux-file-lister-documentation.pdf)


## 📄 Description

`myls.c` is a C program developed as part of a university assignment. Its purpose is to list detailed file information similar to the Unix `ls -l` command. It displays metadata about files such as:

- File name  
- File type  
- Permissions  
- Owner username and group  
- Size in bytes  
- Number of hard links  
- Device information (major and minor numbers)  
- Access, modification, and status change timestamps  

The program can list:
- All files in the **current directory**, or  
- Specific files passed as **command-line arguments**

## 🛠️ Compilation

You can compile the program using `gcc`:

```
gcc -o myls myls.c
```

## ▶️ Usage

```
./myls
```

Lists all files (excluding `.` and `..`) in the current directory.

```
./myls file1.txt file2.c
```

Displays information about the specified files.

## 📚 Features Implemented

- Uses `lstat()` to retrieve file metadata.
- Resolves:
  - Username via `getpwuid()`
  - Group name via `getgrgid()`
- Converts:
  - File type and permissions to human-readable format
  - Unix timestamp to formatted date string
- Handles errors gracefully (e.g., file not found, permission denied).
- Excludes `"."` and `".."` from directory listings.
- Modular design with reusable helper functions:
  - `displayCurrDirFilesInfo()`
  - `displayCurrFileInfo()`
  - `getFileTypeString()`
  - `getFilePermissionsString()`
  - `convertTimeToDateString()`
  - `getMonthString()`

## 📁 Directory Listing Behavior

When run without arguments, the program opens the current directory using `opendir()`, iterates through files using `readdir()`, and prints metadata (excluding `"."` and `".."` entries). 

When specific file names are passed as arguments, it prints metadata for each file provided.

## 🧠 Notes

- File and directory metadata are retrieved using the POSIX `stat` family of system calls.
- Dates are formatted to mimic `ls` output (e.g., if the year is current, only time is shown).
- File types are shown in string format (e.g., `regular`, `directory`, etc.).
- Permissions are printed in symbolic notation (e.g., `rwxr-xr--`).

## ❗ Error Handling

The program provides meaningful error messages, such as:

```
myls: Cannot access 'nonexistent.txt': No such file or directory
```

## 📌 Dependencies

Standard POSIX libraries:
- `<dirent.h>` – Directory traversal  
- `<errno.h>` – Error handling  
- `<grp.h>` – Group info  
- `<pwd.h>` – User info  
- `<stdio.h>` – Input/output  
- `<string.h>` – String operations  
- `<sys/stat.h>` – File info  
- `<sys/sysmacros.h>` – Device macros  
- `<sys/types.h>` – System types  
- `<time.h>` – Time conversion  
- `<unistd.h>` – POSIX API

## 📘 License

This code is for **educational and academic purposes only**.  
Do not redistribute or submit as your own work.  
© 2020 Terence Lee Xin Jun.
