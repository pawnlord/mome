# mome
A portable, simple Version Control System, version 0.1.0  

## the name  
For those confused, a mome is an archaic term for idiot.  

## building
To build, use this command: 
```
g++ source/*.cpp -o mome -std=c++17
```

## usage  
```
mome <command> [args]
```
commands:
- **no command:** general information about mome.  
- **help/h:** list all commands.  
- **version/v:** tell version info.  
- **init/i:** initialize a repository.  
- **info:** info about current repository.
- **commit/c:** commit current repository. 
- **commandline/cli:** start a commandline interface for continuous usage.

## TODO  
- cli has basic shell usage.  
- log: command to list commits and info.
- config: a configuration file to store user info.
- revert: revert to a specific commit.
- diff: ability to diff files.
- branch and merge