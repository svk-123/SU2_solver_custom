/*!
 * \file geometry_structure.inl
 * \brief In-Line subroutines of the <i>geometry_structure.hpp</i> file.
 * \author Aerospace Design Laboratory (Stanford University) <http://su2.stanford.edu>.
 * \version 3.2.3 "eagle"
 *
 * SU2, Copyright (C) 2012-2014 Aerospace Design Laboratory (ADL).
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */
 
#pragma once

inline long CGeometry::GetGlobal_to_Local_Point(long val_ipoint) { return 0; }

inline unsigned short CGeometry::GetGlobal_to_Local_Marker(unsigned short val_imarker) { return 0; }

inline unsigned long CGeometry::GetGlobal_nPoint(void) { return 0; }

inline unsigned long CGeometry::GetGlobal_nPointDomain(void) { return 0; }

inline unsigned long CGeometry::GetGlobal_nElem(void) { return 0; }

inline unsigned long CGeometry::GetGlobal_nElemLine(void) { return 0; }

inline unsigned long CGeometry::GetGlobal_nElemTria(void) { return 0; }

inline unsigned long CGeometry::GetGlobal_nElemQuad(void) { return 0; }

inline unsigned long CGeometry::GetGlobal_nElemTetr(void) { return 0; }

inline unsigned long CGeometry::GetGlobal_nElemHexa(void) { return 0; }

inline unsigned long CGeometry::GetGlobal_nElemWedg(void) { return 0; }

inline unsigned long CGeometry::GetGlobal_nElemPyra(void) { return 0; }

inline unsigned long CGeometry::GetnElemLine(void) { return 0; }

inline unsigned long CGeometry::GetnElemTria(void) { return 0; }

inline unsigned long CGeometry::GetnElemQuad(void) { return 0; }

inline unsigned long CGeometry::GetnElemTetr(void) { return 0; }

inline unsigned long CGeometry::GetnElemHexa(void) { return 0; }

inline unsigned long CGeometry::GetnElemWedg(void) { return 0; }

inline unsigned long CGeometry::GetnElemPyra(void) { return 0; }

inline void CGeometry::Check_IntElem_Orientation(CConfig *config) { }

inline void CGeometry::Check_BoundElem_Orientation(CConfig *config) { }

inline void CGeometry::SetColorGrid(CConfig *config) { }

inline void CGeometry::DivideConnectivity(CConfig *config, unsigned short Elem_Type) { }

inline void CGeometry::SetRotationalVelocity(CConfig *config) { }

inline void CGeometry::SetGridVelocity(CConfig *config, unsigned long iter) { }

inline void CGeometry::SetRestricted_GridVelocity(CGeometry *fine_mesh, CConfig *config) { } 

inline void CGeometry::Set_MPI_Coord(CConfig *config) { } 

inline void CGeometry::Set_MPI_GridVel(CConfig *config) { } 

inline void CGeometry::SetPeriodicBoundary(CConfig *config) { }

inline void CGeometry::SetPeriodicBoundary(CGeometry *geometry, CConfig *config) { }

inline void CGeometry::SetSendReceive(CConfig *config) { }

inline void CGeometry::ComputeWall_Distance(CConfig *config) { }

inline void CGeometry::SetPositive_ZArea(CConfig *config) { }

inline void CGeometry::SetPoint_Connectivity(void) { }

inline void CGeometry::SetRCM_Ordering(CConfig *config) { }

inline void CGeometry::SetCoord_Smoothing (unsigned short val_nSmooth, double val_smooth_coeff, CConfig *config) { }

inline void CGeometry::SetCoord(CGeometry *geometry) { }

inline void CGeometry::SetPoint_Connectivity(CGeometry *fine_grid) { }

inline void CGeometry::SetElement_Connectivity(void) { }

inline unsigned long CGeometry::GetnPoint(void) { return nPoint; }

inline unsigned long CGeometry::GetnPointDomain(void) { return nPointDomain; }

inline unsigned long CGeometry::GetnElem(void) { return nElem; }

inline unsigned short CGeometry::GetnDim(void) { return nDim; }

inline unsigned short CGeometry::GetnZone(void) { return nZone; }

inline unsigned short CGeometry::GetnMarker(void) { return nMarker; }

inline bool CGeometry::GetFinestMGLevel(void) { return FinestMGLevel; }

inline string CGeometry::GetMarker_Tag(unsigned short val_marker) { return Tag_to_Marker[val_marker]; }

inline unsigned long CGeometry::GetMax_GlobalPoint(void) { return Max_GlobalPoint; }

inline void CGeometry::SetnMarker(unsigned short val_nmarker) { nMarker = val_nmarker; }

inline void CGeometry::SetnElem_Bound(unsigned short val_marker, unsigned long val_nelem_bound) { nElem_Bound[val_marker]= val_nelem_bound; }

inline unsigned long CGeometry::GetnElem_Bound(unsigned short val_marker) { return nElem_Bound[val_marker]; }

inline void CGeometry::SetMarker_Tag(unsigned short val_marker, string val_index) { Tag_to_Marker[val_marker] = val_index; }

inline void CGeometry::SetnPoint(unsigned long val_npoint) { nPoint = val_npoint; }

inline void CGeometry::SetnPointDomain(unsigned long val_npoint) { nPointDomain = val_npoint; }

inline void CGeometry::SetnElem(unsigned long val_nelem) { nElem = val_nelem; }

inline void CGeometry::SetnDim(unsigned short val_nDim) { nDim = val_nDim; }

inline unsigned long CGeometry::GetnVertex(unsigned short val_marker) { return nVertex[val_marker]; }

inline unsigned long CGeometry::GetnEdge(void) { return nEdge; }

inline bool CGeometry::FindFace(unsigned long first_elem, unsigned long second_elem, unsigned short &face_first_elem, unsigned short &face_second_elem) {return 0;}

inline void CGeometry::SetBoundVolume(void) { }

inline void CGeometry::SetVertex(void) { }

inline void CGeometry::SetVertex(CConfig *config) { }

inline void CGeometry::SetVertex(CGeometry *fine_grid, CConfig *config) { }

inline void CGeometry::SetCG(void) { }

inline void CGeometry::SetControlVolume(CConfig *config, unsigned short action) { }

inline void CGeometry::SetControlVolume(CConfig *config, CGeometry *geometry, unsigned short action) { }

inline void CGeometry::VisualizeControlVolume(CConfig *config, unsigned short action) { }

inline void CGeometry::MatchNearField(CConfig *config) { }

inline void CGeometry::MatchActuator_Disk(CConfig *config) { }

inline void CGeometry::MatchInterface(CConfig *config) { }

inline void CGeometry::MatchZone(CConfig *config, CGeometry *geometry_donor, CConfig *config_donor, unsigned short val_iZone, unsigned short val_nZone) { }

inline void CGeometry::SetBoundControlVolume(CConfig *config, unsigned short action) { }

inline void CGeometry::SetBoundControlVolume(CConfig *config, CGeometry *geometry, unsigned short action) { }

inline void CGeometry::SetTecPlot(char config_filename[MAX_STRING_SIZE]) { }

inline void CGeometry::SetTecPlot(char config_filename[MAX_STRING_SIZE], bool new_file) { }

inline void CGeometry::SetMeshFile(CConfig *config, string val_mesh_out_filename) { }

inline void CGeometry::SetMeshFile(CGeometry *geometry, CConfig *config, string val_mesh_out_filename) { }

inline void CGeometry::SetMeshFile(CConfig *config, string val_mesh_out_filename, string val_mesh_in_filename) { }

inline void CGeometry::SetBoundTecPlot(char mesh_filename[MAX_STRING_SIZE], bool new_file, CConfig *config) { }

inline double CGeometry::Compute_MaxThickness(double *Plane_P0, double *Plane_Normal, unsigned short iSection, CConfig *config, vector<double> &Xcoord_Airfoil, vector<double> &Ycoord_Airfoil, vector<double> &Zcoord_Airfoil, bool original_surface) { return 0; }

inline double CGeometry::Compute_AoA(double *Plane_P0, double *Plane_Normal, unsigned short iSection, vector<double> &Xcoord_Airfoil, vector<double> &Ycoord_Airfoil, vector<double> &Zcoord_Airfoil, bool original_surface) { return 0; }
  
inline double CGeometry::Compute_Chord(double *Plane_P0, double *Plane_Normal, unsigned short iSection, vector<double> &Xcoord_Airfoil, vector<double> &Ycoord_Airfoil, vector<double> &Zcoord_Airfoil, bool original_surface) { return 0; }

inline double CGeometry::Compute_Thickness(double *Plane_P0, double *Plane_Normal, unsigned short iSection, double Location, CConfig *config, vector<double> &Xcoord_Airfoil, vector<double> &Ycoord_Airfoil, vector<double> &Zcoord_Airfoil, bool original_surface) { return 0; }

inline double CGeometry::Compute_Area(double *Plane_P0, double *Plane_Normal, unsigned short iSection, CConfig *config, vector<double> &Xcoord_Airfoil, vector<double> &Ycoord_Airfoil, vector<double> &Zcoord_Airfoil, bool original_surface) { return 0; }

inline double CGeometry::Compute_Volume(CConfig *config, bool original_surface) { return 0; }

inline void CGeometry::FindNormal_Neighbor(CConfig *config) { }

inline void CGeometry::SetBoundSensitivity(CConfig *config) { }

inline void CPhysicalGeometry::SetPoint_Connectivity(CGeometry *geometry) { CGeometry::SetPoint_Connectivity(geometry); } 

inline void CMultiGridGeometry::SetPoint_Connectivity(void) { CGeometry::SetPoint_Connectivity(); }

inline long CPhysicalGeometry::GetGlobal_to_Local_Point(long val_ipoint) { return Global_to_Local_Point[val_ipoint]; }

inline unsigned short CPhysicalGeometry::GetGlobal_to_Local_Marker(unsigned short val_imarker) { return Global_to_Local_Marker[val_imarker]; }

inline unsigned long CPhysicalGeometry::GetGlobal_nPoint(void) { return Global_nPoint; }

inline unsigned long CPhysicalGeometry::GetGlobal_nPointDomain(void) { return Global_nPointDomain; }

inline unsigned long CPhysicalGeometry::GetGlobal_nElem(void) { return Global_nElem; }

inline unsigned long CPhysicalGeometry::GetGlobal_nElemLine(void) { return Global_nelem_edge; }

inline unsigned long CPhysicalGeometry::GetGlobal_nElemTria(void) { return Global_nelem_triangle; }

inline unsigned long CPhysicalGeometry::GetGlobal_nElemQuad(void) { return Global_nelem_quad; }

inline unsigned long CPhysicalGeometry::GetGlobal_nElemTetr(void) { return Global_nelem_tetra; }

inline unsigned long CPhysicalGeometry::GetGlobal_nElemHexa(void) { return Global_nelem_hexa; }

inline unsigned long CPhysicalGeometry::GetGlobal_nElemWedg(void) { return Global_nelem_wedge; }

inline unsigned long CPhysicalGeometry::GetGlobal_nElemPyra(void) { return Global_nelem_pyramid; }

inline unsigned long CPhysicalGeometry::GetnElemLine(void) { return nelem_edge; }

inline unsigned long CPhysicalGeometry::GetnElemTria(void) { return nelem_triangle; }

inline unsigned long CPhysicalGeometry::GetnElemQuad(void) { return nelem_quad; }

inline unsigned long CPhysicalGeometry::GetnElemTetr(void) { return nelem_tetra; }

inline unsigned long CPhysicalGeometry::GetnElemHexa(void) { return nelem_hexa; }

inline unsigned long CPhysicalGeometry::GetnElemWedg(void) { return nelem_wedge; }

inline unsigned long CPhysicalGeometry::GetnElemPyra(void) { return nelem_pyramid; }

inline void CGeometry::SetGeometryPlanes(CConfig *config) {}

inline vector<double> CGeometry::GetGeometryPlanes() { return XCoordList; }

inline vector<double> CPhysicalGeometry::GetGeometryPlanes() { return XCoordList; }

inline vector<double> CMultiGridGeometry::GetGeometryPlanes() { return XCoordList; }

inline vector<vector<double> > CGeometry::GetXCoord() { return Xcoord_plane; }

inline vector<vector<double> > CPhysicalGeometry::GetXCoord() { return Xcoord_plane; }

inline vector<vector<double> > CMultiGridGeometry::GetXCoord() { return Xcoord_plane; }

inline vector<vector<double> > CGeometry::GetYCoord() { return Ycoord_plane; }

inline vector<vector<double> > CPhysicalGeometry::GetYCoord() { return Ycoord_plane; }

inline vector<vector<double> > CMultiGridGeometry::GetYCoord() { return Ycoord_plane; }

inline vector<vector<double> > CGeometry::GetZCoord() { return Zcoord_plane; }

inline vector<vector<double> > CPhysicalGeometry::GetZCoord() { return Zcoord_plane; }

inline vector<vector<double> > CMultiGridGeometry::GetZCoord() { return Zcoord_plane; }


inline vector<vector<unsigned long> > CGeometry::GetPlanarPoints() { return Plane_points; }

inline vector<vector<unsigned long> > CPhysicalGeometry::GetPlanarPoints() { return Plane_points; }

inline vector<vector<unsigned long> > CMultiGridGeometry::GetPlanarPoints() { return Plane_points; }

