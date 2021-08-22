// GPL v3 (c) 2021, Daniel Williams 
#pragma once

// standard library includes
#include <string>
#include <vector>

// GDAL
#include <cpl_json.h>

// fmt
#include <fmt/core.h>

#include "base-loader.hpp"
#include "chart-box.hpp"
#include "geometry/frame-mapping.hpp"

namespace chartbox::io::geojson {

const std::string extension = ".geojson";

/// \brief Load a GeoJSON file into a layer
///
/// References:
///   - https://geojson.org/
///   - https://datatracker.ietf.org/doc/html/rfc7946
///   - https://gdal.org/drivers/vector/geojson.html
template< typename layer_t >
class BoundaryLoader : BaseLoader<layer_t, BoundaryLoader<layer_t> > {
public:
    const static std::string extension;

    BoundaryLoader( FrameMapping& _mapping, layer_t& _layer );

    bool load(const std::filesystem::path& filename);

private:

    ///  The values of a "bbox" array are "[west, south, east, north]", not
    ///     "[minx, miny, maxx, maxy]" (see Section 5).  --rfc 7946
    bool load_boundary_box( const CPLJSONObject& doc );
    bool load_boundary_polygon( const CPLJSONObject& doc);

private:
    layer_t& layer_;
    FrameMapping& mapping_;

};

} // namespace chart::io

#include "boundary-loader.inl"