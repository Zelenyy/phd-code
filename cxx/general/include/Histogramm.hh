//
// Created by zelenyy on 18.02.20.
//

#ifndef PHD_CODE_HISTOGRAMM_HH
#define PHD_CODE_HISTOGRAMM_HH


#include <vector>
#include <string>
#include <math.h>
#include <iostream>

using namespace std;

class Bins{
public:
    virtual int countIndx(double item)= 0;
    virtual vector<double> getBins() =  0;
    virtual int size() = 0;
    //    vector<double> fBins;
//
//    Bins(vector<double> bins): fBins(std::move(bins)){
//
//    };
//

    string toString(string separator = " "){
        string result = "";
        auto bins = this->getBins();
//        cout<<bins.size()<<endl;
        for (double item: bins){
            result += to_string(item) + separator;
        }
        return result;
    }


};

class UniformBins : public Bins {
public:
    double fRigth;
    double fLeft;
    int fNumber;
    double fStep;

    int countIndx(double item) override {
        int indx = trunc((item - fLeft)/fStep);
        if (indx < 0 || indx > fNumber-1){
            indx = -1;
        }
        return indx;
    }

    int size() override{
        return fNumber;
    }

    vector<double> getBins() override {

        vector<double> bins;
        bins.reserve(fNumber+1);
        bins.push_back(fLeft);
        for (int i = 1; i<fNumber;++i){
            bins.push_back(bins[i-1] + fStep);
        }
        bins.push_back(fRigth);
//        cout<<bins.size()<<endl;
        return bins;
    }


    UniformBins(double left, double rigth, int number) : fLeft(left), fRigth(rigth), fNumber(number) {
        fStep = (rigth - left)/number;

    }


};


class IHistrogramm{
protected:
    vector<int> data;
public:
    string countToString(string separator=" "){
        string result = "";
        for (auto item: data){
            result += to_string(item) + separator;
        }
        return result;

    }

    vector<int> getData(){
        return data;
    }
};

class Histogramm2D : public IHistrogramm{
public:
    Bins* fXbins;
    Bins* fYbins;
    Histogramm2D(
            Bins* xbins,
            Bins* ybins
    ): fXbins(xbins), fYbins(ybins) {
        long size = xbins->size()*ybins->size();
        data.reserve(size);
        for (long i =0; i<size; i++){
            data.push_back(0);
        }
    }




    void add(double x, double y){
        int i = fXbins->countIndx(x);
        int j = fYbins->countIndx(y);
        if ((i<0) || (j<0)){
            return;
        }
        add(i, j);
    }
    pair<int,int> getIndex(double x, double y){
        int i = fXbins->countIndx(x);
        int j = fYbins->countIndx(y);
        pair<int,int> result = {i,j};
        return result;
    }

    int get(double x, double y){
        auto indx = getIndex(x,y);
        return get(indx.first, indx.second);
    }

    int get(int i, int j){
        return data[i*fYbins->size() + j];
    }

    string dataToString(string separator = " "){
        string result = "";
        int x = fXbins->size();
        int y = fYbins->size();
        for (int i=0; i<x;++i){
            for (int j=0; j<y; ++j){
                result += to_string(this->get(i,j)) + separator;
            }
            result += "\n";
        }
        return result;
    }

private:
    void add(int i, int j){
        long indx = i*fYbins->size() + j;
        data[indx]++;
    }

};

class Histogramm3D : IHistrogramm{
public:
    Bins* fXbins;
    Bins* fYbins;
    Bins* fZbins;
    Histogramm3D(
            Bins* xbins,
            Bins* ybins,
            Bins* zbins
    ): fXbins(xbins), fYbins(ybins), fZbins(zbins) {
        long size = xbins->size()*ybins->size()*zbins->size();
        data.reserve(size);
        for (long i =0; i<size; i++){
            data.push_back(0);
        }
    }

    void add(double x, double y, double z){
        int i = fXbins->countIndx(x);
        int j = fYbins->countIndx(y);
        int k = fZbins->countIndx(z);
        if ((i<0) || (j<0) || (k<0)){
            return;
        }
        add(i, j, k);
    }


private:
    void add(int i, int j, int k){
        long indx = i*fYbins->size()*fZbins->size() + j*fZbins->size() + k;
        data[indx]++;
    }

};


class Histogramm4D : public IHistrogramm{
public:
    Bins* fXbins;
    Bins* fYbins;
    Bins* fZbins;
    Bins* fTbins;
    Histogramm4D(
            Bins* xbins,
            Bins* ybins,
            Bins* zbins,
            Bins* tbins
    ): fXbins(xbins), fYbins(ybins), fZbins(zbins), fTbins(tbins) {
        long size = xbins->size()*ybins->size()*zbins->size()*tbins->size();
        data.reserve(size);
        for (long i =0; i<size; i++){
            data.push_back(0);
        }
    }

    void add(double x, double y, double z, double t){
        int i = fXbins->countIndx(x);
        int j = fYbins->countIndx(y);
        int k = fZbins->countIndx(z);
        int l = fTbins->countIndx(t);
        if ((i<0) || (j<0) || (k<0) || (t<0)){
            return;
        }
        add(i, j, k, l);
    }


private:
    void add(int i, int j, int k, int l){
        long indx = i*fYbins->size()*fZbins->size()*fTbins->size() + j*fZbins->size()*fTbins->size() + k*fTbins->size() + l;
        data[indx]++;
    }

};




#endif //PHD_CODE_HISTOGRAMM_HH
