#include "las.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <intrin.h>
#include <iomanip>

void load_las_file(const char *filename, std::vector<char> &data)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    file.seekg(0, std::ios::end);
    std::streampos length = file.tellg();
    file.seekg(0, std::ios::beg);
    data.resize(length);
    file.read(data.data(), length);
    file.close();
}

void parse_las_file(const char *data, size_t size, LASFile *las_file)
{
    parse_las_header(data, &las_file->header);
    size_t offset = las_file->header.header_size;
    las_file->variable_length_records = nullptr;
    if (las_file->header.number_of_variable_length_records > 0)
    {
        las_file->variable_length_records = new VariableLengthRecord[las_file->header.number_of_variable_length_records];
        for (int i = 0; i < las_file->header.number_of_variable_length_records; ++i)
        {
            parse_variable_length_record(data + offset, &las_file->variable_length_records[i]);
            offset += las_file->variable_length_records[i].record_length_after_header;
            offset += 0x36; // Variable length record header size
        }
    }
    size_t point_size = las_file->header.number_of_point_records * las_file->header.point_data_record_length;
    las_file->point_data = new char[point_size];
    std::memcpy(las_file->point_data, data + las_file->header.offset_to_point_data, point_size);
}

void parse_las_header(const char *data, LASHeader *header)
{
    std::memcpy(header->file_signature, data, 0x04);
    std::memcpy(&header->file_source_id, data + 0x04, sizeof(unsigned short));
    std::memcpy(&header->global_encoding, data + 0x06, sizeof(unsigned short));
    std::memcpy(&header->project_guid, data + 0x08, 0x10);
    std::memcpy(&header->version_major, data + 0x18, sizeof(unsigned char));
    std::memcpy(&header->version_minor, data + 0x19, sizeof(unsigned char));
    std::memcpy(header->system_id, data + 0x1A, 0x20);
    std::memcpy(header->generating_software, data + 0x3A, 0x20);
    std::memcpy(&header->file_creation_day, data + 0x5A, sizeof(unsigned short));
    std::memcpy(&header->file_creation_year, data + 0x5C, sizeof(unsigned short));
    std::memcpy(&header->header_size, data + 0x5E, sizeof(unsigned short));
    std::memcpy(&header->offset_to_point_data, data + 0x60, sizeof(unsigned int));
    std::memcpy(&header->number_of_variable_length_records, data + 0x64, sizeof(unsigned int));
    std::memcpy(&header->point_data_format, data + 0x68, sizeof(unsigned char));
    std::memcpy(&header->point_data_record_length, data + 0x69, sizeof(unsigned short));
    std::memcpy(&header->number_of_point_records, data + 0x6B, sizeof(unsigned int));
    std::memcpy(&header->number_of_points_by_return, data + 0x6F, 5 * sizeof(unsigned int));
    std::memcpy(&header->x_scale_factor, data + 0x83, sizeof(double));
    std::memcpy(&header->y_scale_factor, data + 0x8B, sizeof(double));
    std::memcpy(&header->z_scale_factor, data + 0x93, sizeof(double));
    std::memcpy(&header->x_offset, data + 0x9B, sizeof(double));
    std::memcpy(&header->y_offset, data + 0xA3, sizeof(double));
    std::memcpy(&header->z_offset, data + 0xAB, sizeof(double));
    std::memcpy(&header->max_x, data + 0xB3, sizeof(double));
    std::memcpy(&header->min_x, data + 0xBB, sizeof(double));
    std::memcpy(&header->max_y, data + 0xC3, sizeof(double));
    std::memcpy(&header->min_y, data + 0xCB, sizeof(double));
    std::memcpy(&header->max_z, data + 0xD3, sizeof(double));
    std::memcpy(&header->min_z, data + 0xDB, sizeof(double));
}

void parse_point0(const char *data, Point0 *point)
{
    std::memcpy(point, data, sizeof(Point0));
}

void parse_point1(const char *data, Point1 *point)
{
    std::memcpy(point, data, sizeof(Point1));
}

void parse_point2(const char *data, Point2 *point)
{
    std::memcpy(point, data, sizeof(Point2));
}

void parse_point3(const char *data, Point3 *point)
{
    std::memcpy(point, data, sizeof(Point3));
}

void parse_variable_length_record(const char *data, VariableLengthRecord *record)
{
    std::memcpy(record, data, sizeof(VariableLengthRecord));
    record->data = new unsigned char[record->record_length_after_header];
    std::memcpy(record->data, data + 0x36, record->record_length_after_header);
}

void free_las_file(LASFile *las_file)
{
    if (las_file->variable_length_records != nullptr)
    {
        for (int i = 0; i < las_file->header.number_of_variable_length_records; ++i)
        {
            delete[] las_file->variable_length_records[i].data;
        }
        delete[] las_file->variable_length_records;
    }
    delete[] las_file->point_data;
    delete las_file;
}

void cout_hex()
{
    std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0');
}

void cout_dec()
{
    std::cout << std::dec << std::nouppercase << std::setw(0) << std::setfill(' ');
}

void print_las_header(const LASHeader *header)
{
    std::cout << "File signature: " << header->file_signature << std::endl;
    std::cout << "File source ID: " << header->file_source_id << std::endl;
    std::cout << "Global encoding: " << header->global_encoding << std::endl;
    std::cout << "Project ID GUID:";
    cout_hex();
    for (int i = 0; i < 16; ++i)
    {
        std::cout << static_cast<unsigned int>(header->project_guid[i]);
        if (i == 3 || i == 5 || i == 7 || i == 9)
        {
            std::cout << "-";
        }
    }
    std::cout << std::endl;
    cout_dec();
    std::cout << "Version major: " << static_cast<unsigned int>(header->version_major) << std::endl;
    std::cout << "Version minor: " << static_cast<unsigned int>(header->version_minor) << std::endl;
    std::cout << "System identifier: " << std::string(header->system_id) << std::endl;
    std::cout << "Generating software: " << std::string(header->generating_software) << std::endl;
    std::cout << "File creation day of year: " << header->file_creation_day << std::endl;
    std::cout << "File creation year: " << header->file_creation_year << std::endl;
    std::cout << "Header size: " << header->header_size << std::endl;
    std::cout << "Offset to point data: " << header->offset_to_point_data << std::endl;
    std::cout << "Number of variable length records: " << header->number_of_variable_length_records << std::endl;
    std::cout << "Point data format ID: " << static_cast<unsigned int>(header->point_data_format) << std::endl;
    std::cout << "Point data record length: " << header->point_data_record_length << std::endl;
    std::cout << "Number of point records: " << header->number_of_point_records << std::endl;
    std::cout << "Number of points by return: ";
    for (int i = 0; i < 5; ++i)
    {
        std::cout << header->number_of_points_by_return[i] << " ";
    }
    std::cout << std::endl;
}

void print_point0(const Point0 *point)
{
    std::cout << "X: " << point->x << std::endl;
    std::cout << "Y: " << point->y << std::endl;
    std::cout << "Z: " << point->z << std::endl;
    std::cout << "Intensity: " << point->intensity << std::endl;
    std::cout << "Return number: " << point->return_number << std::endl;
    std::cout << "Number of returns: " << point->number_of_returns << std::endl;
    std::cout << "Scan direction flag: " << point->scan_direction_flag << std::endl;
    std::cout << "Edge of flight line: " << point->edge_of_flight_line << std::endl;
    std::cout << "Classification: " << point->classification << std::endl;
    std::cout << "Scan angle rank: " << point->scan_angle_rank << std::endl;
    std::cout << "User data: " << point->user_data << std::endl;
    std::cout << "Point source ID: " << point->point_source_id << std::endl;
}

void print_point1(const Point1 *point)
{
    std::cout << "X: " << point->x << std::endl;
    std::cout << "Y: " << point->y << std::endl;
    std::cout << "Z: " << point->z << std::endl;
    std::cout << "Intensity: " << point->intensity << std::endl;
    std::cout << "Return number: " << point->return_number << std::endl;
    std::cout << "Number of returns: " << point->number_of_returns << std::endl;
    std::cout << "Scan direction flag: " << point->scan_direction_flag << std::endl;
    std::cout << "Edge of flight line: " << point->edge_of_flight_line << std::endl;
    std::cout << "Classification: " << point->classification << std::endl;
    std::cout << "Scan angle rank: " << point->scan_angle_rank << std::endl;
    std::cout << "User data: " << point->user_data << std::endl;
    std::cout << "Point source ID: " << point->point_source_id << std::endl;
    std::cout << "GPS time: " << point->gps_time << std::endl;
}

void print_point2(const Point2 *point)
{
    std::cout << "X: " << point->x << std::endl;
    std::cout << "Y: " << point->y << std::endl;
    std::cout << "Z: " << point->z << std::endl;
    std::cout << "Intensity: " << point->intensity << std::endl;
    std::cout << "Return number: " << point->return_number << std::endl;
    std::cout << "Number of returns: " << point->number_of_returns << std::endl;
    std::cout << "Scan direction flag: " << point->scan_direction_flag << std::endl;
    std::cout << "Edge of flight line: " << point->edge_of_flight_line << std::endl;
    std::cout << "Classification: " << point->classification << std::endl;
    std::cout << "Scan angle rank: " << point->scan_angle_rank << std::endl;
    std::cout << "User data: " << point->user_data << std::endl;
    std::cout << "Point source ID: " << point->point_source_id << std::endl;
    std::cout << "Red: " << point->red << std::endl;
    std::cout << "Green: " << point->green << std::endl;
    std::cout << "Blue: " << point->blue << std::endl;
}

void print_point3(const Point3 *point)
{
    std::cout << "X: " << point->x << std::endl;
    std::cout << "Y: " << point->y << std::endl;
    std::cout << "Z: " << point->z << std::endl;
    std::cout << "Intensity: " << point->intensity << std::endl;
    std::cout << "Return number: " << point->return_number << std::endl;
    std::cout << "Number of returns: " << point->number_of_returns << std::endl;
    std::cout << "Scan direction flag: " << point->scan_direction_flag << std::endl;
    std::cout << "Edge of flight line: " << point->edge_of_flight_line << std::endl;
    std::cout << "Classification: " << point->classification << std::endl;
    std::cout << "Scan angle rank: " << point->scan_angle_rank << std::endl;
    std::cout << "User data: " << point->user_data << std::endl;
    std::cout << "Point source ID: " << point->point_source_id << std::endl;
    std::cout << "GPS time: " << point->gps_time << std::endl;
    std::cout << "Red: " << point->red << std::endl;
    std::cout << "Green: " << point->green << std::endl;
    std::cout << "Blue: " << point->blue << std::endl;
}

void print_variable_length_record(const VariableLengthRecord *record)
{
    std::cout << "Reserved: " << record->reserved << std::endl;
    std::cout << "User ID: " << std::string(record->user_id) << std::endl;
    std::cout << "Record ID: " << record->record_id << std::endl;
    std::cout << "Record length after header: " << record->record_length_after_header << std::endl;
    std::cout << "Description: " << std::string(record->description) << std::endl;
    std::cout << "Data: ";
    cout_hex();
    for (int i = 0; i < record->record_length_after_header; ++i)
    {
        std::cout << static_cast<unsigned int>(record->data[i]) << " ";
    }
    cout_dec();
    std::cout << std::endl;
}

void print_las_file(const LASFile *las_file)
{
    print_las_header(&las_file->header);
    if (las_file->variable_length_records != nullptr)
    {
        for (int i = 0; i < las_file->header.number_of_variable_length_records; ++i)
        {
            print_variable_length_record(&las_file->variable_length_records[i]);
        }
    }
    size_t offset = 0;
    size_t point_data_size = las_file->header.number_of_point_records * las_file->header.point_data_record_length;
    while (offset < point_data_size)
    {
        switch (las_file->header.point_data_format)
        {
        case 0:
        {
            Point0 point;
            parse_point0(las_file->point_data + offset, &point);
            // print_point0(&point);
            offset += sizeof(Point0);
            break;
        }
        case 1:
        {
            Point1 point;
            parse_point1(las_file->point_data + offset, &point);
            // print_point1(&point);
            offset += sizeof(Point1);
            break;
        }
        case 2:
        {
            Point2 point;
            parse_point2(las_file->point_data + offset, &point);
            // print_point2(&point);
            offset += sizeof(Point2);
            break;
        }
        case 3:
        {
            Point3 point;
            parse_point3(las_file->point_data + offset, &point);
            // print_point3(&point);
            offset += sizeof(Point3);
            break;
        }
        default:
            std::cerr << "Unknown point data format: " << las_file->header.point_data_format << std::endl;
            return;
        }
    }
}