//
// Created by zelenyy on 18.02.20.
//

#include "Histogramm.hh"
#include <iostream>
int main(){

    auto bins = new UniformBins(0,1,5);
    auto hist = Histogramm2D(bins, bins);
    hist.add(0.5,0.5);
    std::cout<<bins->toString()<<endl;
    std::cout<<hist.dataToString()<<endl;

    for (int i = 0; i <100; ++i){
        hist.add(i/100.0, i/100.0);
    }
    std::cout<<hist.dataToString()<<endl;

    return 0;
}