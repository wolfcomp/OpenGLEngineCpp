#pragma once
#include <vector>
#include <functional>

struct LASHeader
{
    char file_signature[4];
    unsigned short file_source_id;
    unsigned short global_encoding;
    unsigned char project_guid[16];
    unsigned char version_major;
    unsigned char version_minor;
    char system_id[32];
    char generating_software[32];
    unsigned short file_creation_day;
    unsigned short file_creation_year;
    unsigned short header_size;
    unsigned int offset_to_point_data;
    unsigned int number_of_variable_length_records;
    unsigned char point_data_format;
    unsigned short point_data_record_length;
    unsigned int number_of_point_records;
    unsigned int number_of_points_by_return[5];
    double x_scale_factor;
    double y_scale_factor;
    double z_scale_factor;
    double x_offset;
    double y_offset;
    double z_offset;
    double max_x;
    double min_x;
    double max_y;
    double min_y;
    double max_z;
    double min_z;
};

struct Point0
{
    int x;
    int y;
    int z;
    unsigned short intensity;
    unsigned char return_number : 3;
    unsigned char number_of_returns : 3;
    unsigned char scan_direction_flag : 1;
    unsigned char edge_of_flight_line : 1;
    unsigned char classification;
    unsigned char scan_angle_rank;
    unsigned char user_data;
    unsigned short point_source_id;
};

struct Point1
{
    int x;
    int y;
    int z;
    unsigned short intensity;
    unsigned char return_number : 3;
    unsigned char number_of_returns : 3;
    unsigned char scan_direction_flag : 1;
    unsigned char edge_of_flight_line : 1;
    unsigned char classification;
    unsigned char scan_angle_rank;
    unsigned char user_data;
    unsigned short point_source_id;
    double gps_time;
};

struct Point2
{
    int x;
    int y;
    int z;
    unsigned short intensity;
    unsigned char return_number : 3;
    unsigned char number_of_returns : 3;
    unsigned char scan_direction_flag : 1;
    unsigned char edge_of_flight_line : 1;
    unsigned char classification;
    unsigned char scan_angle_rank;
    unsigned char user_data;
    unsigned short point_source_id;
    unsigned short red;
    unsigned short green;
    unsigned short blue;
};

struct Point3
{
    int x;
    int y;
    int z;
    unsigned short intensity;
    unsigned char return_number : 3;
    unsigned char number_of_returns : 3;
    unsigned char scan_direction_flag : 1;
    unsigned char edge_of_flight_line : 1;
    unsigned char classification;
    unsigned char scan_angle_rank;
    unsigned char user_data;
    unsigned short point_source_id;
    double gps_time;
    unsigned short red;
    unsigned short green;
    unsigned short blue;
};

struct VariableLengthRecord
{
    unsigned short reserved;
    char user_id[16];
    unsigned short record_id;
    unsigned short record_length_after_header;
    char description[32];
    unsigned char *data; // Variable length based on record_length_after_header
};

struct LASFile
{
    LASHeader header;
    VariableLengthRecord *variable_length_records;
    char *point_data;
};

void load_las_file(const char *filename, std::vector<char> &data);
void parse_las_file(const char *data, size_t size, LASFile *las_file);
void parse_las_header(const char *data, LASHeader *header);
void parse_point0(const char *data, Point0 *point);
void parse_point1(const char *data, Point1 *point);
void parse_point2(const char *data, Point2 *point);
void parse_point3(const char *data, Point3 *point);
void parse_variable_length_record(const char *data, VariableLengthRecord *record);
void free_las_file(LASFile *las_file);
void print_las_header(const LASHeader *header);
void print_point0(const Point0 *point);
void print_point1(const Point1 *point);
void print_point2(const Point2 *point);
void print_point3(const Point3 *point);
void print_variable_length_record(const VariableLengthRecord *record);
void print_las_file(const LASFile *las_file);
