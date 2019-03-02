#include "las_io.h"
#include <fstream>
#include <iostream>
#include <SmartArray.h>

namespace mpcdps {
    namespace las_lib {

        bool read_las_file(const std::string& las_file_path, LASDataPtr& data)
        {
            std::ifstream infile(las_file_path, std::ios::binary);

            SmartArray<char> head_buffer(public_header_block_length);
            infile.read(head_buffer.buffer(), public_header_block_length);
            LASPublicHeaderBlock header;
            if (!header.read((uchar*)head_buffer.buffer())) {
                std::cout << "read las file failed: " << las_file_path << std::endl;
                return false;
            }

            data = new LASData;
            data->header = header;

            SmartArray<char> vrl_header_buffer(variable_length_record_header_length);
            for (int i = 0; i < header.number_of_veriable_length_records; ++i) {
                infile.read(vrl_header_buffer.buffer(), variable_length_record_header_length);

                VLR vlr;
                vlr.header.read((uchar*)(vrl_header_buffer.buffer()));
                int buffer_len = vlr.header.record_length_after_header;
                vlr.content = new char[buffer_len];
                infile.read(vlr.content, buffer_len);

                data->vlrs.push_back(vlr);
            }

            //infile.seekg(header.offset_to_point_data);
            infile.seekg(0);
            infile.seekg(header.offset_to_point_data);
            int point_data_record_length = header.point_data_record_length;
            int point_data_record_length_1 = point_data_length(header.point_data_format_id);

            int n_point = header.number_of_point_records;
            SmartArray<uchar> point_data_buffer(point_data_record_length);
            char* point_ptr = (char*)(point_data_buffer.buffer());

            data->vertex_array.resize(n_point);
            data->intensities.resize(n_point);
            data->classification.resize(n_point);
            data->scan_infos.resize(n_point);
            data->scan_angle.resize(n_point);
            data->user_data_array.resize(n_point);
            data->point_source_ids.resize(n_point);

            switch (header.point_data_format_id)
            {
            case 0:
            {
                PointDataRecordFormat0 point;
                for (int i = 0; i < n_point; ++i) {
                    infile.read(point_ptr, point_data_record_length);
                    point.read(point_data_buffer.buffer());

                    data->vertex_array[i][0] = get_absolute_value(point.x, header.x_scale_factor, header.x_offset);
                    data->vertex_array[i][1] = get_absolute_value(point.y, header.y_scale_factor, header.y_offset);
                    data->vertex_array[i][2] = get_absolute_value(point.z, header.z_scale_factor, header.z_offset);

                    data->intensities[i] = point.intensity;
                    data->classification[i] = point.classification;
                    data->scan_infos[i] = point.scan_info;
                    data->scan_angle[i] = point.scan_angle;
                    data->user_data_array[i] = point.user_data;
                    data->point_source_ids[i] = point.point_source_id;
                }

                break;
            }
            case 1:
            {
                PointDataRecordFormat1 point;
                data->gps_times.resize(n_point);

                for (int i = 0; i < n_point; ++i) {
                    infile.read(point_ptr, point_data_record_length);
                    point.read(point_data_buffer.buffer());

                    data->vertex_array[i][0] = get_absolute_value(point.x, header.x_scale_factor, header.x_offset);
                    data->vertex_array[i][1] = get_absolute_value(point.y, header.y_scale_factor, header.y_offset);
                    data->vertex_array[i][2] = get_absolute_value(point.z, header.z_scale_factor, header.z_offset);

                    data->intensities[i] = point.intensity;
                    data->classification[i] = point.classification;
                    data->scan_infos[i] = point.scan_info;
                    data->scan_angle[i] = point.scan_angle;
                    data->user_data_array[i] = point.user_data;
                    data->point_source_ids[i] = point.point_source_id;
                    data->gps_times[i] = point.gps_time;
                }
                break;
            }
            case 2:
            {
                PointDataRecordFormat2 point;
                data->colors.resize(n_point);

                for (int i = 0; i < n_point; ++i) {
                    infile.read(point_ptr, point_data_record_length);
                    point.read(point_data_buffer.buffer());

                    data->vertex_array[i][0] = get_absolute_value(point.x, header.x_scale_factor, header.x_offset);
                    data->vertex_array[i][1] = get_absolute_value(point.y, header.y_scale_factor, header.y_offset);
                    data->vertex_array[i][2] = get_absolute_value(point.z, header.z_scale_factor, header.z_offset);

                    data->intensities[i] = point.intensity;
                    data->classification[i] = point.classification;
                    data->scan_infos[i] = point.scan_info;
                    data->scan_angle[i] = point.scan_angle;
                    data->user_data_array[i] = point.user_data;
                    data->point_source_ids[i] = point.point_source_id;
                    data->colors[i][0] = point.red;
                    data->colors[i][1] = point.green;
                    data->colors[i][2] = point.blue;
                }
                break;
            }
            case 3:
            {
                PointDataRecordFormat3 point;
                data->gps_times.resize(n_point);
                data->colors.resize(n_point);

                for (int i = 0; i < n_point; ++i) {
                    infile.read(point_ptr, point_data_record_length);
                    point.read(point_data_buffer.buffer());

                    data->vertex_array[i][0] = get_absolute_value(point.x, header.x_scale_factor, header.x_offset);
                    data->vertex_array[i][1] = get_absolute_value(point.y, header.y_scale_factor, header.y_offset);
                    data->vertex_array[i][2] = get_absolute_value(point.z, header.z_scale_factor, header.z_offset);

                    data->intensities[i] = point.intensity;
                    data->classification[i] = point.classification;
                    data->scan_infos[i] = point.scan_info;
                    data->scan_angle[i] = point.scan_angle;
                    data->user_data_array[i] = point.user_data;
                    data->point_source_ids[i] = point.point_source_id;
                    data->gps_times[i] = point.gps_time;
                    data->colors[i][0] = point.red;
                    data->colors[i][1] = point.green;
                    data->colors[i][2] = point.blue;
                }
                break;
            }
            default:
                break;
            }
            return true;
        }

        void update_coord_range(LASData* data)
        {
            if (data == NULL) {
                return;
            }
            double xmin = DBL_MAX;
            double xmax = -DBL_MAX;
            double ymin = DBL_MAX;
            double ymax = -DBL_MAX;
            double zmin = DBL_MAX;
            double zmax = -DBL_MAX;

            for (int i = 0; i < data->vertex_array.size(); ++i) {
                double x = data->vertex_array[i][0];
                double y = data->vertex_array[i][1];
                double z = data->vertex_array[i][2];

                if (x < xmin) {
                    xmin = x;
                }
                if (x > xmax) {
                    xmax = x;
                }
                if (y < ymin) {
                    ymin = y;
                }
                if (y > ymax) {
                    ymax = y;
                }
                if (z < zmin) {
                    zmin = z;
                }
                if (z > zmax) {
                    zmax = z;
                }
            }

            data->header.max_x = xmax;
            data->header.min_x = xmin;
            data->header.max_y = ymax;
            data->header.min_y = ymin;
            data->header.max_z = zmax;
            data->header.min_z = zmin;
        }

        void write_las_file(const std::string las_file_path, const LASDataPtr& data)
        {
            if (data.buffer() == NULL) {
                return;
            }

            std::ofstream ofile(las_file_path, std::ios::binary);

            {
                uchar buf[227];
                data->header.write(buf);
                ofile.write((char*)buf, 227);
            }

            {
                uchar buf[54];
                for (int i = 0; i < data->header.number_of_veriable_length_records; ++i) {
                    data->vlrs[i].header.write(buf);
                    ofile.write((char*)buf, 54);
                    ofile.write(data->vlrs[i].content, data->vlrs[i].header.record_length_after_header);
                }
            }

            int point_format_id = data->header.point_data_format_id;
            int point_data_size = data->header.point_data_record_length;

            SmartArray<char> point_buffer(point_data_size);

            switch (point_format_id)
            {
            case 0:
            {
                PointDataRecordFormat0 point;
                for (int i = 0; i < data->header.number_of_point_records; ++i) {
                    double x = data->vertex_array[i][0];
                    double y = data->vertex_array[i][1];
                    double z = data->vertex_array[i][2];

                    long rx = get_relative_value(x, data->header.x_scale_factor, data->header.x_offset);
                    long ry = get_relative_value(y, data->header.y_scale_factor, data->header.y_offset);
                    long rz = get_relative_value(z, data->header.z_scale_factor, data->header.z_offset);

                    point.x = rx;
                    point.y = ry;
                    point.z = rz;
                    point.classification = data->classification[i];
                    point.intensity = data->intensities[i];
                    point.point_source_id = data->point_source_ids[i];
                    point.scan_angle = data->scan_angle[i];
                    point.scan_info = data->scan_infos[i];
                    point.user_data = data->user_data_array[i];

                    write_point_data(point, (uchar*)point_buffer.buffer());
                    ofile.write(point_buffer.buffer(), point_data_size);
                }

                break;
            }
            case 1:
            {
                PointDataRecordFormat1 point;
                for (int i = 0; i < data->header.number_of_point_records; ++i) {
                    double x = data->vertex_array[i][0];
                    double y = data->vertex_array[i][1];
                    double z = data->vertex_array[i][2];

                    long rx = get_relative_value(x, data->header.x_scale_factor, data->header.x_offset);
                    long ry = get_relative_value(y, data->header.y_scale_factor, data->header.y_offset);
                    long rz = get_relative_value(z, data->header.z_scale_factor, data->header.z_offset);

                    point.x = rx;
                    point.y = ry;
                    point.z = rz;
                    point.classification = data->classification[i];
                    point.intensity = data->intensities[i];
                    point.point_source_id = data->point_source_ids[i];
                    point.scan_angle = data->scan_angle[i];
                    point.scan_info = data->scan_infos[i];
                    point.user_data = data->user_data_array[i];
                    point.gps_time = data->gps_times[i];

                    write_point_data(point, (uchar*)point_buffer.buffer());
                    ofile.write(point_buffer.buffer(), point_data_size);
                }

                break;
            }
            case 2:
            {
                PointDataRecordFormat2 point;
                for (int i = 0; i < data->header.number_of_point_records; ++i) {
                    double x = data->vertex_array[i][0];
                    double y = data->vertex_array[i][1];
                    double z = data->vertex_array[i][2];

                    long rx = get_relative_value(x, data->header.x_scale_factor, data->header.x_offset);
                    long ry = get_relative_value(y, data->header.y_scale_factor, data->header.y_offset);
                    long rz = get_relative_value(z, data->header.z_scale_factor, data->header.z_offset);

                    point.x = rx;
                    point.y = ry;
                    point.z = rz;
                    point.classification = data->classification[i];
                    point.intensity = data->intensities[i];
                    point.point_source_id = data->point_source_ids[i];
                    point.scan_angle = data->scan_angle[i];
                    point.scan_info = data->scan_infos[i];
                    point.user_data = data->user_data_array[i];
                    point.red = data->colors[i][0];
                    point.green = data->colors[i][1];
                    point.blue = data->colors[i][2];

                    write_point_data(point, (uchar*)point_buffer.buffer());
                    ofile.write(point_buffer.buffer(), point_data_size);
                }

                break;
            }
            case 3:
            {
                PointDataRecordFormat3 point;
                for (int i = 0; i < data->header.number_of_point_records; ++i) {
                    double x = data->vertex_array[i][0];
                    double y = data->vertex_array[i][1];
                    double z = data->vertex_array[i][2];

                    long rx = get_relative_value(x, data->header.x_scale_factor, data->header.x_offset);
                    long ry = get_relative_value(y, data->header.y_scale_factor, data->header.y_offset);
                    long rz = get_relative_value(z, data->header.z_scale_factor, data->header.z_offset);

                    point.x = rx;
                    point.y = ry;
                    point.z = rz;
                    point.classification = data->classification[i];
                    point.intensity = data->intensities[i];
                    point.point_source_id = data->point_source_ids[i];
                    point.scan_angle = data->scan_angle[i];
                    point.scan_info = data->scan_infos[i];
                    point.user_data = data->user_data_array[i];
                    point.gps_time = data->gps_times[i];
                    point.red = data->colors[i][0];
                    point.green = data->colors[i][1];
                    point.blue = data->colors[i][2];

                    write_point_data(point, (uchar*)point_buffer.buffer());
                    ofile.write(point_buffer.buffer(), point_data_size);
                }

                break;
            }
            default:
                break;
            }

            ofile.flush();
            ofile.close();
        }

    }
}