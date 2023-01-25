# Quic

A simple command line tool that can copy and synchronize files and directories.

## Description

Quic is a command-line utility program that can be used to copy and synchronize files and directories. It can copy new or modified files from a source directory to a destination directory, and can also remove files from the destination directory that have been deleted from the source directory.

## Getting Started

### Dependencies

* Linux or macOS
* GCC compiler

### Installing

* Download the source code for Quic from this repository
* Open a terminal and navigate to the directory where you downloaded the source code
* Run the command "make" to build the executable

### Executing program

* Run the command `./quic <source_path> <destination_path> [flags]`
* `source_path` is the path to the directory you want to synchronize from
* `destination_path` is the path to the directory you want to synchronize to
* Available flags:
    * -v : verbose mode, prints statistics about the synchronization process
    * -l: copy symbolic links instead of copying the files they link to.
    * -d : delete mode, removes files and directories from the destination that don't exist in the source
    > :warning: Before use -d be sure that you don't have files that you need in the destination folder.

Example:

`./quic /home/user/src /home/user/dest -v -d`

This command synchronizes the contents of the directory /home/user/src to the directory /home/user/dest, and also prints statistics about the synchronization process and deletes files and directories from the destination that don't exist in the source.
