//
// Created by zyzshishui on 23-7-20.
//

#ifndef IDS_FLOWFEATURE_H


#define IDS_FLOWFEATURE_H

#include <iostream>
#include <vector>
#include <string>

class FlowFeature {
public:
    FlowFeature(const std::string& name, const std::string& abbr, bool isNumeric = true);

    FlowFeature(const std::string &name, const std::string &abbr);

    FlowFeature(const std::string &name, const std::string &abbr, std::vector<std::string> values);

    std::string getName() const;
    std::string getAbbr() const;
    bool isNumericFeature() const;

    static std::string featureValue2String(const FlowFeature& feature, const std::string& value);
    static std::string getHeader();
    static std::vector<FlowFeature> getFeatureList();
    static std::vector<FlowFeature> getLengthFeature();

private:
    std::string name;
    std::string abbr;
    bool isNumeric = true;
    std::vector<std::string> values;

    static std::string HEADER;
    static const FlowFeature features[];

    static const FlowFeature fid;
    static const FlowFeature src_ip;
    static const FlowFeature src_port;
    static const FlowFeature dst_ip;
    static const FlowFeature dst_port;
    static const FlowFeature prot;
    static const FlowFeature tstp;
    static const FlowFeature fl_dur;
    static const FlowFeature tot_fw_pkt;
    static const FlowFeature tot_bw_pkt;
    static const FlowFeature tot_l_fw_pkt;
    static const FlowFeature tot_l_bw_pkt;
    static const FlowFeature fw_pkt_l_max;
    static const FlowFeature fw_pkt_l_min;
    static const FlowFeature fw_pkt_l_avg;
    static const FlowFeature fw_pkt_l_std;
    static const FlowFeature bw_pkt_l_max;
    static const FlowFeature bw_pkt_l_min;
    static const FlowFeature bw_pkt_l_avg;
    static const FlowFeature bw_pkt_l_std;
    static const FlowFeature fl_byt_s;
    static const FlowFeature fl_pkt_s;
    static const FlowFeature fl_iat_avg;
    static const FlowFeature fl_iat_std;
    static const FlowFeature fl_iat_max;
    static const FlowFeature fl_iat_min;
    static const FlowFeature fw_iat_tot;
    static const FlowFeature fw_iat_avg;
    static const FlowFeature fw_iat_std;
    static const FlowFeature fw_iat_max;
    static const FlowFeature fw_iat_min;
    static const FlowFeature bw_iat_tot;
    static const FlowFeature bw_iat_avg;
    static const FlowFeature bw_iat_std;
    static const FlowFeature bw_iat_max;
    static const FlowFeature bw_iat_min;
    static const FlowFeature fw_psh_flag;
    static const FlowFeature bw_psh_flag;
    static const FlowFeature fw_urg_flag;
    static const FlowFeature bw_urg_flag;
    static const FlowFeature fw_hdr_len;
    static const FlowFeature bw_hdr_len;
    static const FlowFeature fw_pkt_s;
    static const FlowFeature bw_pkt_s;
    static const FlowFeature pkt_len_min;
    static const FlowFeature pkt_len_max;
    static const FlowFeature pkt_len_avg;
    static const FlowFeature pkt_len_std;
    static const FlowFeature pkt_len_var;
    static const FlowFeature fin_cnt;
    static const FlowFeature syn_cnt;
    static const FlowFeature rst_cnt;
    static const FlowFeature pst_cnt;
    static const FlowFeature ack_cnt;
    static const FlowFeature urg_cnt;
    static const FlowFeature CWR_cnt;
    static const FlowFeature ece_cnt;
    static const FlowFeature down_up_ratio;
    static const FlowFeature pkt_size_avg;
    static const FlowFeature fw_seg_avg;
    static const FlowFeature bw_seg_avg;
    static const FlowFeature fw_byt_blk_avg;
    static const FlowFeature fw_pkt_blk_avg;
    static const FlowFeature fw_blk_rate_avg;
    static const FlowFeature bw_byt_blk_avg;
    static const FlowFeature bw_pkt_blk_avg;
    static const FlowFeature bw_blk_rate_avg;
    static const FlowFeature subfl_fw_pkt;
    static const FlowFeature subfl_fw_byt;
    static const FlowFeature subfl_bw_pkt;
    static const FlowFeature subfl_bw_byt;
    static const FlowFeature fw_win_byt;
    static const FlowFeature bw_win_byt;
    static const FlowFeature Fw_act_pkt;
    static const FlowFeature fw_seg_min;
    static const FlowFeature atv_avg;
    static const FlowFeature atv_std;
    static const FlowFeature atv_max;
    static const FlowFeature atv_min;
    static const FlowFeature idl_avg;
    static const FlowFeature idl_std;
    static const FlowFeature idl_max;
    static const FlowFeature idl_min;
    static const FlowFeature Label;
};

#endif //IDS_FLOWFEATURE_H
