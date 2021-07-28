//
// Created by zelenyy on 26.06.2020.
//

#include "Parma.hh"




namespace parma {


    const std::string Model::pname[Model::npart+1] = {"neutro", "proton", "alphaa", "elemag", "elemag", "elemag", "muon--", "muon--",
    "ions  ",
    "ions  ", "ions  ", "ions  ", "ions  ", "ions  "};

    Generator::Generator(int particle, Date date, Coord coord, Model *model) : model(model) {



// calculate parameters
        double s = model->getHPcpp(date.year, date.month, date.day); // solar modulation potential
        double r = model->getrcpp(coord.latitude, coord.longitude);  // Vertical cut-off rigidity (GV)
        double d = model->getdcpp(coord.altitude,
                           coord.latitude);   // Atmospheric depth (g/cm2), set glat = 100 for use US Standard Atmosphere 1976.
        if (IangPart[particle] == 0) {
            std::cout << "Angular distribution is not available for the particle";
            exit(1);
        }

        if (particle == 0 && emin < 1.0e-8) { emin = 1.0e-8; } // Minimum energy for neutron is 10 meV
        if (particle != 0 && emin < 1.0e-2) { emin = 1.0e-2; } // Minimum energy for other particle is 10 keV

        // Make energy and angle mesh
        double elog = log10(emin);
        double estep = (log10(emax) - log10(emin)) / nebin;
        for (ie = 0; ie <= nebin; ie++) {
            ehigh[ie] = pow(10, elog);
            if (ie != 0) emid[ie] = sqrt(ehigh[ie] * ehigh[ie - 1]);
            elog = elog + estep;
        }

        double astep = (amax - amin) / nabin;
        for (ia = 0; ia <= nabin; ia++) {
            ahigh[ia] = amin + astep * ia;
            if (ia != 0) amid[ia] = (ahigh[ia] + ahigh[ia - 1]) * 0.5;
        }

        for (ie = 1; ie <= nebin; ie++) {
            for (ia = 1; ia <= nabin; ia++) {
                atable[ia][ie] = atable[ia - 1][ie] + model->getSpecCpp(particle, s, r, d, emid[ie], g) *
                                                              model->getSpecAngFinalCpp(IangPart[particle], s, r, d, emid[ie], g,
                                                                         amid[ia]) * (2.0 * acos(-1.0)) *
                                                      (ahigh[ia] - ahigh[ia - 1]); // angular integrated value
            }
        }

        for (ie = 1; ie <= nebin; ie++) {
            etable[ie] = etable[ie - 1] + atable[nabin][ie] * (ehigh[ie] - ehigh[ie - 1]); // energy integrated value
        }
        TotalFlux = etable[nebin]; // Total Flux (/cm2/s), used for normalization

// Make probability table (normalized to 1)
        for (ie = 1; ie <= nebin; ie++) {
            etable[ie] = etable[ie] / etable[nebin];
            for (ia = 1; ia <= nabin; ia++) {
                atable[ia][ie] = atable[ia][ie] / atable[nabin][ie];
            }
        }




//    ofstream sf("GeneOut/generation.out",ios::out);
//    sf << "ip= " << ip << " ,FFP(MV)= " << s << " ,Rc(GV)= " << r << " ,depth(g/cm2)= " << d << " ,g= " << g << "\n";
//    sf << "Total Flux (/cm2/s)= " << TotalFlux << "\n";
//    sf << "  Energy(MeV/n)              u              v              w              x              y              z\n";


//    ofstream of("GeneOut/flux.out",ios::out);
//    of << "ip= " << ip << " ,FFP(MV)= " << s << " ,Rc(GV)= " << r << " ,depth(g/cm2)= " << d << " ,g= " << g << "\n";
//    of << "Total Flux (/cm2/s)= " << TotalFlux << "\n";
//    of << "Angular and energy differential fluxes in /cm2/s/(MeV/n)/sr\n";
//    of << "   E_low(MeV/n) /cm2/s/(MeV/n)";
//    for(ia=1;ia<=nabin;ia++) {of << setw(15) << ahigh[ia-1];}
//    of << "\n";
//    for(ie=1;ie<=nebin;ie++) {
//        of << scientific << setw(15) << ehigh[ie-1];
//        for(ia=0;ia<=nabin;ia++) {of << setw(15) << Flux[ia][ie]/nevent;}
//        of << "\n";
//    }

    }


}