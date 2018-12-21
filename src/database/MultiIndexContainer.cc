/*
 * (C) Copyright 2017 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include "database/MultiIndexContainer.h"
#include "fileio/IodaIO.h"
#include "fileio/IodaIOfactory.h"

namespace ioda {
// -----------------------------------------------------------------------------
  ObsSpaceContainer::ObsSpaceContainer(const eckit::Configuration & config,
                                       const util::DateTime & bgn,
                                       const util::DateTime & end,
                                       const eckit::mpi::Comm & commMPI)
        : winbgn_(bgn), winend_(end), commMPI_(commMPI) {
    oops::Log::trace() << "ioda::ObsSpaceContainer Constructor starts " << std::endl;
  }
// -----------------------------------------------------------------------------
  ObsSpaceContainer::~ObsSpaceContainer() {
    oops::Log::trace() << "ioda::ObsSpaceContainer deconstructed " << std::endl;
  }
// -----------------------------------------------------------------------------
  void ObsSpaceContainer::CreateFromFile(const std::string & filename, const std::string & mode,
                                         const util::DateTime & bgn, const util::DateTime & end,
                                         const eckit::mpi::Comm & commMPI) {
    oops::Log::trace() << "ioda::ObsSpaceContainer opening file: " << filename << std::endl;

    std::unique_ptr<ioda::IodaIO> fileio
      {ioda::IodaIOfactory::Create(filename, mode, bgn, end, commMPI)};
    nlocs_ = fileio->nlocs();
    nvars_ = fileio->nvars();

    // Load all valid variables
    std::unique_ptr<boost::any[]> vect;
    std::string group, variable, db_name;

    for (auto iter = (fileio->varlist())->begin(); iter != (fileio->varlist())->end(); ++iter) {
      // Revisit here, improve the readability
      group = std::get<1>(*iter);
      variable = std::get<0>(*iter);
      db_name = variable;
      if (group.size() > 0)
        db_name = variable + "@" + group;
      else
        group = "GroupUndefined";
      vect.reset(new boost::any[nlocs()]);
      fileio->ReadVar_any(db_name, vect.get());
      // All records read from file are read-only
      DataContainer.insert({group, variable, "r", nlocs(), vect});
    }
    oops::Log::trace() << "ioda::ObsSpaceContainer opening file ends " << std::endl;
  }
// -----------------------------------------------------------------------------

bool ObsSpaceContainer::has(const std::string & group, const std::string & variable) const {
  auto var = DataContainer.find(boost::make_tuple(group, variable));
  return (var != DataContainer.end());
}

// -----------------------------------------------------------------------------

void ObsSpaceContainer::dump(const std::string & file_name) const {
  // Open the file for output
  std::unique_ptr<ioda::IodaIO> fileio
    {ioda::IodaIOfactory::Create(file_name, "W", windowStart(), windowEnd(),
    comm(), nlocs(), 0, 0, nvars())};  //  Not sure nrecs and nobs are useful

  // List all records and write out the every record
  auto & var = DataContainer.get<ObsSpaceContainer::by_variable>();
  for (auto iter = var.begin(); iter != var.end(); ++iter)
    fileio->WriteVar_any(iter->variable + "@" + iter->group, iter->data.get());
}

// -----------------------------------------------------------------------------

void ObsSpaceContainer::print(std::ostream & os) const {
  auto & var = DataContainer.get<ObsSpaceContainer::by_variable>();
  os << "ObsSpace Multi.Index Container for IODA" << "\n";
  for (auto iter = var.begin(); iter != var.end(); ++iter)
    os << iter->variable << " @ " << iter->group << "\n";
}

// -----------------------------------------------------------------------------

}  // namespace ioda
