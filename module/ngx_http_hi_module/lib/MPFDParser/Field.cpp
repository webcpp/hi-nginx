#include "Field.h"
#include "Parser.h"
#include <ctime>

MPFD::Field::Field() {
    type = 0;

}

MPFD::Field::~Field() {
    if (type == FileType) {
        if (file.is_open()) {
            file.close();
            remove((TempDir + "/" + TempFile).c_str());
        }

    }

}

void MPFD::Field::SetType(int type) {
    if ((type == TextType) || (type == FileType)) {
        this->type = type;
    } else {
        throw MPFD::Exception("Trying to set type of field, but type is incorrect.");
    }

}

int MPFD::Field::GetType() {
    if (type > 0) {
        return type;
    } else {
        throw MPFD::Exception("Trying to get type of field, but no type was set.");
    }
}

void MPFD::Field::AcceptSomeData(const char *data, long length) {
    if (type == TextType) {
        FieldContent.assign(data, length);
    } else if (type == FileType) {
        if (WhereToStoreUploadedFiles == Parser::StoreUploadedFilesInFilesystem) {
            if (TempDir.length() > 0) {
                if (!file.is_open()) {
                    int i = 1;
                    std::ifstream testfile;
                    std::string tempfile;
                    do {
                        if (testfile.is_open()) {
                            testfile.close();
                        }

                        std::stringstream ss;
                        ss << "MPFD_Temp_" << i;
                        TempFile = ss.str();

                        tempfile = TempDir + "/" + TempFile;

                        testfile.open(tempfile.c_str(), std::ios::in);
                        i++;
                    } while (testfile.is_open());

                    file.open(tempfile.c_str(), std::ios::out | std::ios::binary | std::ios_base::trunc);
                }

                if (file.is_open()) {
                    file.write(data, length);
                    file.flush();
                } else {
                    throw Exception(std::string("Cannot write to file ") + TempDir + "/" + TempFile);
                }
            } else {
                throw MPFD::Exception("Trying to AcceptSomeData for a file but no TempDir is set.");
            }
        } else { // If files are stored in memory
            FieldContent.assign(data, length);
        }
    } else {
        throw MPFD::Exception("Trying to AcceptSomeData but no type was set.");
    }
}

void MPFD::Field::SetTempDir(const std::string& dir) {
    TempDir = dir;
}

unsigned long MPFD::Field::GetFileContentSize() {
    if (type == 0) {
        throw MPFD::Exception("Trying to get file content size, but no type was set.");
    } else {
        if (type == FileType) {
            if (WhereToStoreUploadedFiles == Parser::StoreUploadedFilesInMemory) {
                return FieldContent.size();
            } else {
                throw MPFD::Exception("Trying to get file content size, but uploaded files are stored in filesystem.");
            }
        } else {
            throw MPFD::Exception("Trying to get file content size, but the type is not file.");
        }
    }
}

const std::string& MPFD::Field::GetFileContent() const {
    if (type == 0) {
        throw MPFD::Exception("Trying to get file content, but no type was set.");
    } else {
        if (type == FileType) {
            if (WhereToStoreUploadedFiles == Parser::StoreUploadedFilesInMemory) {
                return FieldContent;
            } else {
                throw MPFD::Exception("Trying to get file content, but uploaded files are stored in filesystem.");
            }
        } else {
            throw MPFD::Exception("Trying to get file content, but the type is not file.");
        }
    }
}

std::string MPFD::Field::GetTextTypeContent() {
    if (type == 0) {
        throw MPFD::Exception("Trying to get text content of the field, but no type was set.");
    } else {
        if (type != TextType) {
            throw MPFD::Exception("Trying to get content of the field, but the type is not text.");
        } else {
            return FieldContent;
        }
    }
}

std::string MPFD::Field::GetTempFileName() {
    if (type == 0) {
        throw MPFD::Exception("Trying to get file temp name, but no type was set.");
    } else {
        if (type == FileType) {
            if (WhereToStoreUploadedFiles == Parser::StoreUploadedFilesInFilesystem) {
                return std::string(TempDir + "/" + TempFile);
            } else {
                throw MPFD::Exception("Trying to get file temp name, but uplaoded files are stored in memory.");
            }
        } else {
            throw MPFD::Exception("Trying to get file temp name, but the type is not file.");
        }
    }
}

std::string MPFD::Field::GetFileName() {
    if (type == 0) {
        throw MPFD::Exception("Trying to get file name, but no type was set.");
    } else {
        if (type == FileType) {
            return FileName;
        } else {
            throw MPFD::Exception("Trying to get file name, but the type is not file.");
        }
    }
}

void MPFD::Field::SetFileName(const std::string& name) {
    FileName = name;

}

void MPFD::Field::SetUploadedFilesStorage(int where) {
    WhereToStoreUploadedFiles = where;
}

void MPFD::Field::SetFileContentType(const std::string& type) {
    FileContentType = type;
}

std::string MPFD::Field::GetFileMimeType() {
    if (type == 0) {
        throw MPFD::Exception("Trying to get mime type of file, but no type was set.");
    } else {
        if (type != FileType) {
            throw MPFD::Exception("Trying to get mime type of the field, but the type is not File.");
        } else {
            return std::string(FileContentType);
        }
    }
}