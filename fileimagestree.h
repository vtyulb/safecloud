#ifndef FILEIMAGESTREE_H
#define FILEIMAGESTREE_H

#include <fileimage.h>

using namespace std;

template <typename T, typename Pred = std::less<T>>
struct ptr_compare : Pred
{
    ptr_compare(Pred const & p = Pred()) : Pred(p) { } //Some magic ;)

    bool operator()(T const * p1, T const * p2) const
    {
        return Pred::operator()(*p1, *p2);
    }
};

class FileImagesTree // something like folder
{
public:
    FileImagesTree();
    FileImagesTree(char *data);
    FileImagesTree(const string &fileName);

    char* readTree(char *data); // returns end of tree in memory
    void readTree(const string &fileName);
    char* getTree();
    void writeTree(const string &fileName);

    void addFileImage(FileImage *image);
    void removeFileImage(const string &imageName);
    FileImage *getFileImage(const string &imageName);

    void createNewSubTree(const string &newTreeName);
    void addSubTree(FileImagesTree *tree);
    void removeSubTree(const string &treeName);

    enum TreeObjectType
    {
        FileImageType,
        FileImagesTreeType
    };

    TreeObjectType getTreeObjectByPath(const string &pathToObject, FileImagesTree* &tree, FileImage* &image);

    bool operator == (const FileImageTree &tree);
    bool operator < (const FileImageTree &tree);

private:
    string treeName; // folder name
    set <FileImagesTree*, ptr_compare<FileImagesTree*> > subtrees; // subfolders
    set <FileImage*, ptr_compare<FileImage*> > images; // files in folder
};

#endif // FILEIMAGESTREE_H
