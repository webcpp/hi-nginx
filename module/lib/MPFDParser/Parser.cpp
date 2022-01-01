#include "Parser.h"

const std::unordered_map<std::string, MPFD::Field *>& MPFD::Parser::GetFieldsMap()const {
    return Fields;
}

MPFD::Field * MPFD::Parser::GetField(const std::string& Name) {
    if (Fields.count(Name)) {
        return Fields[Name];
    } else {
        return NULL;
    }
}

MPFD::Parser::Parser() {
    _HeadersOfTheFieldAreProcessed = false;
    CurrentStatus = Status_LookingForStartingBoundary;

    MaxDataCollectorLength = 16 * 1024 * 1024; // 16 Mb default data collector size.

    SetUploadedFilesStorage(StoreUploadedFilesInFilesystem);
}

MPFD::Parser::~Parser() {
    std::unordered_map<std::string, Field *>::iterator it;
    for (it = Fields.begin(); it != Fields.end(); it++) {
        delete it->second;
    }
}

void MPFD::Parser::SetContentType(const std::string& type) {
    if (type.find("multipart/form-data;") != 0) {
        throw MPFD::Exception(std::string("Content type is not \"multipart/form-data\"\nIt is \"") + type + std::string("\""));
    }


    std::string::size_type bp = type.find("boundary=");

    if (bp == std::string::npos) {
        throw MPFD::Exception(std::string("Cannot find boundary in Content-type: \"") + type + std::string("\""));
    }

    Boundary = std::string("--") + type.substr(bp + 9, type.length() - bp);
}

void MPFD::Parser::AcceptSomeData(const char *data, long length) {
    if (Boundary.length() > 0) {
        // Append data to existing accumulator
        DataCollector.assign(data, length);

        if (DataCollector.size() > MaxDataCollectorLength) {
            throw Exception("Maximum data collector length reached.");
        }

        _ProcessData();
    } else {
        throw MPFD::Exception("Accepting data, but content type was not set.");
    }

}

void MPFD::Parser::_ProcessData() {
    // If some data left after truncate, process it right now.
    // Do not wait for AcceptSomeData called again
    bool NeedToRepeat;

    do {
        NeedToRepeat = false;
        switch (CurrentStatus) {
            case Status_LookingForStartingBoundary:
                if (FindStartingBoundaryAndTruncData()) {
                    CurrentStatus = Status_ProcessingHeaders;
                    NeedToRepeat = true;
                }
                break;

            case Status_ProcessingHeaders:
                if (WaitForHeadersEndAndParseThem()) {
                    CurrentStatus = Status_ProcessingContentOfTheField;
                    NeedToRepeat = true;
                }
                break;

            case Status_ProcessingContentOfTheField:
                if (ProcessContentOfTheField()) {
                    CurrentStatus = Status_LookingForStartingBoundary;
                    NeedToRepeat = true;
                }
                break;
        }
    } while (NeedToRepeat);
}

bool MPFD::Parser::ProcessContentOfTheField() {
    long BoundaryPosition = BoundaryPositionInDataCollector();
    long DataLengthToSendToField;
    if (BoundaryPosition >= 0) {
        // 2 is the \r\n before boundary we do not need them
        DataLengthToSendToField = BoundaryPosition - 2;
    } else {
        // We need to save +2 chars for \r\n chars before boundary
        DataLengthToSendToField = DataCollector.size() - (Boundary.length() + 2);
    }

    if (DataLengthToSendToField > 0) {
        Fields[ProcessingFieldName]->AcceptSomeData(DataCollector.c_str(), DataLengthToSendToField);
        TruncateDataCollectorFromTheBeginning(DataLengthToSendToField);
    }

    if (BoundaryPosition >= 0) {
        CurrentStatus = Status_LookingForStartingBoundary;
        return true;
    } else {
        return false;
    }
}

bool MPFD::Parser::WaitForHeadersEndAndParseThem() {
    size_t dl=DataCollector.size();
    for (size_t i = 0; i < dl - 3; i++) {
        if ((DataCollector[i] == 13) && (DataCollector[i + 1] == 10) && (DataCollector[i + 2] == 13) && (DataCollector[i + 3] == 10)) {
            long headers_length = i;
            _ParseHeaders(DataCollector.substr(0, headers_length));

            TruncateDataCollectorFromTheBeginning(i + 4);
            return true;
        }
    }
    return false;
}

void MPFD::Parser::SetUploadedFilesStorage(int where) {
    WhereToStoreUploadedFiles = where;
}

void MPFD::Parser::SetTempDirForFileUpload(const std::string& dir) {
    TempDirForFileUpload = dir;
}

void MPFD::Parser::_ParseHeaders(const std::string& headers) {
    // Check if it is form data
    if (headers.find("Content-Disposition: form-data;") == std::string::npos) {
        throw Exception(std::string("Accepted headers of field does not contain \"Content-Disposition: form-data;\"\nThe headers are: \"") + headers + std::string("\""));
    }

    // Find name
    std::string::size_type name_pos = headers.find("name=\"");
    if (name_pos == std::string::npos) {
        throw Exception(std::string("Accepted headers of field does not contain \"name=\".\nThe headers are: \"") + headers + std::string("\""));
    } else {
        std::string::size_type name_end_pos = headers.find("\"", name_pos + 6);
        if (name_end_pos == std::string::npos) {
            throw Exception(std::string("Cannot find closing quote of \"name=\" attribute.\nThe headers are: \"") + headers + std::string("\""));
        } else {
            ProcessingFieldName = headers.substr(name_pos + 6, name_end_pos - (name_pos + 6));
            Fields[ProcessingFieldName] = new Field();
        }


        // find filename if exists
        std::string::size_type filename_pos = headers.find("filename=\"");
        if (filename_pos == std::string::npos) {
            Fields[ProcessingFieldName]->SetType(Field::TextType);
        } else {
            Fields[ProcessingFieldName]->SetType(Field::FileType);
            Fields[ProcessingFieldName]->SetTempDir(TempDirForFileUpload);
            Fields[ProcessingFieldName]->SetUploadedFilesStorage(WhereToStoreUploadedFiles);

            std::string::size_type filename_end_pos = headers.find("\"", filename_pos + 10);
            if (filename_end_pos == std::string::npos) {
                throw Exception(std::string("Cannot find closing quote of \"filename=\" attribute.\nThe headers are: \"") + headers + std::string("\""));
            } else {
                std::string filename = headers.substr(filename_pos + 10, filename_end_pos - (filename_pos + 10));
                Fields[ProcessingFieldName]->SetFileName(filename);
            }

            // find Content-Type if exists
            std::string::size_type content_type_pos = headers.find("Content-Type: ");
            if (content_type_pos != std::string::npos) {
                std::string::size_type content_type_end_pos = 0;
                for (std::string::size_type i = content_type_pos + 14; (i < headers.length()) && (!content_type_end_pos); i++) {
                    if ((headers[i] == ' ') || (headers[i] == 10) || (headers[i] == 13)) {
                        content_type_end_pos = i - 1;
                    }
                }
                std::string content_type = headers.substr(content_type_pos + 14, content_type_end_pos - (content_type_pos + 14));
                Fields[ProcessingFieldName]->SetFileContentType(content_type);
            }


        }

    }


}

void MPFD::Parser::SetMaxCollectedDataLength(long max) {
    MaxDataCollectorLength = max;
}

void MPFD::Parser::TruncateDataCollectorFromTheBeginning(long n) {
    long TruncatedDataCollectorLength = DataCollector.size() - n;
    DataCollector = DataCollector.substr(n, TruncatedDataCollectorLength);
}

long MPFD::Parser::BoundaryPositionInDataCollector() {
    const char *b = Boundary.c_str();
    int bl = Boundary.length(), dl = DataCollector.size();
    if (dl >= bl) {
        bool found = false;
        for (int i = 0; (i <= dl - bl) && (!found); i++) {
            found = true;
            for (int j = 0; (j < bl) && (found); j++) {
                if (DataCollector[i + j] != b[j]) {
                    found = false;
                }
            }

            if (found) {
                return i;
            }
        }
    }
    return -1;

}

bool MPFD::Parser::FindStartingBoundaryAndTruncData() {
    long n = BoundaryPositionInDataCollector();
    if (n >= 0) {
        TruncateDataCollectorFromTheBeginning(n + Boundary.length());
        return true;
    } else {
        return false;
    }
}