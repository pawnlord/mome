#ifndef REPO_H
#define REPO_H

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include <string>


typedef struct {
    bool active;
    std::string date_created;

} repoInfo;

class Repo {
    public:
        Repo(bool create = false);
        repoInfo getInfo();
        std::string formatInfo(std::string format);
        repoInfo setInfo();
    private:
        repoInfo r;
};

#endif 