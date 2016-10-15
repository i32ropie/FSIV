#ifndef __STATS_HPP__
#define __STATS_HPP__

#include <iostream>

namespace fsiv {
    class Stats{
        private:
            uint _width;
            uint _height;
            uint _min_value;
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
    };
} /* fsiv */

#endif
