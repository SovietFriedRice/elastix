/*=========================================================================
 *
 *  Copyright UMC Utrecht and contributors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef elxReducedDimensionBSplineInterpolator_h
#define elxReducedDimensionBSplineInterpolator_h

#include "elxIncludes.h" // include first to avoid MSVS warning
#include "itkReducedDimensionBSplineInterpolateImageFunction.h"

namespace elastix
{

/**
 * \class ReducedDimensionBSplineInterpolator
 * \brief An interpolator based on the itkReducedDimensionBSplineInterpolateImageFunction.
 *
 * This interpolator interpolates images with an underlying B-spline
 * polynomial. It only interpolates in the InputImageDimension - 1 dimensions
 * of the image.
 *
 * The parameters used in this class are:
 * \parameter Interpolator: Select this interpolator as follows:\n
 *    <tt>(Interpolator "ReducedDimensionBSplineInterpolator")</tt>
 * \parameter BSplineInterpolationOrder: the order of the B-spline polynomial. \n
 *    example: <tt>(BSplineInterpolationOrder 1 1 1)</tt> \n
 *    The default order is 1. The parameter can be specified for each resolution.\n
 *    If only given for one resolution, that value is used for the other resolutions as well. \n
 *    Currently only first order B-spline interpolation is supported.
 *
 * \ingroup Interpolators
 */

template <class TElastix>
class ITK_TEMPLATE_EXPORT ReducedDimensionBSplineInterpolator
  : public itk::ReducedDimensionBSplineInterpolateImageFunction<typename InterpolatorBase<TElastix>::InputImageType,
                                                                typename InterpolatorBase<TElastix>::CoordRepType,
                                                                double>
  , // CoefficientType
    public InterpolatorBase<TElastix>
{
public:
  /** Standard ITK-stuff. */
  typedef ReducedDimensionBSplineInterpolator Self;
  typedef itk::ReducedDimensionBSplineInterpolateImageFunction<typename InterpolatorBase<TElastix>::InputImageType,
                                                               typename InterpolatorBase<TElastix>::CoordRepType,
                                                               double>
                                        Superclass1;
  typedef InterpolatorBase<TElastix>    Superclass2;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ReducedDimensionBSplineInterpolator, ReducedDimensionBSplineInterpolateImageFunction);

  /** Name of this class.
   * Use this name in the parameter file to select this specific interpolator. \n
   * example: <tt>(Interpolator "ReducedDimensionBSplineInterpolator")</tt>\n
   */
  elxClassNameMacro("ReducedDimensionBSplineInterpolator");

  /** Get the ImageDimension. */
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass1::ImageDimension);

  /** Typedefs inherited from the superclass. */
  using typename Superclass1::OutputType;
  using typename Superclass1::InputImageType;
  using typename Superclass1::IndexType;
  using typename Superclass1::ContinuousIndexType;
  using typename Superclass1::PointType;
  using typename Superclass1::Iterator;
  using typename Superclass1::CoefficientDataType;
  using typename Superclass1::CoefficientImageType;
  using typename Superclass1::CoefficientFilter;
  using typename Superclass1::CoefficientFilterPointer;
  using typename Superclass1::CovariantVectorType;

  /** Typedefs inherited from Elastix. */
  using typename Superclass2::ElastixType;
  using typename Superclass2::ElastixPointer;
  using typename Superclass2::ConfigurationType;
  using typename Superclass2::ConfigurationPointer;
  using typename Superclass2::RegistrationType;
  using typename Superclass2::RegistrationPointer;
  typedef typename Superclass2::ITKBaseType ITKBaseType;

  /** Execute stuff before each new pyramid resolution:
   * \li Set the spline order.
   */
  void
  BeforeEachResolution(void) override;

protected:
  /** The constructor. */
  ReducedDimensionBSplineInterpolator() = default;
  /** The destructor. */
  ~ReducedDimensionBSplineInterpolator() override = default;

private:
  elxOverrideGetSelfMacro;

  /** The deleted copy constructor. */
  ReducedDimensionBSplineInterpolator(const Self &) = delete;
  /** The deleted assignment operator. */
  void
  operator=(const Self &) = delete;
};

} // end namespace elastix

#ifndef ITK_MANUAL_INSTANTIATION
#  include "elxReducedDimensionBSplineInterpolator.hxx"
#endif

#endif // end #ifndef elxReducedDimensionBSplineInterpolator_h
