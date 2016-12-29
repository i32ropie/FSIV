#ifndef __STATS_HPP__
#define __STATS_HPP__

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>

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
            double _acc_sq;
            double _med;
            double _var;
            double _skew_coef;
        public:
            Stats():_width(0), _height(0), _min(255), _max(0), _neg(0), _pos(0), _zeros(0), _area_neg(0), _area_pos(0), _area_total(0), _acc_sq(0), _med(0), _var(0), _skew_coef(0){}
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
            inline double get_acc_sq() const { return _acc_sq; }
            inline double get_med() const { return _med; }
            inline double get_var() const { return _var; }
            inline double get_skew_coef() const { return _skew_coef; }
            inline void set_width(const uint &width) { _width = width; }
            inline void set_height(const uint &height) { _height = height; }
            inline void set_min(const uint &min) { _min = min; }
            inline void set_max(const uint &max) { _max = max; }
            inline void set_neg(const uint &neg) { _neg = neg; }
            inline void set_pos(const uint &pos) { _pos = pos; }
            inline void set_zeros(const uint &zeros) { _zeros = zeros; }
            inline void set_area_neg(const double &area_neg) { _area_neg = area_neg; }
            inline void set_area_pos(const double &area_pos) { _area_pos = area_pos; }
            inline void set_area_total(const double &area_total) { _area_total = area_total; }
            inline void set_acc_sq(const double &acc_sq) { _acc_sq = acc_sq; }
            inline void set_med(const double &med) { _med = med; }
            inline void set_var(const double &var) { _var = var; }
            inline void set_skew_coef(const double &skew_coef) { _skew_coef = skew_coef; }
            void calculate_stats(const cv::Mat &channel, const bool &mask_flag, const cv::Mat &mask){
                double counter = 0;
                this->set_width(channel.cols);
                this->set_height(channel.rows);
                for( int i = 0 ; i < channel.rows ; ++i ){
                    for( int j = 0 ; j < channel.cols ; ++j ){
                        if( !mask_flag || (mask.at<uchar>(i,j) == 255) ){ // If mask, only we only want white pixels
                            counter++;
                            this->set_area_total(this->get_area_total() + channel.at<uchar>(i,j));
                            this->set_acc_sq(this->get_acc_sq() + pow(channel.at<uchar>(i,j), 2));
                            if(channel.at<uchar>(i,j) < this->get_min())
                                this->set_min(channel.at<uchar>(i,j));
                            if(channel.at<uchar>(i,j) > this->get_max())
                                this->set_max(channel.at<uchar>(i,j));
                            if(channel.at<uchar>(i,j) == 0)
                                this->set_zeros(this->get_zeros() + 1);
                            if(channel.at<uchar>(i,j) > 0){
                                this->set_pos(this->get_pos() + 1);
                                this->set_area_pos(this->get_area_pos() + channel.at<uchar>(i,j));
                            }
                            if(channel.at<uchar>(i,j) < 0){
                                this->set_neg(this->get_neg() + 1);
                                this->set_area_neg(this->get_area_neg() + channel.at<uchar>(i,j));
                            }

                        }
                    }
                }
                // If mask, we take care about the studied pixels only
                mask_flag? this->set_med(this->get_area_total() / counter) : this->set_med(this->get_area_total() / (this->get_height() * this->get_width()) );
                for( int i = 0 ; i < channel.rows ; ++i ){
                    for( int j = 0 ; j < channel.cols ; ++j ){
                        if( !mask_flag || (mask.at<uchar>(i,j) == 255) ){ // If mask, only we only want white pixels
                            this->set_var(this->get_var() + pow(channel.at<uchar>(i,j) - this->get_med(), 2));
                            this->set_skew_coef(this->get_skew_coef() + pow(channel.at<uchar>(i,j) - this->get_med(), 3));
                        }
                    }
                }
                // If mask, we take care about the studied pixels only
                mask_flag? this->set_var(this->get_var() / counter) : this->set_var(this->get_var() / (this->get_height() * this->get_width()));
                mask_flag? this->set_skew_coef(this->get_skew_coef() / (counter * pow(sqrt(this->get_var()), 3))) : this->set_skew_coef(this->get_skew_coef() / ((this->get_height() * this->get_width()) * pow(sqrt(this->get_var()), 3)));
            }
            friend std::ostream &operator <<(std::ostream &output, const Stats &s){
                output << "\e[1;32m\tWidth\e[m        \e[1;96m" << s.get_width() << "\e[m" << std::endl;
                output << "\e[1;32m\tHeight\e[m       \e[1;96m" << s.get_height() << "\e[m" << std::endl;
                output << "\e[1;32m\tMin. value\e[m   \e[1;96m" << s.get_min() << "\e[m" << std::endl;
                output << "\e[1;32m\tMax. value\e[m   \e[1;96m" << s.get_max() << "\e[m" << std::endl;
                output << "\e[1;32m\tNeg. values\e[m  \e[1;96m" << s.get_neg() << "\e[m" << std::endl;
                output << "\e[1;32m\tPos. values\e[m  \e[1;96m" << s.get_pos() << "\e[m" << std::endl;
                output << "\e[1;32m\tZero values\e[m  \e[1;96m" << s.get_zeros() << "\e[m" << std::endl;
                output << "\e[1;32m\tNeg. area\e[m    \e[1;96m" << s.get_area_neg() << "\e[m" << std::endl;
                output << "\e[1;32m\tPos. area\e[m    \e[1;96m" << s.get_area_pos() << "\e[m" << std::endl;
                output << "\e[1;32m\tTotal area\e[m   \e[1;96m" << s.get_area_total() << "\e[m" << std::endl;
                output << "\e[1;32m\tAcc. sqr\e[m     \e[1;96m" << s.get_acc_sq() << "\e[m" << std::endl;
                output << "\e[1;32m\tMean\e[m         \e[1;96m" << s.get_med() << "\e[m" << std::endl;
                output << "\e[1;32m\tVariance\e[m     \e[1;96m" << s.get_var() << "\e[m" << std::endl;
                output << "\e[1;32m\tSkewness\e[m     \e[1;96m" << s.get_skew_coef() << "\e[m" << std::endl;
                return output;
            }
    };
} /* fsiv */

#endif
