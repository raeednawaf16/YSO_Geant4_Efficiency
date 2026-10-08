// ImplantProfileSampler.hh
// Samples (x, y) implant positions from the experimental implantXY histogram
// exported from ysoProfile.root (file: implantXY_bins.txt).
// Pure C++ + Geant4 RNG: no ROOT linking needed, thread-safe if each worker
// owns its own instance (e.g. as a member of PrimaryGeneratorAction).

#pragma once
#include "G4Exception.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

class ImplantProfileSampler {
public:
  // xCenter/yCenter: the point in the histogram (mm) that corresponds to
  // x = y = 0 in your Geant4 world (e.g. the centre of the YSO face).
  ImplantProfileSampler(const std::string& file,
                        double xCenter = 24.0, double yCenter = 24.0,
                        double binWidth = 0.024)
    : fXc(xCenter), fYc(yCenter), fW(binWidth)
  {
    std::ifstream in(file);
    if (!in) G4Exception("ImplantProfileSampler", "NoFile", FatalException,
                         ("Cannot open " + file).c_str());
    std::string line; double x, y, n, sum = 0.;
    while (std::getline(in, line)) {
      if (line.empty() || line[0] == '#') continue;
      std::istringstream ss(line);
      if (!(ss >> x >> y >> n) || n <= 0) continue;
      sum += n;
      fX.push_back(x); fY.push_back(y); fCdf.push_back(sum);
    }
    for (auto& c : fCdf) c /= sum;
  }

  // Returns position in Geant4 units, relative to (xCenter, yCenter).
  void Sample(double& x, double& y) const {
    const double r = G4UniformRand();
    const size_t k = std::lower_bound(fCdf.begin(), fCdf.end(), r) - fCdf.begin();
    // uniform smearing inside the 0.024 mm bin
    x = (fX[k] + fW * G4UniformRand() - fXc) * mm;
    y = (fY[k] + fW * G4UniformRand() - fYc) * mm;
  }

private:
  double fXc, fYc, fW;
  std::vector<double> fX, fY, fCdf;
};

/* ---- Usage in PrimaryGeneratorAction ----------------------------------

// .hh
#include "ImplantProfileSampler.hh"
ImplantProfileSampler fSampler{"implantXY_bins.txt"};

// GeneratePrimaries(G4Event* evt)
double x, y;
fSampler.Sample(x, y);
fParticleGun->SetParticlePosition(G4ThreeVector(x, y, zImplant));
fParticleGun->GeneratePrimaryVertex(evt);

------------------------------------------------------------------------ */
