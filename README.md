# mmd5check
A multithreading tool for file md5 check

## Dependency
Boost library >= 1.66.0
Cmake >= 3.20
GNUC++ 20 supported

## Usage
Usage: mmd5check [--help] [--version] --input FILE --output FILE [--thread THREAD] [--version] [--help]

mmd5check, a tool to check md5 of files

Optional arguments:
  -h, --help            shows help message and exits
  -v, --version         prints version information and exits
  -i, --input FILE      file list to check md5 [required]
  -o, --output FILE     output file to save md5 check result [required]
  -t, --thread THREAD   number of threads to check md5 [default: 1]
  -v, --version         show version information
  -h, --help            show help information