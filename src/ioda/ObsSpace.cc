/*
 * (C) Copyright 2017 UCAR
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#include "ObsSpace.h"

#include <map>
#include <string>

#include "eckit/config/Configuration.h"

#include "oops/util/abor1_cpp.h"
#include "oops/util/Logger.h"

#include "Locations.h"
//#include "ObsVector.h"

namespace ioda {
// -----------------------------------------------------------------------------

ObsSpace::ObsSpace(const eckit::Configuration & config,
                   const util::DateTime & bgn, const util::DateTime & end)
  : oops::ObsSpaceBase(config, bgn, end), winbgn_(bgn), winend_(end)
{
  oops::Log::trace() << "ioda::ObsSpace config  = " << config << std::endl;

  const eckit::Configuration * configc = &config;
  obsname_ = config.getString("ObsType");

  if (obsname_ == "Radiance") 
    ioda_obsdb_radiance_setup_f90(keyOspace_, &configc);
  else if (obsname_ == "Radiosonde")
    ioda_obsdb_radiosonde_setup_f90(keyOspace_, &configc);
  else if (obsname_ == "SeaIceFraction")
    ioda_obsdb_seaice_setup_f90(keyOspace_, &configc);
  else if (obsname_ == "StericHeight")
    ioda_obsdb_stericheight_setup_f90(keyOspace_, &configc);  
  else if (obsname_ == "SeaIceThickness")
    ioda_obsdb_seaicethick_setup_f90(keyOspace_, &configc);
  else if (obsname_ == "Aod")
    ioda_obsdb_aod_setup_f90(keyOspace_, &configc);

  oops::Log::trace() << "ioda::ObsSpace contructed name = " << obsname_ << std::endl;
}

// -----------------------------------------------------------------------------

ObsSpace::~ObsSpace() {
  if (obsname_ == "Radiance")
    ioda_obsdb_radiance_delete_f90(keyOspace_);
  else if (obsname_ == "Radiosonde")
    ioda_obsdb_radiosonde_delete_f90(keyOspace_);
  else if (obsname_ == "SeaIceFraction")
    ioda_obsdb_seaice_delete_f90(keyOspace_);
  else if (obsname_ == "StericHeight")
    ioda_obsdb_stericheight_delete_f90(keyOspace_);  
  else if (obsname_ == "SeaIceThickness")
    ioda_obsdb_seaicethick_delete_f90(keyOspace_);
  else if (obsname_ == "Aod")
    ioda_obsdb_aod_delete_f90(keyOspace_);
}

// -----------------------------------------------------------------------------

void ObsSpace::getdb(const std::string & col, int & keyData) const {
  if (obsname_ == "Radiance")
    ioda_obsdb_radiance_get_f90(keyOspace_, col.size(), col.c_str(), keyData);
  else if (obsname_ == "Radiosonde")
    ioda_obsdb_radiosonde_get_f90(keyOspace_, col.size(), col.c_str(), keyData);
  else if (obsname_ == "SeaIceFraction")
    ioda_obsdb_seaice_get_f90(keyOspace_, col.size(), col.c_str(), keyData);
  else if (obsname_ == "SeaIceThickness")
    ioda_obsdb_seaicethick_get_f90(keyOspace_, col.size(), col.c_str(), keyData);  
  else if (obsname_ == "StericHeight")
    ioda_obsdb_stericheight_get_f90(keyOspace_, col.size(), col.c_str(), keyData);      
  else if (obsname_ == "Aod")
    ioda_obsdb_aod_get_f90(keyOspace_, col.size(), col.c_str(), keyData);      
}

// -----------------------------------------------------------------------------

void ObsSpace::putdb(const std::string & col, const int & keyData) const {
  oops::Log::trace() << "In putdb obsname = " << std::endl;
}

// -----------------------------------------------------------------------------

Locations * ObsSpace::locations(const util::DateTime & t1, const util::DateTime & t2) const {
  const util::DateTime * p1 = &t1;
  const util::DateTime * p2 = &t2;
  int keylocs;
  if (obsname_ == "Radiance")
    ioda_obsdb_radiance_getlocations_f90(keyOspace_, &p1, &p2, keylocs);
  else if (obsname_ == "Radiosonde")
    ioda_obsdb_radiosonde_getlocations_f90(keyOspace_, &p1, &p2, keylocs);
  else if (obsname_ == "SeaIceFraction")
    ioda_obsdb_seaice_getlocations_f90(keyOspace_, &p1, &p2, keylocs);
  else if (obsname_ == "StericHeight")
    ioda_obsdb_stericheight_getlocations_f90(keyOspace_, &p1, &p2, keylocs);  
  else if (obsname_ == "SeaIceThickness")
    ioda_obsdb_seaicethick_getlocations_f90(keyOspace_, &p1, &p2, keylocs);
  else if (obsname_ == "Aod")
    ioda_obsdb_aod_getlocations_f90(keyOspace_, &p1, &p2, keylocs);

  return new Locations(keylocs);
}

// -----------------------------------------------------------------------------

int ObsSpace::nobs() const {
  int n;
  if (obsname_ == "Radiance")
    ioda_obsdb_radiance_nobs_f90(keyOspace_, n);
  else if (obsname_ == "Radiosonde")
    ioda_obsdb_radiosonde_nobs_f90(keyOspace_, n);
  else if (obsname_ == "SeaIceFraction")
    ioda_obsdb_seaice_nobs_f90(keyOspace_, n);
  else if (obsname_ == "StericHeight")
    ioda_obsdb_stericheight_nobs_f90(keyOspace_, n);  
  else if (obsname_ == "SeaIceThickness")
    ioda_obsdb_seaicethick_nobs_f90(keyOspace_, n);
  else if (obsname_ == "Aod")
    ioda_obsdb_aod_nobs_f90(keyOspace_, n);

  return n;
}

// -----------------------------------------------------------------------------

void ObsSpace::generateDistribution(const eckit::Configuration & conf) {
  const eckit::Configuration * configc = &conf;

  const util::DateTime * p1 = &winbgn_;
  const util::DateTime * p2 = &winend_;
  if (obsname_ == "Radiance")
    ioda_obsdb_radiance_generate_f90(keyOspace_, &configc, &p1, &p2);
  else if (obsname_ == "Radiosonde")
    ioda_obsdb_radiosonde_generate_f90(keyOspace_, &configc, &p1, &p2);
  else if (obsname_ == "SeaIceFraction")
    ioda_obsdb_seaice_generate_f90(keyOspace_, &configc, &p1, &p2);
  else if (obsname_ == "StericHeight")
    ioda_obsdb_stericheight_generate_f90(keyOspace_, &configc, &p1, &p2);  
  else if (obsname_ == "SeaIceThickness")
    ioda_obsdb_seaicethick_generate_f90(keyOspace_, &configc, &p1, &p2);
  else if (obsname_ == "Aod")
    ioda_obsdb_aod_generate_f90(keyOspace_, &configc, &p1, &p2);

}

// -----------------------------------------------------------------------------

void ObsSpace::print(std::ostream & os) const {
  os << "ObsSpace::print not implemented";
}

// -----------------------------------------------------------------------------

void ObsSpace::printJo(const ObsVector & dy, const ObsVector & grad) {
  oops::Log::info() << "ObsSpace::printJo not implemented" << std::endl;
}

// -----------------------------------------------------------------------------

}  // namespace ioda
