#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <utility>
#include <ios>
#include <iomanip>
#include <sys/ioctl.h>
#include <unistd.h>
// For Original Header
#include "progress.hpp"


// ------------------------------------------------------
// namespace{progress} -> class{display} -> constructor
// ------------------------------------------------------
progress::display::display(const size_t count_max_, const std::pair<size_t, size_t> epoch, const std::vector<std::string> loss_){

    this->count = 0;
    this->count_max = count_max_;
    this->length = 0;
    this->loss = loss_;

    std::stringstream ss;
    ss << "epoch:" << epoch.first << "/" << epoch.second << " ";
    std::cout << ss.str() << std::flush;
    this->header = ss.str().length();

    this->loss_sum = std::vector<float>(this->loss.size(), 0.0);
    this->loss_ave = std::vector<float>(this->loss.size(), 0.0);
    this->start = std::chrono::system_clock::now();

}

progress::display::display(const size_t count_max_, const std::string header1, const std::string header2, const std::vector<std::string> loss_){

    this->count = 0;
    this->count_max = count_max_;
    this->length = 0;
    this->loss = loss_;

    std::stringstream ss;
    ss << header1 << " " << header2 << " ";
    std::cout << ss.str() << std::flush;
    this->header = ss.str().length();

    this->loss_sum = std::vector<float>(this->loss.size(), 0.0);
    this->loss_ave = std::vector<float>(this->loss.size(), 0.0);
    this->start = std::chrono::system_clock::now();

}


// -------------------------------------------------------------
// namespace{progress} -> class{display} -> function{increment}
// -------------------------------------------------------------
void progress::display::increment(const std::vector<float> loss_value){

    // (0) Initialization and Declaration
    size_t i;
    size_t center_length, bar_length;
    size_t ideal_length;
    int percent;
    int elap_min, elap_sec, rem_times, rem_min, rem_sec;
    double sec_per_iter;
    std::string initialize;
    std::string left_str, right_str, center_str;
    std::string elap_min_str, elap_sec_str, sec_per_iter_str, rem_min_str, rem_sec_str;
    std::stringstream ss;

    // (1) Count Up Epochs
    this->count++;

    // (2) Initialization of Terminal Line
    initialize = std::string(this->length, '\b') + std::string(this->length, ' ') + std::string(this->length, '\b');

    // (3) Get Left String
    left_str = "";
    for (i = 0; i < this->loss.size(); i++){
        this->loss_sum.at(i) += loss_value.at(i);
        this->loss_ave.at(i) = this->loss_sum.at(i) / (float)this->count;
        ss.str(""); ss.clear(std::stringstream::goodbit);
        ss << this->loss.at(i) << ":" << loss_value.at(i);
        ss << "(ave:" << this->loss_ave.at(i) << ") ";
        left_str += ss.str();
    }
    ss.str(""); ss.clear(std::stringstream::goodbit);
    percent = (int)((float)this->count / (float)this->count_max * 100.0f);
    ss << std::right << std::setw(4) << percent;
    left_str += ss.str() + "%[";

    // (4) Get Times for Right String
    this->end = std::chrono::system_clock::now();
    for (i = 0; i < 6; i++){
        ss.str(""); ss.clear(std::stringstream::goodbit);
        switch (i){
            case 0:
                elap_min = (int)std::chrono::duration_cast<std::chrono::minutes>(this->end - this->start).count();
                ss << std::setfill('0') << std::right << std::setw(2) << elap_min;
                elap_min_str = ss.str();
                break;
            case 1:
                elap_sec = (int)std::chrono::duration_cast<std::chrono::seconds>(this->end - this->start).count() % 60;
                ss << std::setfill('0') << std::right << std::setw(2) << elap_sec;
                elap_sec_str = ss.str();
                break;
            case 2:
                sec_per_iter = (double)std::chrono::duration_cast<std::chrono::milliseconds>(this->end - this->start).count() * 0.001 / (double)this->count;
                ss << std::setprecision(3) << sec_per_iter;
                sec_per_iter_str = ss.str();
                break;
            case 3:
                rem_times = (int)(sec_per_iter * (double)(this->count_max - this->count));
                break;
            case 4:
                rem_min = rem_times / 60;
                ss << std::setfill('0') << std::right << std::setw(2) << rem_min;
                rem_min_str = ss.str();
                break;
            case 5:
                rem_sec = rem_times % 60;
                ss << std::setfill('0') << std::right << std::setw(2) << rem_sec;
                rem_sec_str = ss.str();
                break;
            default:
                std::cerr << "Error : There is an unexpected value in argument of 'switch'." << std::endl;
                std::exit(1);
        }
    }

    // (5) Get Right String
    ss.str(""); ss.clear(std::stringstream::goodbit);
    ss << "] " << this->count << "/" << this->count_max << " ";
    ss << "[" << elap_min_str << ":" << elap_sec_str << "<" << rem_min_str << ":" << rem_sec_str << ", " << sec_per_iter_str << "s/it]";
    right_str = ss.str();

    // (6) Catch Terminal Size
    struct winsize ws;
    ideal_length = 0;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1){
       ideal_length = ws.ws_col - this->header - 1;
    }
    else{
        std::cerr << "Error : Couldn't get the width of terminal" << std::endl;
        std::exit(1);
    }

    // (7) Get Center String
    center_length = (size_t)std::max((int)1, (int)(ideal_length - left_str.length() - right_str.length()));
    bar_length = (size_t)((float)center_length * (float)this->count / (float)this->count_max);
    center_str = std::string(bar_length, '#') + std::string(center_length - bar_length, ' ');

    // (8) Output All String
    this->length = left_str.length() + center_str.length() + right_str.length();
    std::cout << initialize << std::flush;
    std::cout << left_str << center_str << right_str << std::flush;

    // End Processing
    return;
    
}


// -------------------------------------------------------------
// namespace{progress} -> class{display} -> function{get_iters}
// -------------------------------------------------------------
size_t progress::display::get_iters(){
    return this->count;
}


// -----------------------------------------------------------
// namespace{progress} -> class{display} -> function{get_ave}
// -----------------------------------------------------------
std::vector<float> progress::display::get_ave(){
    return this->loss_ave;
}

float progress::display::get_ave(const int index){
    return this->loss_ave.at(index);
}


// ----------------------------------------------------
// namespace{progress} -> class{display} -> destructor
// ----------------------------------------------------
progress::display::~display(){
    std::cout << std::endl;
}
