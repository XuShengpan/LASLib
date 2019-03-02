#ifndef MPCDPS_LAS_LIB_LAS_READER_H
#define MPCDPS_LAS_LIB_LAS_READER_H

/*
    Author: Xu Shengpan
    Email: jack_1227x@163.com
*/

#include <string>
#include <vector>
#include "las_format.h"
#include <SmartArray2D.h>
#include <SmartPointer.h>

namespace mpcdps {
namespace las_lib {

    struct LASData
    {
        LASPublicHeaderBlock header;
        std::vector<VLR> vlrs;
        SmartArray2D<double, 3> vertex_array;
        std::vector<ushort> intensities;
        std::vector<ScanInfo> scan_infos;
        std::vector<uchar> classification;
        std::vector<uchar> scan_angle;
        std::vector<uchar> user_data_array;
        std::vector<ushort> point_source_ids;
        std::vector<double> gps_times;
        SmartArray2D<ushort, 3> colors; //r, g, b
    };

    typedef SmartPointer<LASData> LASDataPtr;

    bool read_las_file(const std::string& las_file_path, LASDataPtr& data);
    void update_coord_range(LASData* data);
    void write_las_file(const std::string las_file_path, const LASDataPtr& data);
}
}

#endif