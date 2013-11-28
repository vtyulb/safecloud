#ifndef FILEIMAGE_H
#define FILEIMAGE_H

#include <cstdlib>

using namespace std;

class FileImage
{
public:
    FileImage(char *data, int size, const string &name);
    FileImage(char *data, int size, const string &name, const string &realPath);
    FileImage();
    void readData(char *data, int size);
    void renameImage(const string &newName);
    void setRealPath(const string &newPath);
    int getSize();
    char* getData();

    bool operator == (const FileImage &image);
    bool operator < (const FileImage &image);

private:
    char *torrent = NULL; //torrent data
    int dataSize;
    string imageName; // name in cloud
    string realFilePath; // (optional) path to real file
};

#endif // FILEIMAGE_H
