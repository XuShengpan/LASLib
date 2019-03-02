#include "las_io.h"

int main(int argc, char* argv[])
{
    std::string las_file_path = "F:/20190302/pcd_las/lidar20190104094351_0.las";
    mpcdps::las_lib::LASDataPtr las_data;
    mpcdps::las_lib::read_las_file(las_file_path, las_data);
    //mpcdps::las_lib::update_coord_range(las_data);
    return 0;
}