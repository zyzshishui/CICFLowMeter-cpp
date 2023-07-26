//
// Created by zyzshishui on 23-7-22.
//

#include "InsertCsvRow.h"
#include <iostream>
#include <fstream>

InsertCsvRow::InsertCsvRow(std::string header, std::vector<std::string> rows, std::string savepath, std::string filename)
        : header(header), rows(rows), savepath(savepath), filename(filename) {}

InsertCsvRow::InsertCsvRow(std::string header, std::string row, std::string savepath, std::string filename)
        : header(header), savepath(savepath), filename(filename) {
    rows.push_back(row);
}

void InsertCsvRow::run() {
    insert(header, rows, savepath, filename);
}

void InsertCsvRow::insert(std::string header, std::vector<std::string> rows, std::string savepath, std::string filename) {
    if (savepath.empty() || filename.empty() || rows.empty()) {
        std::string ex = "savepath=" + savepath + ", filename=" + filename;
        throw std::invalid_argument(ex);
    }

    if (savepath.back() != '/') {
        savepath += '/';
    }

    std::ofstream output(savepath + filename, std::ios::app);

    if (output.is_open()) {
        if (!header.empty()) {
            output << header << '\n';
        }

        for (const auto& row : rows) {
            output << row << '\n';
        }

        output.close();
    } else {
        std::cerr << "Error opening the file." << std::endl;
    }
}