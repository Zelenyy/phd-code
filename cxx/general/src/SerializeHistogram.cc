//
// Created by zelenyy on 05.03.2020.
//

#include "SerializeHistogram.hh"

void fillPbBins(Bins *bins, histogram::Bins *bins_pb) {
    for (double it :bins->getBins()){
        bins_pb->add_bins(it);
    }
}

void fillPbHistogram2D(Histogramm2D *hist, histogram::Histogram2D *hist_pb) {
    auto xbins = new histogram::Bins();
    fillPbBins(hist->fXbins, xbins);
    auto ybins = new histogram::Bins();
    fillPbBins(hist->fYbins, ybins);
    hist_pb->set_allocated_xbins(xbins);
    hist_pb->set_allocated_ybins(ybins);
    for (int it: hist->getData()){
        hist_pb->add_data(it);
    }
}

void fillPbHistogramm4D(Histogramm4D* hist, histogram::Histogram4D *hist_pb){
    for (int it: hist->getData()){
        hist_pb->add_data(it);
    }
    auto xbins = new histogram::Bins();
    auto ybins = new histogram::Bins();
    auto zbins = new histogram::Bins();
    auto tbins = new histogram::Bins();
    fillPbBins(hist->fXbins, xbins);
    fillPbBins(hist->fYbins, ybins);
    fillPbBins(hist->fZbins, zbins);
    fillPbBins(hist->fTbins, tbins);
    hist_pb->set_allocated_xbins(xbins);
    hist_pb->set_allocated_ybins(ybins);
    hist_pb->set_allocated_zbins(zbins);
    hist_pb->set_allocated_tbins(tbins);


}


