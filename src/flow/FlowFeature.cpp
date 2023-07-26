//
// Created by zyzshishui on 23-7-20.
//

#include <sstream>
#include "FlowFeature.h"

FlowFeature::FlowFeature(const std::string& name, const std::string& abbr, bool isNumeric)
        : name(name), abbr(abbr), isNumeric(isNumeric) {}

FlowFeature::FlowFeature(const std::string& name, const std::string& abbr)
        : name(name), abbr(abbr), isNumeric(true) {}

FlowFeature::FlowFeature(const std::string& name, const std::string& abbr, std::vector<std::string> values)
        : name(name), abbr(abbr), isNumeric(false), values(values) {}

std::string FlowFeature::getName() const {
    return name;
}

std::string FlowFeature::getAbbr() const {
    return abbr;
}

bool FlowFeature::isNumericFeature() const {
    return isNumeric;
}

std::string FlowFeature::HEADER;

const FlowFeature FlowFeature::features[] = {
        FlowFeature("Flow ID", "FID", false),
        FlowFeature("Src IP", "SIP", false),
        FlowFeature("Src Port", "SPT", true),
        FlowFeature("Dst IP", "DIP", false),
        FlowFeature("Dst Port", "DPT", true),
        FlowFeature("Protocol", "PROT", true),
        FlowFeature("Timestamp", "TSTP", false),
        FlowFeature("Flow Duration", "DUR", true),
        FlowFeature("Total Fwd Packet", "TFwP", true),
        FlowFeature("Total Bwd packets", "TBwP", true),
        FlowFeature("Total Length of Fwd Packet", "TLFwP", true),
        FlowFeature("Total Length of Bwd Packet", "TLBwP", true),
        FlowFeature("Fwd Packet Length Max", "FwPLMA", true),
        FlowFeature("Fwd Packet Length Min", "FwPLMI", true),
        FlowFeature("Fwd Packet Length Mean", "FwPLAG", true),
        FlowFeature("Fwd Packet Length Std", "FwPLSD", true),
        FlowFeature("Bwd Packet Length Max", "BwPLMA", true),
        FlowFeature("Bwd Packet Length Min", "BwPLMI", true),
        FlowFeature("Bwd Packet Length Mean", "BwPLAG", true),
        FlowFeature("Bwd Packet Length Std", "BwPLSD", true),
        FlowFeature("Flow Bytes/s", "FB/s", true),
        FlowFeature("Flow Packets/s", "FP/s", true),
        FlowFeature("Flow IAT Mean", "FLIATAG", true),
        FlowFeature("Flow IAT Std", "FLIATSD", true),
        FlowFeature("Flow IAT Max", "FLIATMA", true),
        FlowFeature("Flow IAT Min", "FLIATMI", true),
        FlowFeature("Fwd IAT Total", "FwIATTO", true),
        FlowFeature("Fwd IAT Mean", "FwIATAG", true),
        FlowFeature("Fwd IAT Std", "FwIATSD", true),
        FlowFeature("Fwd IAT Max", "FwIATMA", true),
        FlowFeature("Fwd IAT Min", "FwIATMI", true),
        FlowFeature("Bwd IAT Total", "BwIATTO", true),
        FlowFeature("Bwd IAT Mean", "BwIATAG", true),
        FlowFeature("Bwd IAT Std", "BwIATSD", true),
        FlowFeature("Bwd IAT Max", "BwIATMA", true),
        FlowFeature("Bwd IAT Min", "BwIATMI", true),
        FlowFeature("Fwd PSH Flags", "FwPSH", true),
        FlowFeature("Bwd PSH Flags", "BwPSH", true),
        FlowFeature("Fwd URG Flags", "FwURG", true),
        FlowFeature("Bwd URG Flags", "BwURG", true),
        FlowFeature("Fwd Header Length", "FwHL", true),
        FlowFeature("Bwd Header Length", "BwHL", true),
        FlowFeature("Fwd Packets/s", "FwP/s", true),
        FlowFeature("Bwd Packets/s", "Bwp/s", true),
        FlowFeature("Packet Length Min", "PLMI", true),
        FlowFeature("Packet Length Max", "PLMA", true),
        FlowFeature("Packet Length Mean", "PLAG", true),
        FlowFeature("Packet Length Std", "PLSD", true),
        FlowFeature("Packet Length Variance", "PLVA", true),
        FlowFeature("FIN Flag Count", "FINCT", true),
        FlowFeature("SYN Flag Count", "SYNCT", true),
        FlowFeature("RST Flag Count", "RSTCT", true),
        FlowFeature("PSH Flag Count", "PSHCT", true),
        FlowFeature("ACK Flag Count", "ACKCT", true),
        FlowFeature("URG Flag Count", "URGCT", true),
        FlowFeature("CWR Flag Count", "CWRCT", true),
        FlowFeature("ECE Flag Count", "ECECT", true),
        FlowFeature("Down/Up Ratio", "D/URO", true),
        FlowFeature("Average Packet Size", "PSAG", true),
        FlowFeature("Fwd Segment Size Avg", "FwSgAG", true),
        FlowFeature("Bwd Segment Size Avg", "BwSgAG", true),
        FlowFeature("Fwd Bytes/Bulk Avg", "FwB/BAG", true),
        FlowFeature("Fwd Packet/Bulk Avg", "FwP/BAG", true),
        FlowFeature("Fwd Bulk Rate Avg", "FwBRAG", true),
        FlowFeature("Bwd Bytes/Bulk Avg", "BwB/BAG", true),
        FlowFeature("Bwd Packet/Bulk Avg", "BwP/BAG", true),
        FlowFeature("Bwd Bulk Rate Avg", "BwBRAG", true),
        FlowFeature("Subflow Fwd Packets", "SFFwP", true),
        FlowFeature("Subflow Fwd Bytes", "SFFwB", true),
        FlowFeature("Subflow Bwd Packets", "SFBwP", true),
        FlowFeature("Subflow Bwd Bytes", "SFBwB", true),
        FlowFeature("FWD Init Win Bytes", "FwWB", true),
        FlowFeature("Bwd Init Win Bytes", "BwWB", true),
        FlowFeature("Fwd Act Data Pkts", "FwAP", true),
        FlowFeature("Fwd Seg Size Min", "FwSgMI", true),
        FlowFeature("Active Mean", "AcAG", true),
        FlowFeature("Active Std", "AcSD", true),
        FlowFeature("Active Max", "AcMA", true),
        FlowFeature("Active Min", "AcMI", true),
        FlowFeature("Idle Mean", "IlAG", true),
        FlowFeature("Idle Std", "IlSD", true),
        FlowFeature("Idle Max", "IlMA", true),
        FlowFeature("Idle Min", "IlMI", true),
        FlowFeature("Label", "LBL", false)
};

const FlowFeature FlowFeature::fid("Flow ID", "FID", false);            //1
const FlowFeature FlowFeature::src_ip("Src IP", "SIP", false);          //2
const FlowFeature FlowFeature::src_port("Src Port", "SPT", true);             //3
const FlowFeature FlowFeature::dst_ip("Dst IP","DIP",false);				//4
const FlowFeature FlowFeature::dst_port("Dst Port","DPT", true);					//5
const FlowFeature FlowFeature::prot("Protocol","PROT", true);					//6
const FlowFeature FlowFeature::tstp("Timestamp","TSTP",false);				//7
const FlowFeature FlowFeature::fl_dur("Flow Duration","DUR", true);				//8
const FlowFeature FlowFeature::tot_fw_pkt("Total Fwd Packet","TFwP", true);			//9
const FlowFeature FlowFeature::tot_bw_pkt("Total Bwd packets","TBwP", true);			//10
const FlowFeature FlowFeature::tot_l_fw_pkt("Total Length of Fwd Packet","TLFwP", true);		//11
const FlowFeature FlowFeature::tot_l_bw_pkt("Total Length of Bwd Packet","TLBwP", true);		//12
const FlowFeature FlowFeature::fw_pkt_l_max("Fwd Packet Length Max","FwPLMA", true);		//13
const FlowFeature FlowFeature::fw_pkt_l_min("Fwd Packet Length Min","FwPLMI", true);		//14
const FlowFeature FlowFeature::fw_pkt_l_avg("Fwd Packet Length Mean","FwPLAG", true);		//15
const FlowFeature FlowFeature::fw_pkt_l_std("Fwd Packet Length Std","FwPLSD", true);		//16
const FlowFeature FlowFeature::bw_pkt_l_max("Bwd Packet Length Max","BwPLMA", true);		//17
const FlowFeature FlowFeature::bw_pkt_l_min("Bwd Packet Length Min","BwPLMI", true);		//18
const FlowFeature FlowFeature::bw_pkt_l_avg("Bwd Packet Length Mean","BwPLAG", true);	//19
const FlowFeature FlowFeature::bw_pkt_l_std("Bwd Packet Length Std","BwPLSD", true);	    //20
const FlowFeature FlowFeature::fl_byt_s("Flow Bytes/s","FB/s", true);				//21
const FlowFeature FlowFeature::fl_pkt_s("Flow Packets/s","FP/s", true);				//22
const FlowFeature FlowFeature::fl_iat_avg("Flow IAT Mean","FLIATAG", true);			//23
const FlowFeature FlowFeature::fl_iat_std("Flow IAT Std","FLIATSD", true);			//24
const FlowFeature FlowFeature::fl_iat_max("Flow IAT Max","FLIATMA", true);			//25
const FlowFeature FlowFeature::fl_iat_min("Flow IAT Min","FLIATMI", true);			//26
const FlowFeature FlowFeature::fw_iat_tot("Fwd IAT Total","FwIATTO", true);			//27
const FlowFeature FlowFeature::fw_iat_avg("Fwd IAT Mean","FwIATAG", true);			//28
const FlowFeature FlowFeature::fw_iat_std("Fwd IAT Std","FwIATSD", true);			//29
const FlowFeature FlowFeature::fw_iat_max("Fwd IAT Max","FwIATMA", true);			//30
const FlowFeature FlowFeature::fw_iat_min("Fwd IAT Min","FwIATMI", true);			//31
const FlowFeature FlowFeature::bw_iat_tot("Bwd IAT Total","BwIATTO", true);			//32
const FlowFeature FlowFeature::bw_iat_avg("Bwd IAT Mean","BwIATAG", true);			//33
const FlowFeature FlowFeature::bw_iat_std("Bwd IAT Std","BwIATSD", true);			//34
const FlowFeature FlowFeature::bw_iat_max("Bwd IAT Max","BwIATMA", true);			//35
const FlowFeature FlowFeature::bw_iat_min("Bwd IAT Min","BwIATMI", true);			//36
const FlowFeature FlowFeature::fw_psh_flag("Fwd PSH Flags","FwPSH", true);			//37
const FlowFeature FlowFeature::bw_psh_flag("Bwd PSH Flags","BwPSH", true);			//38
const FlowFeature FlowFeature::fw_urg_flag("Fwd URG Flags","FwURG", true);			//39
const FlowFeature FlowFeature::bw_urg_flag("Bwd URG Flags","BwURG", true);			//40
const FlowFeature FlowFeature::fw_hdr_len("Fwd Header Length","FwHL", true);			//41
const FlowFeature FlowFeature::bw_hdr_len("Bwd Header Length","BwHL", true);			//42
const FlowFeature FlowFeature::fw_pkt_s("Fwd Packets/s","FwP/s", true);				//43
const FlowFeature FlowFeature::bw_pkt_s("Bwd Packets/s","Bwp/s", true);				//44
const FlowFeature FlowFeature::pkt_len_min("Packet Length Min","PLMI", true);			//45
const FlowFeature FlowFeature::pkt_len_max("Packet Length Max","PLMA", true);			//46
const FlowFeature FlowFeature::pkt_len_avg("Packet Length Mean","PLAG", true);			//47
const FlowFeature FlowFeature::pkt_len_std("Packet Length Std","PLSD", true);			//48
const FlowFeature FlowFeature::pkt_len_var("Packet Length Variance","PLVA", true);		//49
const FlowFeature FlowFeature::fin_cnt("FIN Flag Count","FINCT", true);				//50
const FlowFeature FlowFeature::syn_cnt("SYN Flag Count","SYNCT", true);				//51
const FlowFeature FlowFeature::rst_cnt("RST Flag Count","RSTCT", true);				//52
const FlowFeature FlowFeature::pst_cnt("PSH Flag Count","PSHCT", true);				//53
const FlowFeature FlowFeature::ack_cnt("ACK Flag Count","ACKCT", true);				//54
const FlowFeature FlowFeature::urg_cnt("URG Flag Count","URGCT", true);				//55
const FlowFeature FlowFeature::CWR_cnt("CWR Flag Count","CWRCT", true);				//56
const FlowFeature FlowFeature::ece_cnt("ECE Flag Count","ECECT", true);				//57
const FlowFeature FlowFeature::down_up_ratio("Down/Up Ratio","D/URO", true);			//58
const FlowFeature FlowFeature::pkt_size_avg("Average Packet Size","PSAG", true);			//59
const FlowFeature FlowFeature::fw_seg_avg("Fwd Segment Size Avg","FwSgAG", true);		//60
const FlowFeature FlowFeature::bw_seg_avg("Bwd Segment Size Avg","BwSgAG", true);		//61
const FlowFeature FlowFeature::fw_byt_blk_avg("Fwd Bytes/Bulk Avg","FwB/BAG", true);		//63   62 is duplicated with 41,so has been deleted
const FlowFeature FlowFeature::fw_pkt_blk_avg("Fwd Packet/Bulk Avg","FwP/BAG", true);		//64
const FlowFeature FlowFeature::fw_blk_rate_avg("Fwd Bulk Rate Avg","FwBRAG", true);		//65
const FlowFeature FlowFeature::bw_byt_blk_avg("Bwd Bytes/Bulk Avg","BwB/BAG", true);  	//66
const FlowFeature FlowFeature::bw_pkt_blk_avg("Bwd Packet/Bulk Avg","BwP/BAG", true);		//67
const FlowFeature FlowFeature::bw_blk_rate_avg("Bwd Bulk Rate Avg","BwBRAG", true);		//68
const FlowFeature FlowFeature::subfl_fw_pkt("Subflow Fwd Packets","SFFwP", true);		//69
const FlowFeature FlowFeature::subfl_fw_byt("Subflow Fwd Bytes","SFFwB", true);			//70
const FlowFeature FlowFeature::subfl_bw_pkt("Subflow Bwd Packets","SFBwP", true);		//71
const FlowFeature FlowFeature::subfl_bw_byt("Subflow Bwd Bytes","SFBwB", true);			//72
const FlowFeature FlowFeature::fw_win_byt("FWD Init Win Bytes","FwWB", true);			//73
const FlowFeature FlowFeature::bw_win_byt("Bwd Init Win Bytes","BwWB", true);			//74
const FlowFeature FlowFeature::Fw_act_pkt("Fwd Act Data Pkts","FwAP", true);		//75
const FlowFeature FlowFeature::fw_seg_min("Fwd Seg Size Min","FwSgMI", true);			//76
const FlowFeature FlowFeature::atv_avg("Active Mean","AcAG", true);				//77
const FlowFeature FlowFeature::atv_std("Active Std","AcSD", true);				//78
const FlowFeature FlowFeature::atv_max("Active Max","AcMA", true);				//79
const FlowFeature FlowFeature::atv_min("Active Min","AcMI", true);				//80
const FlowFeature FlowFeature::idl_avg("Idle Mean","IlAG", true);				//81
const FlowFeature FlowFeature::idl_std("Idle Std","IlSD", true);					//82
const FlowFeature FlowFeature::idl_max("Idle Max","IlMA", true);					//83
const FlowFeature FlowFeature::idl_min("Idle Min","IlMI", true);					//84
const FlowFeature FlowFeature::Label("Label","LBL", false);	        //85

std::string FlowFeature::getHeader() {
    if (HEADER.empty()) {
        std::ostringstream header;
        for (const FlowFeature& feature : features) {
            header << feature.getName() << ",";
        }
        std::string headerStr = header.str();
        HEADER = headerStr.substr(0, headerStr.length() - 1);
    }
    return HEADER;
}

std::vector<FlowFeature> FlowFeature::getFeatureList() {
    std::vector<FlowFeature> features;
    features.push_back(prot);
    features.push_back(fl_dur);
    features.push_back(tot_fw_pkt);
    features.push_back(tot_bw_pkt);
    features.push_back(tot_l_fw_pkt);
    features.push_back(tot_l_bw_pkt);
    features.push_back(fw_pkt_l_max);
    features.push_back(fw_pkt_l_min);
    features.push_back(fw_pkt_l_avg);
    features.push_back(fw_pkt_l_std);
    features.push_back(bw_pkt_l_max);
    features.push_back(bw_pkt_l_min);
    features.push_back(bw_pkt_l_avg);
    features.push_back(bw_pkt_l_std);
    features.push_back(fl_byt_s);
    features.push_back(fl_pkt_s);
    features.push_back(fl_iat_avg);
    features.push_back(fl_iat_std);
    features.push_back(fl_iat_max);
    features.push_back(fl_iat_min);
    features.push_back(fw_iat_tot);
    features.push_back(fw_iat_avg);
    features.push_back(fw_iat_std);
    features.push_back(fw_iat_max);
    features.push_back(fw_iat_min);
    features.push_back(bw_iat_tot);
    features.push_back(bw_iat_avg);
    features.push_back(bw_iat_std);
    features.push_back(bw_iat_max);
    features.push_back(bw_iat_min);
    features.push_back(fw_psh_flag);
    features.push_back(bw_psh_flag);
    features.push_back(fw_urg_flag);
    features.push_back(bw_urg_flag);
    features.push_back(fw_hdr_len);
    features.push_back(bw_hdr_len);
    features.push_back(fw_pkt_s);
    features.push_back(bw_pkt_s);
    features.push_back(pkt_len_min);
    features.push_back(pkt_len_max);
    features.push_back(pkt_len_avg);
    features.push_back(pkt_len_std);
    features.push_back(pkt_len_var);
    features.push_back(fin_cnt);
    features.push_back(syn_cnt);
    features.push_back(rst_cnt);
    features.push_back(pst_cnt);
    features.push_back(ack_cnt);
    features.push_back(urg_cnt);
    features.push_back(CWR_cnt);
    features.push_back(ece_cnt);
    features.push_back(down_up_ratio);
    features.push_back(pkt_size_avg);
    features.push_back(fw_seg_avg);
    features.push_back(bw_seg_avg);
    features.push_back(fw_byt_blk_avg);
    features.push_back(fw_pkt_blk_avg);
    features.push_back(fw_blk_rate_avg);
    features.push_back(bw_byt_blk_avg);
    features.push_back(bw_pkt_blk_avg);
    features.push_back(bw_blk_rate_avg);
    features.push_back(subfl_fw_pkt);
    features.push_back(subfl_fw_byt);
    features.push_back(subfl_bw_pkt);
    features.push_back(subfl_bw_byt);
    features.push_back(fw_win_byt);
    features.push_back(bw_win_byt);
    features.push_back(Fw_act_pkt);
    features.push_back(fw_seg_min);
    features.push_back(atv_avg);
    features.push_back(atv_std);
    features.push_back(atv_max);
    features.push_back(atv_min);
    features.push_back(idl_avg);
    features.push_back(idl_std);
    features.push_back(idl_max);
    features.push_back(idl_min);
    return features;
}

std::vector<FlowFeature> FlowFeature::getLengthFeature() {
    std::vector<FlowFeature> features;
    features.push_back(tot_l_fw_pkt);
    features.push_back(tot_l_bw_pkt);
    features.push_back(fl_byt_s);
    features.push_back(fl_pkt_s);
    features.push_back(fw_hdr_len);
    features.push_back(bw_hdr_len);
    features.push_back(fw_pkt_s);
    features.push_back(bw_pkt_s);
    features.push_back(pkt_size_avg);
    features.push_back(fw_seg_avg);
    features.push_back(bw_seg_avg);
    return features;
}

std::string FlowFeature::featureValue2String(const FlowFeature& feature, const std::string& value) {
    std::string ret = value;

    if (feature.getName() == "Protocol") {
        try {
            int number = std::stoi(value);
            if (number == 6) {
                ret = "TCP";
            } else if (number == 17) {
                ret = "UDP";
            } else {
                ret = "Others";
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << ". value is " << value << std::endl;
            ret = "Others";
        }
    }

    return ret;
}