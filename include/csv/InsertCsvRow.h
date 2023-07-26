//
// Created by zyzshishui on 23-7-22.
//

#ifndef IDS_INSERTCSVROW_H
#define IDS_INSERTCSVROW_H

#include <string>
#include <vector>

class InsertCsvRow {
private:
    std::string header;
    std::vector<std::string> rows;
    std::string savepath;
    std::string filename;

public:
    InsertCsvRow(std::string header, std::vector<std::string> rows, std::string savepath, std::string filename);
    InsertCsvRow(std::string header, std::string row, std::string savepath, std::string filename);
    void run();
    static void insert(std::string header, std::vector<std::string> rows, std::string savepath, std::string filename);
};

#endif //IDS_INSERTCSVROW_H
