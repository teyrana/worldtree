// GPL v3 (c) 2021, Daniel Williams 

#pragma once

#include <array>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <string>

#include "chart-box/chart-layer-interface.hpp"
#include "chart-box/geometry/polygon.hpp"
#include "chart-box/geometry/bound-box.hpp"

#include "rolling-grid-sector.hpp"
#include "rolling-grid-index.hpp"

namespace chartbox::layer {

//  chart => layer => sector => cell
//            ^^^ you are here
class RollingGridLayer : public ChartLayerInterface< RollingGridLayer > {
public:

    // `*_across_*` variables count the numbe in each dimension of a square grid
    // `*_per_*` variables count the total number in the entire grid

    constexpr static size_t cells_across_sector = 4;
    // constexpr static size_t cells_per_sector = cells_across_sector * cells_across_sector;  ///< not yet needed
    constexpr static double meters_across_cell = 1.0;   //aka precision
    // constexpr static double area_per_cell = meters_across_cell * meters_across_cell;

    constexpr static size_t sectors_across_view = 3;
    // constexpr static size_t sectors_per_view = sectors_across_view * sectors_across_view;
    constexpr static double meters_across_sector = meters_across_cell * cells_across_sector; 
    // constexpr static double area_per_sector = area_per_cell * cells_per_sector;

    constexpr static size_t cells_across_view = cells_across_sector * sectors_across_view;

    constexpr static double precision = meters_across_cell;

    /// \brief name of this layer's type
    constexpr static char type_name_[] = "RollingGridLayer";

    // used to address a cell within a sector
    // Is explicitly a different type to prevent cross-talk
    typedef Index2u32<cells_across_sector> CellIndex;

    // used to address a sector within the active view
    // Is explicitly a different type to prevent cross-talk
    typedef Index2u32<sectors_across_view> SectorIndex;

    // used to address a cell within the active view
    // Is explicitly a different type to prevent cross-talk
    typedef Index2u32<cells_across_sector*sectors_across_view> ViewIndex;

public:
    /// \brief Constructs a new 2d square grid
    RollingGridLayer() = delete;
    
    /// \brief Constructs a new 2d square grid
    // RollingGridLayer(const geometry::BoundBox<UTMLocation>& _bounds);

    RollingGridLayer(const geometry::BoundBox<UTMLocation>& _bounds);

    ~RollingGridLayer(){};

    bool fill( const uint8_t value );

    bool focus( const BoundBox<LocalLocation>& new_bounds );

    uint8_t get(const LocalLocation& p) const;

    uint8_t get( double easting, double northing ) const;

    // size_t lookup( const LocalLocation& p ) const;
    // size_t lookup( const LocalLocation& p ) const;

    /// \brief Draws a simple debug representation of this grid to stderr
    std::string print_contents() const;

    /// \brief Access the value at an (x, y) Eigen::Vector2d
    ///
    /// \param p - the x,y coordinates to search at:
    /// \param new_value - the value to stored at the specified location
    /// \return true if successful
    bool store(const LocalLocation& p, uint8_t new_value);

    /// \brief Get the bounds of the tracked (overall) area
    const BoundBox<LocalLocation>& tracked() const;
    bool tracked(const LocalLocation& p) const;

    /// \brief Get the currently visible (active) bounds of this layer
    const BoundBox<LocalLocation>& visible() const;
    bool visible(const LocalLocation& p) const;

private:

    std::filesystem::path cache_path;

    geometry::BoundBox<LocalLocation> tracked_bounds_;
    geometry::BoundBox<LocalLocation> view_bounds_;

    // this tracks the outer bounds (that the whole chart is tracking)
    // geometry::BoundBox<LocalLocation>& super().bounds_;

    //  chart => layer => sector => cell
    //                 ^^ you are here
    std::array<RollingGridSector<cells_across_sector>, sectors_across_view * sectors_across_view > sectors;

    // optional (?)
    // typedef uint8_t sector_map_t;  ///< placeholder
    // std::array<sector_grid_t*, layer_dimension * layer_dimension> sector_map;


private:

    ChartLayerInterface<RollingGridLayer>& super() {
        return *static_cast< ChartLayerInterface<RollingGridLayer>* >(this);
    }

    const ChartLayerInterface<RollingGridLayer>& super() const {
        return *static_cast< const ChartLayerInterface<RollingGridLayer>* >(this);
    }
};


} // namespace
