//
// Created by josqu on 24/06/2021.
//

#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include <bgpstream.h>
#include <vector>
//#include <boost/range/combine.hpp>
#include <boost/algorithm/string.hpp>

#include "funcFromApp.h"

using namespace std;

unsigned int from_myencoding(string str);
void from_myencodingPath(string str, vector<unsigned int> &vect);
void from_myencodingPref(string str, bgpstream_pfx_t *inpfx );

void PrefixPath::fromRedis(string str){
    vector<string> results;
    vector<unsigned int> pathVect;
    results.clear();
    boost::split(results,str, [](char c){return c == ':';});
    int i=0;
    hash = from_myencoding(results[i++]);
    from_myencodingPath(results[i++], pathVect);
    shortPathLength = pathVect.size();
    shortPath = new unsigned int[shortPathLength];
    memcpy(shortPath,&pathVect[0],sizeof(unsigned int)*shortPathLength);
    pathLength = from_myencoding(results[i++]);
    prefNum = from_myencoding(results[i++]);
    lastChange  = from_myencoding(results[i++]);
    meanUp= from_myencoding(results[i++])/10000;
    meanDown = from_myencoding(results[i++])/10000;
    collector= from_myencoding(results[i++]);
    if (results[i++] == "T")
        active = true;
    else
        active = false;
}

unsigned int from_myencoding(string str){
    unsigned int val=0,coef=1,l;
    for(unsigned long i=0;i<str.length();i++){
        if (str[i]<58)
            l=str[i]-33;
        else
            l=str[i]-33-1;
        val = val+(unsigned char)l*coef;
        coef = 92*coef;
    }
    return val;
}

void from_myencodingPath(string str, vector<unsigned int> &vect){
    string str1;
    for(int i=0;i<str.length();i=i+4){
        str1=str.substr(i,4);
        vect.push_back(from_myencoding(str1));
    }
}

void from_myencodingPref(string str, bgpstream_pfx_t *inpfx ){
    unsigned long val=0,coef=1,l, ipval;
    unsigned int len;
    string pfxstr="";
    for(unsigned long i=0;i<str.length();i++){
        if (str[i]<58)
            l=str[i]-33;
        else
            l=str[i]-33-1;
        val += (unsigned char)l*coef;
        coef = 92*coef;
    }
    len= (uint8_t)((val & 0X000000FF00000000)>>32);
    ipval = val & 0x00000000FFFFFFFF;
    for (int i=0;i<4;i++){
        pfxstr += to_string(ipval % 256);
        if (i<3)
            pfxstr +=".";
        ipval = ipval/256;
    }
    pfxstr+="/"+to_string(len);
    //bgpstream_str2pfx(pfxstr.c_str(),inpfx);
}