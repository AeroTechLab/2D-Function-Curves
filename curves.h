//////////////////////////////////////////////////////////////////////////////////////
//                                                                                  //
//  Copyright (c) 2016-2017 Leonardo Consoni <consoni_2519@hotmail.com>             //
//                                                                                  //
//  This file is part of Simple Function Curves.                                    //
//                                                                                  //
//  Simple Function Curves is free software: you can redistribute it and/or modify  //
//  it under the terms of the GNU Lesser General Public License as published        //
//  by the Free Software Foundation, either version 3 of the License, or            //
//  (at your option) any later version.                                             //
//                                                                                  //
//  Simple Function Curves is distributed in the hope that it will be useful,       //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                    //
//  GNU Lesser General Public License for more details.                             //
//                                                                                  //
//  You should have received a copy of the GNU Lesser General Public License        //
//  along with Simple Function Curves. If not, see <http://www.gnu.org/licenses/>.  //
//                                                                                  //
//////////////////////////////////////////////////////////////////////////////////////


/// @file curves.h
/// @brief Abstraction interface for storing curve data
///
/// Interface for calculation of polynomial or spline interpolation between curve points.


#ifndef CURVES_H
#define CURVES_H


#include <stddef.h>

#define SPLINE3_COEFFS_NUMBER 4 

typedef struct _CurveData CurveData;      ///< Single curve internal data structure    
typedef CurveData* Curve;                 ///< Opaque reference to curve internal data structure

                                                                        
/// @brief Creates curve data structure and loads segments data                                              
/// @return reference/pointer to newly created and filled curve data structure
Curve Curve_Create( void );

/// @brief Deallocates internal data of given curve                        
/// @param[in] curve reference to curve
void Curve_Discard( Curve curve );

/// @brief Adds segment to curve based on provided cubic spline boundary conditions
/// @param[in] curve reference to curve
/// @param[in] splineValues array of 4 curve values at its boundaries (initial, initial derivative, final, final derivative)
/// @param[in] splineBounds lower and upper bounds of segment domain
void Curve_AddSpline3Segment( Curve curve, double* splineValues, double splineBounds[ 2 ] );

/// @brief Adds segment to curve based on provided polynomial coefficients
/// @param[in] curve reference to curve
/// @param[in] polyCoeffs array of polynomial coefficients, from lowest to highest order
/// @param[in] coeffsNumber number of polynomial coefficients provided
/// @param[in] polyBounds lower and upper bounds of segment domain
void Curve_AddPolySegment( Curve curve, double* polyCoeffs, size_t coeffsNumber, double polyBounds[ 2 ] );

/// @brief Gets given curve value on specified position
/// @param[in] curve reference to curve
/// @param[in] valuePosition position (x coordinate) where curve value will be calculated
/// @param[in] defaultValue value to be returned if curve value can't be obtained
/// @return calculated curve value or default one
double Curve_GetValue( Curve curve, double valuePosition, double defaultValue );

/// @brief Sets multiplier to be applied to calculated curve value before offset (y axis scaling)
/// @param[in] curve reference to curve
/// @param[out] scaleFactor factor that will multiply next curve values
void Curve_SetScale( Curve curve, double scaleFactor );
                                                                
/// @brief Sets given curve offset (y axis displacement)
/// @param[in] curve reference to curve
/// @param[in] offset value to add to calculated curve value
void Curve_SetOffset( Curve curve, double offset );

/// @brief Limit maximum absolute value calculated for given curve        
/// @param[in] curve reference to curve
/// @param[in] maxAmplitude maximum absolute value ( less than zero for no limit)
void Curve_SetMaxAmplitude( Curve curve, double maxAmplitude );


#endif  // CURVES_H
