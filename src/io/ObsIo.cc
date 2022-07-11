/*
 * (C) Copyright 2017-2019 UCAR
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#include "oops/util/abor1_cpp.h"

#include "ioda/io/ObsIo.h"
#include "ioda/Variables/Variable.h"

////////////////////////////////////////////////////////////////////////
// Implementation of ObsIo base class
////////////////////////////////////////////////////////////////////////

namespace ioda {

//--------------------------- public functions ---------------------------------------
//------------------------------------------------------------------------------------
ObsIo::ObsIo() {
    oops::Log::trace() << "Constructing ObsIo" << std::endl;
}

//------------------------------------------------------------------------------------
bool ObsIo::isVarDimByNlocs(const std::string & varName) const {
    bool isDimByNlocs = false;
    for (auto & ivar : dims_attached_to_vars_) {
        if (ivar.first.name == varName) {
            // Found varName, now check if first dimension is "nlocs"
            if (ivar.second[0].name == "nlocs") {
                isDimByNlocs = true;
            }
        }
    }
    return isDimByNlocs;
}

//------------------------------------------------------------------------------------
void ObsIo::updateVarDimInfo() {
    VarUtils::collectVarDimInfo(obs_group_, var_list_, dim_var_list_, dims_attached_to_vars_,
                      max_var_size_);
}

//------------------------ protected functions ---------------------------------------
void ObsIo::print(std::ostream & os) const {}

}  // namespace ioda
