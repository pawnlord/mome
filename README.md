# mome
A portable, simple Version Control System, version 0.2.0  

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
- **help/h:** get help with mome.  
    - **default:** list all commands.
    - **`help <command>`:** get help for specific command.  
- **version/v:** tell version info.  
- **init:** initialize a repository.  
- **info/i:** info about current repository.
- **commit/c:** commit current repository.
    - **format:** `mome commit [<message>]`
    - **message:** info you want attached to the commit (shown in log).
- **log/l:** log info about previous commits in current repo. 
- **commandline/cli:** start a commandline interface for continuous usage.

## TODO  
- status: status to see changes in current commit.
- config: a configuration file to store user info.
- multi-line commit messages
- allow someone to specify the directory to commit.
- add: add something to list of things to commit.
- revert: revert to a specific commit.
- add more information to log.
- make log easier to read when you have many commits.
- diff: ability to diff files.
- branch and merge