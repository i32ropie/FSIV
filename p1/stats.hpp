#ifndef __STATS_HPP__
#define __STATS_HPP__

#include <iostream>

namespace fsiv {
    class Stats{
        private:
            uint _width;
            uint _height;
            uint _min;
            uint _max;
            uint _neg;
            uint _pos;
            uint _zeros;
            double _area_neg;
            double _area_pos;
            double _area_total;
            double _sum_sq;
            double _med;
            double _var;
            double _asim_coef;
        public:
            inline uint get_width() const { return _width; }
            inline uint get_height() const { return _height; }
            inline uint get_min() const { return _min; }
            inline uint get_max() const { return _max; }
            inline uint get_neg() const { return _neg; }
            inline uint get_pos() const { return _pos; }
            inline uint get_zeros() const { return _zeros; }
            inline double get_area_neg() const { return _area_neg; }
            inline double get_area_pos() const { return _area_pos; }
            inline double get_area_total() const { return _area_total; }
            inline double get_sum_sq() const { return _sum_sq; }
            inline double get_med() const { return _med; }
            inline double get_var() const { return _var; }
            inline double get_asim_coef() const { return _asim_coef; }
            inline void set_width(const uint &width) { _width = width; }
            inline void set_height(const uint &height) { _height = height; }
            inline void set_min(const uint &min) { _min = min; }
            inline void set_max(const uint &max) { _max = max; }
            inline void set_neg(const uint &neg) { _neg = neg; }
            inline void set_pos(const uint &pos) { _pos = pos; }
            inline void set_zeros(const uint &zeros) { _zeros = zeros; }
            inline void get_area_neg(const double &area_neg) { _area_neg = area_neg; }
            inline void get_area_pos(const double &area_pos) { _area_pos = area_pos; }
            inline void get_area_total(const double &area_total) { _area_total = area_total; }
            inline void get_sum_sq(const double &sum_sq) { _sum_sq = sum_sq; }
            inline void get_med(const double &med) { _med = med; }
            inline void get_var(const double &var) { _var = var; }
            inline void get_asim_coef(const double &asim_coef) { _asim_coef = asim_coef; }
    };
} /* fsiv */

#endif
