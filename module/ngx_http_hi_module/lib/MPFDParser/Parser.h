#ifndef _PARSER_H
#define _PARSER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include "Exception.h"
#include "Field.h"
#include <string.h>
#include <stdlib.h>

namespace MPFD {

    class Parser {
    public:
        static const int StoreUploadedFilesInFilesystem = 1, StoreUploadedFilesInMemory = 2;


        Parser();
        ~Parser();

        void SetContentType(const std::string& type);

        void AcceptSomeData(const char *data, long length);



        void SetMaxCollectedDataLength(long max);
        void SetTempDirForFileUpload(const std::string& dir);
        void SetUploadedFilesStorage(int where);

        const std::unordered_map<std::string, Field *>& GetFieldsMap()const;
        Field * GetField(const std::string& Name);

    private:
        int WhereToStoreUploadedFiles;

        std::unordered_map<std::string, Field *> Fields;

        std::string TempDirForFileUpload;
        int CurrentStatus;

        // Work statuses
        static int const Status_LookingForStartingBoundary = 1;
        static int const Status_ProcessingHeaders = 2;
        static int const Status_ProcessingContentOfTheField = 3;

        std::string Boundary;
        std::string ProcessingFieldName;
        bool _HeadersOfTheFieldAreProcessed;
        long ContentLength;
        std::string DataCollector;
        size_t MaxDataCollectorLength;
        bool FindStartingBoundaryAndTruncData();
        void _ProcessData();
        void _ParseHeaders(const std::string& headers);
        bool WaitForHeadersEndAndParseThem();
        void TruncateDataCollectorFromTheBeginning(long n);
        long BoundaryPositionInDataCollector();
        bool ProcessContentOfTheField();
    };
}

#endif /* _PARSER_H */

