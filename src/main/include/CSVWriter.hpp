#ifndef _CPP_UTILS_CSV_WRITER_HEADER__
#define _CPP_UTILS_CSV_WRITER_HEADER__

#include <vector>
#include <stdio.h>
#include <iostream>
#include "vectorplus.hpp"

namespace cpp_utils {

    template <typename... TYPES>
    class CSVWriter {
        typedef CSVWriter<TYPES...> CSVWriterInstance;
    /**
     * @brief a structure allowing you to print rows in a CSV.
     * 
     * In order to use this class, you need to exactly know the columns and the types of the csv
     * 
     * @code
     * CwdWriter<int, bool> writer{boost::filesystem::path{"./hello.csv"}, ',' vectorplus<std::string>{"intField", "boolField"}};
     * writer.writeRow(5, true);
     * writer.writeRow(5, false);
     * //whe it goes out of scope the file is closed
     * @endcode
     * 
     */
    public:
        /**
         * @brief Create a new csv handler
         * 
         * @note
         * the filename, if exists, will be truncated
         * 
         * @param filename 
         * @param separator 
         * @param header 
         */
        CSVWriter(const boost::filesystem::path& filename, char separator, const vectorplus<std::string>& header) : filename{filename}, separator{separator}, header{header} {
            this->f.open(filename.string(), std::fstream::trunc);
            this->f << "sep=" << separator << std::endl;
            for (int i=0; i<header.size(); ++i) {
                this->f << header[i];
                if ((i+1) != header.size()) {
                    this->f << this->separator;
                }
            }
            this->f << std::endl;
        }
        virtual ~CSVWriter() {
            this->f.close();
        }
        CSVWriter(const CSVWriterInstance& other) = delete;
        CSVWriter(CSVWriterInstance&& other) = delete;
        CSVWriterInstance& operator =(const CSVWriterInstance& other) = delete;
        CSVWriterInstance& operator =(CSVWriterInstance&& other) = delete;
    public:
        /**
         * @brief print a row within the CSV
         * 
         * @code
         * CSVWriter<int,bool,int> csv{"hello.csv", {"a", "b", "c"}, ','};
         * csv.writeRow(4, true, 5);
         * @endcode
         * 
         * @param a the first value you need to add
         * @param other all the other values you need to add
         */
        void writeRow(const TYPES&... other) {
            this->_write(this->header.size(), other...);
        }
        
    protected:
        void _write(int left) {
            this->f << std::endl;
        }
        template <typename FIRST, typename ... OTHER>
        void _write(int left, const FIRST& a, const OTHER&... other) {
            this->f << a;
            if (left > 1) {
                this->f << this->separator;
            }
            this->_write(left - 1, other...);
        }
    private:
        const boost::filesystem::path& filename;
        std::ofstream f;
        char separator;
        vectorplus<std::string> header;
    };

}

#endif