#include "las_format.h"

namespace mpcdps {
namespace las_lib {

    bool LASPublicHeaderBlock::read(const uchar* buffer)
    {
        if (buffer[0] != 'L' || buffer[1] != 'A' || buffer[2] != 'S' || buffer[3] != 'F') {
            return false;
        }

        memcpy(file_signature, buffer, 4);
        file_source_id = *((ushort*)(buffer + 4));
        global_encoding = *((ushort*)(buffer + 6));
        project_id_guid_1 = *((ulong*)(buffer + 8));
        project_id_guid_2 = *((ushort*)(buffer + 12));
        project_id_guid_3 = *((ushort*)(buffer + 14));
        memcpy(project_id_guid_4, buffer + 16, 8);
        version_major = buffer[24];
        version_minor = buffer[25];
        memcpy(system_identifier, buffer + 26, 32);
        memcpy(generating_software, buffer + 58, 32);
        file_creation_day_of_year = *((ushort*)(buffer + 90));
        file_creation_year = *((ushort*)(buffer + 92));
        header_size = *((ushort*)(buffer + 94));
        offset_to_point_data = *((ulong*)(buffer + 96));
        number_of_veriable_length_records = *((ulong*)(buffer + 100));
        point_data_format_id = buffer[104];
        point_data_record_length = *((ushort*)(buffer + 105));
        number_of_point_records = *((ulong*)(buffer + 107));

        const ulong* ptr1 = (ulong*)(buffer + 111);
        for (int i = 0; i < 5; ++i) {
            number_of_points_by_return[i] = ptr1[i];
        }

        const double* ptr2 = (double*)(buffer + 131);
        x_scale_factor = ptr2[0];
        y_scale_factor = ptr2[1];
        z_scale_factor = ptr2[2];
        x_offset = ptr2[3];
        y_offset = ptr2[4];
        z_offset = ptr2[5];
        max_x = ptr2[6];
        min_x = ptr2[7];
        max_y = ptr2[8];
        min_y = ptr2[9];
        max_z = ptr2[10];
        min_z = ptr2[11];

        return true;
    }

    void LASPublicHeaderBlock::write(uchar buffer[227]) const
    {
        buffer[0] = 'L';
        buffer[1] = 'A';
        buffer[2] = 'S';
        buffer[3] = 'F';

        memcpy(buffer, file_signature, 4);

        *((ushort*)(buffer + 4)) = file_source_id;
        *((ushort*)(buffer + 6)) = global_encoding;
        *((ulong*)(buffer + 8)) = project_id_guid_1;
        *((ushort*)(buffer + 12)) = project_id_guid_2;
        *((ushort*)(buffer + 14)) = project_id_guid_3;
        memcpy(buffer + 16, project_id_guid_4, 8);
        buffer[24] = version_major;
        buffer[25] = version_minor;
        memcpy(buffer + 26, system_identifier, 32);
        memcpy(buffer + 58, generating_software, 32);
        *((ushort*)(buffer + 90)) = file_creation_day_of_year;
        *((ushort*)(buffer + 92)) = file_creation_year;
        *((ushort*)(buffer + 94)) = header_size;
        *((ulong*)(buffer + 96)) = offset_to_point_data;
        *((ulong*)(buffer + 100)) = number_of_veriable_length_records;
        buffer[104] = point_data_format_id;
        *((ushort*)(buffer + 105)) = point_data_record_length;
        *((ulong*)(buffer + 107)) = number_of_point_records;

        ulong* ptr1 = (ulong*)(buffer + 111);
        for (int i = 0; i < 5; ++i) {
            ptr1[i] = number_of_points_by_return[i];
        }

        double* ptr2 = (double*)(buffer + 131);
        ptr2[0] = x_scale_factor;
        ptr2[1] = y_scale_factor;
        ptr2[2] = z_scale_factor;
        ptr2[3] = x_offset;
        ptr2[4] = y_offset;
        ptr2[5] = z_offset;
        ptr2[6] = max_x;
        ptr2[7] = min_x;
        ptr2[8] = max_y;
        ptr2[9] = min_y;
        ptr2[10] = max_z;
        ptr2[11] = min_z;
    }

    void VLRHeader::read(const uchar* buffer)
    {
        reserved = *(ushort*)buffer;
        memcpy(user_id, buffer + 2, 16);
        record_id = *((ushort*)(buffer + 18));
        record_length_after_header = *((ushort*)(buffer + 20));
        memcpy(description, buffer + 22, 32);
    }

    void VLRHeader::write(uchar buffer[54]) const
    {
        *(ushort*)buffer = reserved;
        memcpy(buffer + 2, user_id, 16);
        *((ushort*)(buffer + 18)) = record_id;
        *((ushort*)(buffer + 20)) = record_length_after_header;
        memcpy(buffer + 22, description, 32);
    }

    void PointDataRecordFormat0::read(const uchar* buffer)
    {
        long* ptr = (long*)buffer;
        x = ptr[0];
        y = ptr[1];
        z = ptr[2];
        intensity = *((ushort*)(buffer + 12));
        memcpy(&scan_info, buffer + 14, 1);
        classification = buffer[15];
        scan_angle = buffer[16];
        user_data = buffer[17];
        point_source_id = *((ushort*)(buffer + 18));
    }

    void PointDataRecordFormat1::read(const uchar* buffer)
    {
        PointDataRecordFormat0::read(buffer);
        gps_time = *(double*)(buffer + 20);
    }

    void PointDataRecordFormat2::read(const uchar* buffer)
    {
        PointDataRecordFormat0::read(buffer);
        ushort* color = (ushort*)(buffer + 20);
        red = color[0];
        green = color[1];
        blue = color[2];
    }

    void PointDataRecordFormat3::read(const uchar* buffer)
    {
        PointDataRecordFormat0::read(buffer);
        gps_time = *(double*)(buffer + 20);
        ushort* color = (ushort*)(buffer + 28);
        red = color[0];
        green = color[1];
        blue = color[2];
    }

    void write_point_data(const PointDataRecordFormat0& point, uchar buffer[20])
    {
        write_point_data_format_0(point, buffer);
    }

    void write_point_data(const PointDataRecordFormat1& point, uchar buffer[28])
    {
        write_point_data_format_0(point, buffer);
        memcpy(buffer + 20, &(point.gps_time), 8);
    }

    void write_point_data(const PointDataRecordFormat2& point, uchar buffer[26])
    {
        write_point_data_format_0(point, buffer);
        ushort* color = (ushort*)(buffer + 20);
        color[0] = point.red;
        color[1] = point.green;
        color[2] = point.blue;
    }

    void write_point_data(const PointDataRecordFormat3& point, uchar buffer[34])
    {
        write_point_data_format_0(point, buffer);
        memcpy(buffer + 20, &(point.gps_time), 8);
        ushort* color = (ushort*)(buffer + 28);
        color[0] = point.red;
        color[1] = point.green;
        color[2] = point.blue;
    }
}
}