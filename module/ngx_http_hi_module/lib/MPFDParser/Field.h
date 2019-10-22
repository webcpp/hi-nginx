#ifndef _FIELD_H
#define	_FIELD_H

#include "Exception.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <string>

namespace MPFD {

    class Field {
    public:
        static const int TextType = 1, FileType = 2;

        Field();
        virtual ~Field();

        void SetType(int type);
        int GetType();

        void AcceptSomeData(const char *data, long length);


        // File functions
        void SetUploadedFilesStorage(int where);
        void SetTempDir(const std::string& dir);

        void SetFileName(const std::string& name);
        std::string GetFileName();

        void SetFileContentType(const std::string& type);
        std::string GetFileMimeType();

        const std::string& GetFileContent()const;
        unsigned long GetFileContentSize();

        std::string GetTempFileName();

        // Text field operations
        std::string GetTextTypeContent();




    private:

        int WhereToStoreUploadedFiles;

        std::string TempDir, TempFile;
        std::string FileContentType, FileName;

        int type;
        std::string FieldContent;
        std::ofstream file;

    };
}
#endif	/* _FIELD_H */

