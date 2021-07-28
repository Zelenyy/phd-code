//
// Created by zelenyy on 05.03.2020.
//

#ifndef PHD_CODE_SERIALIZEHISTOGRAM_HH
#define PHD_CODE_SERIALIZEHISTOGRAM_HH

#include "histogram.pb.h"
#include "Histogramm.hh"




void fillPbBins(Bins* bins, histogram::Bins* bins_pb);


void fillPbHistogram2D(Histogramm2D* hist, histogram::Histogram2D* hist_pb);



#endif //PHD_CODE_SERIALIZEHISTOGRAM_HH
