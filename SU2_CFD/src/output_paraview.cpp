/*!
 * \file output_paraview.cpp
 * \brief Main subroutines for output solver information.
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

#include "../include/output_structure.hpp"

void COutput::SetParaview_ASCII(CConfig *config, CGeometry *geometry, unsigned short val_iZone, unsigned short val_nZone, bool surf_sol) {
    
	/*--- Local variables and initialization ---*/
	unsigned short iDim, iVar, nDim = geometry->GetnDim();
	unsigned short Kind_Solver = config->GetKind_Solver();
    
	unsigned long iPoint, iElem, iNode;
	unsigned long iExtIter = config->GetExtIter();
  unsigned long *LocalIndex = NULL;
  bool *SurfacePoint = NULL;
  
  unsigned long nSurf_Elem_Storage;
  unsigned long nGlobal_Elem_Storage;
  
	bool grid_movement  = config->GetGrid_Movement();
	bool adjoint = config->GetAdjoint();
    
	char cstr[200], buffer[50];
	string filename;
    
	/*--- Write file name with extension ---*/
  if (surf_sol) {
    if (adjoint)
      filename = config->GetSurfAdjCoeff_FileName();
    else
      filename = config->GetSurfFlowCoeff_FileName();
  }
  else {
    if (adjoint)
      filename = config->GetAdj_FileName();
    else
      filename = config->GetFlow_FileName();
  }
  
	if (Kind_Solver == LINEAR_ELASTICITY)
		filename = config->GetStructure_FileName().c_str();
  
	if (Kind_Solver == WAVE_EQUATION)
		filename = config->GetWave_FileName().c_str();
  
	if (Kind_Solver == POISSON_EQUATION)
		filename = config->GetStructure_FileName().c_str();

  if (Kind_Solver == HEAT_EQUATION)
		filename = config->GetHeat_FileName().c_str();
    
#ifdef HAVE_MPI
	int nProcessor;
	/*--- Remove the domain number from the surface csv filename ---*/
	MPI_Comm_size(MPI_COMM_WORLD, &nProcessor);
	if (nProcessor > 1) filename.erase (filename.end()-2, filename.end());
#endif
    
	strcpy (cstr, filename.c_str());
	if (Kind_Solver == POISSON_EQUATION) strcpy (cstr, config->GetStructure_FileName().c_str());
    
	/*--- Special cases where a number needs to be appended to the file name. ---*/
	if ((Kind_Solver == EULER || Kind_Solver == NAVIER_STOKES || Kind_Solver == RANS) &&
        (val_nZone > 1) && (config->GetUnsteady_Simulation() != TIME_SPECTRAL)) {
		sprintf (buffer, "_%d", int(val_iZone));
		strcat(cstr,buffer);
	}
    
	/*--- Special cases where a number needs to be appended to the file name. ---*/
	if (((Kind_Solver == ADJ_EULER) || (Kind_Solver == ADJ_NAVIER_STOKES) || (Kind_Solver == ADJ_RANS)) &&
        (val_nZone > 1) && (config->GetUnsteady_Simulation() != TIME_SPECTRAL)) {
		sprintf (buffer, "_%d", int(val_iZone));
		strcat(cstr,buffer);
	}
    
	if (config->GetUnsteady_Simulation() == TIME_SPECTRAL) {
		if (int(val_iZone) < 10) sprintf (buffer, "_0000%d.vtk", int(val_iZone));
		if ((int(val_iZone) >= 10) && (int(val_iZone) < 100)) sprintf (buffer, "_000%d.vtk", int(val_iZone));
		if ((int(val_iZone) >= 100) && (int(val_iZone) < 1000)) sprintf (buffer, "_00%d.vtk", int(val_iZone));
		if ((int(val_iZone) >= 1000) && (int(val_iZone) < 10000)) sprintf (buffer, "_0%d.vtk", int(val_iZone));
		if (int(val_iZone) >= 10000) sprintf (buffer, "_%d.vtk", int(val_iZone));
        
	} else if (config->GetUnsteady_Simulation() && config->GetWrt_Unsteady()) {
		if (int(iExtIter) < 10) sprintf (buffer, "_0000%d.vtk", int(iExtIter));
		if ((int(iExtIter) >= 10) && (int(iExtIter) < 100)) sprintf (buffer, "_000%d.vtk", int(iExtIter));
		if ((int(iExtIter) >= 100) && (int(iExtIter) < 1000)) sprintf (buffer, "_00%d.vtk", int(iExtIter));
		if ((int(iExtIter) >= 1000) && (int(iExtIter) < 10000)) sprintf (buffer, "_0%d.vtk", int(iExtIter));
		if (int(iExtIter) >= 10000) sprintf (buffer, "_%d.vtk", int(iExtIter));
	} else {
		sprintf (buffer, ".vtk");
	}
    
	strcat(cstr,buffer);
    
	/*--- Open Paraview ASCII file and write the header. ---*/
	ofstream Paraview_File;
	Paraview_File.open(cstr, ios::out);
  Paraview_File.precision(6);
  Paraview_File << "# vtk DataFile Version 3.0\n";
  Paraview_File << "vtk output\n";
  Paraview_File << "ASCII\n";
	Paraview_File << "DATASET UNSTRUCTURED_GRID\n";
  
  /*--- If it's a surface output, print only the points 
   that are in the element list, change the numbering ---*/
  
  if (surf_sol) {
        
    LocalIndex = new unsigned long [nGlobal_Poin+1];
    SurfacePoint = new bool [nGlobal_Poin+1];

    for (iPoint = 0; iPoint < nGlobal_Poin+1; iPoint++) SurfacePoint[iPoint] = false;

    for(iElem = 0; iElem < nGlobal_Line; iElem++) {
      iNode = iElem*N_POINTS_LINE;
      SurfacePoint[Conn_Line[iNode+0]] = true;
      SurfacePoint[Conn_Line[iNode+1]] = true;
    }
    for(iElem = 0; iElem < nGlobal_BoundTria; iElem++) {
      iNode = iElem*N_POINTS_TRIANGLE;
      SurfacePoint[Conn_BoundTria[iNode+0]] = true;
      SurfacePoint[Conn_BoundTria[iNode+1]] = true;
      SurfacePoint[Conn_BoundTria[iNode+2]] = true;
    }
    for(iElem = 0; iElem < nGlobal_BoundQuad; iElem++) {
      iNode = iElem*N_POINTS_QUADRILATERAL;
      SurfacePoint[Conn_BoundQuad[iNode+0]] = true;
      SurfacePoint[Conn_BoundQuad[iNode+1]] = true;
      SurfacePoint[Conn_BoundQuad[iNode+2]] = true;
      SurfacePoint[Conn_BoundQuad[iNode+3]] = true;
    }
    
    nSurf_Poin = 0;
    for (iPoint = 0; iPoint < nGlobal_Poin+1; iPoint++) {
      LocalIndex[iPoint] = 0;
      if (SurfacePoint[iPoint]) { nSurf_Poin++; LocalIndex[iPoint] = nSurf_Poin; }
    }
    
  }
  
  /*--- Write the header ---*/
  if (surf_sol) Paraview_File << "POINTS "<< nSurf_Poin <<" float\n";
  else Paraview_File << "POINTS "<< nGlobal_Poin <<" float\n";
  
	/*--- Write surface and volumetric solution data. ---*/
  for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
    
    if (surf_sol) {
      
      if (LocalIndex[iPoint+1] != 0) {
        
          /*--- Write the node coordinates ---*/
          if (config->GetKind_SU2() != SU2_SOL) {
            for(iDim = 0; iDim < nDim; iDim++)
              Paraview_File << scientific << Coords[iDim][iPoint] << "\t";
            if (nDim == 2) Paraview_File << scientific << "0.0" << "\t";
          }
          else {
            for(iDim = 0; iDim < nDim; iDim++)
              Paraview_File << scientific << Data[iDim][iPoint] << "\t";
            if (nDim == 2) Paraview_File << scientific << "0.0" << "\t";
          }
        
      }
      
    } else {
      
        if (config->GetKind_SU2() != SU2_SOL) {
          for(iDim = 0; iDim < nDim; iDim++)
            Paraview_File << scientific << Coords[iDim][iPoint] << "\t";
          if (nDim == 2) Paraview_File << scientific << "0.0" << "\t";
        }
        else {
          for(iDim = 0; iDim < nDim; iDim++)
            Paraview_File << scientific << Data[iDim][iPoint] << "\t";
          if (nDim == 2) Paraview_File << scientific << "0.0" << "\t";
        }
        
    }
  }
  
  /*--- Write the header ---*/
  nSurf_Elem_Storage = nGlobal_Line*3 +nGlobal_BoundTria*4 + nGlobal_BoundQuad*5;
  nGlobal_Elem_Storage = nGlobal_Tria*4 + nGlobal_Quad*5 + nGlobal_Tetr*5 + nGlobal_Hexa*9 + nGlobal_Wedg*7 + nGlobal_Pyra*6;
  
  if (surf_sol) Paraview_File << "\nCELLS " << nSurf_Elem << "\t" << nSurf_Elem_Storage << "\n";
  else Paraview_File << "\nCELLS " << nGlobal_Elem << "\t" << nGlobal_Elem_Storage << "\n";
  
  if (surf_sol) {
    
    iNode = 0;
    for(iElem = 0; iElem < nGlobal_Line; iElem++) {
      iNode = iElem*N_POINTS_LINE;
      Paraview_File << N_POINTS_LINE << "\t";
      Paraview_File << LocalIndex[Conn_Line[iNode+0]]-1 << "\t";
      Paraview_File << LocalIndex[Conn_Line[iNode+1]]-1 << "\t";
    }
    
    iNode = 0;
    for(iElem = 0; iElem < nGlobal_BoundTria; iElem++) {
      iNode = iElem*N_POINTS_TRIANGLE;
      Paraview_File << N_POINTS_TRIANGLE << "\t";
      Paraview_File << LocalIndex[Conn_BoundTria[iNode+0]]-1 << "\t";
      Paraview_File << LocalIndex[Conn_BoundTria[iNode+1]]-1 << "\t";
      Paraview_File << LocalIndex[Conn_BoundTria[iNode+2]]-1 << "\t";
    }
    
    iNode = 0;
    for(iElem = 0; iElem < nGlobal_BoundQuad; iElem++) {
      iNode = iElem*N_POINTS_QUADRILATERAL;
      Paraview_File << N_POINTS_QUADRILATERAL << "\t";
      Paraview_File << LocalIndex[Conn_BoundQuad[iNode+0]]-1 << "\t";
      Paraview_File << LocalIndex[Conn_BoundQuad[iNode+1]]-1 << "\t";
      Paraview_File << LocalIndex[Conn_BoundQuad[iNode+2]]-1 << "\t";
      Paraview_File << LocalIndex[Conn_BoundQuad[iNode+3]]-1 << "\t";
    }
    
  }
  else {
    
    iNode = 0;
    for(iElem = 0; iElem < nGlobal_Tria; iElem++) {
      iNode = iElem*N_POINTS_TRIANGLE;
      Paraview_File << N_POINTS_TRIANGLE << "\t";
      Paraview_File << Conn_Tria[iNode+0]-1 << "\t";
      Paraview_File << Conn_Tria[iNode+1]-1 << "\t";
      Paraview_File << Conn_Tria[iNode+2]-1 << "\t";
    }
    
    iNode = 0;
    for(iElem = 0; iElem < nGlobal_Quad; iElem++) {
      iNode = iElem*N_POINTS_QUADRILATERAL;
      Paraview_File << N_POINTS_QUADRILATERAL << "\t";
      Paraview_File << Conn_Quad[iNode+0]-1 << "\t";
      Paraview_File << Conn_Quad[iNode+1]-1 << "\t";
      Paraview_File << Conn_Quad[iNode+2]-1 << "\t";
      Paraview_File << Conn_Quad[iNode+3]-1 << "\t";
    }
    
    iNode = 0;
    for(iElem = 0; iElem < nGlobal_Tetr; iElem++) {
      iNode = iElem*N_POINTS_TETRAHEDRON;
      Paraview_File << N_POINTS_TETRAHEDRON << "\t";
      Paraview_File << Conn_Tetr[iNode+0]-1 << "\t" << Conn_Tetr[iNode+1]-1 << "\t";
      Paraview_File << Conn_Tetr[iNode+2]-1 << "\t" << Conn_Tetr[iNode+3]-1 << "\t";
    }
    
    iNode = 0;
    for(iElem = 0; iElem < nGlobal_Hexa; iElem++) {
      iNode = iElem*N_POINTS_HEXAHEDRON;
      Paraview_File << N_POINTS_HEXAHEDRON << "\t";
      Paraview_File << Conn_Hexa[iNode+0]-1 << "\t" << Conn_Hexa[iNode+1]-1 << "\t";
      Paraview_File << Conn_Hexa[iNode+2]-1 << "\t" << Conn_Hexa[iNode+3]-1 << "\t";
      Paraview_File << Conn_Hexa[iNode+4]-1 << "\t" << Conn_Hexa[iNode+5]-1 << "\t";
      Paraview_File << Conn_Hexa[iNode+6]-1 << "\t" << Conn_Hexa[iNode+7]-1 << "\t";
    }
    
    iNode = 0;
    for(iElem = 0; iElem < nGlobal_Wedg; iElem++) {
      iNode = iElem*N_POINTS_WEDGE;
      Paraview_File << N_POINTS_WEDGE << "\t";
      Paraview_File << Conn_Wedg[iNode+0]-1 << "\t" << Conn_Wedg[iNode+1]-1 << "\t";
      Paraview_File << Conn_Wedg[iNode+2]-1 << "\t" << Conn_Wedg[iNode+3]-1 << "\t";
      Paraview_File << Conn_Wedg[iNode+4]-1 << "\t" << Conn_Wedg[iNode+5]-1 << "\t";
    }
    
    iNode = 0;
    for(iElem = 0; iElem < nGlobal_Pyra; iElem++) {
      iNode = iElem*N_POINTS_PYRAMID;
      Paraview_File << N_POINTS_PYRAMID << "\t";
      Paraview_File << Conn_Pyra[iNode+0]-1 << "\t" << Conn_Pyra[iNode+1]-1 << "\t";
      Paraview_File << Conn_Pyra[iNode+2]-1 << "\t" << Conn_Pyra[iNode+3]-1 << "\t";
      Paraview_File << Conn_Pyra[iNode+4]-1 << "\t";
    }
  }
  
  /*--- Write the header ---*/
  if (surf_sol) Paraview_File << "\nCELL_TYPES " << nSurf_Elem << "\n";
  else Paraview_File << "\nCELL_TYPES " << nGlobal_Elem << "\n";
  
  if (surf_sol) {
    for(iElem = 0; iElem < nGlobal_Line; iElem++) Paraview_File << "3\t";    
    for(iElem = 0; iElem < nGlobal_BoundTria; iElem++) Paraview_File << "5\t";    
    for(iElem = 0; iElem < nGlobal_BoundQuad; iElem++) Paraview_File << "9\t";
    
  }
  else {
    for(iElem = 0; iElem < nGlobal_Tria; iElem++) Paraview_File << "5\t";
    for(iElem = 0; iElem < nGlobal_Quad; iElem++) Paraview_File << "9\t";
    for(iElem = 0; iElem < nGlobal_Tetr; iElem++) Paraview_File << "10\t";
    for(iElem = 0; iElem < nGlobal_Hexa; iElem++) Paraview_File << "12\t";
    for(iElem = 0; iElem < nGlobal_Wedg; iElem++) Paraview_File << "13\t";
    for(iElem = 0; iElem < nGlobal_Pyra; iElem++) Paraview_File << "14\t";
  }
  
  
  
  /*--- Write the header ---*/
  if (surf_sol) Paraview_File << "\nPOINT_DATA "<< nSurf_Poin <<"\n";
  else Paraview_File << "\nPOINT_DATA "<< nGlobal_Poin <<"\n";
  
  unsigned short VarCounter = 0;
  
  if (config->GetKind_SU2() == SU2_SOL) {
    
    /*--- If SU2_SOL called this routine, we already have a set of output
     variables with the appropriate string tags stored in the config class. ---*/
    for (unsigned short iField = 1; iField < config->fields.size(); iField++) {
      
      double output_variable = true;
      size_t found = config->fields[iField].find("\"x\"");
      if (found!=string::npos) output_variable = false;
      found = config->fields[iField].find("\"y\"");
      if (found!=string::npos) output_variable = false;
      found = config->fields[iField].find("\"z\"");
      if (found!=string::npos) output_variable = false;
      
      if (output_variable)  {
        Paraview_File << "\nSCALARS " << config->fields[iField] << " float 1\n";
        Paraview_File << "LOOKUP_TABLE default\n";
        
        for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
          if (surf_sol) {
            if (LocalIndex[iPoint+1] != 0) {
              /*--- Loop over the vars/residuals and write the values to file ---*/
              Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
            }
          } else {
            /*--- Loop over the vars/residuals and write the values to file ---*/
            Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
          }
        }
      }
      
      VarCounter++;

      
    }
    
  }  
  
  else {
    
    for (iVar = 0; iVar < nVar_Consv; iVar++) {
      
      Paraview_File << "\nSCALARS Conservative_" << iVar+1 << " float 1\n";
      Paraview_File << "LOOKUP_TABLE default\n";
      
      for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
        if (surf_sol) {
          if (LocalIndex[iPoint+1] != 0) {
            /*--- Loop over the vars/residuals and write the values to file ---*/
            Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
          }
        } else {
          /*--- Loop over the vars/residuals and write the values to file ---*/
          Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
        }
      }
      VarCounter++;
    }
    
    if (config->GetWrt_Limiters()) {
      for (iVar = 0; iVar < nVar_Consv; iVar++) {
        
        Paraview_File << "\nSCALARS Limiter_" << iVar+1 << " float 1\n";
        Paraview_File << "LOOKUP_TABLE default\n";
        
        for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
          if (surf_sol) {
            if (LocalIndex[iPoint+1] != 0) {
              /*--- Loop over the vars/residuals and write the values to file ---*/
              Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
            }
          } else {
            /*--- Loop over the vars/residuals and write the values to file ---*/
            Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
          }
        }
        VarCounter++;
      }
    }
    
    if (config->GetWrt_Residuals()) {
      for (iVar = 0; iVar < nVar_Consv; iVar++) {
        
        Paraview_File << "\nSCALARS Residual_" << iVar+1 << " float 1\n";
        Paraview_File << "LOOKUP_TABLE default\n";
        
        for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
          if (surf_sol) {
            if (LocalIndex[iPoint+1] != 0) {
              /*--- Loop over the vars/residuals and write the values to file ---*/
              Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
            }
          } else {
            /*--- Loop over the vars/residuals and write the values to file ---*/
            Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
          }
        }
        VarCounter++;
      }
    }
    
    /*--- Add names for any extra variables (this will need to be adjusted). ---*/
    if (grid_movement) {
      
      Paraview_File << "\nSCALARS Grid_Velx float 1\n";
      Paraview_File << "LOOKUP_TABLE default\n";
      
      for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
        if (surf_sol) {
          if (LocalIndex[iPoint+1] != 0) {
            /*--- Loop over the vars/residuals and write the values to file ---*/
            Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
          }
        } else {
          /*--- Loop over the vars/residuals and write the values to file ---*/
          Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
        }
      }
      VarCounter++;
      
      Paraview_File << "\nSCALARS Grid_Vely float 1\n";
      Paraview_File << "LOOKUP_TABLE default\n";
      
      for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
        if (surf_sol) {
          if (LocalIndex[iPoint+1] != 0) {
            /*--- Loop over the vars/residuals and write the values to file ---*/
            Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
          }
        } else {
          /*--- Loop over the vars/residuals and write the values to file ---*/
          Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
        }
      }
      VarCounter++;
      
      if (nDim == 3) {
        
        Paraview_File << "\nSCALARS Grid_Velz float 1\n";
        Paraview_File << "LOOKUP_TABLE default\n";
        
        for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
          if (surf_sol) {
            if (LocalIndex[iPoint+1] != 0) {
              /*--- Loop over the vars/residuals and write the values to file ---*/
              Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
            }
          } else {
            /*--- Loop over the vars/residuals and write the values to file ---*/
            Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
          }
        }
        VarCounter++;
        
      }
    }
    
    if (config->GetKind_Regime() == FREESURFACE) {
      
      Paraview_File << "\nSCALARS Density float 1\n";
      Paraview_File << "LOOKUP_TABLE default\n";
      
      for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
        if (surf_sol) {
          if (LocalIndex[iPoint+1] != 0) {
            /*--- Loop over the vars/residuals and write the values to file ---*/
            Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
          }
        } else {
          /*--- Loop over the vars/residuals and write the values to file ---*/
          Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
        }
      }
      VarCounter++;
      
    }
    
    if ((Kind_Solver == EULER) || (Kind_Solver == NAVIER_STOKES) || (Kind_Solver == RANS)) {
      
      Paraview_File << "\nSCALARS Pressure float 1\n";
      Paraview_File << "LOOKUP_TABLE default\n";
      
      for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
        if (surf_sol) {
          if (LocalIndex[iPoint+1] != 0) {
            /*--- Loop over the vars/residuals and write the values to file ---*/
            Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
          }
        } else {
          /*--- Loop over the vars/residuals and write the values to file ---*/
          Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
        }
      }
      VarCounter++;
      
      Paraview_File << "\nSCALARS Temperature float 1\n";
      Paraview_File << "LOOKUP_TABLE default\n";

      for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
    	  if (surf_sol) {
    		  if (LocalIndex[iPoint+1] != 0) {
    			  /*--- Loop over the vars/residuals and write the values to file ---*/
    			  Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
    		  }
    	  } else {
    		  /*--- Loop over the vars/residuals and write the values to file ---*/
    		  Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
    	  }
      }
      VarCounter++;

      Paraview_File << "\nSCALARS Pressure_Coefficient float 1\n";
      Paraview_File << "LOOKUP_TABLE default\n";
      
      for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
        if (surf_sol) {
          if (LocalIndex[iPoint+1] != 0) {
            /*--- Loop over the vars/residuals and write the values to file ---*/
            Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
          }
        } else {
          /*--- Loop over the vars/residuals and write the values to file ---*/
          Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
        }
      }
      VarCounter++;
      
      Paraview_File << "\nSCALARS Mach float 1\n";
      Paraview_File << "LOOKUP_TABLE default\n";
      
      for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
        if (surf_sol) {
          if (LocalIndex[iPoint+1] != 0) {
            /*--- Loop over the vars/residuals and write the values to file ---*/
            Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
          }
        } else {
          /*--- Loop over the vars/residuals and write the values to file ---*/
          Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
        }
      }
      VarCounter++;
      
    }
    
    if ((Kind_Solver == NAVIER_STOKES) || (Kind_Solver == RANS)) {

      Paraview_File << "\nSCALARS Laminar_Viscosity float 1\n";
      Paraview_File << "LOOKUP_TABLE default\n";
      
      for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
        if (surf_sol) {
          if (LocalIndex[iPoint+1] != 0) {
            /*--- Loop over the vars/residuals and write the values to file ---*/
            Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
          }
        } else {
          /*--- Loop over the vars/residuals and write the values to file ---*/
          Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
        }
      }
      VarCounter++;
      
      Paraview_File << "\nSCALARS Skin_Friction_Coefficient float 1\n";
      Paraview_File << "LOOKUP_TABLE default\n";
      
      for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
        if (surf_sol) {
          if (LocalIndex[iPoint+1] != 0) {
            /*--- Loop over the vars/residuals and write the values to file ---*/
            Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
          }
        } else {
          /*--- Loop over the vars/residuals and write the values to file ---*/
          Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
        }
      }
      VarCounter++;
      
      Paraview_File << "\nSCALARS Heat_Flux float 1\n";
      Paraview_File << "LOOKUP_TABLE default\n";
      
      for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
        if (surf_sol) {
          if (LocalIndex[iPoint+1] != 0) {
            /*--- Loop over the vars/residuals and write the values to file ---*/
            Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
          }
        } else {
          /*--- Loop over the vars/residuals and write the values to file ---*/
          Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
        }
      }
      VarCounter++;
      
      Paraview_File << "\nSCALARS Y_Plus float 1\n";
      Paraview_File << "LOOKUP_TABLE default\n";
      
      for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
        if (surf_sol) {
          if (LocalIndex[iPoint+1] != 0) {
            /*--- Loop over the vars/residuals and write the values to file ---*/
            Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
          }
        } else {
          /*--- Loop over the vars/residuals and write the values to file ---*/
          Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
        }
      }
      VarCounter++;
      
    }
    
    if (Kind_Solver == RANS) {
      
      Paraview_File << "\nSCALARS Eddy_Viscosity float 1\n";
      Paraview_File << "LOOKUP_TABLE default\n";
      
      for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
        if (surf_sol) {
          if (LocalIndex[iPoint+1] != 0) {
            /*--- Loop over the vars/residuals and write the values to file ---*/
            Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
          }
        } else {
          /*--- Loop over the vars/residuals and write the values to file ---*/
          Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
        }
      }
      VarCounter++;
      
    }
    
    if (( Kind_Solver == ADJ_EULER         ) ||
        ( Kind_Solver == ADJ_NAVIER_STOKES ) ||
        ( Kind_Solver == ADJ_RANS          )   ) {
      
      Paraview_File << "\nSCALARS Surface_Sensitivity float 1\n";
      Paraview_File << "LOOKUP_TABLE default\n";
      
      for (iPoint = 0; iPoint < nGlobal_Poin; iPoint++) {
        if (surf_sol) {
          if (LocalIndex[iPoint+1] != 0) {
            /*--- Loop over the vars/residuals and write the values to file ---*/
            Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
          }
        } else {
          /*--- Loop over the vars/residuals and write the values to file ---*/
          Paraview_File << scientific << Data[VarCounter][iPoint] << "\t";
        }
      }
      VarCounter++;
      
    }
    
  }
  
	Paraview_File.close();
  
  if (surf_sol) delete [] LocalIndex;
  
}
