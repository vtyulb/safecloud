#ifndef SAFECLOUDCLIENT_H
#define SAFECLOUDCLIENT_H

#include <libtorrent/create_torrent.hpp>
#include <fileimage.h>
#include <fileimagestree.h>

using namespace std;
using namespace libtorrent;

class SafeCloudClient
{
public:
    SafeCloudClient();
    ~SafeCloudClient();
    //void connectToServer();
    //void sendTreeToServer();
    //void getTreeFromServer(); something like this

    //??? getTreeDif(FileImagesTree* a, FileImagesTree* b)

    void addFile(const string &imageFilePath, const string &realFilePath);
    void deleteImageFile(const string &imageFilePath);
    void renameImageFile(const string &imageFilePath, const string &newFilePath);

    FileImagesTree* getFileTree();
    void saveFilesTree(const string &filePath);
    void loadFilesTree(const string &filePath);

    void changeLocalTree(FileImagesTree *newTree);

private:
    FileImagesTree* localTree;
    FileImagesTree* serverTree;
};

#endif // SAFECLOUDCLIENT_H
