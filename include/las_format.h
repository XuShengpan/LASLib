#ifndef MPCDPS_LAS_LIB_LAS_FORMAT_H
#define MPCDPS_LAS_LIB_LAS_FORMAT_H

/*
    Author: Xu Shengpan
    Email: jack_1227x@163.com
*/

#include <string>
#include <cassert>
#include <PublicInfo.h>
#include <SmartPointer.h>
#include <vector>

namespace mpcdps {
    namespace las_lib {

        const int public_header_block_length = 227;
        const int variable_length_record_header_length = 54;

        const int point_data_length_format_0 = 20;
        const int point_data_length_format_1 = 28;
        const int point_data_length_format_2 = 26;
        const int point_data_length_format_3 = 34;

        inline int point_data_length(int point_format)
        {
            switch (point_format) {
            case 0:
                return point_data_length_format_0;
            case 1:
                return point_data_length_format_1;
            case 2:
                return point_data_length_format_2;
            case  3:
                return point_data_length_format_3;
            default:
                assert(false);
                return 0;
            }
            return 0;
        }

        struct LASPublicHeaderBlock
        {
            char file_signature[4];
            ushort file_source_id;
            ushort global_encoding;
            ulong project_id_guid_1;
            ushort project_id_guid_2;
            ushort project_id_guid_3;
            uchar project_id_guid_4[8];
            uchar version_major;
            uchar version_minor;
            char system_identifier[32];
            char generating_software[32];
            ushort file_creation_day_of_year;
            ushort file_creation_year;
            ushort header_size;
            ulong offset_to_point_data;
            ulong number_of_veriable_length_records;
            uchar point_data_format_id;
            ushort point_data_record_length;
            ulong number_of_point_records;
            ulong number_of_points_by_return[5];
            double x_scale_factor = 1;
            double y_scale_factor = 1;
            double z_scale_factor = 1;
            double x_offset = 0;
            double y_offset = 0;
            double z_offset = 0;
            double max_x;
            double min_x;
            double max_y;
            double min_y;
            double max_z;
            double min_z;

            bool read(const uchar* buffer);

            void write(uchar buffer[227]) const;
        };

        //variable length record header
        struct VLRHeader
        {
            ushort reserved;
            char user_id[16];
            ushort record_id;
            ushort record_length_after_header;
            char description[32];

            void read(const uchar* buffer);
            void write(uchar buffer[54]) const;
        };

        struct VLR
        {
            VLRHeader header;
            char* content = nullptr;

            ~VLR()
            {
                if (content) {
                    delete[] content;
                }
            }
        };

        struct ScanInfo
        {
            uchar return_number : 3;
            uchar number_of_returns : 3;
            uchar scan_direction_flag : 1;
            uchar edge_of_flight_line : 1;
        };

        struct PointDataRecordFormat0
        {
            long x;
            long y;
            long z;
            ushort intensity;
            ScanInfo scan_info;
            uchar classification;
            char scan_angle;
            uchar user_data;
            ushort point_source_id;

            void read(const uchar* buffer);
        };

        struct PointDataRecordFormat1: public PointDataRecordFormat0
        {
            double gps_time;

            void read(const uchar* buffer);
        };

        struct PointDataRecordFormat2 : public PointDataRecordFormat0
        {
            ushort red;
            ushort green;
            ushort blue;
            void read(const uchar* buffer);
        };

        struct PointDataRecordFormat3 : public PointDataRecordFormat0
        {
            double gps_time;
            ushort red;
            ushort green;
            ushort blue;
            void read(const uchar* buffer);
        };

        inline void write_point_data_format_0(const PointDataRecordFormat0& point, uchar buffer[20])
        {
            long* ptr = (long*)buffer;
            ptr[0] = point.x;
            ptr[1] = point.y;
            ptr[2] = point.z;
            *((ushort*)(buffer + 12)) = point.intensity;
            memcpy(buffer + 14, &(point.scan_info), 1);
            buffer[15] = point.classification;
            buffer[16] = point.scan_angle;
            buffer[17] = point.user_data;
            *((ushort*)(buffer + 18)) = point.point_source_id;
        }

        void write_point_data(const PointDataRecordFormat0& point, uchar buffer[20]);
        void write_point_data(const PointDataRecordFormat1& point, uchar buffer[28]);
        void write_point_data(const PointDataRecordFormat2& point, uchar buffer[26]);
        void write_point_data(const PointDataRecordFormat3& point, uchar buffer[34]);

        inline double get_absolute_value(long relative_v, double scale, double offset)
        {
            return relative_v * scale + offset;
        }

        inline long get_relative_value(double absolute_v, double scale, double offset)
        {
            return long((absolute_v - offset) / scale + 0.5);
        }
    }
}

#endif