#pragma once
/*
 * (C) Copyright 2020-2021 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */
/// \file HH.h
/// \brief HDF5 interface for ioda.

#include <string>
#include <utility>

#include "../defs.h"
#include "Capabilities.h"
#include "Factory.h"

namespace ioda {
class Group;

namespace Engines {
/// \brief Functions that are helpful in creating a new ioda::Group that is backed by HDF5.
namespace HH {
/// Library's format versions
enum class HDF5_Version {
  Earliest,  ///< Use the earliest possible HDF5 format for storing objects.
  V18,       ///< Use the latest HDF5 v1.8 format for storing objects.
  V110,      ///< Use the latest HDF5 v1.10 format for storing objects.
  V112,      ///< Use the latest HDF5 v1.12 format for storing objects.
  Latest     ///< Use the latest possible HDF5 format for storing objects.
};
typedef std::pair<HDF5_Version, HDF5_Version> HDF5_Version_Range;
IODA_DL HDF5_Version_Range defaultVersionRange();

/// \brief Convenience function to generate a random file name.
/// \see createMemoryFile
IODA_DL std::string genUniqueName();

/// \brief Create a ioda::Group backed by an HDF5 file.
/// \param filename is the file name.
/// \param mode is the creation mode.
/// \param compat is the range of HDF5 versions that should be able to access this file.
IODA_DL Group createFile(const std::string& filename, BackendCreateModes mode,
                         HDF5_Version_Range compat = defaultVersionRange());

/// \brief Open a ioda::Group backed by an HDF5 file.
/// \param filename is the file name.
/// \param mode is the access mode.
/// \param compat is the range of HDF5 versions that should be able to access this file.
IODA_DL Group openFile(const std::string& filename, BackendOpenModes mode,
                       HDF5_Version_Range compat = defaultVersionRange());

/// \brief Create a ioda::Group backed by the HDF5 in-memory-store.
/// \param filename is the name of the file if it gets flushed
///   (written) to disk. Otherwise, it is a unique identifier.
///   If this id is reused, then we are re-opening the store.
/// \param mode is the creation mode. This only matters if
///   the file is flushed to disk.
/// \param flush_on_close instructs us to flush the memory
///   image to the disk when done.
///   If false, then the image is never written to disk.
/// \param increment_len_bytes is the length (in bytes) of
///   the initial memory image. As the image grows larger,
///   then additional memory allocations will be performed.
/// \param compat is the range of HDF5 versions that should be able to access this file.
IODA_DL Group createMemoryFile(const std::string& filename,
                               BackendCreateModes mode,  // = BackendCreateModes::Fail_If_Exists,
                               bool flush_on_close = false, size_t increment_len_bytes = 1000000,
                               HDF5_Version_Range compat = defaultVersionRange());

/// \brief Map an HDF5 file in memory and open a ioda::Group.
/// \param filename is the name of the file to be opened.
/// \param mode is the access mode.
/// \param flush_on_close instructs us to flush the memory image
///   to the disk when done. If false, then the image is never written to disk.
/// \param increment_len_bytes is the length (in bytes) of the initial
///   memory image. As the image grows larger, then additional memory
///   allocations will be performed of increment_len bytes.
///
/// \details
/// While using openMemoryFile to open an existing file, if
/// flush_on_close is set to 1 and the mode is set to
/// BackendOpenModes::Read_Write, any change to the file contents are
/// saved to the file when the file is closed. If flush_on_close is set
/// to 0 and the flags for mode is set to BackendOpenModes::Read_Write,
/// any change to the file contents will be lost when the file is closed.
/// If the mode for openMemoryFile is set to BackendOpenModes::Read_Only,
/// no change to the file is allowed either in memory or on file.
/// \param compat is the range of HDF5 versions that should be able to access this file.
IODA_DL Group openMemoryFile(const std::string& filename,
                             BackendOpenModes mode = BackendOpenModes::Read_Only,
                             bool flush_on_close = false, size_t increment_len_bytes = 1000000,
                             HDF5_Version_Range compat = defaultVersionRange());

/// Get capabilities of the HDF5 file-backed engine
IODA_DL Capabilities getCapabilitiesFileEngine();

/// Get capabilities of the HDF5 memory-backed engine
IODA_DL Capabilities getCapabilitiesInMemoryEngine();
}  // namespace HH
}  // namespace Engines
}  // namespace ioda
