//
// Created by zelenyy on 22.02.2020.
//

#ifndef PHD_CODE_DATATHUNDERSTORM_HH
#define PHD_CODE_DATATHUNDERSTORM_HH

#include "G4Track.hh"
#include "histogram.pb.h"
#include "thunderstorm.pb.h"
#include "G4SystemOfUnits.hh"
#include "DataFileManager.hh"
#include "Histogramm.hh"
#include "Settings.hh"

using namespace CLHEP;

class CylinderId : public ProtoWrapper<thunderstorm::CylinderIdList> {
public:
    void initializeEvent(int eventID) override {
        ProtoWrapper::initializeEvent(eventID);
        protoList->set_eventid(eventID);
    }

    void addTrack(const G4Track *track) override {
        auto data = protoList->add_cylinderid();
        data->set_id(track->GetTrackID());
        data->set_parent_id(track->GetParentID());
        data->set_particle(track->GetDefinition()->GetPDGEncoding());
        data->set_energy(track->GetKineticEnergy() / MeV);
        const G4ThreeVector &momentumDir = track->GetMomentumDirection();
        const G4ThreeVector &position = track->GetPosition();
        data->set_theta(momentumDir.getTheta() / radian);
        data->set_radius(position.perp() / meter);
        data->set_z(position.getZ() / meter);
        data->set_time(track->GetGlobalTime() / ns);
        ProtoWrapper::addTrack(track);
    }
};

class ParticleDetectorList : public ProtoWrapper<thunderstorm::ParticleDetectorList> {
public:
    void initializeEvent(int eventID) override {
        ProtoWrapper::initializeEvent(eventID);
        protoList->set_eventid(eventID);
    }

    void addTrack(const G4Track *track) override {
        auto data = protoList->add_data();
        data->set_particle(track->GetDefinition()->GetPDGEncoding());
        data->set_energy(track->GetKineticEnergy() / MeV);
        const G4ThreeVector &momentumDir = track->GetMomentumDirection();
        const G4ThreeVector &position = track->GetPosition();
        data->set_theta(momentumDir.getTheta() / radian);
        data->set_radius(position.perp() / meter);
        data->set_time(track->GetGlobalTime() / ns);
        ProtoWrapper::addTrack(track);
    }

};

struct SuperviseTreeNode {
    double z;
    int numberOfSecondaries;
};

class SuperviseTree {
private:
    SuperviseTreeNode trackingItem;
public:
    static SuperviseTree *instance() {
        static SuperviseTree superviseTree;
        return &superviseTree;
    }

    void preTracking(const G4Track *track) {
        trackingItem = {track->GetPosition().getZ(), 0};
        data->insert(make_pair(track->GetTrackID(), trackingItem));
    }

    void stepping(const G4Step *step) {
        trackingItem.numberOfSecondaries += step->GetNumberOfSecondariesInCurrentStep();
    }

    double getParentZandDecrementSecondaries(const G4Track *track) {
        auto item = data->find(track->GetParentID());
        if (item != data->end()) {
            double z = item->second.z;
            item->second.numberOfSecondaries--;
            if (item->second.numberOfSecondaries == 0) {
                data->erase(item);
            }
            return z;
        }
        return -DBL_MAX;
    }

    void reset() {
        data->clear();
    }


private:
    SuperviseTree() {
        data = new map<G4int, SuperviseTreeNode>();
    }

    SuperviseTree(SuperviseTree const &);

    SuperviseTree &operator=(SuperviseTree const &);

    ~SuperviseTree() {
        delete data;
    }

    map<G4int, SuperviseTreeNode> *data;
};


class Cumulator1D {
private:
    thunderstorm::Cumulator1D *cumulator1D;
    UniformBins *bins;
public:
    explicit Cumulator1D(UniformBins *bins) : bins(bins) {
        cumulator1D = new thunderstorm::Cumulator1D();
        cumulator1D->set_number(bins->fNumber);
        cumulator1D->set_right(bins->fRigth);
        cumulator1D->set_left(bins->fLeft);

        for (int i = 0; i < bins->fNumber; ++i) {
            cumulator1D->add_data(0.0);
        }
    }

    void accumulate_between(double left, double right, double value = 1.0) {
        int indx1 = bins->countIndx(left);
        int indx2 = bins->countIndx(right);
        if (indx1 != -1 && indx2 != -1) {
            accumulate_between(indx1, indx2, value);
        }

    }

    void accumulate_between(int indx1, int indx2, double value = 1.0) {
        for (int i = indx1; i < indx2 + 1; ++i) {
            cumulator1D->set_data(i, cumulator1D->data(i) + value);
        }
    }

    void save(ostream *output) {
        long size = cumulator1D->ByteSizeLong();
        output->write(reinterpret_cast<char *>(&size), sizeof size);
        cumulator1D->SerializeToOstream(output);
    }

    void reset() {
        for (int i = 0; i < bins->fNumber; ++i) {
            cumulator1D->set_data(i, 0.0);
        }
    }

};


class Cumulator2D {
private:
    thunderstorm::Cumulator2D *cumulator2D;
    UniformBins *x;
    UniformBins *y;
    int size;
public:
    Cumulator2D(UniformBins *x, UniformBins *y) : x(x), y(y) {
        size = x->fNumber * y->fNumber;
        cumulator2D = new thunderstorm::Cumulator2D();
        auto bins_x = new thunderstorm::UniformBins();
        bins_x->set_number(x->fNumber);
        bins_x->set_right(x->fRigth);
        bins_x->set_left(x->fLeft);
        auto bins_y = new thunderstorm::UniformBins();
        bins_y->set_number(y->fNumber);
        bins_y->set_right(y->fRigth);
        bins_y->set_left(y->fLeft);
        cumulator2D->set_allocated_x(bins_x);
        cumulator2D->set_allocated_y(bins_y);
        for (int i = 0; i < size; ++i) {
            cumulator2D->add_data(0.0);
        }
    }

    void add(double i, double j, double value = 1.0) {
        add(x->countIndx(i), y->countIndx(j), value);
    }

    void add(int i, int j, double value = 1.0) {
        int indx = i * y->fNumber + j;
        cumulator2D->set_data(indx, cumulator2D->data(indx) + value);
    }

    void save(ostream *output) {
        long size = cumulator2D->ByteSizeLong();
        output->write(reinterpret_cast<char *>(&size), sizeof size);
        cumulator2D->SerializeToOstream(output);
    }

    void reset() {
        for (int i = 0; i < size; ++i) {
            cumulator2D->set_data(i, 0.0);
        }
    }

};


class ElectronCounter2D {
private:
    UniformBins *zBins;
    UniformBins *timeBins;
    Cumulator2D *numbers;
    Cumulator2D *deposit;

    struct Point {
        double z;
        double time;
    };
    struct Indx {
        int z;
        int time;
    };

    static double distance(Point point1, Point point2) {
        return sqrt(sqr(point1.z - point2.z) + sqr(point1.time - point2.time));
    }

    Point lastPoint;

public:
    ElectronCounter2D(UniformBins *zBins, UniformBins *timeBins) : zBins(zBins), timeBins(timeBins) {
        numbers = new Cumulator2D(timeBins, zBins);
        deposit = new Cumulator2D(timeBins, zBins);
    }

    void preTrack(double z, double time) {
        lastPoint.z = z;
        lastPoint.time = time;
    }

    void add(double nextZ, double nextTime, double depositByStep) {
        double delta_time = nextTime - lastPoint.time;
        double delta_z = nextZ - lastPoint.z;

        if (abs(delta_z) < zBins->fStep && abs(delta_time) < timeBins->fStep) {
            deposit->add(nextTime, nextZ, depositByStep);
        } else {
            double distance_step = sqrt(delta_time * delta_time + delta_z * delta_z);

            double k;
            if (delta_time == 0.0) {
                k = 0;
            } else {
                k = delta_z / delta_time;
            }
            if (k == 0) {
                cout<<"k=0"<<endl;
                //TODO()
            }
            double b = nextZ - k * nextTime;

            Indx indx = {zBins->countIndx(lastPoint.z), timeBins->countIndx(lastPoint.time)};
            Indx lastIndx = {zBins->countIndx(nextZ), timeBins->countIndx(nextTime)};
            Point p1 = {lastPoint.z, lastPoint.time};
            double depositTemp = depositByStep;
            do {
                Indx current_indx = indx;
                double temp_time = timeBins->countValue(indx.time + 1);
                double temp_z = zBins->countValue(indx.z + 1);
                double proposed_z = k * temp_time + b;
                double proposed_time;
                if (proposed_z > temp_z) {
                    proposed_time = (temp_z - b) / k;
                    proposed_z = temp_z;
                    indx.z++;
                } else {
                    proposed_time = temp_time;
                    indx.time++;
                }
                Point p2 = {proposed_z, proposed_time};
                double ratio = distance(p1, p2) / distance_step;
                p1 = p2;
                double temp = ratio * depositByStep;
                depositTemp -= temp;
                deposit->add(current_indx.time, current_indx.z, temp);
                numbers->add(current_indx.time, current_indx.z);
            } while ((indx.time != lastIndx.time) && (indx.z != lastIndx.z));
            deposit->add(lastIndx.time, lastIndx.time, depositTemp);
        }

        lastPoint.z = nextZ;
        lastPoint.time = nextTime;
    }
    void reset(){
        deposit->reset();
        numbers->reset();
    }
    void save(ostream *outDeposit, ostream *outNumber) {
        deposit->save(outDeposit);
        numbers->save(outNumber);
    }
};

class ElectronsCounter {
private:
    UniformBins *zBins;
    UniformBins *timeBins;
    Cumulator1D *zCumulator;
    Cumulator1D *timeCumulator;
    ElectronCounter2D* electronCounter2D;
    string name;
    double startZ;
    double startTime;


private:
    Settings *pSettings;
public:
    ElectronsCounter(Settings *settings, string name) :
    pSettings(settings), name(name) {
        double length = settings->geometrySettings->cloud_length / meter;
        zBins = new UniformBins(0.0, length, length);
        timeBins = new UniformBins(0, 10000, 10000);
        zCumulator = new Cumulator1D(zBins);
        timeCumulator = new Cumulator1D(timeBins);
        electronCounter2D = new ElectronCounter2D(zBins, timeBins);
    }


    void preTracking(const G4Track *track) {
        startZ = track->GetPosition().getZ() / meter;
        startTime = track->GetGlobalTime() / ns;
        electronCounter2D->preTrack(startZ, startTime);
    }

    void stepping(const G4Step *step) {
        electronCounter2D->add(
                step->GetTrack()->GetPosition().getZ() / meter,
                step->GetTrack()->GetGlobalTime() / ns,
                step->GetTotalEnergyDeposit()/ MeV
                );

    }


    void postTracking(const G4Track *track) {
        double endZ = track->GetPosition().getZ() / meter;
        double length = pSettings->geometrySettings->cloud_length / meter;
        if (endZ >= length) { endZ = length - (zBins->fStep / 4.0); }
        double endTime = track->GetGlobalTime() / ns;
        if (startZ > endZ) { swap(startZ, endZ); }
        zCumulator->accumulate_between(startZ, endZ);
        timeCumulator->accumulate_between(startTime, endTime);
    }

    void endEvent() {
        auto dataFile = DataFileManager::instance();
        zCumulator->save(dataFile->getBinaryFile(name+"_z_cumulator"));
        timeCumulator->save(dataFile->getBinaryFile(name + "_time_cumulator"));
        electronCounter2D->save(dataFile->getBinaryFile(name + "_deposit_cumulator2d"),
                                dataFile->getBinaryFile(name + "_number_cumulator2d"));
        zCumulator->reset();
        timeCumulator->reset();
        electronCounter2D->reset();
    }
};


#endif //PHD_CODE_DATATHUNDERSTORM_HH
