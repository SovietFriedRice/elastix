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
#ifndef elxStatisticalShapePenalty_h
#define elxStatisticalShapePenalty_h

#include "elxIncludes.h" // include first to avoid MSVS warning
#include "itkStatisticalShapePointPenalty.h"

#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>

namespace elastix
{
using namespace itk;

/**
 * \class StatisticalShapePenalty
 * \brief An metric based on the itk::StatisticalShapePointPenalty.
 *
 * The parameters used in this class are:
 * \parameter Metric: Select this metric as follows:\n
 *    <tt>(Metric "StatisticalShapePenalty")</tt>
 * \parameter ShrinkageIntensity: The mixing ratio ($\beta$) of the provided
 *   covariance matrix and an identity matrix.
 *   $\Sigma' = (1-\beta)\Sigma + \beta \sigma_0^2 I$
 *   Can be defined for each resolution\n
 *    example: <tt>(ShrinkageIntensity 0.2)</tt>
 * \parameter BaseVariance: The width ($\sigma_0^2$) of the non-informative prior.
 *   Can be defined for each resolution\n
 *    example: <tt>(BaseVariance 1000.0)</tt>
 *
 * \author F.F. Berendsen, Image Sciences Institute, UMC Utrecht, The Netherlands
 * \note This work was funded by the projects Care4Me and Mediate.
 * \note If you use the StatisticalShapePenalty anywhere we would appreciate if you cite the following article:\n
 * F.F. Berendsen et al., Free-form image registration regularized by a statistical shape model:
 * application to organ segmentation in cervical MR, Comput. Vis. Image Understand. (2013),
 * http://dx.doi.org/10.1016/j.cviu.2012.12.006
 *
 * \ingroup Metrics
 */

template <class TElastix>
class ITK_TEMPLATE_EXPORT StatisticalShapePenalty
  : public StatisticalShapePointPenalty<typename MetricBase<TElastix>::FixedPointSetType,
                                        typename MetricBase<TElastix>::MovingPointSetType>
  , public MetricBase<TElastix>
{
public:
  /** Standard ITK-stuff. */
  typedef StatisticalShapePenalty Self;
  typedef StatisticalShapePointPenalty<typename MetricBase<TElastix>::FixedPointSetType,
                                       typename MetricBase<TElastix>::MovingPointSetType>
                                   Superclass1;
  typedef MetricBase<TElastix>     Superclass2;
  typedef SmartPointer<Self>       Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(StatisticalShapePenalty, StatisticalShapePointPenalty);

  /** Name of this class.
   * Use this name in the parameter file to select this specific metric. \n
   * example: <tt>(Metric "StatisticalShapePenalty")</tt>\n
   */
  elxClassNameMacro("StatisticalShapePenalty");

  /** Typedefs from the superclass. */
  using typename Superclass1::CoordinateRepresentationType;
  using typename Superclass1::FixedPointSetType;
  using typename Superclass1::FixedPointSetConstPointer;
  using typename Superclass1::MovingPointSetType;
  using typename Superclass1::MovingPointSetConstPointer;

  //  using typename Superclass1::FixedImageRegionType;
  using typename Superclass1::TransformType;
  using typename Superclass1::TransformPointer;
  using typename Superclass1::InputPointType;
  using typename Superclass1::OutputPointType;
  using typename Superclass1::TransformParametersType;
  using typename Superclass1::TransformJacobianType;
  //  using typename Superclass1::RealType;
  using typename Superclass1::FixedImageMaskType;
  using typename Superclass1::FixedImageMaskPointer;
  using typename Superclass1::MovingImageMaskType;
  using typename Superclass1::MovingImageMaskPointer;
  using typename Superclass1::MeasureType;
  using typename Superclass1::DerivativeType;
  using typename Superclass1::ParametersType;

  typedef typename OutputPointType::CoordRepType CoordRepType;
  typedef vnl_vector<CoordRepType>               VnlVectorType;

  /** Other typedef's. */
  typedef itk::Object ObjectType;
  /*typedef itk::AdvancedTransform<
    CoordRepType,
    itkGetStaticConstMacro( FixedImageDimension ),
    itkGetStaticConstMacro( MovingImageDimension ) >  ITKBaseType;
    */
  typedef itk::AdvancedCombinationTransform<CoordRepType, Self::FixedImageDimension> CombinationTransformType;
  typedef typename CombinationTransformType::InitialTransformType                    InitialTransformType;

  /** Typedefs inherited from elastix. */
  using typename Superclass2::ElastixType;
  using typename Superclass2::ElastixPointer;
  using typename Superclass2::ConfigurationType;
  using typename Superclass2::ConfigurationPointer;
  using typename Superclass2::RegistrationType;
  using typename Superclass2::RegistrationPointer;
  typedef typename Superclass2::ITKBaseType ITKBaseType;
  using typename Superclass2::FixedImageType;
  using typename Superclass2::MovingImageType;

  /** The fixed image dimension. */
  itkStaticConstMacro(FixedImageDimension, unsigned int, FixedImageType::ImageDimension);

  /** The moving image dimension. */
  itkStaticConstMacro(MovingImageDimension, unsigned int, MovingImageType::ImageDimension);

  /** Assuming fixed and moving pointsets are of equal type, which implicitly
   * assumes that the fixed and moving image are of the same type.
   */
  typedef FixedPointSetType PointSetType;
  typedef FixedImageType    ImageType;

  /** Sets up a timer to measure the initialization time and calls the
   * Superclass' implementation.
   */
  void
  Initialize(void) override;

  /**
   * Do some things before registration:
   * \li Load and set the pointsets.
   */
  void
  BeforeRegistration(void) override;

  void
  BeforeEachResolution(void) override;

  /** Function to read the corresponding points. */
  unsigned int
  ReadLandmarks(const std::string &                    landmarkFileName,
                typename PointSetType::Pointer &       pointSet,
                const typename ImageType::ConstPointer image);

  unsigned int
  ReadShape(const std::string &                    ShapeFileName,
            typename PointSetType::Pointer &       pointSet,
            const typename ImageType::ConstPointer image);

  /** Overwrite to silence warning. */
  void
  SelectNewSamples(void) override
  {}

protected:
  /** The constructor. */
  StatisticalShapePenalty() = default;
  /** The destructor. */
  ~StatisticalShapePenalty() override = default;

private:
  elxOverrideGetSelfMacro;

  /** The deleted copy constructor. */
  StatisticalShapePenalty(const Self &) = delete;
  /** The deleted assignment operator. */
  void
  operator=(const Self &) = delete;
};

} // end namespace elastix

#ifndef ITK_MANUAL_INSTANTIATION
#  include "elxStatisticalShapePenalty.hxx"
#endif

#endif // end #ifndef elxStatisticalShapePenalty_h
