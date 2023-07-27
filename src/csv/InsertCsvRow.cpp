//
// Created by zyzshishui on 23-7-22.
//

#include "InsertCsvRow.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>

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
    std::ofstream output;

    try {
        std::ifstream file(filename);
        if (file.good()) {
            output.open(filename, std::ios::app); // Append mode
        } else {
            output.open(filename); // Create a new file
            if (!header.empty()) {
                output << header << '\n';
            }
        }
        for (const std::string& row : rows) {
            output << row << '\n';
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}