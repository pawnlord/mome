# mome
A portable, simple Version Control System, version 0.2.0  

## the name  
For those confused, a mome is an archaic term for idiot.  

## building
To build, use this command: 
```
g++ source/*.cpp -o mome -std=c++18
```

## usage  
```
mome <command> [args]
```
commands:
- **no command:** general information about mome.  
- **help/h:** list all commands.  
- **version/v:** tell version info.  
- **init:** initialize a repository.  
- **info/i:** info about current repository.
- **commit/c:** commit current repository.
    - **format:** `mome commit [<message>]`
    - **message:** info you want attached to the commit (shown in log).
- **log/l:** log info about previous commits in current repo. 
- **commandline/cli:** start a commandline interface for continuous usage.

## TODO  
- cli has basic shell usage.  
- add more information to log.
- make log easier to read when you have manny commits.
- multi-line commit messages
- config: a configuration file to store user info.
- revert: revert to a specific commit.
- diff: ability to diff files.
- branch and merge