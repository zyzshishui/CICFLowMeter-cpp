//
// Created by zyzshishui on 23-7-20.
//

#include <string>
#include <vector>
#include "BasicFlow.h"
#include "FlowListener.h"
#include "FlowFeature.h"
#include "InsertCsvRow.h"


void FlowListener::onFlowGenerated(const BasicFlow& flow) {
    std::string flowDump = flow.dumpFlowBasedFeaturesEx();
    std::vector<std::string> flowStringList;
    flowStringList.push_back(flowDump);
    InsertCsvRow::insert(FlowFeature::getHeader(), flowStringList, outPath, fileName + ".csv");

    cnt++;

    std::cout << fileName << "->" << cnt << "flows." << std::endl;
}