// ---------------------------------------------------------------------
// Copyright (C) 2015 Chris Garry
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
// ---------------------------------------------------------------------


#ifndef PIPP_SER_H
#define PIPP_SER_H

#include <QCoreApplication>
#include <QString>
#include <stdint.h>
#include "pipp_buffer.h"


// Codes for ColourID
#define COLOURID_MONO          0
#define COLOURID_BAYER_RGGB    8
#define COLOURID_BAYER_GRBG    9
#define COLOURID_BAYER_GBRG    10
#define COLOURID_BAYER_BGGR    11
#define COLOURID_BAYER_CYYM    16
#define COLOURID_BAYER_YCMY    17
#define COLOURID_BAYER_YMCY    18
#define COLOURID_BAYER_MYYC    19
#define COLOURID_RGB           100
#define COLOURID_BGR           101


class c_pipp_ser {
    Q_DECLARE_TR_FUNCTIONS(c_pipp_ser)

    // ------------------------------------------
    // Private definitions
    // ------------------------------------------
    private:
        struct s_ser_header {
            int32_t lu_id; // Lumenera camera series ID
            int32_t colour_id;  // Colour ID
            int32_t little_endian;  // Little Endian
            int32_t image_width;  // Image Width
            int32_t image_height;  // Image Height
            int32_t pixel_depth;  // Pixel Depth
            int32_t frame_count;  // Frame Count
            char observer[40];  // Name of observer
            char instrument[40];  // Name of camera
            char telescope[40];  // Name of telescope
            uint32_t date_time[2];  // Date and time
            uint32_t date_time_utc[2];  // Date and time in UTC
        };


        // Member variables
        QString m_filename;
        FILE *mp_ser_file;
        int64_t m_filesize;
        uint32_t m_framesize;
        s_ser_header m_header;
        int32_t m_bytes_depth;
        int32_t m_colour;
        uint32_t m_current_frame;
        uint64_t m_timestamp;
        int32_t m_fps_rate;  // Frame Per Second Rate
        int32_t m_fps_scale;  // Frame Per Second Scale

        c_pipp_buffer m_temp_buffer;
        c_pipp_buffer m_timestamp_buffer;
        uint64_t *mp_timestamp;
        int64_t m_timestamp_correction_value;
        int64_t m_utc_to_local_offset;
        QString m_error_string;
        QString m_file_id;


    // ------------------------------------------
    // Public definitions
    // ------------------------------------------
    public:
    
        // ------------------------------------------
        // Constructor
        // ------------------------------------------
        c_pipp_ser() :
            mp_ser_file(nullptr),
            m_filesize(0),
            m_bytes_depth(0),
            m_colour(0),
            mp_timestamp(nullptr),
            m_error_string("") {}


        // ------------------------------------------
        // Destructor
        // ------------------------------------------
        ~c_pipp_ser() {
        }


        // ------------------------------------------
        // Open SER file
        // ------------------------------------------
        int32_t open(
            const QString &filename,
            int32_t bpp,
            int32_t quiet);

        
        // ------------------------------------------
        // Close AVI file
        // ------------------------------------------
        int32_t close();


        // ------------------------------------------
        // Get filename
        // ------------------------------------------
        QString get_filename(){
            return m_filename;
        }



        // ------------------------------------------
        // Get size of buffer required to store frame
        // ------------------------------------------
        int32_t get_buffer_size();


        // ------------------------------------------
        // Get width
        // ------------------------------------------
        int32_t get_width() {
            return m_header.image_width;
        }


        // ------------------------------------------
        // Get height
        // ------------------------------------------
        int32_t get_height() {
            return m_header.image_height;
        }


        // ------------------------------------------
        // Get fps_rate
        // ------------------------------------------
        int32_t get_fps_rate() {
            return m_fps_rate;
        }


        // ------------------------------------------
        // Get fps_scale
        // ------------------------------------------
        int32_t get_fps_scale() {
            return m_fps_scale;
        }


        // ------------------------------------------
        // Get colour_id
        // ------------------------------------------
        int32_t get_colour_id() {
            return m_header.colour_id;
        }


        // ------------------------------------------
        // Get file_id
        // ------------------------------------------
        QString get_file_id() {
            return m_file_id;
        }


        // ------------------------------------------
        // Get lu_id
        // ------------------------------------------
        int32_t get_lu_id() {
            return m_header.lu_id;
        }


        // ------------------------------------------
        // Get colour
        // ------------------------------------------
        int32_t get_colour() {
            return m_colour;
        }


        // ------------------------------------------
        // Get little_endian
        // ------------------------------------------
        int32_t get_little_endian() {
            return m_header.little_endian;
        }


        // ------------------------------------------
        // Get byte_depth
        // ------------------------------------------
        int32_t get_byte_depth() {
            return m_bytes_depth;
        }


        // ------------------------------------------
        // Get pixel_depth
        // ------------------------------------------
        int32_t get_pixel_depth() {
            return m_header.pixel_depth;
        }


        // ------------------------------------------
        // Get observer strings
        // ------------------------------------------
        QString get_observer_string();


        // ------------------------------------------
        // Get instrument strings
        // ------------------------------------------
        QString get_instrument_string();


        // ------------------------------------------
        // Get telescope strings
        // ------------------------------------------
        QString get_telescope_string();


        // ------------------------------------------
        // Get information about timestamps
        // ------------------------------------------
        QString get_timestamp_info();


        // ------------------------------------------
        // Get date_time
        // ------------------------------------------
        uint64_t get_data_time()
        {
            return (uint64_t)m_header.date_time[1] << 32 | m_header.date_time[0];
        }


        // ------------------------------------------
        // Get date_time_utc
        // ------------------------------------------
        uint64_t get_data_time_utc()
        {
            return (uint64_t)m_header.date_time_utc[1] << 32 | m_header.date_time_utc[0];
        }


        // ------------------------------------------
        // Get error string
        // ------------------------------------------
        QString& get_error_string();


        // ------------------------------------------
        // Get next frame from SER file
        // ------------------------------------------
        int32_t get_frame (
            uint8_t *buffer);


        // ------------------------------------------
        // Get particular frame from SER file
        // ------------------------------------------
        int32_t get_frame (
            uint32_t frame_number,
            uint8_t *buffer);

        //
        // Return is SER file has timestamps
        //
        bool has_timestamps() {
            return mp_timestamp != nullptr;
        }


        //
        // Get timestamp from SER file
        //
        uint64_t get_timestamp() {
            return m_timestamp + m_timestamp_correction_value;
        }


        //
        // Get diff between universal time and local time
        //
        int64_t get_utc_to_local_diff() {
            return m_utc_to_local_offset;
        }


    private:
        //
        // Find pixel depth from specified frame
        //
        int32_t find_pixel_depth(
            uint32_t frame_number);
};

    
#endif  // PIPP_SER_H
