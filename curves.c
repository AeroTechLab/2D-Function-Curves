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


#include "curves.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

//#include "debug/sync_debug.h"


typedef struct _SegmentData
{
  double* coeffs;
  size_t coeffsNumber;
  double bounds[ 2 ];
  double offset;
}
SegmentData;

typedef SegmentData* Segment;

struct _CurveData
{
  Segment segmentsList;
  size_t segmentsNumber;
  double scaleFactor, offset;
  double maxAbsoluteValue;
};


Curve Curve_Create( void )
{
  Curve newCurve = (Curve) malloc( sizeof(CurveData) );
  
  newCurve->scaleFactor = 1.0;
  newCurve->offset = 0.0;
  newCurve->maxAbsoluteValue = -1.0;
  newCurve->segmentsList = NULL;
  newCurve->segmentsNumber = 0;
  
  return newCurve;
}

void Curve_Discard( Curve curve )
{
  //DEBUG_PRINT( "unloading curve %p data", curve );
  
  if( curve != NULL )
  {
    if( curve->segmentsList != NULL )
    {
      //for( size_t segmentIndex = 0; segmentIndex < curve->segmentsNumber; segmentIndex++ )
      //  free( curve->segmentsList[ curve->segmentsNumber ].coeffs );
      free( curve->segmentsList );
    }
    
    free( curve );
  }
}

void Curve_AddSpline3Segment( Curve curve, double* splineValues, double splineBounds[ 2 ] )
{
  double splineLength = splineBounds[ 1 ] - splineBounds[ 0 ];
  
  double initialValue = splineValues[ 3 ];
  double initialDerivative = splineValues[ 2 ];
  double finalValue = splineValues[ 1 ];
  double finalDerivative = splineValues[ 0 ];
  
  // Curve ( y = d + c*(x-offset) + b*(x-offset)^2 + a*(x-offset)^3 ) coefficients calculation
  splineValues[ 0 ] = initialValue;
  splineValues[ 1 ] = initialDerivative;
  splineValues[ 2 ] = ( 3 * ( finalValue - initialValue ) - splineLength * ( 2 * initialDerivative + finalDerivative ) ) / pow( splineLength, 2 );
  splineValues[ 3 ] = ( 2 * ( initialValue - finalValue ) + splineLength * ( initialDerivative + finalDerivative ) ) / pow( splineLength, 3 );
  
  //DEBUG_PRINT( " adding spline: %g %g %g %g", splineValues[ 0 ], splineValues[ 1 ], splineValues[ 2 ], splineValues[ 3 ] );
  
  Curve_AddPolySegment( curve, (double*) splineValues, SPLINE3_COEFFS_NUMBER, splineBounds );
  Segment newSegment = &(curve->segmentsList[ curve->segmentsNumber - 1 ]);
  
  if( newSegment != NULL ) newSegment->offset = newSegment->bounds[ 0 ];
}

void Curve_AddPolySegment( Curve curve, double* polyCoeffs, size_t coeffsNumber, double polyBounds[ 2 ] )
{
  if( curve == NULL ) return;
  
  if( coeffsNumber == 0 ) return;
  
  curve->segmentsList = (Segment) realloc( curve->segmentsList, ( curve->segmentsNumber + 1 ) * sizeof(SegmentData) );
  
  Segment newSegment = &(curve->segmentsList[ curve->segmentsNumber++ ]);
  
  newSegment->coeffs = (double*) calloc( coeffsNumber, sizeof(double) );
  newSegment->coeffsNumber = coeffsNumber;

  newSegment->bounds[ 0 ] = polyBounds[ 0 ];
  newSegment->bounds[ 1 ] = polyBounds[ 1 ];
  
  newSegment->offset = 0.0;
  
  memcpy( newSegment->coeffs, polyCoeffs, coeffsNumber * sizeof(double) );
}

void Curve_SetScale( Curve curve, double scaleFactor )
{
  if( curve == NULL ) return;
  
  curve->scaleFactor = scaleFactor;
}

void Curve_SetOffset( Curve curve, double offset )
{
  if( curve == NULL ) return;
  
  curve->offset = offset;
}

void Curve_SetMaxAmplitude( Curve curve, double maxAmplitude )
{
  if( curve == NULL ) return;
  
  curve->maxAbsoluteValue = maxAmplitude;
}

double Curve_GetValue( Curve curve, double valuePosition, double defaultValue )
{
  double curveValue = defaultValue;
  
  if( curve != NULL )
  {
    if( curve->segmentsNumber > 0 )
    {
      for( size_t segmentIndex = 0; segmentIndex < curve->segmentsNumber; segmentIndex++ )
      {
        double* segmentBounds = (double*) curve->segmentsList[ segmentIndex ].bounds;
        if( valuePosition >= segmentBounds[ 0 ] && valuePosition < segmentBounds[ 1 ] )
        {
          double* curveCoeffs = curve->segmentsList[ segmentIndex ].coeffs;
          size_t coeffsNumber = curve->segmentsList[ segmentIndex ].coeffsNumber;
          double positionOffset = curve->segmentsList[ segmentIndex ].offset;
          
          curveValue = 0.0;
          double relativePosition = valuePosition - positionOffset;
          for( size_t coeffIndex = 0; coeffIndex < coeffsNumber; coeffIndex++ )
            curveValue += curveCoeffs[ coeffIndex ] * pow( relativePosition, (double) coeffIndex );
          
          break;
        }
      }
    }
    
    curveValue = curve->scaleFactor * curveValue + curve->offset;
    if( curve->maxAbsoluteValue > 0.0 )
    {
      if( curveValue > curve->maxAbsoluteValue ) curveValue = curve->maxAbsoluteValue;
      else if( curveValue < -curve->maxAbsoluteValue ) curveValue = -curve->maxAbsoluteValue;
    }
  }
  
  return curveValue;
}
